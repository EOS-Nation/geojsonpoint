/**
 * ACTION createnode
 */
uint64_t xy::createnode(
    const name              owner,
    const point             node,
    const vector<tag>       tags
) {
    require_auth( owner );
    uint64_t id = emplace_node( owner, node, tags );
    update_bounds( node );
    return id;
}

/**
 * ACTION move
 */
void xy::move(
    const name          user,
    const uint64_t      id,
    const point         node
) {
    require_auth( user );
    move_node( id, node );
    update_node_version( user, id );
    update_bounds( node );
}

uint64_t xy::emplace_node( name owner, point node, vector<tag> tags ) {
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key();

    // Create row in `node` TABLE
    _node.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.node       = node;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.user       = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return id;
}

void xy::move_node( uint64_t id, point node ) {
    check_node_exists( id );

    auto node_itr = _node.find( id );
    _node.modify( node_itr, _self, [&](auto & row) {
        row.node = node;
    });
}

void xy::update_node_version( name user, uint64_t id ) {
    check_node_exists( id );

    auto node_itr = _node.find( id );
    _node.modify( node_itr, _self, [&](auto & row) {
        row.user        = user;
        row.version     = row.version + 1;
        row.timestamp   = current_time_point();
        row.changeset   = get_trx_id();
    });
}

bool xy::node_exists( uint64_t id ) {
    auto node_itr = _node.find( id );
    return node_itr != _node.end();
}

void xy::check_node_exists( uint64_t id ) {
    check( node_exists( id ), "[id] no node matching results" );
}
