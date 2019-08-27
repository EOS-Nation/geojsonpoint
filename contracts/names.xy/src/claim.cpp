void names::claim_account( const name bidder, const name name, const asset quantity )
{
    check( !is_account( name ), "account name already exist");
    auto claim_itr = _claim.find( name.value );
    check( claim_itr == _claim.end(), "name already claimed");

    auto rates_itr = _rates.find( name.length() );

    _claim.emplace( get_self(), [&]( auto & row ) {
        row.name       = name;
        row.bidder     = bidder;
        row.amount     = quantity;
        row.timestamp  = current_time_point();
    });
}

void names::send_fees( const asset quantity )
{
    // external actions
    token::transfer_action transfer( "eosio.token"_n, { get_self(), "active"_n } );

    // send eosio.token to fee.xy
    transfer.send( get_self(), "fee.xy"_n, quantity, "XY.network::names.transfer");
}

name names::parse_memo( const string memo )
{
    check( memo.length() > 0 && memo.length() <= 12, "memo must include a valid EOSIO account name");
    check( memo.length() >= 4, "account name must be 4 or greater in length");
    name account = name{memo};
    check( account.suffix() == "xy"_n, "*.xy name suffix is required");

    return account;
}
