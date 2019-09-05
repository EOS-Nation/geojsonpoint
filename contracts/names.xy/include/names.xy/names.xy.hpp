#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

#include <string>

#include <token.xy/token.xy.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosio.system/native.hpp>

#include <names.xy/abieos.hpp>

using namespace eosio;
using namespace eosiosystem;
using namespace std;

/**
 * STRUCT `memo_object`
 *
 * @param {name} name - account name to create
 * @param {public_key} key - public key
 * @param {name} [ref=""] - optional referral account name
 *
 * @example
 * {
 *   "name": "myaccount.xy",
 *   "key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV",
 *   "ref": "refaccount"
 * }
 */
struct memo_object {
    eosio::name        name;
    eosio::public_key  key;
    eosio::name        ref;
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
            _rates( get_self(), get_self().value ),
            _sold( get_self(), get_self().value )
    {}

    /**
     * ACTION `init`
     *
     * @example
     *
     * cleos push action names.xy init '["4,EOS"]' -p names.xy
     */
    [[eosio::action]]
    void init( const symbol core_symbol = symbol{ "EOS", 4 } );

    /**
     * ACTION `setrate`
     *
     * @example
     *
     * cleos push action names.xy setrate '[4, "100.0000 EOS"]' -p names.xy
     */
    [[eosio::action]]
    void setrate( const uint8_t size, const asset amount );

    /**
     * Notify contract when eosio.token deposits core token
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    /**
     * Clean tables
     *
     * Requires ADMIN permissions
     */
    [[eosio::action]]
    void clean();

    using init_action = eosio::action_wrapper<"init"_n, &names::init>;
    using setrate_action = eosio::action_wrapper<"setrate"_n, &names::setrate>;

private:
    /**
     * TABLE `rates`
     *
     * @param {uint8_t} length - length of account name
     * @param {asset} amount - quantity amount to claim name
     *
     * @example
     * {
     *   "length": 4,
     *   "amount": "30.0000 EOS",
     * }
     */
    struct [[eosio::table("rates")]] rates_row {
        uint8_t         length;
        asset           amount;

        uint8_t primary_key() const { return length; }
    };

    /**
     * TABLE `sold`
     *
     * @param {name} name - account name created
     * @param {name} bidder - bidder account name
     * @param {name} ref - referral account name
     * @param {asset} amount - amount paid for name
     * @param {checksum256} trx_id - creation transaction id
     * @param {time_point_sec} timestamp - name creation timestamp
     *
     * @example
     * {
     *   "name": "account.xy",
     *   "bidder": "myaccount",
     *   "ref": "refaccount",
     *   "amount": "1.0000 EOS",
     *   "trx_id": "3ab7022027117c4e901597bdfe6e078526f5228f806ca9e03ae729614641e5c2",
     *   "timestamp": "2019-08-07T18:37:37"
     * }
     */
    struct [[eosio::table("sold")]] sold_row {
        eosio::name     name;
        eosio::name     bidder;
        eosio::name     ref;
        asset           amount;
        checksum256     trx_id;
        time_point_sec  timestamp;

        uint64_t primary_key() const { return name.value; }
    };

    // Tables
    typedef multi_index<"rates"_n, rates_row> rates_table;
    typedef multi_index<"sold"_n, sold_row> sold_table;

    // local instances of the multi indexes
    rates_table     _rates;
    sold_table      _sold;

    // private helpers
    // ========================
    void send_fees( const eosio::name from, const eosio::name ref, const eosio::asset quanity );
    void set_rate( const uint8_t length, const eosio::asset quantity );

    // utils
    memo_object parse_memo( const name from, const string memo );
    vector<string> split( const string& str, const string& delim );
    checksum256 get_trx_id();

    // purchase account
    void purchase_account( const eosio::name name, const eosio::name bidder, const eosio::name ref, const eosio::asset quantity );
    void new_account( const eosio::name creator, const eosio::name name, const eosio::public_key key );
    authority public_key_to_authority( const eosio::public_key key );
};