/**
 * ACTION erase
 */
void xy::erase( const name              owner,
                const vector<uint64_t>  ids )
{
    require_auth( owner );
    check( ids.size() <= 255, "[ids] cannot have more than 255 elements");

    bool is_erased = false;

    // Authentication user by id owner
    for ( auto const id : ids ) {
        check( get_id( id ).owner == owner, "owner does not match id");
    }

    if ( erase_nodes( ids ) ) is_erased = true;
    if ( erase_ways( ids ) ) is_erased = true;
    check( is_erased, "[ids] no ids found to erase");
}

bool xy::erase_nodes( const vector<uint64_t> ids )
{
    bool is_erased = false;

    for ( auto const id : ids ) {
        if ( erase_node( id ) ) is_erased = true;
    }
    return is_erased;
}

bool xy::erase_ways( const vector<uint64_t> ids )
{
    bool is_erased = false;

    for ( auto const id : ids ) {
        if ( erase_way( id ) ) is_erased = true;
    }
    return is_erased;
}

bool xy::erase_node( const uint64_t id )
{
    if ( !node_exists( id ) ) return false;

    auto node_itr = _node.find( id );
    _node.erase( node_itr );
    return true;
}

bool xy::erase_way( const uint64_t id )
{
    if ( !way_exists( id ) ) return false;

    auto way_itr = _way.find( id );
    _way.erase( way_itr );
    return true;
}
