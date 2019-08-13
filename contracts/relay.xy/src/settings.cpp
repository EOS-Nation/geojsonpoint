void relay::init( const extended_symbol chain, const extended_symbol reserve )
{
    require_auth( get_self() );
    check(!_settings.exists(), "relay is already initalized");

    // set max fee
    auto settings = _settings.get_or_default();
    settings.chain = chain;
    settings.reserve = reserve;
    _settings.set(settings, get_self());
}

void relay::update( const bool enabled )
{
    check(_settings.exists(), "relay is not initialized");

    auto settings = _settings.get();
    extended_symbol chain = settings.chain;
    extended_symbol reserve = settings.reserve;

    // check if balance exists of eosio.token & token.xy
    asset balance = token::get_balance( chain.get_contract(), get_self(), chain.get_symbol().code() );
    asset balance_xy = token::get_balance( reserve.get_contract(), get_self(), reserve.get_symbol().code() );
    settings.require_balance = !(balance.amount > 0 && balance_xy.amount > 0);

    if (enabled) check(!settings.require_balance, "relay requires balance before being enabled");

    settings.enabled = enabled;
    _settings.set( settings, get_self() );
}