void xy::consume( name from )
{
    asset quantity = asset{10000, symbol{"XY", 4}};
    string memo = "consume XY token";

    // can specify the contract to send the action to as first argument
    token::transfer_action transfer("token.xy"_n, {from, "active"_n});

    // transfer arguments are now passed as postional arguments
    transfer.send(from, get_self(), quantity, memo);

    // can specify the contract to send the action to as first argument
    token::retire_action retire("token.xy"_n, {get_self(), "active"_n});

    // retire arguments are now passed as postional arguments
    retire.send(quantity, memo);
}

