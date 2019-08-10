uint64_t xy::global_available_primary_key() {
    auto global = _global.get_or_default();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set(global, get_self());
    return available_primary_key;
}

/**
 * ACTION setrate
 */
void xy::setrate( asset rate )
{
    require_auth( get_self() );

    auto sym_code = rate.symbol.code();
    const asset supply = token::get_supply( TOKEN_CONTRACT, sym_code );

    check( rate.is_valid(), "invalid rate" );
    check( rate.amount > 0, "must set rate positive rate" );
    check( rate.symbol == supply.symbol, "symbol precision mismatch" );

    auto global = _global.get_or_default();
    global.rate = rate;
    _global.set(global, get_self());
}
