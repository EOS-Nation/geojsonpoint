#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy

eosio-cpp -abigen -I include -R resource -contract $NETWORK -o $NETWORK.wasm src/$NETWORK.cpp
eosio-cpp -abigen -I include -R resource -contract $TOKEN -o $TOKEN.wasm src/$TOKEN.cpp