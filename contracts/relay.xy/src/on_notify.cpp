[[eosio::on_notify("eosio.token::transfer")]]
void relay::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // Prevent token convert from `xy`
    if ( from == "xy"_n || from == "names.xy"_n ) return;

    // Prevent token convert by memo
    if ( memo == "init" ) return;

    // Skip until initialized
    if (!_settings.exists()) return;

    // authenticate incoming `from` account
    require_auth( from );

    auto settings = _settings.get();
    extended_symbol chain = settings.chain;
    extended_symbol reserve = settings.reserve;

    check(settings.enabled, "relay is not enabled");

    asset balance_from = token::get_balance( chain.get_contract(), get_self(), chain.get_symbol().code() );
    asset balance_to = token::get_balance( reserve.get_contract(), get_self(), reserve.get_symbol().code() );
    const int64_t amount = static_cast<int64_t>(bancor_formula( balance_from.amount, balance_to.amount, quantity.amount ));

    const asset convert_quantity = asset{amount, reserve.get_symbol()};

    token::transfer_action transfer(reserve.get_contract(), { get_self(), "active"_n });
    transfer.send( get_self(), from, convert_quantity, "convert");
}

[[eosio::on_notify("token.xy::transfer")]]
void relay::transfer_xy( const name&    from,
                         const name&    to,
                         const asset&   quantity,
                         const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // Prevent token convert from `xy`
    if ( from == "xy"_n ) return;

    // Prevent token convert by memo
    if ( memo == "init" ) return;

    // Skip until initialized
    if (!_settings.exists()) return;

    // authenticate incoming `from` account
    require_auth( from );

    check(_settings.exists(), "relay is not initialized");

    auto settings = _settings.get();
    extended_symbol chain = settings.chain;
    extended_symbol reserve = settings.reserve;

    check(settings.enabled, "relay is not enabled");

    asset balance_from = token::get_balance( reserve.get_contract(), get_self(), reserve.get_symbol().code() );
    asset balance_to = token::get_balance( chain.get_contract(), get_self(), chain.get_symbol().code() );
    const int64_t amount = static_cast<int64_t>(bancor_formula( balance_from.amount, balance_to.amount, quantity.amount ));

    const asset convert_quantity = asset{amount, chain.get_symbol()};

    token::transfer_action transfer(chain.get_contract(), { get_self(), "active"_n });
    transfer.send( get_self(), from, convert_quantity, "convert");
}