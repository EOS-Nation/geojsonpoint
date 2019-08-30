#!/usr/bin/env bash

NETWORK=xy

# Error Details:
# assertion failure with message: uid is reserved for owner account
cleos push action $NETWORK node \
    '["bob",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p bob

# Error Details:
# assertion failure with message: uid is reserved for owner account
cleos push action $NETWORK node \
    '["alice.xy",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p alice.xy

cleos push action $NETWORK node \
    '["bob.xy",[45.123, 150.123], [{"k":"building","v":"yes"}], "bob.xy"]' \
    -p bob.xy

cleos push action $NETWORK node \
    '["bob",[45.0, 130.0], [{"k":"building","v":"house"}], ""]' \
    -p bob

# cleos push action $NETWORK way \
#     '["bob",[[-25, -45], [65, 180]], [{"k":"key","v":"value"}], "myway2.xy"]' \
#     -p bob

# cleos push action $NETWORK relation \
#     '["bob",[{"type": "way", "ref": 1, "role": ""}, {"type": "node", "ref": 0, "role": ""}], [{"k":"foo", "v": "bar"}], "myrelation"]' \
#     -p bob

# cleos push action $NETWORK move \
#     '["bob",0, [20, 50]]' \
#     -p bob

# echo "ERROR OK: [node] must be different than current point"
# cleos push action $NETWORK move \
#     '["bob",0, [20, 50]]' \
#     -p bob

# echo "ERROR OK: [tag.k] all key names must be unique"
# cleos -v push action $NETWORK modify \
#     '["bob", 0, [{"k":"duplicate", "v": "key"}, {"k":"duplicate", "v": "key"}]]' \
#     -p bob
