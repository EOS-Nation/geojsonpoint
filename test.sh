#!/usr/bin/env bash

CODE=xy
TOKEN=token.xy
USER=bob

ENDPOINT=http://localhost:8888

cleos push action $CODE setrate '["0.1000 EOSXY"]' -p $CODE

cleos -u $ENDPOINT transfer $USER $CODE "1.0000 EOS"

# cleos -u $ENDPOINT push action $CODE createnode \
#     "[${USER},[45.123, 150.123], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CODE createway \
#     "[${USER},[[-25, -45], [65, 180]], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CODE createrel \
#     "[${USER},[{\"type\": \"way\", \"ref\": 1, \"role\": \"\"}, {\"type\": \"node\", \"ref\": 0, \"role\": \"\"}], [{\"k\":\"foo\", \"v\": \"bar\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CODE move \
#     "[${USER},0, [20, 50]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $CODE modify \
#     "[${USER}, 1, [{\"k\":\"second\", \"v\": \"updated\"}]]" \
#     -p $USER
