#!/usr/bin/env bash

RELAY=relay.xy
USER=bob

cleos push action $RELAY init '[true]' -p $RELAY

cleos -v transfer -f bob relay.xy "1.0000 XY" --contract token.xy
cleos -v transfer -f bob relay.xy "1.0000 XY" --contract token.xy
cleos -v transfer bob relay.xy "1.0000 EOS"
