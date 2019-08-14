#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
RELAY=relay.xy
NAMES=names.xy

eosio-cpp -abigen -R contracts/$NETWORK/resource -contract $NETWORK -o $NETWORK.wasm contracts/$NETWORK/src/$NETWORK.cpp -I contracts/$NETWORK/include -I contracts/$TOKEN/include
eosio-cpp -abigen -R contracts/$TOKEN/resource -contract $TOKEN -o $TOKEN.wasm contracts/$TOKEN/src/$TOKEN.cpp -I contracts/$TOKEN/include
eosio-cpp -abigen -R contracts/$RELAY/resource -contract $RELAY -o $RELAY.wasm contracts/$RELAY/src/$RELAY.cpp -I contracts/$RELAY/include -I contracts/$TOKEN/include
eosio-cpp -abigen -R contracts/$NAMES/resource -contract $NAMES -o $NAMES.wasm contracts/$NAMES/src/$NAMES.cpp -I contracts/$NAMES/include -I contracts/$TOKEN/include
