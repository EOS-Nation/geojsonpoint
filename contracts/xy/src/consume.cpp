void xy::consume_token( name owner, int64_t nodes, int64_t tags, string memo )
{
    check( _settings.exists(), "network is not initialized");
    auto settings = _settings.get();
    extended_symbol token = settings.token;
    extended_symbol relay = settings.relay;

    // calculate required tokens to be consumed
    set_rate( settings.chain );
    int64_t amount = calculate_consume( nodes, tags );
    asset consume_quantity = asset{ amount, token.get_symbol() };
    asset relay_quantity = asset{ amount, relay.get_symbol() };

    // make sure user has enough tokens in their balance
    asset balance = token::get_balance( token.get_contract(), owner, token.get_symbol().code() );
    check( balance.amount >= consume_quantity.amount, "consume requires a minimum balance of " + consume_quantity.to_string() );

    // send consume action from token contract (burns X amount of tokens)
    token::consume_action consume( token.get_contract(), { get_self(), "active"_n } );
    consume.send( owner, consume_quantity, memo );

    token::issue_action issue( token.get_contract(), { get_self(), "active"_n } );
    issue.send( get_self(), relay_quantity, memo );
}

int64_t xy::calculate_consume( int64_t nodes, int64_t tags )
{
    check( _settings.exists(), "network is not initialized");
    auto settings = _settings.get();

    int64_t nodes_amount = settings.consume_rate_node * nodes; // ex: 1.0000 token per node
    int64_t tags_amount = settings.consume_rate_tag * tags; // 0.1000 token per tag

    return nodes_amount + tags_amount;
}
