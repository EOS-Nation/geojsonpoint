/**
 * ACTION way
 */
name xy::way( const name              owner,
              const vector<point>     way,
              const vector<tag>       tags,
              const name              uid )
{
    require_auth( owner );
    check_way( way );
    const name way_uid = emplace_way( owner, way, tags, uid );
    consume_token( owner, way.size(), tags.size(), "XY.network::way" );
    return way_uid;
}

name xy::emplace_way( const name owner, const vector<point> way, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;

    // get unique id
    const name type = name{"way"};
    const name way_uid = set_uid( owner, uid, type );

    // node id cointainer
    vector<name> refs;

    for (auto const& node: way) {
        name uid = emplace_node( owner, node, vector<tag>(), name{""} );
        refs.push_back(uid);
    }

    // Create row in `node` TABLE
    _way.emplace( _self, [&]( auto & row ) {
        row.uid        = way_uid;
        row.refs       = refs;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.owner      = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return way_uid;
}

bool xy::way_exists( const name uid )
{
    auto way_itr = _way.find( uid.value );
    return way_itr != _way.end();
}

void xy::check_way_exists( const name uid )
{
    check( way_exists( uid ), "[uid] no way matching results" );
}

void xy::check_way( const vector<point> way )
{
    check( way.size() > 1, "[way] must contain at least 2 nodes");
    check( way.size() <= 255, "[way] cannot contain be greater than 255 nodes");
}