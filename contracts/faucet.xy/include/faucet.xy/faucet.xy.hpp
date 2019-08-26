#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>

#include <string>
#include <optional>

#include <token.xy/token.xy.hpp>
#include <eosio.system/exchange_state.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("faucet.xy")]] faucet : public contract {
public:
    using contract::contract;

    /**
     * Construct a new contract given the contract name
     *
     * @param {name} receiver - The name of this contract
     * @param {name} code - The code name of the action this contract is processing.
     * @param {datastream} ds - The datastream used
     */
    faucet( name receiver, name code, eosio::datastream<const char*> ds )
        : contract( receiver, code, ds ),
            _request( get_self(), get_self().value )
    {}

    /**
     * ACTION `request`
     *
     * Account can request `XY` tokens from `faucet.xy` contract
     *
     * @param {name} owner - the owner account to execute the request action for
     * @example
     *
     * cleos push action faucet.xy request '["myaccount"]' -p myaccount
     */
    [[eosio::action]]
    void request( const name owner );

    /**
     * Account can exchange `eosio.token` in exchange for `XY` tokens.
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    using request_action = eosio::action_wrapper<"request"_n, &faucet::request>;

private:

    /**
     * TABLE `request`
     *
     * @param {name} owner - the owner account to execute the request action for
     * @param {time_point_sec} timestamp - last time request occured
     * @example
     *
     * {
     *   "owner": "myaccount",
     *   "timestamp": "2019-08-07T18:37:37"
     * }
     */
    struct [[eosio::table("request")]] request_row {
        name                owner;
        time_point_sec      timestamp;

        uint64_t primary_key() const { return owner.value; }
    };

    // Multi-Index table
    typedef eosio::multi_index< "request"_n, request_row> request_table;

    // local instances of the multi indexes
    request_table       _request;

    // private helpers
    int64_t calculate_rate();
    void exchange_token( const name to, const asset quantity );
    void send_fees( const asset quantity );
};
