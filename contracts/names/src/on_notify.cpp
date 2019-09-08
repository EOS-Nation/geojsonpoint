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

    // parse memo
    auto object = parse_memo( from, memo );

    // Handle account creation & deposits
    new_account( "xy"_n, object.name, object.key );
    purchase_account( object.name, from, object.ref, quantity );
    send_fees( from, object.ref, quantity );
}

void names::purchase_account( const eosio::name name, const eosio::name bidder, const eosio::name ref, const eosio::asset quantity )
{
    const rates_row rates = _rates.get( name.length(), "cannot find rates" );

    // check valid quantity
    check( rates.amount.amount == quantity.amount, "invalid quantity, requires exact amount of " + rates.amount.to_string() );

    check( _sold.find( name.value ) == _sold.end(), "name already exists");

    // add user name to claim table
    _sold.emplace( get_self(), [&]( auto & row ) {
        row.name       = name;
        row.bidder     = bidder;
        row.ref        = ref;
        row.amount     = quantity;
        row.trx_id     = eosiosystem::get_trx_id();
        row.timestamp  = current_time_point();
    });
}

void names::send_fees( const eosio::name from, const eosio::name ref, const eosio::asset quantity )
{
    // external actions
    token::transfer_action transfer( "eosio.token"_n, { get_self(), "active"_n } );

    // using referral
    if (ref.length() > 0) {
        const symbol sym = quantity.symbol;
        const asset quantity_ref = asset{ quantity.amount / 10, sym };
        const asset quantity_fee = asset{ quantity.amount - quantity_ref.amount, sym };

        transfer.send( get_self(), "fee.xy"_n, quantity_fee, "XY.network::names");
        transfer.send( get_self(), ref, quantity_ref, "XY.network::names.referral");
    } else {
        transfer.send( get_self(), "fee.xy"_n, quantity, "XY.network::names");
    }
}