void xy::modify(
    const name            user,
    const uint64_t        id,
    const vector<tag>     tags
) {
    require_auth( user );
    check_owner( user, id );
    check_tags( tags );
    modify_tags( id, tags );
    update_version( id );
}

void xy::modify_tags( uint64_t id, vector<tag> tags ) {
    if (node_exists( id )) {
        auto node_itr = _node.find( id );
        _node.modify( node_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else if (way_exists( id )) {
        auto way_itr = _way.find( id );
        _way.modify( way_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else if (relation_exists( id )) {
        auto relation_itr = _relation.find( id );
        _relation.modify( relation_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else {
        check(false, "[id] not found to modify tags");
    }
}

void xy::check_tags( vector<tag> tags ) {
    check( tags.size() <= 255, "[tags] cannot have more than 255 elements");

    for ( auto const tag : tags ) {
        check_tag( tag );
    }
}

void xy::check_tag( tag tag ) {
    check( tag.k.length() > 0, "[tag.k] must contain at least 1 character");
    check( tag.k.length() <= 255, "[tag.k] cannot be greater than 255 characters");
    check( tag.v.length() <= 255, "[tag.v] cannot be greater than 255 characters");
}
