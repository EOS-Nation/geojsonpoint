uint64_t xy::global_available_primary_key( const name owner, const name type, const name uid )
{
    uint64_t available_primary_key = _global.available_primary_key();

    // user can provide custom UID (unique identifier)
    // user can define a UID for any node/way/relation using a custom name (ex: myuid.xy)
    // using UID's lead to easier XY object tracking compared to using auto-incrementing integer
    if ( uid.length() > 0 ) {
        check( !uid_exists( uid ), "uid already exists" );

        // reserve uid for owner account name
        if ( is_account( uid ) ) {
            check( uid == owner, "uid is reserved for owner account");
        } else {
            // owner is no-premium *.xy account
            const name suffix = owner.suffix();

            if (suffix != "xy"_n) {
                // must be 12 characters
                check( uid.length() == 12, "uid is only availble for *.xy premium accounts (cannot be <12 length)");

                // cannot contain `.`
                check( uid.suffix() == uid , "uid is only availble for *.xy premium accounts (cannot contain '.')");
            }
        }
    }

    _global.emplace( get_self(), [&]( auto & row ) {
        row.id         = available_primary_key;
        row.uid        = uid;
        row.owner      = owner;
        row.type       = type;
    });
    return available_primary_key;
}
