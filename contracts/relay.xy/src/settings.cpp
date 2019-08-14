void relay::init( const extended_symbol chain, const extended_symbol reserve, const uint64_t fee )
{
    require_auth( get_self() );

    check( chain.get_symbol().is_valid(), "chain symbol is invalid" );
    check( reserve.get_symbol().is_valid(), "reserve symbol is invalid" );

    check( is_account( chain.get_contract() ), "chain account does not exist");
    check( is_account( reserve.get_contract() ), "reserve account does not exist");

    auto settings = _settings.get_or_default();
    check(fee < settings.max_fee, "fee cannot be higher then maximum fee");

    // define settings
    settings.chain = chain;
    settings.reserve = reserve;
    settings.fee = fee;
    _settings.set(settings, get_self());
}

void relay::update( const bool enabled, const uint64_t fee )
{
    check(_settings.exists(), "relay is not initialized");

    auto settings = _settings.get();
    extended_symbol chain = settings.chain;
    extended_symbol reserve = settings.reserve;

    check(fee < settings.max_fee, "fee cannot be higher then maximum fee");

    // check if balance exists of eosio.token & token.xy
    asset balance = token::get_balance( chain.get_contract(), get_self(), chain.get_symbol().code() );
    asset balance_xy = token::get_balance( reserve.get_contract(), get_self(), reserve.get_symbol().code() );
    settings.require_balance = !(balance.amount > 0 && balance_xy.amount > 0);

    if (enabled) check(!settings.require_balance, "relay requires balance before being enabled");

    // update settings
    settings.enabled = enabled;
    settings.fee = fee;
    _settings.set( settings, get_self() );
}