void relay::accountfee( const name account )
{
    require_auth( get_self() );

    auto settings = _settings.get_or_default();

    check( is_account( account ), "account does not exist");

    settings.account_fee = account;
    _settings.set( settings, get_self() );
}

void relay::enable( const bool enabled )
{
    require_auth( get_self() );

    auto settings = _settings.get_or_default();
    settings.enabled = enabled;
    _settings.set( settings, get_self() );
}

void relay::setreserve( const extended_symbol base, const extended_symbol quote, const uint64_t fee )
{
    require_auth( get_self() );
    check( fee <= _settings.get_or_default().max_fee, "fee cannot exceed maximum fee");

    auto index = _reserves.get_index<"bysymbols"_n>();
    uint128_t key = symbols_key( base.get_symbol().code(), quote.get_symbol().code() );
    auto reserves_itr = index.find( key );

    // TO-DO support other precisions
    check( base.get_symbol().precision() == 4, "only reserves with 4 precision are currently supported");
    check( quote.get_symbol().precision() == 4, "only reserves with 4 precision are currently supported");

    // check if balance exists of base & quote
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
            row.fee = fee;
        });
    // modify reserve
    } else {
        index.modify( reserves_itr, get_self(), [&]( auto & row ) {
            row.base = base;
            row.quote = quote;
            row.fee = fee;
        });
    }
}
