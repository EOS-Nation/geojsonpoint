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
    auto global_itr = _global.get( id, "id not found");
    name type = global_itr.type;
    name uid = global_itr.uid;

    if ( type == "node"_n ) {
        auto node_itr = _node.get( id, "node does not exist" );
        consume_modify_tags(user, uid, node_itr.tags.size(), tags.size());
        _node.modify( node_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else if ( type == "way"_n ) {
        auto way_itr = _way.get( id, "way does not exist" );
        consume_modify_tags(user, uid, way_itr.tags.size(), tags.size());
        _way.modify( way_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
    }
    else if ( type == "relation"_n ) {
        auto relation_itr = _relation.get( id, "relation does not exist" );
        consume_modify_tags(user, uid, relation_itr.tags.size(), tags.size());
        _relation.modify( relation_itr, get_self(), [&]( auto & row ) {
            row.tags = tags;
        });
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

void xy::consume_modify_tags( const name user, const name uid, const int64_t before, const int64_t after )
{
    if (after > before) consume_token(user, uid, 0, after - before, "XY.network::modify");
}
