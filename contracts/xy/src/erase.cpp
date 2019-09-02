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

        if ( type == "node"_n ) erase_node( owner, uid, id );
        else if ( type == "way"_n ) erase_way( owner, uid, id );
        else if ( type == "relation"_n ) erase_relation( owner, uid, id );
    }
}

void xy::erase_node( const name owner, const name uid, const uint64_t id )
{
    uid_table _uid( "xy"_n, owner.value );
    _uid.erase( _uid.find( uid.value ));
    _node.erase( _node.find( id ));
}

void xy::erase_way( const name owner, const name uid, const uint64_t id )
{
    uid_table _uid( "xy"_n, owner.value );
    _uid.erase( _uid.find( uid.value ));
    _way.erase( _way.find( id ));
}

void xy::erase_relation( const name owner, const name uid, const uint64_t id )
{
    uid_table _uid( "xy"_n, owner.value );
    _uid.erase( _uid.find( uid.value ));
    _relation.erase( _relation.find( id ));
}