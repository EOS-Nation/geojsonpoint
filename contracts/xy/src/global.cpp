/**
 * ACTION setrate
 */
void xy::setrate( symbol chain )
{
    require_auth( get_self() );
    check( chain.is_valid(), "[chain] invalid symbol" );

    auto global = _global.get_or_default();
    if (_global.exists()) check( chain == global.chain, "[chain] symbol is mismatched");

    global.chain = chain;
    global.rate = calculate_rate( chain );

    _global.set(global, get_self());

    deferred_setrate( chain );
}

void xy::deferred_setrate( symbol chain )
{
    eosio::transaction deferred;

    cancel_deferred(get_self().value);

    deferred.actions.emplace_back(
        permission_level{ get_self(), "active"_n },
        get_self(), "setrate"_n,
        std::make_tuple( chain )
    );

    deferred.expiration = time_point_sec(now() + 660);
    deferred.delay_sec = 600;

    deferred.send(get_self().value, get_self());
}

int64_t xy::get_rammarket( symbol chain )
{
    eosiosystem::rammarket rammarket( "eosio"_n, "eosio"_n.value );
    auto rammarket_itr = rammarket.find( chain.code().raw() );

    if (rammarket_itr == rammarket.end()) return 1000; // set default price to approx. eosio rammarket

    const asset base = rammarket_itr->base.balance;
    const asset quote = rammarket_itr->quote.balance;

    return quote.amount / base.amount * 1024;
}

asset xy::calculate_rate( symbol chain )
{
    symbol sym = symbol{chain.code().to_string() + "XY", chain.precision()};
    int64_t rammarket = get_rammarket( chain );
    int64_t amount = rammarket;

    return asset{amount, sym};
}

void xy::consume_token( name owner, int64_t points, int64_t tags, string memo )
{
    // get required tokens to be consumed
    symbol sym  = _global.get().rate.symbol;
    int64_t amount = calculate_consume( points, tags );
    asset quantity = asset{ amount, sym };

    // make sure user has enough tokens in their balance
    asset balance = token::get_balance( TOKEN_CONTRACT, owner, sym.code() );
    check( balance.amount >= quantity.amount, "consume requires a minimum balance of " + quantity.to_string() );

    // send consume action from token contract (burns X amount of tokens)
    token::consume_action consume( TOKEN_CONTRACT, { get_self(), "active"_n } );
    consume.send( owner, quantity, memo );
}

int64_t xy::calculate_consume( int64_t points, int64_t tags )
{
    int64_t points_amount = 10000 * points; // 1 token per node
    int64_t tags_amount = 1000 * tags; // 0.1 token per tag

    return points_amount + tags_amount;
}

uint64_t xy::global_available_primary_key()
{
    check(_global.exists(), "`setrate` must first be executed");

    auto global = _global.get();
    uint64_t available_primary_key = global.available_primary_key;
    global.available_primary_key += 1;
    _global.set(global, get_self());
    return available_primary_key;
}