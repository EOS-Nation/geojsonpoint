import path from "path";
import fs from "fs";
import { nodeos, delay, exec, eosiocpp } from "./utils";
import { unlock, getPassword, importKey } from "./wallet"

export const CONTRACT_ACTIVE_PRIVATE_KEY = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3" // EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
export const TEST_ACTIVE_PRIVATE_KEY = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3" // EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
export const TEST_OWNER_PRIVATE_KEY = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3" // EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
export const EOSIO_PVT = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3" // EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

export async function wallet() {
    await unlock(getPassword());
    await importKey(CONTRACT_ACTIVE_PRIVATE_KEY);
    await importKey(TEST_ACTIVE_PRIVATE_KEY);
    await importKey(TEST_OWNER_PRIVATE_KEY);
    await importKey(EOSIO_PVT);
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
        await delay(2000);
        return resolve();
    });
}

export function kill() {
    return exec("pkill nodeos");
}

export async function compile() {
    const input = path.join(__dirname, "..", "src", "geojsonpoint.cpp");
    const wasm = path.join(__dirname, "..", "build", "geojsonpoint.wasm");
    const include = path.join(__dirname, "..", "include");
    const resources = path.join(__dirname, "..", "resources");

    try { fs.mkdirSync(path.join(__dirname, "..", "build")); } catch {}
    try {
        await eosiocpp(`-abigen ${input} -o ${wasm} -I ${include} -R ${resources}`);
    } catch (e) {
        console.error(e);
    }
}
