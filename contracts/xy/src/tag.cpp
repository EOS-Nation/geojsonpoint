void xy::modify( const name            owner,
                 const name            uid,
                 const vector<tag>     tags )
{
    require_auth( owner );
    check_tags( tags );
    modify_tags( owner, uid, tags );
    update_version( owner, uid );
}

void xy::modify_tags( const name owner, const name uid, const vector<tag> tags )
{
    auto object = get_uid( owner, uid );
    name type = object.type;
    uint64_t id = object.id;

    if ( type == "node"_n ) modify_tags_node( owner, id, tags );
    else if ( type == "way"_n ) modify_tags_way( owner, id, tags );
    else if ( type == "relation"_n ) modify_tags_relation( owner, id, tags );
}

void xy::modify_tags_node( const name owner, const uint64_t id, const vector<tag> tags )
{
    auto itr = _node.find( id );
    check( itr != _node.end(), "node does not exist" );
    consume_modify_tags( owner, itr->tags.size(), tags.size() );
    _node.modify( itr, get_self(), [&]( auto & row ) {
        row.tags = tags;
    });
}

void xy::modify_tags_way( const name owner, const uint64_t id, const vector<tag> tags )
{
    auto itr = _way.find( id );
    check( itr != _way.end(), "way does not exist" );
    consume_modify_tags( owner, itr->tags.size(), tags.size() );
    _way.modify( itr, get_self(), [&]( auto & row ) {
        row.tags = tags;
    });
}

void xy::modify_tags_relation( const name owner, const uint64_t id, const vector<tag> tags )
{
    auto itr = _relation.find( id );
    check( itr != _relation.end(), "relation does not exist" );
    consume_modify_tags( owner, itr->tags.size(), tags.size() );
    _relation.modify( itr, get_self(), [&]( auto & row ) {
        row.tags = tags;
    });
}

void xy::check_tags( const vector<tag> tags )
{
    check( tags.size() <= 255, "[tags] cannot have more than 255 elements");

    std::set<name> duplicates{};
    for ( auto const tag : tags ) {
        check(duplicates.insert(tag.k).second, "[tag.k] all key names must be unique");
        check_tag( tag );
    }
}

void xy::check_tag( const tag tag )
{
    check( tag.k.length() > 0, "[tag.k] must contain at least 1 character");
    check( tag.v.length() <= 255, "[tag.v] cannot be greater than 255 characters");
}
