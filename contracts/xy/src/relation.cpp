/**
 * ACTION relation
 */
uint64_t xy::relation( const name              owner,
                       const vector<member>    members,
                       const vector<tag>       tags )
{
    require_auth( owner );
    uint64_t id = emplace_relation( owner, members, tags );
    consume_token( owner, 1, tags.size() + members.size(), "XY.network::relation");
    return id;
}

uint64_t xy::emplace_relation( name owner, vector<member> members, vector<tag> tags )
{
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    uint64_t id = global_available_primary_key( owner, name{"relation"}, name{""} );

    // validate member
    for (auto const& member: members) {
        check( member.type == "way"_n || member.type == "node"_n, "[member.type] is invalid (way or node)");
        if ( member.type  == "way"_n) check( way_exists(member.ref), "[member.ref] way does not exist" );
        if ( member.type  == "node"_n) check( node_exists(member.ref), "[member.ref] node does not exist" );
    }

    // Create row in `node` TABLE
    _relation.emplace( _self, [&]( auto & row ) {
        row.id         = id;
        row.members    = members;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.owner      = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return id;
}

bool xy::relation_exists( uint64_t id )
{
    auto relation_itr = _relation.find( id );
    return relation_itr != _relation.end();
}

void xy::check_relation_exists( uint64_t id )
{
    check( relation_exists( id ), "[id] no relation matching results" );
}
