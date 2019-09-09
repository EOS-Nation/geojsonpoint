#!/usr/bin/env bash

cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# Create accounts
cleos create account eosio xy EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio alice EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio fee.xy EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio relay.xy EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.xy EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.usdt EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.cusd EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio usdt EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio cusd EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# Setup eosio.code permissions
cleos set account permission relay.xy active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "relay.xy","permission": "eosio.code"},"weight": 1}]}' owner

# Deploy smart contracts
cleos set contract relay.xy ./dist relay.wasm relay.abi
cleos set contract eosio.token ./dist eosio.token.wasm eosio.token.abi
cleos set contract token.xy ./dist eosio.token.wasm eosio.token.abi
cleos set contract token.usdt ./dist eosio.token.wasm eosio.token.abi
cleos set contract token.cusd ./dist eosio.token.wasm eosio.token.abi

# Create & Transfer XY
cleos push action token.xy create '["xy", "1000000000.0000 XY"]' -p token.xy
cleos push action token.xy issue '["xy", "10000.0000 XY", "init"]' -p xy
cleos push action token.xy transfer '["xy", "relay.xy" "1000.0000 XY", "init"]' -p xy
cleos push action token.xy transfer '["xy", "bob" "100.0000 XY", "init"]' -p xy
cleos push action token.xy transfer '["xy", "alice" "100.0000 XY", "init"]' -p xy

# Create & Transfer EOS
cleos push action eosio.token create '["eosio", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "12000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "relay.xy", "1000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "bob", "1000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "alice", "1000.0000 EOS", "init"]' -p eosio

# Create USDT/CUSD tokens
cleos push action token.usdt create '["usdt", "1000000000.000000 USDT"]' -p token.usdt
cleos push action token.usdt issue '["usdt", "60000.000000 USDT", "init"]' -p usdt
cleos push action token.usdt transfer '["usdt", "bob", "10000.000000 USDT", "init"]' -p usdt
cleos push action token.usdt transfer '["usdt", "relay.xy", "25000.000000 USDT", "init"]' -p usdt

cleos push action token.cusd create '["cusd", "1000000000.00 CUSD"]' -p token.cusd
cleos push action token.cusd issue '["cusd", "60000.00 CUSD", "init"]' -p cusd
cleos push action token.cusd transfer '["cusd", "bob", "10000.00 CUSD", "init"]' -p cusd
cleos push action token.cusd transfer '["cusd", "relay.xy", "25000.00 CUSD", "init"]' -p cusd
