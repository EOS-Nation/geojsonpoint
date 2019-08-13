#!/usr/bin/env bash

RELAY=relay.xy
USER=bob

ENDPOINT=http://localhost:8888

cleos push action $RELAY init '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}]' -p $RELAY
cleos push action $RELAY update '[true]' -p $RELAY
cleos push action $RELAY setreserve '[{"contract": "eosio.token", "symbol": "4,EOS"}, 500000]' -p $RELAY

cleos transfer bob relay.xy "1.0000 XY" "" --contract token.xy
cleos transfer bob relay.xy "1.0000 EOS" ""
