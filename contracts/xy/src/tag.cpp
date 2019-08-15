void xy::modify( const name            user,
                 const uint64_t        id,
                 const vector<tag>     tags )
{
    require_auth( user );
    check_owner( user, id );
    check_tags( tags );
    modify_tags( user, id, tags );
    update_version( id );
}

void xy::modify_tags( name user, uint64_t id, vector<tag> tags )
{
    if (node_exists( id )) {
        auto node_itr = _node.find( id );
        consume_modify_tags(user, node_itr->tags.size(), tags.size());
        _node.modify( node_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else if (way_exists( id )) {
        auto way_itr = _way.find( id );
        consume_modify_tags(user, way_itr->tags.size(), tags.size());
        _way.modify( way_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else if (relation_exists( id )) {
        auto relation_itr = _relation.find( id );
        consume_modify_tags(user, relation_itr->tags.size(), tags.size());
        _relation.modify( relation_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else {
        check(false, "[id] not found to modify tags");
    }
}

void xy::check_tags( vector<tag> tags )
{
    check( tags.size() <= 255, "[tags] cannot have more than 255 elements");

    std::set<name> duplicates{};
    for ( auto const tag : tags ) {
        check(duplicates.insert(tag.k).second, "[tag.k] all key names must be unique");
        check_tag( tag );
    }
}

void xy::check_tag( tag tag )
{
    check( tag.k.length() > 0, "[tag.k] must contain at least 1 character");
    check( tag.v.length() <= 255, "[tag.v] cannot be greater than 255 characters");
}

void xy::consume_modify_tags( name user, int64_t before, int64_t after )
{
    if (after > before) consume_token(user, 0, after - before, "modify tags");
}
