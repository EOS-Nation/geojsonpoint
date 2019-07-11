// void geopoint::updatetag(
//     const name          user,
//     const uint64_t      id,
//     const tag           tag
// ) {
//     require_auth( user );
//     modify_tag( tag_id, key, value );
//     update_node_version( user, id );
// }

void geopoint::replacetags(
    const name            user,
    const uint64_t        id,
    const vector<tag>     tags
) {
    require_auth( user );
    delete_tags( id );
    create_tags( id, tags );
    update_node_version( user, id );
}

void geopoint::deletetags( name user, uint64_t id ) {
    require_auth( user );
    delete_tags( id );
    update_node_version( user, id );
}

void geopoint::deletetag( name user, uint64_t id, name k ) {
    require_auth( user );
    update_node_version( user, id );
}

void geopoint::delete_tags( uint64_t id ) {
    check_node_exists( id );

    auto tag_index = _tag.get_index<"byid"_n>();
    auto tag_itr = tag_index.find( id );

    while ( tag_itr != tag_index.end() ) {
        tag_itr = tag_index.erase( tag_itr );
    }
}

void geopoint::delete_tag( uint64_t id, name k ) {
    // check_tag_exists( tag_id );

    // auto tag_itr = _tag.find( tag_id );
    // _tag.erase( tag_itr );
}

void geopoint::create_tags( uint64_t id, vector<tag> tags ) {
    check( tags.size() <= 255, "[tags] cannot have more than 255 elements");

    for ( auto const tag : tags ) {
        create_tag( id, tag );
    }
}

void geopoint::create_tag( uint64_t id, tag tag ) {
    check_tag( tag );

    uint64_t tag_id = _tag.available_primary_key();
    _tag.emplace( _self, [&]( auto & row ) {
        row.tag_id     = tag_id;
        row.id         = id;
        row.k          = tag.k;
        row.v          = tag.v;
    });
}

// void geopoint::modify_tag( uint64_t id, tag tag ) {
//     check_tag( tag tag );

//     auto tag_itr = _tag.find( tag_id );
//     _tag.modify( tag_itr, _self, [&](auto & row){
//         row.tag = tag;
//     });
//     return tag_itr->id;
// }

void geopoint::check_tag( tag tag ) {
    check( tag.k.length() > 0, "[tag.k] must contain at least 1 character");
    check( tag.k.length() <= 255, "[tag.k] cannot be greater than 255 characters");
    check( tag.v.length() <= 255, "[tag.v] cannot be greater than 255 characters");
}

bool geopoint::tag_exists( uint64_t tag_id ) {
    auto tag_itr = _tag.find( tag_id );
    return tag_itr != _tag.end();
}

void geopoint::check_tag_exists( uint64_t tag_id ) {
    check( tag_exists( tag_id ), "[tag_id] no matching results" );
}

// uint128_t geopoint::compute_by_id_key(const uint64_t id, const name key) {
//     return ((uint128_t) key.value) << 64 | id;
// }