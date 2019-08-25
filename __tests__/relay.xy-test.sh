#!/usr/bin/env bash

RELAY=relay.xy
USER=bob

cleos push action $RELAY init '
[
    true,
    0
]' -p $RELAY

cleos transfer bob relay.xy "0.0100 XY" --contract token.xy
cleos -v transfer bob relay.xy "0.0100 EOS"
