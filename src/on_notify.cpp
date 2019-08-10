#include <math.h>       /* pow */

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

    asset rate = _global.get_or_default().rate;
    symbol sym = symbol{quantity.symbol.code().to_string() + "XY", 4};
    int64_t amount = quantity.amount / (rate.amount / pow(10, rate.symbol.precision()) );
    asset convert_quantity = asset{amount, sym};

    token::issue_action issue(TOKEN_CONTRACT, {get_self(), "active"_n});
    issue.send(get_self(), convert_quantity, "convert");

    token::transfer_action transfer(TOKEN_CONTRACT, {get_self(), "active"_n});
    transfer.send(get_self(), from, convert_quantity, "convert");
}
