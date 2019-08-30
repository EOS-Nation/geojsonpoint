uint64_t xy::global_available_primary_key( const name owner, const name type, const name uid )
{
    uint64_t available_primary_key;

    // user can provide custom UID (unique identifier)
    // user can define a UID for any node/way/relation using a custom name (ex: myuid.xy)
    // using UID's lead to easier XY object tracking compared to using auto-incrementing integer
    if ( uid.length() > 0 ) {
        check( _global.find( uid.value ) != _global.end(), "uid already exists" );
        available_primary_key = uid.value;
        print( uid.suffix() );
    } else {
        available_primary_key = _global.available_primary_key();
    }

    _global.emplace( get_self(), [&]( auto & row ) {
        row.id         = available_primary_key;
        row.uid        = uid;
        row.owner      = owner;
        row.type       = type;
    });
    return available_primary_key;
}
