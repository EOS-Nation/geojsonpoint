#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
USER=bob

ENDPOINT=http://localhost:8888

cleos create account eosio $NETWORK EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $TOKEN EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos set account permission xy active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "xy","permission": "eosio.code"},"weight": 1}]}' owner
cleos set account permission token.xy active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "xy","permission": "eosio.code"},"weight": 1}]}' owner

cleos -u $ENDPOINT set contract $NETWORK ./ $NETWORK.wasm $NETWORK.abi
cleos -u $ENDPOINT set contract $TOKEN ./ $TOKEN.wasm $TOKEN.abi
cleos -u $ENDPOINT set contract eosio.token ./ $TOKEN.wasm $TOKEN.abi

cleos push action $TOKEN create '["xy", "1000000000.0000 XY"]' -p $TOKEN
cleos push action $TOKEN issue '["xy", "1.0000 XY", "init"]' -p $NETWORK

cleos push action eosio.token create '["eosio.token", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio.token", "1000000000.0000 EOS", "init"]' -p eosio.token
cleos push action eosio.token transfer '["eosio.token", "bob", "100.0000 EOS", "init"]' -p eosio.token
