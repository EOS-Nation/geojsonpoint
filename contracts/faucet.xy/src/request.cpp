void faucet::request( const name owner )
{
    asset quantity = asset{ 100000, symbol{"XY", 4} };

    // external actions
    token::transfer_action token_transfer( "token.xy"_n, { get_self(), "active"_n });

    // issue & transfer XY tokens to user
    token_transfer.send( get_self(), owner, quantity, "XY.network::faucet:request");
}