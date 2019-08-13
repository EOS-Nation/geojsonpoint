void xy::set_rate( extended_symbol chain )
{
    auto global = _global.get_or_default();
    global.rate = calculate_rate( chain );
    _global.set( global, get_self() );
}

int64_t xy::get_rammarket( extended_symbol chain )
{
    eosiosystem::rammarket rammarket( "eosio"_n, "eosio"_n.value );
    auto rammarket_itr = rammarket.find( chain.get_symbol().code().raw() );

    // set default price to approx. eosio rammarket
    // localhost chain doesn't have rammarket table
    if (rammarket_itr == rammarket.end()) return 1000;

    const asset base = rammarket_itr->base.balance;
    const asset quote = rammarket_itr->quote.balance;

    return quote.amount / base.amount * 1024;
}

asset xy::calculate_rate( extended_symbol chain )
{
    symbol sym = symbol{chain.get_symbol().code().to_string() + "XY", chain.get_symbol().precision()};
    int64_t rammarket = get_rammarket( chain );
    int64_t amount = rammarket;

    return asset{amount, sym};
}

uint64_t xy::global_available_primary_key()
{
    auto global = _global.get_or_default();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set(global, get_self());
    return available_primary_key;
}