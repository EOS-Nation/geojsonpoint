#!/usr/bin/env bash

USER=eosio
CONTRACT=eosio
ENDPOINT=http://localhost:8888

# USER=geojsonspec2
# CONTRACT=geojsonspec2
# ENDPOINT=https://jungle.eosn.io

# cleos -u $ENDPOINT push action $CONTRACT clean \
#     "[]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CONTRACT create \
#     "{\"owner\":\"${USER}\",\"lat\": 10, \"lon\": 95, \"tags\":[{\"k\":\"foo\",\"v\":\"bar\"}, {\"k\":\"hello\",\"v\":\"world\"}]}" \
#     -p $USER

# cleos -u $ENDPOINT push action $CONTRACT create \
#     "[${USER},[45.123, 150.123], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CONTRACT createway \
#     "[${USER},[[-25, -45], [65, 180]], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CONTRACT move \
#     "[${USER},0, 20, 50]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CONTRACT modify \
#     "[${USER},1, [{\"k\":\"second\", \"v\": \"updated\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CONTRACT erase \
#     "[${USER},[6]]" \
#     -p $USER

cleos -u $ENDPOINT push action $CONTRACT createrel \
    "[${USER},[{\"type\": \"way\", \"ref\": 14, \"role\": \"\"}, {\"type\": \"node\", \"ref\": 8, \"role\": \"\"}], [{\"k\":\"foo\", \"v\": \"bar\"}]]" \
    -p $USER
