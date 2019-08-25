void names::create_account( name creator, name name, public_key key )
{
    symbol sym = _settings.get().chain.get_symbol();

    authority auth = public_key_to_authority( key );
    native::newaccount_action newaccount( "eosio"_n, { creator, "active"_n } );
    newaccount.send( creator, name, auth, auth );

    system_contract::delegatebw_action delegatebw( "eosio"_n, { creator, "active"_n } );
    delegatebw.send( creator, name, asset{ 1000, sym }, asset{ 5000, sym }, true );

    system_contract::buyrambytes_action buyrambytes( "eosio"_n, { creator, "active"_n } );
    buyrambytes.send( creator, name, 1024 * 4 );
}

void names::key( eosio::public_key key )
{
    // // trim EOS from key
    // if (key.find("EOS") != string::npos) key.replace(0, 3, "");
    // print(key, "\n");
    // eosio::public_key pub = string_to_public_key( 1, key );
    print( std::string(key.data.begin(), key.data.end()), "\n" );
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

eosio::public_key names::string_to_public_key( unsigned int const key_type, string public_key_str )
{
    // remove EOS suffix if present
    if (public_key_str.find("EOS") != string::npos) public_key_str.replace(0, 3, "");

    eosio::public_key public_key;
    public_key.type = key_type; // Could be K1 or R1 enum
    for(int i = 0; i < 33; ++i)
    {
        public_key.data.at(i) = public_key_str.at(i);
    }
    return public_key;
}

vector<string> names::split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;

    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

memo_format names::parse_memo( const string memo )
{
    // split memo into multiple parts
    vector<string> memo_parts = split( memo, "," );

    // Error handling
    check(memo_parts.size() >= 2, "invalid memo format \"<ACCOUNT>,<EOSIO PUBLIC KEY>,<?REF>\"");
    check(name{memo_parts[0]}.suffix() == "xy"_n, "invalid suffix \"foo.xy\"");
    check(memo_parts[1].length() == 53, "invalid EOSIO public key");

    // parse each part
    name newaccount = name{memo_parts[0]};
    public_key key = string_to_public_key( 0, memo_parts[1]);
    name ref = ""_n;

    // referral account optional
    if (memo_parts.size() >= 3) {
        ref = name{memo_parts[2]};
        check( is_account(ref), "ref account does not exist");
    }

    return memo_format{newaccount, key, ref};
}