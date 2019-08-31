
uint64_t xy::global_available_primary_key()
{
    auto global = _global.get_or_default();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set(global, get_self());
    return available_primary_key;
}

void xy::update_version( uint64_t id )
{
    time_point_sec timestamp = current_time_point();
    checksum256 changeset = get_trx_id();

    auto owner_itr = _owner.get( id, "id not found" );
    name type = owner_itr.type;

    if (type == "node"_n) {
        auto node_itr = _node.find( id );
        _node.modify( node_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if (type == "way"_n) {
        auto way_itr = _way.find( id );
        _way.modify( way_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if ( type == "relation"_n) {
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