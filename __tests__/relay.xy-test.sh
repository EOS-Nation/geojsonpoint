#!/usr/bin/env bash

# Init
cleos push action relay.xy enable '[true]' -p relay.xy
cleos push action relay.xy setfee '[100, "fee.xy"]' -p relay.xy

# XY/EOS
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]' -p relay.xy
cleos push action relay.xy setreserve '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}]' -p relay.xy

# CUSD/USDT
cleos push action relay.xy setreserve '[{"contract": "token.cusd", "symbol": "2,CUSD"}, {"contract": "token.usdt", "symbol": "6,USDT"}]' -p relay.xy
cleos push action relay.xy setreserve '[{"contract": "token.usdt", "symbol": "6,USDT"}, {"contract": "token.cusd", "symbol": "2,CUSD"}]' -p relay.xy

# Actions
cleos -v transfer -f bob relay.xy "1.0000 XY" "EOS" --contract token.xy
cleos -v transfer bob relay.xy "1.0000 EOS" "XY"

cleos -v transfer bob relay.xy "1.000000 USDT" "CUSD" --contract token.usdt
cleos -v transfer -f bob relay.xy "1.00 CUSD" "USDT" --contract token.cusd

# # get tables
# cleos get table relay.xy relay.xy reserves | jq .
cleos get currency balance token.xy relay.xy XYZ
cleos get currency balance token.xy relay.xy ABC
cleos get currency balance eosio.token relay.xy EOS