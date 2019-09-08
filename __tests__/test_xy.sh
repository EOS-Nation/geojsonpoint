#!/usr/bin/env bash

cleos push action xy node \
    '["bob",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob"]' \
    -p bob

cleos push action xy node \
    '["bob.xy",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p bob.xy

cleos push action xy node \
    '["bob.xy",[45.123, 150.123], [{"k":"building","v":"yes"}], "foo.xy"]' \
    -p bob.xy

cleos push action xy node \
    '["bob.xy",[45.0, 130.0], [{"k":"building","v":"house"}], "mynode"]' \
    -p bob.xy

cleos push action xy way \
    '["bob.xy",[[-25, -45], [65, 180]], [{"k":"key","v":"value"}], "myway"]' \
    -p bob.xy

cleos push action xy relation \
    '["bob.xy",[{"owner":"bob.xy", "type": "way", "ref": "myway", "role": "road"}, {"owner": "bob.xy", "type": "node", "ref": "mynode", "role": "building"}], [{"k":"key", "v": "value"}], ""]' \
    -p bob.xy

cleos push action --force-unique xy move \
    '["bob.xy", "mynode", [22, 50]]' \
    -p bob.xy

cleos -v push action xy modify \
    '["bob.xy", "myway", [{"k":"change", "v": "key"}]]' \
    -p bob.xy
