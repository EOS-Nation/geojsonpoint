#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

#include <string>

#include <token.xy/token.xy.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosio.system/native.hpp>

using namespace eosio;
using namespace eosiosystem;
using namespace std;

/**
 * memo format
 *
 * @example
 * {
 *   "newaccount", "foo.xy",
 *   "key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV",
 *   "ref": "referral"
 * }
 */
struct memo_format {
    name newaccount;
    public_key key;
    name ref;
};

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
     * cleos push action names.xy init '[{"contract": "eosio.token", "symbol": "4,EOS"}, {"contract": "token.xy", "symbol": "4,EOSXY"}, {"contract": "relay.xy", "symbol": "4,XY"}, "xy"]'
     */
    [[eosio::action]]
    void init( const extended_symbol chain, const extended_symbol token, const extended_symbol relay, const name network );

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
     *   "token": {
     *      "symbol": "4,EOSXY",
     *      "contract": "token.xy"
     *   },
     *   "relay": {
     *      "symbol": "4,XY",
     *      "contract": "relay.xy"
     *   },
     *   "relay": "relay.xy"
     * }
     */
    struct [[eosio::table("settings")]] settings_row {
        extended_symbol     chain;
        extended_symbol     token;
        extended_symbol     relay;
        name                network;
    };

    // Singleton table
    typedef singleton<"settings"_n, settings_row> settings_table;

    // local instances of the multi indexes
    settings_table      _settings;

    // private helpers
    // ========================
    void handle_deposit( const name from, const asset quantity, const name ref );
    public_key string_to_public_key( unsigned int const key_type, const string public_key_str );
    authority public_key_to_authority( const public_key key );
    void create_account( const name creator, const name name, const public_key key );
    vector<string> split(const string& str, const string& delim);
    memo_format parse_memo( const string memo );
};
