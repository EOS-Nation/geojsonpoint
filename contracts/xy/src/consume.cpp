void xy::consume_token( name owner, int64_t nodes, int64_t tags, string memo )
{
    check( _settings.exists(), "network is not initialized");
    auto settings = _settings.get();

    // get network <chain>XY token
    symbol sym = settings.token.get_symbol();
    name contract = settings.token.get_contract();

    // calculate required tokens to be consumed
    int64_t amount = calculate_consume( nodes, tags );
    asset quantity = asset{ amount, sym };

    // make sure user has enough tokens in their balance
    asset balance = token::get_balance( contract, owner, sym.code() );
    check( balance.amount >= quantity.amount, "consume requires a minimum balance of " + quantity.to_string() );

    // send consume action from token contract (burns X amount of tokens)
    token::consume_action consume( contract, { get_self(), "active"_n } );
    consume.send( owner, quantity, memo );
}

int64_t xy::calculate_consume( int64_t nodes, int64_t tags )
{
    check( _settings.exists(), "network is not initialized");
    auto settings = _settings.get();

    int64_t nodes_amount = settings.consume_rate_node * nodes; // ex: 1.0000 token per node
    int64_t tags_amount = settings.consume_rate_tag * tags; // 0.1000 token per tag

    return nodes_amount + tags_amount;
}
