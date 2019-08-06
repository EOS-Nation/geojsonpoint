void geopoint::update_bounds( point node ) {
    bounds_row bounds = _bounds.get_or_create( get_self(), {node.x, node.y, node.x, node.y} );

    bool modified = false;

    if (node.x < bounds.min_x) { bounds.min_x = node.x; modified = true; }
    if (node.x > bounds.max_x) { bounds.max_x = node.x; modified = true; }
    if (node.y < bounds.min_y) { bounds.min_y = node.y; modified = true; }
    if (node.y > bounds.max_y) { bounds.max_y = node.y; modified = true; }

    if (modified) { _bounds.set( bounds, get_self() ); }
}
