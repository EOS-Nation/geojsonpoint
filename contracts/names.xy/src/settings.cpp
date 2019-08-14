void names::init( const extended_symbol chain, const extended_symbol token, const extended_symbol relay, const name network )
{
    require_auth( get_self() );

    check( chain.get_symbol().is_valid(), "chain symbol is invalid" );
    check( token.get_symbol().is_valid(), "token symbol is invalid" );
    check( relay.get_symbol().is_valid(), "relay symbol is invalid" );

    check( is_account( chain.get_contract() ), "chain account does not exist");
    check( is_account( token.get_contract() ), "token account does not exist");
    check( is_account( relay.get_contract() ), "relay account does not exist");
    check( is_account( network ), "network account does not exist");

    // define settings
    settings_row settings;
    settings.chain = chain;
    settings.token = token;
    settings.relay = relay;
    settings.network = network;
    _settings.set( settings, get_self() );
}