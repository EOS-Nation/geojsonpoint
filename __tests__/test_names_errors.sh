#!/usr/bin/env bash

# errors
cleos transfer alice names.xy "5.0000 EOS" ""
cleos transfer alice names.xy "5.0000 EOS" "foo.xy"
cleos transfer alice names.xy "5.0000 EOS" "c.foo.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
cleos transfer alice names.xy "25.0000 EOS" "...xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
cleos transfer alice names.xy "0.1000 EOS" "bar.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"
cleos transfer alice names.xy "25.0000 EOS" "bar.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV,xxx"
