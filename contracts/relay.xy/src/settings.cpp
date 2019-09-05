void relay::init( const bool enabled )
{
    auto settings = _settings.get_or_default();

    // // check if balance exists of eosio.token & token.xy
    // asset balance = token::get_balance( "eosio.token"_n, get_self(), core_symbol.code() );
    // asset balance_xy = token::get_balance( "token.xy"_n, get_self(), symbol_code{"XY"} );
    // bool require_balance = !(balance.amount > 0 && balance_xy.amount > 0);

    // if (enabled) check(!require_balance, "relay requires balance before being enabled");

    // init settings
    settings.enabled = enabled;
    _settings.set( settings, get_self() );
}

void relay::setreserve( const extended_symbol base, const extended_symbol quote )
{
    require_auth( get_self() );

    auto reserves_itr = _reserves.find( base.get_symbol().code().raw() );

    // add new reserve
    if (reserves_itr == _reserves.end()) {
        _reserves.emplace( get_self(), [&]( auto & row ) {
            row.base = base;
            row.quote = quote;
        });
    // modify reserve
    } else {
        _reserves.modify( reserves_itr, get_self(), [&]( auto & row ) {
            row.base = base;
            row.quote = quote;
        });
    }

    // // check if balance exists of eosio.token & token.xy
    // asset balance = token::get_balance( "eosio.token"_n, get_self(), core_symbol.code() );
    // asset balance_xy = token::get_balance( "token.xy"_n, get_self(), symbol_code{"XY"} );
    // bool require_balance = !(balance.amount > 0 && balance_xy.amount > 0);
}
