#!/usr/bin/env bash

NETWORK=xy
TOKEN=token.xy
RELAY=relay.xy
USER=bob

ENDPOINT=http://localhost:8888

cleos push action $RELAY init '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}]' -p $RELAY
cleos push action $RELAY update '[true]' -p $RELAY

cleos transfer bob relay.xy "1.0000 XY" "" --contract token.xy
cleos transfer bob relay.xy "1.0000 EOS" ""

# cleos push action $NETWORK setrate '["4,EOS"]' -p $NETWORK

# cleos -u $ENDPOINT transfer $USER $NETWORK "1.0000 EOS"

# cleos -u $ENDPOINT push action $NETWORK node \
#     "[${USER},[45.123, 150.123], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK way \
#     "[${USER},[[-25, -45], [65, 180]], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK relation \
#     "[${USER},[{\"type\": \"way\", \"ref\": 1, \"role\": \"\"}, {\"type\": \"node\", \"ref\": 0, \"role\": \"\"}], [{\"k\":\"foo\", \"v\": \"bar\"}]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK move \
#     "[${USER},0, [20, 50]]" \
#     -p $USER

# cleos -u $ENDPOINT push action $NETWORK modify \
#     "[${USER}, 1, [{\"k\":\"second\", \"v\": \"updated\"}, {\"k\":\"foo\", \"v\": \"bar\"}]]" \
#     -p $USER
