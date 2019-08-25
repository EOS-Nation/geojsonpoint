/**
 * ACTION node
 */
uint64_t xy::node( const name             owner,
                   const point            node,
                   const vector<tag>      tags )
{
    require_auth( owner );
    uint64_t id = emplace_node( owner, node, tags );
    consume_token( owner, 1, tags.size(), "XY.network::node" );
    return id;
}

/**
 * ACTION move
 */
void xy::move( const name          user,
               const uint64_t      id,
               const point         node )
{
    require_auth( user );
    check_owner( user, id );
    move_node( id, node );
    update_version( id );
}

uint64_t xy::emplace_node( name owner, point node, vector<tag> tags )
{
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key();

    // Create row in `node` TABLE
    _node.emplace( get_self(), [&]( auto & row ) {
        row.id         = id;
        row.node       = node;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.owner      = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return id;
}

void xy::move_node( uint64_t id, point node )
{
    check_node_exists( id );

    auto node_itr = _node.find( id );
    check( node_itr->node != node, "[node] must be different than current point");
    _node.modify( node_itr, get_self(), [&](auto & row) {
        row.node = node;
    });
}

bool xy::node_exists( uint64_t id )
{
    auto node_itr = _node.find( id );
    return node_itr != _node.end();
}

void xy::check_node_exists( uint64_t id )
{
    check( node_exists( id ), "[id] no node matching results" );
}
