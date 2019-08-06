#!/usr/bin/env bash

CODE=xy

eosio-cpp -abigen -I include -R resource -contract $CODE -o $CODE.wasm src/$CODE.cpp