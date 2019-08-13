#!/usr/bin/env bash

NAMES=names.xy
USER1=bob
USER2=alice

ENDPOINT=http://localhost:8888

cleos push action $NAMES init '[{"contract": "eosio.token", "symbol": "4,EOS"}, "relay.xy"]' -p $NAMES

cleos transfer $USER1 $NAMES "1.0000 EOS" ""
cleos transfer $USER1 $NAMES "1.0000 EOS" $USER2
