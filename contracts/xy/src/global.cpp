
uint64_t xy::global_available_primary_key()
{
    auto global = _global.get_or_default();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set(global, get_self());
    return available_primary_key;
}

void xy::update_version( const name uid )
{
    time_point_sec timestamp = current_time_point();
    checksum256 changeset = get_trx_id();

    auto uid_row = get_uid( uid );
    name type = uid_row.type;

    if (type == "node"_n) {
        auto node_itr = _node.find( uid.value );
        check( node_itr != _node.end(), "node does not exist" );
        _node.modify( node_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if (type == "way"_n) {
        auto way_itr = _way.find( uid.value );
        check( way_itr != _way.end(), "way does not exist" );
        _way.modify( way_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if ( type == "relation"_n) {
        auto relation_itr = _relation.find( uid.value );
        check( relation_itr != _relation.end(), "relation does not exist" );
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
