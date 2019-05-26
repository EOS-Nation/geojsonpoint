import { boot, kill, build, configureWallet, createAccounts, setContract } from "./boot";

beforeAll(async () => {
    jest.setTimeout(20000);
    await boot();
    await build();
    await configureWallet();
    await createAccounts();
    await setContract();
});

afterAll(async () => {
    await kill()
});

test("geojsonpoint", async () => {
    console.log("foo");
    expect(true).toEqual(true);
});
