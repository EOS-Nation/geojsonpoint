#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

#include <string>

#include <token.xy/token.xy.hpp>

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
     * cleos push action relay.xy init '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}, 0]'
     */
    [[eosio::action]]
    void init( const extended_symbol chain, const extended_symbol reserve, const uint64_t fee );

    /**
     * ACTION `update`
     *
     * @example
     *
     * cleos push action relay.xy update '[true, 0]'
     */
    [[eosio::action]]
    void update( const bool enabled, const uint64_t fee );

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
    using update_action = eosio::action_wrapper<"update"_n, &relay::update>;

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
     *   "enabled": false,
     *   "required_balance": true,
     *   "max_fee": 30000,
     *   "fee": 0
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        extended_symbol     chain;
        extended_symbol     reserve;
        bool                enabled = false;
        bool                require_balance = true;
        uint64_t            max_fee = 30000;
        uint64_t            fee = 0;
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;

    // local instances of the multi indexes
    settings_table      _settings;

    // bancor - private helpers
    // ========================
    double bancor_formula( double balance_from, double balance_to, double amount);
    double to_fixed( double num, int precision );
};
