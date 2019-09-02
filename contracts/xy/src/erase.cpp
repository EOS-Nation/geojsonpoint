/**
 * ACTION erase
 */
void xy::erase( const name              owner,
                const vector<name>      uids )
{
    require_auth( owner );
    check( uids.size() <= 255, "cannot erase more than 255 objects");
    erase_objects( owner, uids );
}

void xy::erase_objects( const name owner, const vector<name> uids )
{
    // iterate over uids
    for ( auto const uid : uids ) {
        auto object = get_uid( uid );

        // only owner can erase objects
        check( object.owner == owner, "owner does not match uid");

        if (object.type == "node"_n) erase_node( object.id );
        else if (object.type == "way"_n) erase_way( object.id );
        else if (object.type == "relation"_n) erase_relation( object.id );
    }
}

void xy::erase_node( const uint64_t id )
{
    _uid.erase( _uid.find( id ));
    _node.erase( _node.find( id ));
}

void xy::erase_way( const uint64_t id )
{
    _uid.erase( _uid.find( id ));
    _way.erase( _way.find( id ));
}

void xy::erase_relation( const uint64_t id )
{
    _uid.erase( _uid.find( id ));
    _relation.erase( _relation.find( id ));
}