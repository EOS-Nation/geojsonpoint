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
            _reserves( get_self(), get_self().value ),
            _settings( get_self(), get_self().value )
    {}

    /**
     * ACTION `init`
     *
     * @example
     *
     * cleos push action relay.xy init '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,XY"}]'
     */
    [[eosio::action]]
    void init( const extended_symbol chain, const extended_symbol reserve );

    /**
     * ACTION `update`
     *
     * @example
     *
     * cleos push action relay.xy update '[true]'
     */
    [[eosio::action]]
    void update( const bool enabled );

    /**
     * ACTION `setreserve`
     *
     * @example
     *
     * cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, 500000]'
     */
    [[eosio::action]]
    void setreserve( const extended_symbol reserve, uint64_t ratio );

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
    using setreserve_action = eosio::action_wrapper<"setreserve"_n, &relay::setreserve>;

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
     *   "required_balance": true
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        extended_symbol     chain;
        extended_symbol     reserve;
        bool                enabled = false;
        bool                require_balance = true;
    };

    /**
     * TABLE `reserves`
     *
     * @example
     *
     * {
     *   "reserve": {
     *      "symbol": "4,XY",
     *      "contract": "token.xy"
     *   },
     *   "ratio": 500000,
     * }
     */
    struct [[eosio::table("reserves")]] reserves_row {
        extended_symbol     reserve;
        uint64_t            ratio;

        uint64_t primary_key() const { return reserve.get_contract().value; }
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;

    // Multi-Index table
    typedef eosio::multi_index< "reserves"_n, reserves_row> reserves_table;

    // local instances of the multi indexes
    reserves_table      _reserves;
    settings_table      _settings;

    // bancor - private helpers
    // ========================
    double bancor_formula( double balance_from, double balance_to, double amount);
    double to_fixed( double num, int precision );
};
