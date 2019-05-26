import fs from "fs";
import { nodeos, cleos, delay, exec, eosiocpp } from "./utils";
import { unlock, getPassword, importKey } from "./wallet"
import {
    PRIVATE_KEY,
    PUBLIC_KEY,
    ACCOUNT,
    INPUT_PATH,
    OUTPUT_PATH,
    INCLUDE_PATH,
    RESOURCES_PATH,
    BUILD_PATH,
} from "./config";

export async function configureWallet() {
    await unlock(getPassword());
    await importKey(PRIVATE_KEY);
}

export function boot() {
    return new Promise(async (resolve) => {
        nodeos("-e -p eosio \
            --plugin eosio::http_plugin \
            --plugin eosio::chain_plugin \
            --plugin eosio::chain_api_plugin \
            --plugin eosio::producer_plugin \
            --plugin eosio::history_plugin \
            --plugin eosio::history_api_plugin \
            --plugin eosio::state_history_plugin \
            --http-server-address=0.0.0.0:8888 \
            --access-control-allow-origin=* \
            --http-validate-host=false \
            --max-transaction-time=200 \
            --replay-blockchain \
            --hard-replay-blockchain \
            --disable-replay-opts \
            --delete-all-blocks \
            --contracts-console \
            --filter-on=* \
            --filter-out=eosio:onblock: \
            --trace-history \
            --chain-state-history");
        await delay(1000);
        return resolve();
    });
}

export function kill() {
    return exec("pkill nodeos");
}

export async function build() {
    try { fs.mkdirSync(BUILD_PATH); } catch {}
    try {
        await eosiocpp(`-abigen ${INPUT_PATH} -o ${OUTPUT_PATH} -I ${INCLUDE_PATH} -R ${RESOURCES_PATH}`);
    } catch (e) {
        console.error(e);
    }
}

export async function setContract() {
    try {
        await cleos(`set contract ${ACCOUNT} ${BUILD_PATH} ${ACCOUNT}.wasm ${ACCOUNT}.abi`)
    } catch (e) {
        console.error(e);
    }
}

export async function createAccounts() {
    await createAccount("eosio", "geojsonpoint", PUBLIC_KEY);
    await createAccount("eosio", "user1", PUBLIC_KEY);
    await createAccount("eosio", "user2", PUBLIC_KEY);
}

export async function createAccount(creator: string, name: string, key: string) {
    try {
        await cleos(`create account ${creator} ${name} ${key}`);
    } catch (e) {
        console.error(e);
    }
}
