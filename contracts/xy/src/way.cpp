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
    consume_token( owner, way.size(), tags.size(), "XY.network::way" );
    return emplace_way( owner, way, tags, uid );
}

name xy::emplace_way( const name owner, const vector<point> way, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    const time_point_sec timestamp = current_time_point();
    const uint32_t version = 1;

    // get unique id
    const uint64_t id = global_available_primary_key();

    // node id cointainer
    vector<name> refs;

    for (auto const& node: way) {
        name uid = emplace_node( owner, node, vector<tag>(), name{""} );
        refs.push_back(uid);
    }

    // Create row in `node` TABLE
    _way.emplace( get_self(), [&]( auto & row ) {
        row.id         = id;
        row.refs       = refs;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });

    // define unique identifier
    return set_uid( owner, id, uid, name{"way"} );
}

void xy::check_way( const vector<point> way )
{
    check( way.size() > 1, "[way] must contain at least 2 nodes");
    check( way.size() <= 255, "[way] cannot contain be greater than 255 nodes");
}