void geopoint::clean()
{
    // Only `_self` can call `clean` action
    require_auth( get_self() );

    // Remove all rows from `node` TABLE
    auto node_itr = _node.begin();
    while ( node_itr != _node.end() ) {
        node_itr = _node.erase(node_itr);
    }

    // Remove all rows from `tag` TABLE
    auto tag_itr = _tag.begin();
    while ( tag_itr != _tag.end() ) {
        tag_itr = _tag.erase(tag_itr);
    }

    // Set Global to 0
    _global.remove();
}
