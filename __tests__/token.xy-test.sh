#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
USER=bob

cleos push action $TOKEN issue '["xy", "10.0000 EOSXY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "bob" "10.0000 EOSXY", "init"]' -p $NETWORK
cleos push action $TOKEN consume '["bob" "10.0000 EOSXY", "consume"]' -p $NETWORK
