void geopoint::validate_properties( vector<tag> properties ) {
    check( properties.size() <= 100, "[properties] cannot have more than 100 entries");
}

bool geopoint::id_exists( name id ) {
    auto itr = _node.find( id.value );
    return itr != _node.end();
}