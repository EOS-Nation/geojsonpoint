
name xy::get_owner( uint64_t id ) {
    if (node_exists( id )) {
        auto node_itr = _node.find( id );
        return node_itr->owner;
    }
    else if (way_exists( id )) {
        auto way_itr = _way.find( id );
        return way_itr->owner;
    }
    else if (relation_exists( id )) {
        auto relation_itr = _relation.find( id );
        return relation_itr->owner;
    }
    else {
        check(false, "[id] owner not found");
        return ""_n;
    }
}

void xy::check_owner( name user, uint64_t id ) {
    check( get_owner(id) == user, "[user] does not match id owner");
}

void xy::update_version( uint64_t id ) {
    time_point_sec timestamp = current_time_point();
    checksum256 changeset = get_trx_id();

    if (node_exists( id )) {
        auto node_itr = _node.find( id );
        _node.modify( node_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if (way_exists( id )) {
        auto way_itr = _way.find( id );
        _way.modify( way_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if (relation_exists( id )) {
        auto relation_itr = _relation.find( id );
        _relation.modify( relation_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else {
        check(false, "[id] id not found");
    }
}

uint64_t xy::now()
{
    return current_time_point().sec_since_epoch();
}