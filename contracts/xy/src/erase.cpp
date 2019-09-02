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
        auto object = get_uid( owner, uid );
        const name type = object.type;
        const uint64_t id = object.id;

        // only owner can erase objects
        check( object.owner == owner, "owner does not match uid");

        if ( type == "node"_n ) erase_node( owner, id );
        else if ( type == "way"_n ) erase_way( owner, id );
        else if ( type == "relation"_n ) erase_relation( owner, id );

        erase_uid( owner, uid );
    }
}

void xy::erase_uid( const name owner, const name uid )
{
    uid_table _uid( "xy"_n, owner.value );
    _uid.erase( _uid.find( uid.value ));
}

void xy::erase_node( const name owner, const uint64_t id )
{
    node_table _node( get_self(), owner.value );
    _node.erase( _node.find( id ));
}

void xy::erase_way( const name owner, const uint64_t id )
{
    way_table _way( get_self(), owner.value );
    _way.erase( _way.find( id ));
}

void xy::erase_relation( const name owner, const uint64_t id )
{
    relation_table _relation( get_self(), owner.value );
    _relation.erase( _relation.find( id ));
}