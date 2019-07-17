void geopoint::modify(
    const name            user,
    const uint64_t        id,
    const vector<tag>     tags
) {
    require_auth( user );
    modify_tags( id, tags );
    update_node_version( user, id );
}

void geopoint::modify_tags( uint64_t id, vector<tag> tags ) {
    check_node_exists( id );
    check_tags( tags );

    auto node_itr = _node.find( id );
    _node.modify( node_itr, get_self() , [&]( auto & row ) {
        row.tags = tags;
    });
}

void geopoint::check_tags( vector<tag> tags ) {
    check( tags.size() <= 255, "[tags] cannot have more than 255 elements");

    for ( auto const tag : tags ) {
        check_tag( tag );
    }
}

void geopoint::check_tag( tag tag ) {
    check( tag.k.length() > 0, "[tag.k] must contain at least 1 character");
    check( tag.k.length() <= 255, "[tag.k] cannot be greater than 255 characters");
    check( tag.v.length() <= 255, "[tag.v] cannot be greater than 255 characters");
}
