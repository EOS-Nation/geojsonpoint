/**
 * ACTION create
 */
uint64_t geopoint::createline(
    const name                  owner,
    const linestring<double>    line,
    const vector<tag>           tags
) {
    require_auth( owner );
    uint64_t id = emplace_linestring( owner, line, tags );
    return id;
}

uint64_t geopoint::emplace_linestring( name owner, linestring<double> line, vector<tag> tags ) {
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key();

    // Create row in `node` TABLE
    _linestring.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.linestring = line;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.user       = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return id;
}