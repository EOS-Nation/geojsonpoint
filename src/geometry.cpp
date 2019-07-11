void geopoint::create(
    const name              owner,
    const double            lat,
    const double            lon,
    const vector<tag>       properties
) {
    // validation
    require_auth( owner );
    validate_properties( properties );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = _node.available_primary_key();

    // Create row in `node` TABLE
    _node.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.lat        = lat;
        row.lon        = lon;

        // Version Control Attributes
        row.user       = owner;
        row.version    = version;
        row.timestamp  = timestamp;
    });

    // Create multiple rows in `tag` TABLE for each properties tag
    for ( auto const prop : properties ) {
        _tag.emplace( _self, [&]( auto & row ) {
            row.tag_id     = _tag.available_primary_key();
            row.id         = id;
            row.k          = prop.k;
            row.v          = prop.v;
        });
    }
}

void geopoint::move(
    const name      user,
    const uint64_t  id,
    const double    lat,
    const double    lon
) {
    // Validate user input
    require_auth( user );

    // Find Point Unique Point Name
    auto points_itr = _node.find( id );
    check( points_itr != _node.end(), "[id] no matching results" );

    // Update `points` table with new coordinates
    _node.modify( points_itr, _self, [&](auto & row){
        row.lat = lat;
        row.lon = lon;

        // Version Control Attributes
        row.user = user;
        row.version = row.version + 1;
        row.timestamp = current_time_point();
    });
}

void geopoint::update(
    const name            user,
    const name            geo_id,
    const vector<name>    keys,
    const vector<string>  values
) {
    // // Validate user input
    // require_auth( user );
    // check( geo_id.length() > 0, "[geo_id] is empty");
    // check( keys.size() == values.size(), "[keys] & [values] must have the same size");

    // // Find Point Unique Point Name
    // auto itr = _properties.find( geo_id.value );
    // check( itr != _properties.end(), "[geo_id] no matching results" );

    // // Update `properties` table
    // _properties.modify( itr, _self, [&](auto & row){
    //     row.keys        = keys;
    //     row.values      = values;
    //     row.user        = user;
    //     row.version     = row.version + 1;
    //     row.timestamp   = current_time_point();
    // });
}
