void xy::clean()
{
    // Only `_self` can call `clean` action
    require_auth( get_self() );

    // Remove all rows from `node` TABLE
    auto node_itr = _node.begin();
    while ( node_itr != _node.end() ) {
        node_itr = _node.erase(node_itr);
    }

    // Remove all rows from `way` TABLE
    auto way_itr = _way.begin();
    while ( way_itr != _way.end() ) {
        way_itr = _way.erase(way_itr);
    }

    // Remove Singletons
    _global.remove();
    _bounds.remove();
}
