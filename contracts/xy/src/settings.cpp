/**
 * ACTION `init`
 */
void xy::init( extended_symbol chain,
               extended_symbol token,
               extended_symbol relay,
               uint64_t consume_rate_node,
               uint64_t consume_rate_tag )
{
    require_auth( get_self() );
    check(!_settings.exists(), "network settings are already initalized");

    check( chain.get_symbol().is_valid(), "chain symbol is invalid" );
    check( token.get_symbol().is_valid(), "token symbol is invalid" );
    check( relay.get_symbol().is_valid(), "relay symbol is invalid" );

    check( is_account( chain.get_contract() ), "chain account does not exist");
    check( is_account( token.get_contract() ), "token account does not exist");
    check( is_account( relay.get_contract() ), "relay account does not exist");

    // define settings
    auto settings = _settings.get_or_default();
    settings.chain = chain;
    settings.token = token;
    settings.relay = relay;
    settings.consume_rate_node = consume_rate_node;
    settings.consume_rate_tag = consume_rate_tag;
    _settings.set( settings, get_self() );

    // define global rate
    set_rate( chain );
}
