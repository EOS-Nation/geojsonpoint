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
     * ACTION `enable`
     *
     * @param {bool} [enabled=false] - determine if relay is enabled or not
     *
     * @example
     *
     * cleos push action relay.xy enable '[true]' -p relay.xy
     */
    [[eosio::action]]
    void enable( const bool enabled = true );

    /**
     * ACTION `setfee`
     *
     * @param {uint64_t} [fee=0] - relay fee (ex: 500 = 0.5%)
     * @param {name} [account=""] - account to redirect relay fee
     *
     * @example
     *
     * cleos push action relay.xy setfee '[500, "myfees"]' -p relay.xy
     */
    [[eosio::action]]
    void setfee( const uint64_t fee = 0, const name account = ""_n );

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
     * cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]'  -p relay.xy
     */
    [[eosio::action]]
    void setreserve( const extended_symbol base, const extended_symbol quote );

    /**
     * Notify contract when any token transfer notifiers relay contract
     */
    [[eosio::on_notify("*::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    /**
     * Create unique key between base & quote symbol codes
     */
    static uint128_t symbols_key( const symbol_code base, const symbol_code quote ) {
        return ((uint128_t) base.raw()) << 64 | quote.raw();
    }

    using enable_action = eosio::action_wrapper<"enable"_n, &relay::enable>;
    using setreserve_action = eosio::action_wrapper<"setreserve"_n, &relay::setreserve>;

private:
    /**
     * TABLE `settings`
     *
     * @param {bool} [enabled=false] - determine if relay is enabled or not
     * @param {uint64_t} [fee=0] - relay fee (ex: 500 = 0.5%)
     * @param {name} [account=""] - account to redirect relay fee
     * @param {uint64_t} [max_fee=30000] - maximum fee (3%)
     *
     * @example
     *
     * {
     *   "enabled": false,
     *   "fee": 500,
     *   "fee_account": "myfees",
     *   "max_fee": 30000
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        bool enabled = false;
        uint64_t fee = 0;
        name fee_account = ""_n;
        uint64_t max_fee = 30000;
    };

    /**
     * TABLE `reserves`
     *
     * @param {uint64_t} id - unique identifer
     * @param {extended_symbol} base - base symbol
     * @param {extended_symbol} quote - quote symbol
     *
     * @example
     *
     * {
     *   "id": 0,
     *   "base": {"contract": "token.xy", "symbol": "4,XY"},
     *   "quote": {"contract": "eosio.token", "symbol": "4,EOS"}
     * }
     */
    struct [[eosio::table("reserves")]] reserves_row {
        uint64_t            id;
        extended_symbol     base;
        extended_symbol     quote;

        uint64_t primary_key() const { return id; }
        uint128_t by_symbols() const { return symbols_key( base.get_symbol().code(), quote.get_symbol().code() ); }
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;
    typedef multi_index<"reserves"_n, reserves_row,
        indexed_by<"bysymbols"_n, const_mem_fun<reserves_row, uint128_t, &reserves_row::by_symbols>>
    > reserves_table;

    // local instances of the multi indexes
    settings_table      _settings;
    reserves_table      _reserves;

    // private helpers
    // ===============
    double bancor_formula( double balance_from, double balance_to, double amount );
    double to_fixed( double num, int precision );
    symbol get_core_symbol();
    void convert( const name to, const asset quantity, const string memo );
    symbol_code parse_memo( const string memo );
};
