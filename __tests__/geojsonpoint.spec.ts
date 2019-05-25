import { boot, kill, compile, wallet } from "./boot";

beforeAll(async () => {
    jest.setTimeout(20000);
    await compile();
    await boot();
    await wallet();
});

afterAll(async () => {
    await kill()
});

test("geojsonpoint", async () => {
    console.log("foo");
    expect(true).toEqual(true);
});
