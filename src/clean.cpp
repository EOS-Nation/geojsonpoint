void geojsonpoint::clean()
{
    // Only `geojsonpoint` can call `clean` action
    require_auth(_self);

    // Remove all rows from `points` table
    auto points_itr = _points.begin();
    while ( points_itr != _points.end() ) {
        _points.erase(points_itr);
    }

    // Remove all rows from `geometries` table
    auto properties = _properties.begin();
    while ( properties != _properties.end() ) {
        _properties.erase(properties);
    }
}
