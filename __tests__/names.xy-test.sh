#!/usr/bin/env bash

NAMES=names.xy
USER1=bob
USER2=alice

cleos push action $NAMES init '["4,EOS"]' -p $NAMES
cleos push action $NAMES setrate '["4", "100.0000 EOS"]' -p $NAMES

cleos transfer bob names.xy "25.0000 EOS" "foo.xy"
cleos transfer alice names.xy "100.0000 EOS" "a.xy"

cleos push action names.xy claimaccount '["alice", "a.xy", "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p alice
cleos push action names.xy claimaccount '["bob", "foo.xy", "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p bob
