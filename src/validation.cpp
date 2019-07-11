void geojsonpoint::validate_properties( vector<tag> properties ) {
    check( properties.size() <= 100, "[properties] cannot have more than 100 entries");
}

void geojsonpoint::validate_geometry( point geometry ) {
    check( geometry.type == "point"_n, "type must be [point]");
    check( geometry.coordinates.size() >= 2, "[geometry.coordinates] must have at least 2 elements" );
    check( geometry.coordinates.size() <= 3, "[geometry.coordinates] cannot have more than 3 elements" );
}

bool geojsonpoint::id_exists( name id ) {
    auto itr = _points.find( id.value );
    return itr != _points.end();
}