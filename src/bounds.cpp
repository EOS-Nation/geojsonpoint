void geopoint::update_bounds( node node ) {
    bounds_row bounds = _bounds.get_or_default();

    bool modified = false;

    if (node.lat < bounds.minlat) { bounds.minlat = node.lat; modified = true; }
    if (node.lat > bounds.maxlat) { bounds.maxlat = node.lat; modified = true; }
    if (node.lon < bounds.minlon) { bounds.minlon = node.lon; modified = true; }
    if (node.lon > bounds.maxlon) { bounds.maxlon = node.lon; modified = true; }

    if (modified) { _bounds.set( bounds, get_self() ); }
}
