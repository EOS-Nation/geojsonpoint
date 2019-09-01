/**
 * ACTION node
 */
name xy::node( const name           owner,
               const point          node,
               const vector<tag>    tags,
               const name           uid )
{
    require_auth( owner );
    name node_uid = emplace_node( owner, node, tags, uid );
    consume_token( owner, 1, tags.size(), "XY.network::node" );
    return node_uid;
}

/**
 * ACTION move
 */
void xy::move( const name       owner,
               const name       uid,
               const point      node )
{
    require_auth( owner );
    check( get_uid( uid ).owner == owner, "owner does not match uid");
    move_node( uid, node );
    update_version( uid );
}

name xy::emplace_node( const name owner, const point node, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;

    // get unique id
    const name type = name{"node"};
    const name node_uid = set_uid( owner, uid, type );

    // Create row in `node` TABLE
    _node.emplace( get_self(), [&]( auto & row ) {
        row.uid        = node_uid;
        row.node       = node;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.owner      = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return node_uid;
}

void xy::move_node( const name uid, const point node )
{
    check_node_exists( uid );

    auto node_itr = _node.find( uid.value );
    check( node_itr->node != node, "[node] must be different than current point");
    _node.modify( node_itr, get_self(), [&](auto & row) {
        row.node = node;
    });
}

bool xy::node_exists( const name uid )
{
    auto node_itr = _node.find( uid.value );
    return node_itr != _node.end();
}

void xy::check_node_exists( const name uid )
{
    check( node_exists( uid ), "[uid] no node matching results" );
}
