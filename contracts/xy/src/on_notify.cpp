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
    extended_symbol relay = _settings.get().relay;
    extended_symbol chain = _settings.get().chain;
    asset rate = _global.get().rate;

    // calculate asset amounts
    int64_t amount = quantity.amount / (rate.amount / pow(10, rate.symbol.precision()) );
    asset convert_quantity = asset{ amount, token.get_symbol() };
    asset relay_quantity = asset{ quantity.amount / 2, quantity.symbol }; // 50% goes to relay liquidity / 50% goes to network operations

    // external actions
    token::issue_action token_issue( token.get_contract(), { get_self(), "active"_n });
    token::transfer_action token_transfer( token.get_contract(), { get_self(), "active"_n });
    token::transfer_action chain_transfer( chain.get_contract(), { get_self(), "active"_n } );

    // issue & transfer <chain>XY token to user
    token_issue.send( get_self(), convert_quantity, "convert");
    token_transfer.send( get_self(), from, convert_quantity, "convert");

    // transfer incoming chain token to relay
    chain_transfer.send( get_self(), relay.get_contract(), relay_quantity, "convert" );
}
