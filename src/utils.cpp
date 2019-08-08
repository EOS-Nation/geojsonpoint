#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>

checksum256 xy::get_trx_id() {
    size_t size = transaction_size();
    char buf[size];
    size_t read = read_transaction( buf, size );
    check( size == read, "read_transaction failed");
    return sha256( buf, read );
}

uint64_t xy::global_available_primary_key() {
    uint64_t available_primary_key = _global.get_or_default().available_primary_key;
    _global.set(global_row{ available_primary_key + 1 }, get_self());
    return available_primary_key;
}

name xy::get_owner( uint64_t id ) {
    if (node_exists( id )) {
        auto node_itr = _node.find( id );
        return node_itr->owner;
    }
    else if (way_exists( id )) {
        auto way_itr = _way.find( id );
        return way_itr->owner;
    }
    else if (relation_exists( id )) {
        auto relation_itr = _relation.find( id );
        return relation_itr->owner;
    }
    else {
        check(false, "[id] owner not found");
        return ""_n;
    }
}

void xy::check_owner( name user, uint64_t id ) {
    check( get_owner(id) == user, "[user] does not match id owner");
}

void xy::update_version( uint64_t id ) {
    time_point_sec timestamp = current_time_point();
    checksum256 changeset = get_trx_id();

    if (node_exists( id )) {
        auto node_itr = _node.find( id );
        _node.modify( node_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if (way_exists( id )) {
        auto way_itr = _way.find( id );
        _way.modify( way_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else if (relation_exists( id )) {
        auto relation_itr = _relation.find( id );
        _relation.modify( relation_itr, get_self(), [&](auto & row) {
            row.version     = row.version + 1;
            row.timestamp   = timestamp;
            row.changeset   = changeset;
        });
    }
    else {
        check(false, "[id] id not found");
    }
}