void names::init( symbol core_symbol )
{
    require_auth( get_self() );

    // 100.0000 EOS
    set_rate( 4,  asset{1000000, core_symbol} );

    // 50.0000 EOS
    set_rate( 5,  asset{500000, core_symbol} );

    // 25.0000 EOS
    set_rate( 6,  asset{250000, core_symbol} );

    // 10.0000 EOS
    set_rate( 7,  asset{100000, core_symbol} );
    set_rate( 8,  asset{100000, core_symbol} );
    set_rate( 9,  asset{100000, core_symbol} );

    // 5.0000 EOS
    set_rate( 10, asset{50000, core_symbol} );
    set_rate( 11, asset{50000, core_symbol} );
    set_rate( 12, asset{50000, core_symbol} );
}

void names::setrate( uint8_t length, asset amount )
{
    require_auth( get_self() );
    set_rate( length, amount );
}

void names::set_rate( uint8_t length, asset amount )
{
    auto rate_itr = _rates.find( length );
    if ( rate_itr == _rates.end() ) {
        _rates.emplace( get_self(), [&]( auto & row ) {
            row.length  = length;
            row.amount  = amount;
        });
    } else {
        _rates.modify( rate_itr, get_self(), [&]( auto & row ) {
            row.amount = amount;
        });
    }
}