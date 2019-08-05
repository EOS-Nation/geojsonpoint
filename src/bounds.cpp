void geopoint::update_bounds( point<double> point ) {
    bounds_row bounds = _bounds.get_or_default();

    bool modified = false;

    if (point.y < bounds.minlat) { bounds.minlat = point.y; modified = true; }
    if (point.y > bounds.maxlat) { bounds.maxlat = point.y; modified = true; }
    if (point.x < bounds.minlon) { bounds.minlon = point.x; modified = true; }
    if (point.x > bounds.maxlon) { bounds.maxlon = point.x; modified = true; }

    if (modified) { _bounds.set( bounds, get_self() ); }
}
