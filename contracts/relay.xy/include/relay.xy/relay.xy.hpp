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
            _settings( get_self(), get_self().value ),
            _reserves( get_self(), get_self().value )
    {}

    /**
     * ACTION `init`
     *
     * @example
     *
     * cleos push action relay.xy init '[true]'
     */
    [[eosio::action]]
    void init( const bool enabled = true );


    /**
     * ACTION `setreserve`
     *
     * add relay setreserve
     *
     * @param {extended_symbol} base - relay base
     * @param {extended_symbol} quote - relay quote
     *
     * @example
     *
     * cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]'
     */
    [[eosio::action]]
    void setreserve( const extended_symbol base, const extended_symbol quote );

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
    using setreserve_action = eosio::action_wrapper<"setreserve"_n, &relay::setreserve>;

private:
    /**
     * TABLE `settings`
     *
     * @example
     *
     * {
     *   "enabled": false
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        bool    enabled = false;
    };

    /**
     * TABLE `reserves`
     *
     * @param {extended_symbol} base
     * @param {extended_symbol} quote
     *
     * @example
     *
     * {
     *   "base": {"contract": "token.xy", {"symbol": "XY", "precision": 4}},
     *   "quote": {"contract": "eosio.token", {"symbol": "EOS", "precision": 4}}
     * }
     */
    struct [[eosio::table("reserves")]] reserves_row {
        extended_symbol     base;
        extended_symbol     quote;

        uint64_t primary_key() const { return base.get_symbol().code().raw(); }
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;
    typedef multi_index<"reserves"_n, reserves_row> reserves_table;

    // local instances of the multi indexes
    settings_table      _settings;
    reserves_table      _reserves;

    // private helpers
    // ===============
    double bancor_formula( double balance_from, double balance_to, double amount );
    double to_fixed( double num, int precision );
    symbol get_core_symbol();
    void convert( const name to, const asset quantity, const extended_symbol base, const extended_symbol quote );
};
