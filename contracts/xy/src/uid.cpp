name xy::set_uid( const name owner, const uint64_t id, name uid, const name type )
{
    // user can provide custom UID (unique identifier)
    // user can define a UID for any node/way/relation using a custom name (ex: myuid.xy)
    // using UID's lead to easier XY object tracking compared to using auto-incrementing integer
    if ( uid.length() > 0 ) {
        check( !uid_exists( owner, uid ), "uid already exists" );

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
     } else {
        uid = name{ id };
    }
    // add owner to table if does not exist
    if ( _owner.find( owner.value ) == _owner.end() ) {
        _owner.emplace( get_self(), [&]( auto & row ) {
            row.owner      = owner;
        });
    }

    // Add uid to table
    uid_table _uid( get_self(), owner.value );
    _uid.emplace( get_self(), [&]( auto & row ) {
        row.uid        = uid;
        row.id         = id;
        row.owner      = owner;
        row.type       = type;
    });

    return uid;
}
