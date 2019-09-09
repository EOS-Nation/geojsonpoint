#!/usr/bin/env bash

mkdir -p dist

# relay
eosio-cpp -abigen -contract relay \
  -R resource \
  -o ./dist/relay.wasm \
  -I ../relay/include \
  -I ../eosio.token/include \
  -I ../bancor/include \
  src/relay.cpp

# eosio.token
eosio-cpp -abigen -contract eosio.token \
  -R ../eosio.token/resource \
  -o ./dist/eosio.token.wasm \
  -I ../eosio.token/include \
  ../eosio.token/src/eosio.token.cpp
