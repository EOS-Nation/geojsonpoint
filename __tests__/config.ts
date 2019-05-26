import path from "path";

// File paths
export const ACCOUNT = "geojsonpoint";
export const INPUT_PATH = path.join(__dirname, "..", "src", ACCOUNT + ".cpp");
export const OUTPUT_PATH = path.join(__dirname, "..", "build", ACCOUNT + ".wasm");
export const INCLUDE_PATH = path.join(__dirname, "..", "include");
export const RESOURCES_PATH = path.join(__dirname, "..", "resources");
export const BUILD_PATH = path.join(__dirname, "..", "build");

// Private Keys
export const PRIVATE_KEY = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3";
export const PUBLIC_KEY = "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV";
