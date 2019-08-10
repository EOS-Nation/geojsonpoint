#!/usr/bin/env bash

CODE=xy
TOKEN=token.xy

eosio-cpp -abigen -I include -R resource -contract $CODE -o $CODE.wasm src/$CODE.cpp
eosio-cpp -abigen -I include -R resource -contract $TOKEN -o $TOKEN.wasm src/$TOKEN.cpp