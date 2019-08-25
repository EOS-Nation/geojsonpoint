[[eosio::on_notify("eosio.token::transfer")]]
void relay::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // Prevent token convert from `fee.xy`
    if ( from == "fee.xy"_n ) return;

    // Prevent token convert by memo
    if ( memo == "init" ) return;

    // authenticate incoming `from` account
    require_auth( from );

    // settings
    auto settings = _settings.get_or_default();
    check(settings.enabled, "relay is not enabled");

    // convert
    extended_symbol base = extended_symbol{settings.core_symbol, "eosio.token"_n};
    extended_symbol quote = extended_symbol{symbol{"XY", 4}, "token.xy"_n};
    convert( from, quantity, base, quote, settings.fee );
}

[[eosio::on_notify("token.xy::transfer")]]
void relay::transfer_xy( const name&    from,
                         const name&    to,
                         const asset&   quantity,
                         const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // Prevent token convert from `fee.xy`
    if ( from == "fee.xy"_n ) return;

    // Prevent token convert by memo
    if ( memo == "init" ) return;

    // authenticate incoming `from` account
    require_auth( from );

    // settings
    auto settings = _settings.get_or_default();
    check(settings.enabled, "relay is not enabled");

    // convert
    extended_symbol base = extended_symbol{symbol{"XY", 4}, "token.xy"_n};
    extended_symbol quote = extended_symbol{settings.core_symbol, "eosio.token"_n};
    convert( from, quantity, base, quote, settings.fee );
}

void relay::convert( const name to,
                     const asset quantity,
                     const extended_symbol base,
                     const extended_symbol quote,
                     const int64_t fee )
{
    // Calculate convert amount
    asset balance_from = token::get_balance( base.get_contract(), get_self(), base.get_symbol().code() );
    asset balance_to = token::get_balance( quote.get_contract(), get_self(), quote.get_symbol().code() );
    const int64_t amount = static_cast<int64_t>(bancor_formula( balance_from.amount, balance_to.amount, quantity.amount ));

    check( amount > 0, "must transfer a higher quantity amount");
    check( quantity.amount < balance_from.amount, "must transfer a lower quantity amount");

    // Calculate fee
    const int64_t amount_fee = amount * fee / 1000000;
    const asset quantity_convert = asset{ amount - amount_fee, quote.get_symbol() };
    const asset quantity_fee = asset{ amount_fee, quote.get_symbol() };

    // Send transfers
    token::transfer_action transfer(quote.get_contract(), { get_self(), "active"_n });
    transfer.send( get_self(), to, quantity_convert, "XY.network::relay.transfer");

    if ( fee ) {
        check( amount_fee > 0, "must transfer a higher quantity amount to cover relay fee");
        transfer.send( get_self(), "fee.xy"_n, quantity_fee, "XY.network::relay.transfer");
    }
}