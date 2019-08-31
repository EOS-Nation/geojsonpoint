void xy::set_owner( const name owner, const uint64_t id, const name type )
{
    // Add owner to table
    _owner.emplace( get_self(), [&]( auto & row ) {
        row.id         = id;
        row.owner      = owner;
        row.type       = type;
    });
}

name xy::get_owner( uint64_t id )
{
    auto owner_itr = _owner.get( id, "id not found" );
    return owner_itr.owner;
}

void xy::check_owner( name user, uint64_t id )
{
    check( get_owner(id) == user, "user does not match id owner");
}
