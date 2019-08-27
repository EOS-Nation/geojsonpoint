[[eosio::on_notify("eosio.token::transfer")]]
void names::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // authenticate incoming `from` account
    require_auth( from );

    // Handle account creation & deposits
    name account = parse_memo( memo );
    claim_account( from, account, quantity );
    send_fees( quantity );
}
