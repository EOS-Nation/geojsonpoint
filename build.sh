#!/usr/bin/env bash

CODE=geopoint

eosio-cpp -abigen -I include -R resource -contract $CODE -o $CODE.wasm src/$CODE.cpp