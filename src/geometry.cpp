void geojsonpoint::create(
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
    uint64_t id = _points.available_primary_key();

    // Create row in `points` TABLE
    _points.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.lat        = lat;
        row.lon        = lon;

        // Version Control Attributes
        row.user       = owner;
        row.version    = version;
        row.timestamp  = timestamp;
    });
}

