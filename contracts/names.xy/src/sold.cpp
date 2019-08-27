void names::newaccount( const name bidder, const name name, const public_key key )
{
    require_auth( bidder );
    new_account( bidder, name, key );
}

void names::new_account( const name bidder, const name name, const public_key key )
{
    const eosio::name creator = "xy"_n;

    // claim validation
    auto claim_itr = _claim.find( name.value );
    check( claim_itr != _claim.end(), "account has not yet been claimed, transfer is first required");
    check( claim_itr->bidder == bidder, "bidder does not match claimed account");

    // Create account with single public key as owner & active
    authority auth = public_key_to_authority( key );
    native::newaccount_action newaccount( "eosio"_n, { creator, "active"_n } );
    newaccount.send( creator, name, auth, auth );

    // // buy 3.5kb RAM
    // system_contract::buyrambytes_action buyrambytes( "eosio"_n, { creator, "active"_n } );
    // buyrambytes.send( creator, name, 1024 * 3.5 );

    // move entry from claim => sold
    _sold.emplace( get_self(), [&]( auto & row ) {
        row.name       = name;
        row.bidder     = bidder;
        row.amount     = claim_itr->amount;
        row.timestamp  = current_time_point();
        row.trx_id     = get_trx_id();
    });

    // delete claim
    _claim.erase( claim_itr );
}

authority names::public_key_to_authority( public_key const key )
{
    authority auth;

    vector<key_weight> keys;
    vector<permission_level_weight> accounts;
    vector<wait_weight> waits;

    // add single key to authority
    keys.push_back(key_weight{key, 1});

    auth.threshold = 1;
    auth.keys = keys;
    auth.accounts = accounts;
    auth.waits = waits;

    return auth;
}
