#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
RELAY=relay.xy
NAMES=names.xy
FAUCET=faucet.xy
FEE=fee.xy
OPS=ops.xy
USER1=bob
USER2=alice

cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# Create accounts
cleos create account eosio $NETWORK EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $TOKEN EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER1.xy EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $USER2.xy EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $RELAY EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $NAMES EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $FAUCET EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $FEE EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio $OPS EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.usdt EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.cusd EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# Setup eosio.code permissions
cleos set account permission $NETWORK active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "faucet.xy","permission": "eosio.code"},"weight": 1}, {"permission": {"actor": "names.xy","permission": "eosio.code"},"weight": 1}, {"permission": {"actor": "xy","permission": "eosio.code"},"weight": 1}]}' owner
cleos set account permission $RELAY active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "relay.xy","permission": "eosio.code"},"weight": 1}]}' owner
cleos set account permission $NAMES active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "names.xy","permission": "eosio.code"},"weight": 1}]}' owner
cleos set account permission $FAUCET active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission": {"actor": "faucet.xy","permission": "eosio.code"},"weight": 1}]}' owner

# Deploy smart contracts
cleos set contract $NETWORK ./dist $NETWORK.wasm $NETWORK.abi
cleos set contract $TOKEN ./dist $TOKEN.wasm $TOKEN.abi
cleos set contract $RELAY ./dist $RELAY.wasm $RELAY.abi
cleos set contract $NAMES ./dist $NAMES.wasm $NAMES.abi
cleos set contract $FAUCET ./dist $FAUCET.wasm $FAUCET.abi
cleos set contract eosio.token ./dist $TOKEN.wasm $TOKEN.abi
cleos set contract token.usdt ./dist $TOKEN.wasm $TOKEN.abi
cleos set contract token.cusd ./dist $TOKEN.wasm $TOKEN.abi

# Create & Transfer XY
cleos push action $TOKEN create '["xy", "1000000000.0000 XY"]' -p $TOKEN
cleos push action $TOKEN issue '["xy", "10000.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "relay.xy" "1000.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "faucet.xy" "1000.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "bob" "100.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "bob.xy" "100.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "alice" "100.0000 XY", "init"]' -p $NETWORK
cleos push action $TOKEN transfer '["xy", "alice.xy" "100.0000 XY", "init"]' -p $NETWORK

# Create & Transfer EOS
cleos push action eosio.token create '["eosio", "1000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "12000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "relay.xy", "1000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "bob", "1000.0000 EOS", "init"]' -p eosio
cleos push action eosio.token transfer '["eosio", "alice", "1000.0000 EOS", "init"]' -p eosio

# Create USDT/CUSD tokens
cleos push action token.usdt create '["xy", "1000000000.000000 USDT"]' -p token.usdt
cleos push action token.usdt issue '["xy", "60000.000000 USDT", "init"]' -p $NETWORK
cleos push action token.usdt transfer '["xy", "bob", "10000.000000 USDT", "init"]' -p $NETWORK
cleos push action token.usdt transfer '["xy", "relay.xy", "25000.000000 USDT", "init"]' -p $NETWORK

cleos push action token.cusd create '["xy", "1000000000.00 CUSD"]' -p token.cusd
cleos push action token.cusd issue '["xy", "60000.00 CUSD", "init"]' -p $NETWORK
cleos push action token.cusd transfer '["xy", "bob", "10000.00 CUSD", "init"]' -p $NETWORK
cleos push action token.cusd transfer '["xy", "relay.xy", "25000.00 CUSD", "init"]' -p $NETWORK
