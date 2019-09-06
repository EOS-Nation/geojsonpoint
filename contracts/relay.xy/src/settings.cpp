void relay::enable( const bool enabled )
{
    auto settings = _settings.get_or_default();
    settings.enabled = enabled;
    _settings.set( settings, get_self() );
}

void relay::setreserve( const extended_symbol base, const extended_symbol quote )
{
    require_auth( get_self() );

    auto index = _reserves.get_index<"bysymbols"_n>();
    uint128_t key = symbols_key( base.get_symbol().code(), quote.get_symbol().code() );
    auto reserves_itr = index.find( key );

    // check if balance exists of eosio.token & token.xy
    asset base_balance = token::get_balance( base.get_contract(), get_self(), base.get_symbol().code() );
    asset quote_balance = token::get_balance( quote.get_contract(), get_self(), quote.get_symbol().code() );

    check( base_balance.amount > 0, "missing base balance");
    check( base_balance.amount > 0, "missing quote balance");

    // add new reserve
    if ( reserves_itr == index.end() ) {
        _reserves.emplace( get_self(), [&]( auto & row ) {
            row.id = _reserves.available_primary_key();
            row.base = base;
            row.quote = quote;
        });
    // modify reserve
    } else {
        index.modify( reserves_itr, get_self(), [&]( auto & row ) {
            row.base = base;
            row.quote = quote;
        });
    }
}
