/**
 * ACTION relation
 */
name xy::relation( const name              owner,
                   const vector<member>    members,
                   const vector<tag>       tags,
                   const name              uid )
{
    require_auth( owner );
    consume_token( owner, 1, tags.size() + members.size(), "XY.network::relation");
    return emplace_relation( owner, members, tags, uid );
}

name xy::emplace_relation( const name owner, const vector<member> members, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    const time_point_sec timestamp = current_time_point();
    const uint32_t version = 1;

    // get unique id
    const uint64_t id = global_available_primary_key();

    // validate member
    for ( auto const& member: members ) {
        check( member.type == "way"_n || member.type == "node"_n, "[member.type] is invalid (way or node)");
        check( get_uid( member.owner, member.ref ).type == member.type, member.type.to_string() + " ref does not exist" );
    }

    // Create row in `node` TABLE
    _relation.emplace( get_self(), [&]( auto & row ) {
        row.id         = id;
        row.members    = members;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });

    // define unique identifier
    return set_uid( owner, id, uid, name{"relation"} );
}
