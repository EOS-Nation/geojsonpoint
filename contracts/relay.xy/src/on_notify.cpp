#include <math.h>       /* round, floor, ceil, trunc */

[[eosio::on_notify("*::transfer")]]
void relay::transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    // Only monitor incoming transfers to get_self() account
    if ( to != get_self() ) return;

    // Prevent token convert by memo
    if ( memo == "init" ) return;

    // authenticate incoming `from` account
    require_auth( from );

    // settings
    check( _settings.exists(), "relay is not initialized");
    check( _settings.get().enabled, "relay is not enabled");

    convert( from, quantity, memo );
}

void relay::convert( const name to,
                     const asset quantity,
                     const string memo )
{
    auto settings = _settings.get_or_default();

    // get reserves
    auto index = _reserves.get_index<"bysymbols"_n>();
    uint128_t key = symbols_key( quantity.symbol.code(), parse_memo( memo ) );
    auto reserve = index.get( key, "symbol code does not match any reserves");

    // check if incoming quantity contract matches reserve base contract
    check(reserve.base.get_contract() == get_first_receiver(), "invalid base quantity contract");

    // Calculate fee from incoming asset (round up)
    asset fee = quantity;
    fee.amount = ceil((static_cast<double>(quantity.amount) * (settings.fee)) / 100000);

    // Calculate convert amount
    asset balance_from = token::get_balance( reserve.base.get_contract(), get_self(), reserve.base.get_symbol().code() );
    asset balance_to = token::get_balance( reserve.quote.get_contract(), get_self(), reserve.quote.get_symbol().code() );
    const int64_t amount = static_cast<int64_t>(bancor_formula( balance_from.amount, balance_to.amount, quantity.amount - fee.amount ));

    // assets
    const asset quantity_convert = asset{ amount, reserve.quote.get_symbol() };

    // validation
    check( amount > 0, "must transfer a higher quantity");
    check( quantity.amount <= balance_from.amount - quantity.amount, "must transfer a lower quantity");

    // Send transfers
    token::transfer_action transfer( reserve.quote.get_contract(), { get_self(), "active"_n });
    transfer.send( get_self(), to, quantity_convert, "convert");

    if ( fee.amount && settings.fee_account ) {
        token::transfer_action transfer_fee( reserve.base.get_contract(), { get_self(), "active"_n } );
        transfer_fee.send( get_self(), settings.fee_account, fee, "convert.fee");
    }
}

symbol_code relay::parse_memo( const string memo )
{
    check( memo.size() > 0, "memo requires symbol code - ex: \"EOS\"");
    auto sym = symbol_code{ memo };
    check( sym.is_valid(), "symbol code is not valid");
    return sym;
}
