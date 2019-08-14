#include <math.h>       /* pow */

[[eosio::on_notify("eosio.token::transfer")]]
void xy::transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // Skip until initialized
    if (!_settings.exists()) return;

    // authenticate incoming `from` account
    require_auth( from );
    token_swap( from, quantity, memo );
}

void xy::token_swap( const name&    from,
                     const asset&   quantity,
                     const string&  memo )
{
    check(_settings.exists(), "XY.network settings is already initialized");
    extended_symbol token = _settings.get().token;
    extended_symbol relay = _settings.get().relay;
    extended_symbol chain = _settings.get().chain;
    asset rate = _global.get().rate;

    // calculate asset amounts
    int64_t amount = quantity.amount / (rate.amount / pow(10, rate.symbol.precision()) );
    asset convert_quantity = asset{ amount, token.get_symbol() };
    asset relay_quantity = asset{ quantity.amount / 2, quantity.symbol }; // 50% goes to relay liquidity & 40% goes to XY.network operations
    asset ref_quantity = asset{ quantity.amount / 10, quantity.symbol }; // 10% goes to referral account

    // external actions
    token::issue_action token_issue( token.get_contract(), { get_self(), "active"_n });
    token::transfer_action token_transfer( token.get_contract(), { get_self(), "active"_n });
    token::transfer_action chain_transfer( chain.get_contract(), { get_self(), "active"_n } );

    // issue & transfer <chain>XY token to user
    token_issue.send( get_self(), convert_quantity, convert_quantity.symbol.code().to_string() + " network utility tokens");
    token_transfer.send( get_self(), from, convert_quantity, convert_quantity.symbol.code().to_string() + " network utility tokens");

    // transfer incoming chain token to relay
    chain_transfer.send( get_self(), relay.get_contract(), relay_quantity, "XY.network liquidity deposit" );

    // include referral if memo is a valid EOSIO account name
    if ( memo.size() > 0 && memo.size() <= 12 && is_account(name{memo}) ) {
        name ref = name{memo};
        check(ref != from, "referral account cannot also be the sender");
        chain_transfer.send( get_self(), ref, ref_quantity, "XY.network referral deposit" );
    }
}