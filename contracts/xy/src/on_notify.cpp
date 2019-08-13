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

    check(_settings.exists(), "network is not initialized");
    extended_symbol token = _settings.get().token;
    asset rate = _global.get().rate;

    int64_t amount = quantity.amount / (rate.amount / pow(10, rate.symbol.precision()) );
    asset convert_quantity = asset{amount, token.get_symbol()};

    token::issue_action issue(token.get_contract(), { get_self(), "active"_n });
    issue.send(get_self(), convert_quantity, "convert");

    token::transfer_action transfer(token.get_contract(), { get_self(), "active"_n });
    transfer.send(get_self(), from, convert_quantity, "convert");
}
