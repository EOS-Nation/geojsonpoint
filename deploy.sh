#!/usr/bin/env bash

CODE=geopoint
CONTRACT=eosio
ENDPOINT=http://localhost:8888
# CONTRACT=geojsonspec2
# ENDPOINT=https://jungle.eosn.io

cleos -u $ENDPOINT set contract $CONTRACT ./ $CODE.wasm $CODE.abi
