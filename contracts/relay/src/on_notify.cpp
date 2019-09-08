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
