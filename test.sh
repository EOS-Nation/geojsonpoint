#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
USER=bob

ENDPOINT=http://localhost:8888

# cleos -u $ENDPOINT push action $NETWORK clean \
#     "[]" \
#     -p $USER

cleos -u $ENDPOINT transfer $USER $NETWORK "1.0000 EOS"

cleos -u $ENDPOINT push action $NETWORK createnode \
    "[${USER},[45.123, 150.123], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
    -p $USER

# cleos -u $ENDPOINT push action $NETWORK createway \
#     "[${USER},[[-25, -45], [65, 180]], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK createrel \
#     "[${USER},[{\"type\": \"way\", \"ref\": 1, \"role\": \"\"}, {\"type\": \"node\", \"ref\": 0, \"role\": \"\"}], [{\"k\":\"foo\", \"v\": \"bar\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK move \
#     "[${USER},0, [20, 50]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK modify \
#     "[${USER}, 1, [{\"k\":\"second\", \"v\": \"updated\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK erase \
#     "[${USER},[3]]" \
#     -p $USER

