/**
 * ACTION node
 */
name xy::node( const name           owner,
               const point          node,
               const vector<tag>    tags,
               const name           uid )
{
    require_auth( owner );
    consume_token( owner, 1, tags.size(), "XY.network::node" );
    return emplace_node( owner, node, tags, uid );
}

/**
 * ACTION move
 */
void xy::move( const name       owner,
               const name       uid,
               const point      node )
{
    require_auth( owner );
    check_owner( owner, uid );
    move_node( uid, node );
    update_version( uid );
}

name xy::emplace_node( const name owner, const point node, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    const time_point_sec timestamp = current_time_point();
    const uint32_t version = 1;

    // global object sequence
    const uint64_t id = global_available_primary_key();

    // Create row in `node` TABLE
    _node.emplace( get_self(), [&]( auto & row ) {
        row.id         = id;
        row.node       = node;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });

    // define unique identifier
    return set_uid( owner, id, uid, name{"node"} );
}

void xy::move_node( const name uid, const point node )
{
    auto object = get_uid( uid );
    check( object.type == "node"_n, "move action is only valide for node objects");

    auto node_itr = _node.find( object.id );
    check( node_itr->node != node, "[node] must be different than current point");
    _node.modify( node_itr, get_self(), [&](auto & row) {
        row.node = node;
    });
}
