import fs from "fs";
import path from "path";
import os from "os";
import { cleos } from "./utils";

export async function unlock(password: string) {
    try {
        await cleos(`wallet unlock --password ${password}`);
    } catch (e) {
        if (e.stderr.match("3120007")) { } // Wallet is already unlocked: default
        else console.error(e);
    }
}

export async function importKey(privateKey: string) {
    try {
        await cleos(`wallet import --private-key ${privateKey}`);
    } catch (e) {
        if (e.stderr.match("3120008")) { } // Key already in wallet
        else console.error(e);
    }
}

export function getPassword() {
    const passFilePath = path.join(os.homedir(), "eosio-wallet", ".pass");

    // Fetch ~/eosio-wallet/.pass
    if (!fs.existsSync(passFilePath)) {
        throw new Error(`${passFilePath} does not exists\n\ncleos wallet create --file ~/eosio-wallet/.pass\n`);
    }
    return fs.readFileSync(passFilePath, {encoding: "utf8"});
}
