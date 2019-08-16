#!/usr/bin/env bash

NETWORK=xy
USER1=bob
USER2=alice

cleos push action $NETWORK init '[{"contract":"eosio.token", "symbol": "4,EOS"},{"contract":"token.xy", "symbol": "4,EOSXY"},{"contract":"relay.xy", "symbol": "4,XY"}, 10000, 1000]' -p $NETWORK

cleos transfer $USER1 $NETWORK "1.0000 EOS" $USER2

cleos push action $NETWORK node \
    "[${USER1},[45.123, 150.123], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
    -p $USER1

cleos push action $NETWORK way \
    "[${USER1},[[-25, -45], [65, 180]], [{\"k\":\"second\",\"v\":\"value0\"}]]" \
    -p $USER1

cleos push action $NETWORK relation \
    "[${USER1},[{\"type\": \"way\", \"ref\": 1, \"role\": \"\"}, {\"type\": \"node\", \"ref\": 0, \"role\": \"\"}], [{\"k\":\"foo\", \"v\": \"bar\"}]]" \
    -p $USER1

cleos push action $NETWORK move \
    "[${USER1},0, [20, 50]]" \
    -p $USER1

cleos -v push action $NETWORK modify \
    "[${USER1}, 0, [{\"k\":\"second\", \"v\": \"updated\"}, {\"k\":\"foo\", \"v\": \"bar\"}]]" \
    -p $USER1

cleos -v push action $NETWORK modify \
    "[${USER1}, 0, [{\"k\":\"duplicate\", \"v\": \"key\"}, {\"k\":\"duplicate\", \"v\": \"key\"}]]" \
    -p $USER1
