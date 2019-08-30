
name xy::get_owner( uint64_t id )
{
    auto global_itr = _global.get( id, "id not found" );
    return global_itr.owner;
}

void xy::check_owner( name user, uint64_t id )
{
    check( get_owner(id) == user, "user does not match id owner");
}

void xy::update_version( uint64_t id )
{
    time_point_sec timestamp = current_time_point();
    checksum256 changeset = get_trx_id();

    auto global_itr = _global.get( id, "id not found" );
    name type = global_itr.type;

    if (type == "node"_n) {
        auto node_itr = _node.find( id );
        _node.modify( node_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    } else if (type == "way"_n) {
        auto way_itr = _way.find( id );
        _way.modify( way_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    } else if ( type == "relation"_n) {
        auto relation_itr = _relation.find( id );
        _relation.modify( relation_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
}

uint64_t xy::now()
{
    return current_time_point().sec_since_epoch();
}