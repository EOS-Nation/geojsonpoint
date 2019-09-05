#!/usr/bin/env bash

cleos push action names.xy init '["4,EOS"]' -p names.xy
cleos push action names.xy setrate '["4", "100.0000 EOS"]' -p names.xy

# simple usage
cleos transfer bob names.xy "25.0000 EOS" "foo.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"

# using referral
cleos transfer alice names.xy "100.0000 EOS" "a.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV,bob"

# errors
cleos transfer alice names.xy "5.0000 EOS" ""
cleos transfer alice names.xy "5.0000 EOS" "foo.xy"
cleos transfer alice names.xy "5.0000 EOS" "c.foo.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
cleos transfer alice names.xy "25.0000 EOS" "...xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
cleos transfer alice names.xy "0.1000 EOS" "bar.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
cleos transfer alice names.xy "25.0000 EOS" "bar.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV,xxx"
