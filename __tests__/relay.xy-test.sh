#!/usr/bin/env bash

# Init
cleos push action relay.xy init '[true]' -p relay.xy

# XY
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]' -p relay.xy
cleos push action relay.xy setreserve '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}]' -p relay.xy

# XYZ
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XYZ"}, {"contract": "eosio.token", "symbol": "4,EOS"}]' -p relay.xy
cleos push action relay.xy setreserve '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XYZ"}]' -p relay.xy

# ABC
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,ABC"}, {"contract": "eosio.token", "symbol": "4,EOS"}]' -p relay.xy
cleos push action relay.xy setreserve '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,ABC"}]' -p relay.xy

# ABC/XYZ
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,ABC"}, {"contract": "token.xy", "symbol": "4,XYZ"}]' -p relay.xy
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XYZ"}, {"contract": "token.xy", "symbol": "4,ABC"}]' -p relay.xy

# Actions
cleos -v transfer -f bob relay.xy "1.0000 XY" "EOS" --contract token.xy
cleos -v transfer bob relay.xy "1.0000 EOS" "XY"
cleos -v transfer -f bob relay.xy "1.0000 ABC" "EOS" --contract token.xy
cleos -v transfer bob relay.xy "1.0000 EOS" "ABC"
cleos -v transfer -f bob relay.xy "1.0000 XYZ" "EOS" --contract token.xy
cleos -v transfer bob relay.xy "1.0000 EOS" "XYZ"
cleos -v transfer -f bob relay.xy "1.0000 ABC" "XYZ" --contract token.xy

# # get tables
cleos get table relay.xy relay.xy reserves | jq .
cleos get currency balance token.xy relay.xy XYZ
cleos get currency balance token.xy relay.xy ABC
cleos get currency balance eosio.token relay.xy EOS