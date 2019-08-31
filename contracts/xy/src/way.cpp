/**
 * ACTION way
 */
uint64_t xy::way( const name              owner,
                  const vector<point>     way,
                  const vector<tag>       tags,
                  const name              uid )
{
    require_auth( owner );
    check_way( way );
    uint64_t id = emplace_way( owner, way, tags, uid );
    name type = name{"way"};
    set_uid( owner, id, uid, type );
    set_owner( owner, id, type );
    consume_token( owner, way.size(), tags.size(), "XY.network::way" );
    return id;
}

uint64_t xy::emplace_way( const name owner, const vector<point> way, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key();

    // node id cointainer
    vector<uint64_t> refs;

    for (auto const& node: way) {
        uint64_t id = emplace_node( owner, node, vector<tag>(), name{""} );
        refs.push_back(id);
    }

    // Create row in `node` TABLE
    _way.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.refs       = refs;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return id;
}

bool xy::way_exists( uint64_t id )
{
    auto way_itr = _way.find( id );
    return way_itr != _way.end();
}

void xy::check_way_exists( uint64_t id )
{
    check( way_exists( id ), "[id] no way matching results" );
}

void xy::check_way( vector<point> way )
{
    check( way.size() > 1, "[way] must contain at least 2 nodes");
    check( way.size() <= 255, "[way] cannot contain be greater than 255 nodes");
}