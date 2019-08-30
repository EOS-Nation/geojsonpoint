#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

#include <string>

#include <token.xy/token.xy.hpp>
#include <eosio.system/exchange_state.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("relay.xy")]] relay : public contract {
public:
    using contract::contract;

    /**
     * Construct a new contract given the contract name
     *
     * @param {name} receiver - The name of this contract
     * @param {name} code - The code name of the action this contract is processing.
     * @param {datastream} ds - The datastream used
     */
    relay( name receiver, name code, eosio::datastream<const char*> ds )
        : contract( receiver, code, ds ),
            _settings( get_self(), get_self().value )
    {}

    /**
     * ACTION `init`
     *
     * @example
     *
     * cleos push action relay.xy init '[true]'
     */
    [[eosio::action]]
    void init( const bool enabled );

    /**
     * Notify contract when eosio.token deposits core token
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    /**
     * Notify contract when token.xy deposits XY network token
     */
    [[eosio::on_notify("token.xy::transfer")]]
    void transfer_xy( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo );

    using init_action = eosio::action_wrapper<"init"_n, &relay::init>;

private:
    /**
     * TABLE `settings`
     *
     * @example
     *
     * {
     *   "enabled": false,
     *   "core_symbol": {"symbol": "EOS", "precision": 4}
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        bool                enabled = false;
        symbol              core_symbol = symbol{"EOS", 4};
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;

    // local instances of the multi indexes
    settings_table      _settings;

    // private helpers
    // ===============
    double bancor_formula( double balance_from, double balance_to, double amount);
    double to_fixed( double num, int precision );
    symbol get_core_symbol();
    void convert( const name to, const asset quantity, const extended_symbol base, const extended_symbol quote );
};
