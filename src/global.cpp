uint64_t geopoint::global_available_primary_key() {
    uint64_t available_primary_key = _global.get_or_default().available_primary_key;
    _global.set(global_row{ available_primary_key + 1 }, get_self());
    return available_primary_key;
}

checksum256 geopoint::get_trx_id() {
    size_t size = transaction_size();
    char buf[size];
    size_t read = read_transaction( buf, size );
    check( size == read, "read_transaction failed");
    return sha256( buf, read );
}

// // generate unique key based on node id + key name
// static uint128_t geopoint::compute_by_id_key( const uint64_t id, const name key ) {
//     return ((uint128_t) key.value) << 64 | id;
// }