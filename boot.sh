#!/usr/bin/env bash

nodeos -e -p eosio \
    --plugin eosio::http_plugin \
    --plugin eosio::chain_plugin \
    --plugin eosio::chain_api_plugin \
    --plugin eosio::producer_plugin \
    --plugin eosio::history_plugin \
    --plugin eosio::history_api_plugin \
    --plugin eosio::state_history_plugin \
    --http-server-address=0.0.0.0:8888 \
    --access-control-allow-origin=* \
    --http-validate-host=false \
    --max-transaction-time=200 \
    --replay-blockchain \
    --hard-replay-blockchain \
    --disable-replay-opts \
    --delete-all-blocks \
    --contracts-console \
    --filter-on=* \
    --filter-out=eosio:onblock: \
    --trace-history \
    --chain-state-history
