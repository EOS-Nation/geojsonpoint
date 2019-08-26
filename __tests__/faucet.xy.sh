#!/usr/bin/env bash

FAUCET=faucet.xy

cleos -v push action $FAUCET request \
    '["bob"]' \
    -p bob

cleos transfer bob $FAUCET "1.0000 EOS"
