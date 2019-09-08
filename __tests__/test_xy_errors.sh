#!/usr/bin/env bash

# Error Details:
# assertion failure with message: [node] must be different than current point
cleos push action --force-unique  xy move \
    '["bob.xy", "mynode", [22, 50]]' \
    -p bob.xy

# Error Details:
# assertion failure with message: [tag.k] all key names must be unique
cleos -v push action xy modify \
    '["bob.xy", "myway", [{"k":"duplicate", "v": "key"}, {"k":"duplicate", "v": "key"}]]' \
    -p bob.xy


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
# assertion failure with message: uid is only availble for *.xy premium accounts (cannot contain '.')
cleos push action xy node \
    '["alice",[45.123, 150.123], [{"k":"building","v":"yes"}], "myaccount.xy"]' \
    -p alice