[[eosio::on_notify("eosio.token::transfer")]]
void xy::transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // authenticate incoming `from` account
    require_auth( from );

    asset swap_quantity = asset{10000, symbol{"XY", 4}};

    token::issue_action issue(TOKEN_CONTRACT, {get_self(), "active"_n});
    issue.send(get_self(), swap_quantity, "token swap");

    token::transfer_action transfer(TOKEN_CONTRACT, {get_self(), "active"_n});
    transfer.send(get_self(), from, swap_quantity, "token swap");
}