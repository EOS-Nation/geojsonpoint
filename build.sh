#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
RELAY=relay.xy
NAMES=names.xy
FAUCET=faucet.xy
SYSTEM=eosio.system

mkdir -p dist
# eosio-cpp -abigen -R contracts/$NETWORK/resource -contract $NETWORK -o ./dist/$NETWORK.wasm contracts/$NETWORK/src/$NETWORK.cpp -I contracts/$NETWORK/include -I contracts/$TOKEN/include
# eosio-cpp -abigen -R contracts/$TOKEN/resource -contract $TOKEN -o ./dist/$TOKEN.wasm contracts/$TOKEN/src/$TOKEN.cpp -I contracts/$TOKEN/include
# eosio-cpp -abigen -R contracts/$FAUCET/resource -contract $FAUCET -o ./dist/$FAUCET.wasm contracts/$FAUCET/src/$FAUCET.cpp -I contracts/$FAUCET/include -I contracts/$TOKEN/include -I contracts/$SYSTEM/include
# eosio-cpp -abigen -R contracts/$RELAY/resource -contract $RELAY -o ./dist/$RELAY.wasm contracts/$RELAY/src/$RELAY.cpp -I contracts/$RELAY/include -I contracts/$TOKEN/include -I contracts/$SYSTEM/include

eosio-cpp -abigen -R contracts/$NAMES/resource -contract $NAMES -o ./dist/$NAMES.wasm contracts/$NAMES/src/$NAMES.cpp -I contracts/$NAMES/include -I contracts/$TOKEN/include -I contracts/$SYSTEM/include