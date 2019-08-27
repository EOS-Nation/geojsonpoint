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

// TO-DO - NOT WORKING
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