/**
 * ACTION erase
 */
void xy::erase( const name              owner,
                const vector<name>      uids )
{
    require_auth( owner );
    check( uids.size() <= 255, "uids cannot have more than 255 elements");

    bool is_erased = false;

    // Authentication user by id owner
    for ( auto const uid : uids ) {
        check( get_uid( uid ).owner == owner, "owner does not match uid");
    }

    if ( erase_nodes( uids ) ) is_erased = true;
    if ( erase_ways( uids ) ) is_erased = true;
    check( is_erased, "no uids found to erase");
}

bool xy::erase_nodes( const vector<name> uids )
{
    bool is_erased = false;

    for ( auto const uid : uids ) {
        if ( erase_node( uid ) ) is_erased = true;
    }
    return is_erased;
}

bool xy::erase_ways( const vector<name> uids )
{
    bool is_erased = false;

    for ( auto const uid : uids ) {
        if ( erase_way( uid ) ) is_erased = true;
    }
    return is_erased;
}

bool xy::erase_node( const name uid )
{
    if ( !node_exists( uid ) ) return false;

    auto node_itr = _node.find( uid.value );
    _node.erase( node_itr );
    return true;
}

bool xy::erase_way( const name uid )
{
    if ( !way_exists( uid ) ) return false;

    auto way_itr = _way.find( uid.value );
    _way.erase( way_itr );
    return true;
}
