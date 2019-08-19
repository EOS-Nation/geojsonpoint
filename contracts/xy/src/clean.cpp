void xy::clean()
{
    require_auth(_self);

    auto node_itr = _node.begin();
    while ( node_itr != _node.end() ) {
        node_itr = _node.erase(node_itr);
    }

    auto way_itr = _way.begin();
    while ( way_itr != _way.end() ) {
        way_itr = _way.erase(way_itr);
    }

    auto relation_itr = _relation.begin();
    while ( relation_itr != _relation.end() ) {
        relation_itr = _relation.erase(relation_itr);
    }

    if ( _global.exists() ) _global.remove();
    if ( _settings.exists() ) _settings.remove();
}