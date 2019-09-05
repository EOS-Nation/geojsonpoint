void names::new_account( const eosio::name creator, const eosio::name name, const eosio::public_key key )
{
    // claim validation
    check( !is_account( name ), "account name already exist");

    // Create account with single public key as owner & active
    authority auth = public_key_to_authority( key );
    native::newaccount_action newaccount( "eosio"_n, { creator, "active"_n } );
    newaccount.send( creator, name, auth, auth );

    // localhost does not have RAM
    eosiosystem::rammarket rammarket( "eosio"_n, "eosio"_n.value );
    auto rammarket_itr = rammarket.find( symbol{"RAMCORE", 4}.raw() );

    if (rammarket_itr != rammarket.end()) {
        // buy 3.5kb RAM
        system_contract::buyrambytes_action buyrambytes( "eosio"_n, { creator, "active"_n } );
        buyrambytes.send( creator, name, 1024 * 3.5 );

        // TO-DO delegatebw 0.1 EOS
    }
}

authority names::public_key_to_authority( eosio::public_key const key )
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
