void geopoint::clean()
{
    // Only `_self` can call `clean` action
    require_auth( get_self() );

    // Remove all rows from `node` TABLE
    auto point_itr = _point.begin();
    while ( point_itr != _point.end() ) {
        point_itr = _point.erase(point_itr);
    }

    // Remove Singletons
    _global.remove();
    _bounds.remove();
}
