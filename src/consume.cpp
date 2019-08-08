void xy::consume_token( name owner )
{
    asset quantity = asset{10000, symbol{"XY", 4}};

    asset balance = token::get_balance( TOKEN_CONTRACT, owner, symbol_code{"XY"} );

    check( balance.amount >= quantity.amount, "overdrawn XY token balance" );

    token::consume_action consume(TOKEN_CONTRACT, {get_self(), "active"_n});
    consume.send(owner, quantity);
}
