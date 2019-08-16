#!/usr/bin/env bash

NAMES=names.xy
USER1=bob
USER2=alice

cleos push action $NAMES init '
[
    {"contract": "eosio.token", "symbol": "4,EOS"},
    {"contract": "token.xy", "symbol": "4,EOSXY"},
    {"contract": "relay.xy", "symbol": "4,XY"},
    "xy",
]' -p $NAMES

cleos -v transfer $USER1 $NAMES "1.0000 EOS" "foo.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV,alice"
