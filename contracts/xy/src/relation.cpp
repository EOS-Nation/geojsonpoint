/**
 * ACTION relation
 */
name xy::relation( const name              owner,
                   const vector<member>    members,
                   const vector<tag>       tags,
                   const name              uid )
{
    require_auth( owner );
    const name relation_uid = emplace_relation( owner, members, tags, uid );
    consume_token( owner, 1, tags.size() + members.size(), "XY.network::relation");
    return relation_uid;
}

name xy::emplace_relation( const name owner, const vector<member> members, const vector<tag> tags, const name uid )
{
    check_tags( tags );

    // Point default attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;

    // get unique id
    const name type = name{"relation"};
    const name relation_uid = set_uid( owner, uid, type );

    // validate member
    for ( auto const& member: members ) {
        check( member.type == "way"_n || member.type == "node"_n, "[member.type] is invalid (way or node)");
        if ( member.type  == "way"_n) check( way_exists(member.ref), "[member.ref] way does not exist" );
        if ( member.type  == "node"_n) check( node_exists(member.ref), "[member.ref] node does not exist" );
    }

    // Create row in `node` TABLE
    _relation.emplace( _self, [&]( auto & row ) {
        row.uid        = relation_uid;
        row.members    = members;
        row.tags       = tags;

        // Initial version vontrol attributes
        row.owner      = owner;
        row.version    = version;
        row.timestamp  = timestamp;
        row.changeset  = get_trx_id();
    });
    return relation_uid;
}

bool xy::relation_exists( const name uid )
{
    auto relation_itr = _relation.find( uid.value );
    return relation_itr != _relation.end();
}

void xy::check_relation_exists( const name uid )
{
    check( relation_exists( uid ), "[uid] no relation matching results" );
}
