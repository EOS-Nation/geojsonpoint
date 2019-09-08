#!/usr/bin/env bash

mkdir -p dist

# xy
eosio-cpp -abigen -contract xy \
  -R contracts/xy/resource \
  -o ./dist/xy.wasm \
  -I contracts/xy/include \
  -I contracts/token/include \
  -I contracts/mapbox/include \
  contracts/xy/src/xy.cpp

# token
eosio-cpp -abigen -contract token \
  -R contracts/token/resource \
  -o ./dist/token.wasm \
  -I contracts/token/include \
  contracts/token/src/token.cpp

# names
eosio-cpp -abigen -contract names \
  -R contracts/names/resource \
  -o ./dist/names.wasm \
  -I contracts/names/include \
  -I contracts/eosio.system/include \
  -I contracts/eosio.token/include \
  contracts/names/src/names.cpp

# faucet
eosio-cpp -abigen -contract faucet \
  -R contracts/faucet/resource \
  -o ./dist/faucet.wasm \
  -I contracts/faucet/include \
  -I contracts/eosio.system/include \
  -I contracts/eosio.token/include \
  contracts/faucet/src/faucet.cpp

# relay
eosio-cpp -abigen -contract relay \
  -R contracts/relay/resource \
  -o ./dist/relay.wasm \
  -I contracts/relay/include \
  -I contracts/eosio.token/include \
  -I contracts/bancor/include \
  contracts/relay/src/relay.cpp

# eosio.token
eosio-cpp -abigen -contract eosio.token \
  -R contracts/eosio.token/resource \
  -o ./dist/eosio.token.wasm \
  -I contracts/eosio.token/include \
  contracts/eosio.token/src/eosio.token.cpp