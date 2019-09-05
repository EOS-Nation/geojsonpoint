#!/usr/bin/env bash

# cleos push action relay.xy init '[true]' -p relay.xy
# cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]' -p relay.xy
# cleos push action relay.xy setreserve '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}]' -p relay.xy
# cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,EOSXY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]' -p relay.xy

cleos -v transfer -f bob relay.xy "1.0000 XY" --contract token.xy
# cleos -v transfer -f bob relay.xy "1.0000 EOSXY" --contract token.xy
# cleos -v transfer bob relay.xy "1.0000 EOS"

# # get tables
# cleos get table relay.xy relay.xy reserves
