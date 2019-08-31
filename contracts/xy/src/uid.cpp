void xy::set_uid( const name owner, const uint64_t id, const name uid, const name type )
{
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
        // Add uid to table
        _uid.emplace( get_self(), [&]( auto & row ) {
            row.uid        = uid;
            row.id         = id;
            row.type       = type;
        });
    }
}


// /**
//  * Convert unique identifier name to global object identifier
//  *
//  * @param {name} uid - unique identifier name
//  * @returns {uint64_t} id - global object identifier
//  * @example
//  *
//  * uid_to_id("mynode.xy"_n); // => 123
//  */
// uint64_t xy::uid_to_id( const name uid )
// {
//     return _uid.get( uid.value, "uid does not exist" ).id;
// }

// /**
//  * Check if unique identifier exists
//  *
//  * @param {name} uid - unique identifier
//  * @returns {bool} true/false
//  * @example
//  *
//  * uid_exists("mynode"); // => true/false
//  */
// bool xy::uid_exists( const name uid )
// {
//     return _uid.find( uid.value ) != index.end();
// }