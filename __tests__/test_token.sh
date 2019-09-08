#!/usr/bin/env bash

cleos push action token.xy issue '["xy", "10.0000 XY", "init"]' -p xy
cleos push action token.xy transfer '["xy", "bob" "10.0000 XY", "init"]' -p xy
cleos push action token.xy consume '["bob" "10.0000 XY", "consume"]' -p xy
