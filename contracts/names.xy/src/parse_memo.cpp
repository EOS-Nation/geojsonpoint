vector<string> names::split( const string& str, const string& delim )
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;

    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (token.length() > 0) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

memo_object names::parse_memo( const name from, const string memo )
{
    // {eosio::name "name"},{eosio::public_key "key"},{eosio::name "ref"}
    const vector<string> parts = split( memo, "," );
    check( parts.size() > 0, "invalid memo format - ex: \"{name},{public_key},{referral}\"");
    check( parts.size() > 1, "invalid memo format - public_key is required");

    // name part
    eosio::name name = eosio::name{ parts[0] };
    // check( name.is_valid(), "name is invalid" );
    check( name.suffix() == "xy"_n, "*.xy name suffix is required");
    check( !is_account( name ), "account name already exist");

    // TO-DO handle subdomain accounts
    // waiting for eosio.cdt .prefix() support
    check( split(name.to_string(), ".").size() == 2, "subdomain accounts (*.*.xy) are not yet supported" );

    // public_key part
    eosio::public_key key = abieos::string_to_public_key( parts[1] );

    // optional referral part
    eosio::name ref = eosio::name{""};
    if ( parts.size() > 2 ) {
        ref = eosio::name{ parts[2] };
        // check( ref.is_valid(), "referral name is invalid" );
        check( is_account( ref ), "referral account does not exist");
        check( from != ref, "referral account name cannot also be the sender");
    }

    return memo_object{ name, key, ref };
}
