/**
 * ACTION create
 */
uint64_t geopoint::create(
    const name              owner,
    const point<double>     pt,
    const vector<tag>       tags
) {
    require_auth( owner );
    uint64_t id = emplace_point( owner, pt, tags );
    update_bounds( pt );
    return id;
}

/**
 * ACTION move
 */
void geopoint::move(
    const name          user,
    const uint64_t      id,
    const point<double> pt
) {
    require_auth( user );
    move_point( id, pt );
    update_point_version( user, id );
    update_bounds( pt );
}

/**
 * ACTION erase
 */
void geopoint::erase(
    const name              user,
    const vector<uint64_t>  ids
) {
    require_auth( user );
    erase_points( ids );
}

uint64_t geopoint::emplace_point( name owner, point<double> pt, vector<tag> tags ) {
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key();

    // Create row in `node` TABLE
    _point.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.point      = pt;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.user       = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return id;
}

void geopoint::erase_points( vector<uint64_t> ids ) {
    check( ids.size() <= 255, "[ids] cannot have more than 255 elements");

    for ( auto const id : ids ) {
        erase_point( id );
    }
}

void geopoint::erase_point( uint64_t id ) {
    check_point_exists( id );

    auto point_itr = _point.find( id );
    _point.erase( point_itr );
}


void geopoint::move_point( uint64_t id, point<double> pt ) {
    check_point_exists( id );

    auto point_itr = _point.find( id );
    _point.modify( point_itr, _self, [&](auto & row) {
        row.point = pt;
    });
}

void geopoint::update_point_version( name user, uint64_t id ) {
    check_point_exists( id );

    auto point_itr = _point.find( id );
    _point.modify( point_itr, _self, [&](auto & row) {
        row.user        = user;
        row.version     = row.version + 1;
        row.timestamp   = current_time_point();
        row.changeset   = get_trx_id();
    });
}

bool geopoint::point_exists( uint64_t id ) {
    auto point_itr = _point.find( id );
    return point_itr != _point.end();
}

void geopoint::check_point_exists( uint64_t id ) {
    check( point_exists( id ), "[id] no matching results" );
}
