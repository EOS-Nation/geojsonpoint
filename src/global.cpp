uint64_t xy::global_available_primary_key() {
    uint64_t available_primary_key = _global.get_or_default().available_primary_key;
    _global.set(global_row{ available_primary_key + 1 }, get_self());
    return available_primary_key;
}
