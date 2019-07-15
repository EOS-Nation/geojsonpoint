void geopoint::updatetags(
    const name            user,
    const uint64_t        id,
    const vector<tag>     tags
) {
    require_auth( user );
    update_tags( id, tags );
    update_node_version( user, id );
}

void geopoint::replacetags(
    const name            user,
    const uint64_t        id,
    const vector<tag>     tags
) {
    require_auth( user );
    erase_tags( id );
    emplace_tags( id, tags );
    update_node_version( user, id );
}

void geopoint::erasetags( name user, uint64_t id ) {
    require_auth( user );
    erase_tags( id );
    update_node_version( user, id );
}

void geopoint::erasekeys( name user, uint64_t id, vector<name> keys ) {
    require_auth( user );
    erase_keys( id, keys );
    update_node_version( user, id );
}

void geopoint::emplace_tags( uint64_t id, vector<tag> tags ) {
    check( tags.size() <= 255, "[tags] cannot have more than 255 elements");

    for ( auto const tag : tags ) {
        emplace_tag( id, tag );
    }
}

void geopoint::update_tags( uint64_t id, vector<tag> tags ) {
    for ( auto const tag : tags ) {
        emplace_tag( id, tag );
    }
}

void geopoint::update_tag( uint64_t id, tag tag ) {
    auto key_id = compute_by_id_key( id, tag.key );

    if ( tag_exists( key_id ) ) {
        modify_tag( id, tag );
    } else {
        emplace_tag( id, tag );
    }
}

void geopoint::emplace_tag( uint64_t id, tag tag ) {
    check_tag( tag );

    uint64_t tag_id = _tag.available_primary_key();
    _tag.emplace( _self, [&]( auto & row ) {
        row.tag_id     = tag_id;
        row.id         = id;
        row.key        = tag.key;
        row.value      = tag.value;
    });
}

void geopoint::modify_tag( uint64_t id, tag tag ) {
    auto key_id = compute_by_id_key( id, tag.key );
    check_tag_exists( key_id );
    check_tag( tag );

    auto tag_index = _tag.get_index<"bykey"_n>();
    auto tag_itr = tag_index.find( key_id );

    tag_index.modify( tag_itr, _self, [&](auto & row) {
        row.key = tag.key;
        row.value = tag.value;
    });
}

void geopoint::erase_tags( uint64_t id ) {
    check_node_exists( id );

    auto tag_index = _tag.get_index<"byid"_n>();
    auto tag_itr = tag_index.find( id );

    while ( tag_itr != tag_index.end() ) {
        tag_itr = tag_index.erase( tag_itr );
    }
}

void geopoint::erase_keys( uint64_t id, vector<name> keys ) {
    check_node_exists( id );
    check( keys.size() <= 255, "[keys] cannot have more than 255 elements");

    for ( auto const key : keys ) {
        erase_key( id, key );
    }
}

void geopoint::erase_key( uint64_t id, name key ) {
    auto key_id = compute_by_id_key( id, key );
    check_tag_exists( key_id );

    auto tag_index = _tag.get_index<"bykey"_n>();
    auto tag_itr = tag_index.find( key_id );
    tag_index.erase( tag_itr );
}

void geopoint::check_tag( tag tag ) {
    check( tag.key.length() > 0, "[tag.key] must contain at least 1 character");
    check( tag.key.length() <= 255, "[tag.key] cannot be greater than 255 characters");
    check( tag.value.length() <= 255, "[tag.value] cannot be greater than 255 characters");
}

bool geopoint::tag_exists( uint128_t key_id ) {
    auto tag_index = _tag.get_index<"bykey"_n>();
    auto tag_itr = tag_index.find( key_id );
    return tag_itr != tag_index.end();
}

void geopoint::check_tag_exists( uint128_t key_id ) {
    check( tag_exists( key_id ), "[key_id] no matching results" );
}
