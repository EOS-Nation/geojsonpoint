void relay::init( const extended_symbol chain, const extended_symbol reserve )
{
    require_auth( get_self() );
    check(!_settings.exists(), "relay is already initalized");

    // set max fee
    auto settings = _settings.get_or_default();
    settings.chain = chain;
    settings.reserve = reserve;
    _settings.set(settings, get_self());

    _reserves.emplace( get_self(), [&]( auto & row ) {
        row.reserve      = chain;
        row.ratio        = 500000;
    });

    // create token.xy reserve
    _reserves.emplace( get_self(), [&]( auto & row ) {
        row.reserve      = reserve;
        row.ratio        = 500000;
    });
}