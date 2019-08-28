void names::clean()
{
    require_auth( get_self() );

    auto rates_itr = _rates.begin();
    while ( rates_itr != _rates.end() ) {
        rates_itr = _rates.erase(rates_itr);
    }

    auto claim_itr = _claim.begin();
    while ( claim_itr != _claim.end() ) {
        claim_itr = _claim.erase(claim_itr);
    }

    auto sold_itr = _sold.begin();
    while ( sold_itr != _sold.end() ) {
        sold_itr = _sold.erase(sold_itr);
    }
}