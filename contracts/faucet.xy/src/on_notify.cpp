#include <math.h>       /* pow */

[[eosio::on_notify("eosio.token::transfer")]]
void faucet::transfer( const name&    from,
                       const name&    to,
                       const asset&   quantity,
                       const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    require_auth( from );
    exchange_token( from, quantity );
    send_fees( quantity );
}

void faucet::exchange_token( const name to, const asset quantity )
{
    // calculate asset amounts
    int64_t amount = quantity.amount / calculate_rate() * 10000;
    asset convert_quantity = asset{ amount, symbol{"XY", 4} };

    // external actions
    token::issue_action token_issue( "token.xy"_n, { "xy"_n, "active"_n });
    token::transfer_action token_xy_transfer( "token.xy"_n, { "xy"_n, "active"_n });
    token::transfer_action token_transfer( "token.xy"_n, { get_self(), "active"_n });

    // issue & transfer XY tokens to user
    token_issue.send( "xy"_n, convert_quantity, "XY.network::faucet.transfer");
    token_xy_transfer.send( "xy"_n, get_self(), convert_quantity, "XY.network::faucet.transfer");
    token_transfer.send( get_self(), to, convert_quantity, "XY.network::faucet.transfer");
}

void faucet::send_fees( const asset quantity )
{
    // external actions
    token::transfer_action chain_transfer( "eosio.token"_n, { get_self(), "active"_n } );

    // send eosio.token to fee.xy
    chain_transfer.send( get_self(), "fee.xy"_n, quantity, "XY.network::faucet.transfer");
}

int64_t faucet::calculate_rate()
{
    eosiosystem::rammarket rammarket( "eosio"_n, "eosio"_n.value );
    auto rammarket_itr = rammarket.find( symbol{"RAMCORE", 4}.raw() );

    // localhost chain doesn't have rammarket table
    if (rammarket_itr == rammarket.end()) return 850;

    asset base = rammarket_itr->base.balance;
    asset quote = rammarket_itr->quote.balance;

    return (quote.amount * 1024) / base.amount;
}