#!/usr/bin/env bash

USER=geojsonspec2
CONTRACT=geojsonspec2
ENDPOINT=https://jungle.eosn.io

cleos -u $ENDPOINT push action $CONTRACT clean \
    "[]" \
    -p $USER

cleos -u $ENDPOINT push action $CONTRACT createnode \
    "{\"owner\":\"${USER}\",\"node\": {\"lat\":10, \"lon\": 95}, \"tags\":[{\"k\":\"foo\",\"v\":\"bar\"}, {\"k\":\"hello\",\"v\":\"world\"}]}" \
    -p $USER

cleos -u $ENDPOINT push action $CONTRACT createnode \
    "[${USER},{\"lat\":45, \"lon\": 110}, [{\"k\":\"second\",\"v\":\"value0\"}]]" \
    -p $USER

cleos -u $ENDPOINT push action $CONTRACT movenode \
    "[${USER},0, {\"lat\":20, \"lon\": 50}]" \
    -p $USER

cleos -u $ENDPOINT push action $CONTRACT updatetags \
    "[${USER},1, [{\"k\":\"second\", \"v\": \"updated\"}]]" \
    -p $USER
