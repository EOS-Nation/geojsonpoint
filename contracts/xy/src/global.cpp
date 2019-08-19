void xy::set_rate()
{
    auto global = _global.get_or_default();

    eosiosystem::rammarket rammarket( "eosio"_n, "eosio"_n.value );
    auto rammarket_itr = rammarket.find( symbol{"RAMCORE", 4}.raw() );

    // localhost chain doesn't have rammarket table
    if (rammarket_itr == rammarket.end()) {
        global.base = asset{60000000000, symbol{"RAM", 0}};
        global.quote = asset{50000000000, symbol{"EOS", 4}};
    } else {
        global.base = rammarket_itr->base.balance;
        global.quote = rammarket_itr->quote.balance;
    }

    global.rate = calculate_rate(global.quote, global.base);
    _global.set( global, get_self() );
}

asset xy::calculate_rate( const asset quote, const asset base )
{

    symbol sym = _settings.get().token.get_symbol();
    int64_t amount = (quote.amount * 1024) / base.amount;
    asset const rate = asset{amount, sym};

    return rate;
}

uint64_t xy::global_available_primary_key()
{
    auto global = _global.get_or_default();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set(global, get_self());
    return available_primary_key;
}