/**
 * ACTION
 */
void names::claimaccount( const name bidder, const name name, const public_key key )
{
    require_auth( bidder );
    new_account( bidder, name, key );
}

void names::purchase_account( const name bidder, const name name, const asset quantity )
{
    // check user already exists or claimed
    check( !is_account( name ), "account name already exist");
    auto claim_itr = _claim.find( name.value );
    check( claim_itr == _claim.end(), "name already claimed");

    // check valid quantity
    auto rates_itr = _rates.find( name.length() );
    check( rates_itr != _rates.end(), "cannot find rates");
    asset amount = rates_itr->amount;
    check( amount.amount == quantity.amount, "invalid quantity, required amount " + amount.to_string() );

    // add user name to claim table
    _claim.emplace( get_self(), [&]( auto & row ) {
        row.name       = name;
        row.bidder     = bidder;
        row.amount     = quantity;
        row.trx_id     = get_trx_id();
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

void names::new_account( const name bidder, const name name, const public_key key )
{
    const eosio::name creator = "xy"_n;

    // claim validation
    check( !is_account( name ), "account name already exist");
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
        row.trx_id     = get_trx_id();
        row.timestamp  = current_time_point();
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
