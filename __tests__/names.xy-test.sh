#!/usr/bin/env bash

NAMES=names.xy
USER1=bob
USER2=alice

cleos push action $NAMES init '["4,EOS"]' -p $NAMES

cleos transfer bob names.xy "1.0000 EOS" "foo.xy"
cleos push action names.xy newaccount '["bob", "foo.xy", "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p bob
