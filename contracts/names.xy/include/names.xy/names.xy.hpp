#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

#include <string>

#include <token.xy/token.xy.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("names.xy")]] names : public contract {
public:
    using contract::contract;

    /**
     * Construct a new contract given the contract name
     *
     * @param {name} receiver - The name of this contract
     * @param {name} code - The code name of the action this contract is processing.
     * @param {datastream} ds - The datastream used
     */
    names( name receiver, name code, eosio::datastream<const char*> ds )
        : contract( receiver, code, ds ),
            _settings( get_self(), get_self().value )
    {}

    /**
     * ACTION `init`
     *
     * @example
     *
     * cleos push action names.xy init '[{"contract": "eosio.token", "symbol": "4,EOS"}, "relay.xy"]'
     */
    [[eosio::action]]
    void init( const extended_symbol chain, const name relay );

    /**
     * Notify contract when eosio.token deposits core token
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    using init_action = eosio::action_wrapper<"init"_n, &names::init>;

private:
    /**
     * TABLE `settings`
     *
     * @example
     *
     * {
     *   "chain": {
     *      "symbol": "4,EOS",
     *      "contract": "eosio.token"
     *   },
     *   "network": "xy",
     *   "relay": "relay.xy"
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        extended_symbol     chain;
        name                network;
        name                relay;
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;

    // local instances of the multi indexes
    settings_table      _settings;

    // private helpers
    // ========================
};
