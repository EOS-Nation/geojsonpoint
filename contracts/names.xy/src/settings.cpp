void names::init( const extended_symbol chain, const name relay )
{
    require_auth( get_self() );
    check(!_settings.exists(), "XY.network::names settings are already initalized");

    check( chain.get_symbol().is_valid(), "chain symbol is invalid" );

    check( is_account( chain.get_contract() ), "chain account does not exist");
    check( is_account( relay ), "relay account does not exist");

    // define settings
    auto settings = _settings.get_or_default();
    settings.chain = chain;
    settings.relay = relay;
    _settings.set( settings, get_self() );
}