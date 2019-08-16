#!/usr/bin/env bash

RELAY=relay.xy
USER=bob

cleos push action $RELAY init '
[
    {"contract": "eosio.token", "symbol": "4,EOS"},
    {"contract": "token.xy", "symbol": "4,XY"},
    10000
]' -p $RELAY
cleos push action $RELAY update '[true, 10000]' -p $RELAY

cleos transfer bob relay.xy "1.0000 XY" "" --contract token.xy
cleos transfer bob relay.xy "1.0000 EOS" ""
