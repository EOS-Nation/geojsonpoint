#!/usr/bin/env bash

cleos -v push action faucet.xy request '["bob"]' -p bob
cleos -v transfer bob faucet.xy "1.0000 EOS"
