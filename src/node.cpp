void geopoint::createnode(
    const name              owner,
    const node              node,
    const vector<tag>       tags
) {
    require_auth( owner );
    uint64_t id = emplace_node( owner, node );
    emplace_tags( id, tags );
}

void geopoint::movenode(
    const name      user,
    const uint64_t  id,
    const node      node
) {
    require_auth( user );
    move_node( id, node );
    update_node_version( user, id );
}

void geopoint::erasenodes(
    const name              user,
    const vector<uint64_t>  ids
) {
    require_auth( user );
    erase_nodes( ids );
}

uint64_t geopoint::emplace_node( name owner, node node ) {
    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key();

    // Create row in `node` TABLE
    _node.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.lat        = node.lat;
        row.lon        = node.lon;

        // Initial version vontrol attributes
        row.user       = owner;
        row.version    = version;
        row.timestamp  = timestamp;
    });
    return id;
}

void geopoint::erase_nodes( vector<uint64_t> ids ) {
    check( ids.size() <= 255, "[ids] cannot have more than 255 elements");

    for ( auto const id : ids ) {
        erase_node( id );
    }
}

void geopoint::erase_node( uint64_t id ) {
    check_node_exists( id );

    auto node_itr = _node.find( id );
    _node.erase( node_itr );
}


void geopoint::move_node( uint64_t id, node node ) {
    check_node_exists( id );

    auto node_itr = _node.find( id );
    _node.modify( node_itr, _self, [&](auto & row) {
        row.lat = node.lat;
        row.lon = node.lon;
    });
}

void geopoint::update_node_version( name user, uint64_t id ) {
    check_node_exists( id );

    auto node_itr = _node.find( id );
    _node.modify( node_itr, _self, [&](auto & row) {
        row.user        = user;
        row.version     = row.version + 1;
        row.timestamp   = current_time_point();
    });
}

bool geopoint::node_exists( uint64_t id ) {
    auto node_itr = _node.find( id );
    return node_itr != _node.end();
}

void geopoint::check_node_exists( uint64_t id ) {
    check( node_exists( id ), "[id] no matching results" );
}
