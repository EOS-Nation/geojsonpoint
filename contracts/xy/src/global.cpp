
uint64_t xy::global_available_primary_key()
{
    auto global = _global.get_or_default();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set( global, get_self() );
    return available_primary_key;
}

void xy::update_version( const name owner, const name uid )
{
    time_point_sec timestamp = current_time_point();
    checksum256 changeset = get_trx_id();

    auto object = get_uid( owner, uid );
    const name type = object.type;
    const uint64_t id = object.id;

    if (type == "node"_n) update_version_node( owner, id, timestamp, changeset );
    else if (type == "way"_n) update_version_way( owner, id, timestamp, changeset );
    else if ( type == "relation"_n) update_version_relation( owner, id, timestamp, changeset );
}

void xy::update_version_node( const name owner, const uint64_t id, const time_point_sec timestamp, const checksum256 changeset )
{
    node_table _node( get_self(), owner.value );
    auto itr = _node.find( id );
    check( itr != _node.end(), "node does not exist" );
    _node.modify( itr, get_self(), [&](auto & row) {
        row.version     = row.version + 1;
        row.timestamp   = timestamp;
        row.changeset   = changeset;
    });
}

void xy::update_version_way( const name owner, const uint64_t id, const time_point_sec timestamp, const checksum256 changeset )
{
    way_table _way( get_self(), owner.value );
    auto itr = _way.find( id );
    check( itr != _way.end(), "way does not exist" );
    _way.modify( itr, get_self(), [&](auto & row) {
        row.version     = row.version + 1;
        row.timestamp   = timestamp;
        row.changeset   = changeset;
    });
}

void xy::update_version_relation( const name owner, const uint64_t id, const time_point_sec timestamp, const checksum256 changeset )
{
    relation_table _relation( get_self(), owner.value );
    auto itr = _relation.find( id );
    check( itr != _relation.end(), "relation does not exist" );
    _relation.modify( itr, get_self(), [&](auto & row) {
        row.version     = row.version + 1;
        row.timestamp   = timestamp;
        row.changeset   = changeset;
    });
}

uint64_t xy::now()
{
    return current_time_point().sec_since_epoch();
}
