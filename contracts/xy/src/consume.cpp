void xy::consume_token( const name owner, const name uid, const int64_t nodes, const int64_t tags, const string memo )
{
    // do not charge XY tokens if uid is the owner name
    if ( owner == uid ) return;

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

int64_t xy::calculate_consume( int64_t nodes, int64_t tags )
{
    int64_t nodes_amount = 10000 * nodes; // ex: 1.0000 XY token per node
    int64_t tags_amount = 1000 * tags; // 0.1000 XY token per tag

    return nodes_amount + tags_amount;
}
