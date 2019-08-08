#!/usr/bin/env bash

CODE=xy
TOKEN=token.xy

# eosio-cpp -abigen -I contracts/$CODE/include -R contracts/$CODE/resource -contract $CODE -o $CODE.wasm contracts/$CODE/src/$CODE.cpp
eosio-cpp -abigen -I contracts/$TOKEN/include -R contracts/$TOKEN/resource -contract $TOKEN -o $TOKEN.wasm contracts/$TOKEN/src/$TOKEN.cpp