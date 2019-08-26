void relay::init( const bool enabled )
{
    auto settings = _settings.get_or_default();

    // check if balance exists of eosio.token & token.xy
    symbol core_symbol = get_core_symbol();
    asset balance = token::get_balance( "eosio.token"_n, get_self(), core_symbol.code() );
    asset balance_xy = token::get_balance( "token.xy"_n, get_self(), symbol_code{"XY"} );
    bool require_balance = !(balance.amount > 0 && balance_xy.amount > 0);

    if (enabled) check(!require_balance, "relay requires balance before being enabled");

    // init settings
    settings.enabled = enabled;
    settings.core_symbol = core_symbol;
    _settings.set( settings, get_self() );
}

symbol relay::get_core_symbol()
{
    eosiosystem::rammarket rammarket( "eosio"_n, "eosio"_n.value );
    auto rammarket_itr = rammarket.find( symbol{"RAMCORE", 4}.raw() );

    // localhost chain doesn't have rammarket table
    if (rammarket_itr == rammarket.end()) return symbol{"EOS", 4};

    return rammarket_itr->quote.balance.symbol;
}