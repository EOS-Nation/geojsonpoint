[[eosio::on_notify("eosio.token::transfer")]]
void names::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // authenticate incoming `from` account
    require_auth( from );

    check(_settings.exists(), "XY.network::names is not initialized");

    // settings
    auto settings = _settings.get();
    extended_symbol chain = settings.chain;
    name relay = settings.relay;

    // calculate asset amounts
    asset relay_quantity = asset{ quantity.amount / 2, quantity.symbol }; // 50% goes to relay liquidity & 40% goes to XY.network::names operations
    asset ref_quantity = asset{ quantity.amount / 10, quantity.symbol }; // 10% goes to referral account

    // external actions
    token::transfer_action transfer( chain.get_contract(), { get_self(), "active"_n } );

    // transfer incoming chain token to relay
    transfer.send( get_self(), relay, relay_quantity, "XY.network::names liquidity deposit" );

    // include referral if memo is a valid EOSIO account name
    if ( memo.size() > 0 && memo.size() <= 12 && is_account(name{memo}) ) {
        name ref = name{memo};
        check(ref != from, "referral account cannot also be the sender");
        transfer.send( get_self(), ref, ref_quantity, "XY.network::names referral deposit" );
    }
}
