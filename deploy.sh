#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
RELAY=relay.xy
NAMES=names.xy
USER1=bob
USER2=alice
ENDPOINT=http://localhost:8888

cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# Create accounts
cleos create account eosio $NETWORK EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $TOKEN EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $RELAY EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $NAMES EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# Setup eosio.code permissions
cleos set account permission $NETWORK active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "names.xy","permission": "eosio.code"},"weight": 1}, {"permission": {"actor": "xy","permission": "eosio.code"},"weight": 1}]}' owner
cleos set account permission $RELAY active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "relay.xy","permission": "eosio.code"},"weight": 1}]}' owner
cleos set account permission $NAMES active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "names.xy","permission": "eosio.code"},"weight": 1}]}' owner

# Deploy smart contracts
cleos -u $ENDPOINT set contract $NETWORK ./ $NETWORK.wasm $NETWORK.abi
cleos -u $ENDPOINT set contract $TOKEN ./ $TOKEN.wasm $TOKEN.abi
cleos -u $ENDPOINT set contract $RELAY ./ $RELAY.wasm $RELAY.abi
cleos -u $ENDPOINT set contract $NAMES ./ $NAMES.wasm $NAMES.abi
cleos -u $ENDPOINT set contract eosio.token ./ $TOKEN.wasm $TOKEN.abi

# Create & Transfer XY
cleos push action $TOKEN create '["xy", "1000000000.0000 EOSXY"]' -p $TOKEN
cleos push action $TOKEN create '["xy", "1000000000.0000 XY"]' -p $TOKEN
cleos push action $TOKEN issue '["xy", "1010.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "relay.xy" "1000.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "bob" "10.0000 XY", "init"]' -p $NETWORK

# Create & Transfer EOS
cleos push action eosio.token create '["eosio", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "1010.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "relay.xy", "1000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "bob", "10.0000 EOS", "init"]' -p eosio
