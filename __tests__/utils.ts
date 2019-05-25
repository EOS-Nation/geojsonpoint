import util from "util";
import child_process from "child_process";

export const exec = util.promisify(child_process.exec);
export const cleos = (command: string) => { return exec(`cleos ${command}`); }
export const nodeos = (command: string) => { return exec(`nodeos ${command}`); }
export const eosiocpp = (command: string) => { return exec(`eosio-cpp ${command}`); }

export function delay(ms: number): Promise<void> {
    return new Promise((resolve) => {
        setTimeout(() => {
            return resolve();
        }, ms)
    })
}