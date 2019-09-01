void xy::consume_token( const name owner, const int64_t nodes, const int64_t tags, const string memo )
{
    // calculate consume rate
    symbol sym = symbol{"XY", 4};
    int64_t amount = calculate_consume( nodes, tags );
    asset quantity = asset{ amount, sym };

    // make sure user has enough tokens in their balance
    asset balance = token::get_balance( "token.xy"_n, owner, sym.code() );
    check( balance.amount >= quantity.amount, "consume requires a minimum balance of " + quantity.to_string() );

    // send consume action from token contract (burns X amount of tokens)
    token::consume_action consume( "token.xy"_n, { get_self(), "active"_n } );
    consume.send( owner, quantity, memo );
}

int64_t xy::calculate_consume( const int64_t nodes, const int64_t tags )
{
    int64_t nodes_amount = 10000 * nodes; // ex: 1.0000 XY token per node
    int64_t tags_amount = 1000 * tags; // 0.1000 XY token per tag

    return nodes_amount + tags_amount;
}

void xy::consume_modify_tags( const name owner, const int64_t before, const int64_t after )
{
    if (after > before) consume_token( owner, 0, after - before, "XY.network::modify" );
}
