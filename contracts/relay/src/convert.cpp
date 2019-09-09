void relay::convert( const name to,
                     const asset quantity,
                     const string memo )
{
    auto settings = _settings.get_or_default();

    // get reserves
    auto index = _reserves.get_index<"bysymbols"_n>();
    uint128_t key = symbols_key( quantity.symbol.code(), parse_memo( memo ) );
    auto reserve = index.get( key, "symbol code does not match any reserves");
    extended_symbol base = reserve.base;
    extended_symbol quote = reserve.quote;

    // check if incoming quantity contract matches reserve base contract
    check( base.get_contract() == get_first_receiver(), "invalid base quantity contract");

    // Calculate fee from incoming asset (round up)
    asset fee = quantity;
    fee.amount = ceil((static_cast<double>(quantity.amount) * (reserve.fee)) / 10000);

    // Calculate convert amount
    asset balance_from = token::get_balance( base.get_contract(), get_self(), base.get_symbol().code() );
    asset balance_to = token::get_balance( quote.get_contract(), get_self(), quote.get_symbol().code() );
    const int64_t amount = static_cast<int64_t>(bancor::convert( balance_from.amount, balance_to.amount, quantity.amount - fee.amount ));

    // assets
    const asset quantity_convert = asset{ amount, quote.get_symbol() };

    // validation
    check( amount > 0, "must transfer a higher quantity");
    check( quantity.amount <= balance_from.amount - quantity.amount, "must transfer a lower quantity");

    // Send transfers
    token::transfer_action transfer( quote.get_contract(), { get_self(), "active"_n });
    transfer.send( get_self(), to, quantity_convert, "convert");

    // send fee if exists
    if ( fee.amount && settings.account_fee ) {
        token::transfer_action transfer_fee( base.get_contract(), { get_self(), "active"_n } );
        transfer_fee.send( get_self(), settings.account_fee, fee, "convert.fee");
    }
}

symbol_code relay::parse_memo( const string memo )
{
    check( memo.size() > 0, "memo requires symbol code - ex: \"EOS\"");
    auto sym = symbol_code{ memo };
    check( sym.is_valid(), "symbol code is not valid");
    return sym;
}
