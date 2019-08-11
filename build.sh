#!/usr/bin/env bash

CODE=xy
TOKEN=token.xy

eosio-cpp -abigen -R contracts/$CODE/resource  -contract $CODE  -o $CODE.wasm  contracts/$CODE/src/$CODE.cpp   -I contracts/$TOKEN/include -I contracts/$CODE/include
eosio-cpp -abigen -R contracts/$TOKEN/resource -contract $TOKEN -o $TOKEN.wasm contracts/$TOKEN/src/$TOKEN.cpp -I contracts/$TOKEN/include