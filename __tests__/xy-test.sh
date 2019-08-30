#!/usr/bin/env bash

NETWORK=xy

# Error Details:
# assertion failure with message: uid is reserved for owner account
cleos push action xy node \
    '["bob",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p bob

# Error Details:
# assertion failure with message: uid is reserved for owner account
cleos push action xy node \
    '["alice.xy",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p alice.xy

# Error Details:
# assertion failure with message: uid is only availble for *.xy premium accounts (cannot be <12 length)
cleos push action xy node \
    '["alice",[45.123, 150.123], [{"k":"building","v":"yes"}], "alice"]' \
    -p alice

# Error Details:
# assertion failure with message: uid is only availble for *.xy premium accounts (cannot contain '.')
cleos push action xy node \
    '["alice",[45.123, 150.123], [{"k":"building","v":"yes"}], "myaccount.xy"]' \
    -p alice

cleos push action xy node \
    '["alice",[45.123, 150.123], [{"k":"building","v":"yes"}], "alice"]' \
    -p alice

cleos push action xy node \
    '["bob.xy",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p bob.xy

cleos push action xy node \
    '["bob",[45.0, 130.0], [{"k":"building","v":"house"}], ""]' \
    -p bob

cleos push action xy way \
    '["bob",[[-25, -45], [65, 180]], [{"k":"key","v":"value"}], ""]' \
    -p bob

cleos push action xy relation \
    '["bob",[{"type": "way", "ref": 2, "role": ""}, {"type": "node", "ref": 0, "role": ""}], [{"k":"foo", "v": "bar"}], ""]' \
    -p bob

cleos push action --force-unique xy move \
    '["bob.xy",0, [22, 50]]' \
    -p bob.xy

cleos -v push action xy modify \
    '["bob.xy", 0, [{"k":"change", "v": "key"}]]' \
    -p bob.xy

# Error Details:
# assertion failure with message: [node] must be different than current point
cleos push action --force-unique  xy move \
    '["bob.xy",0, [22, 50]]' \
    -p bob.xy

# Error Details:
# assertion failure with message: [tag.k] all key names must be unique
cleos -v push action xy modify \
    '["bob.xy", 0, [{"k":"duplicate", "v": "key"}, {"k":"duplicate", "v": "key"}]]' \
    -p bob.xy
