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

namespace xy {

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
            _claim( get_self(), get_self().value ),
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
    void init( const symbol core_symbol );

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
     * ACTION `newaccount`
     *
     * Account can create a new account using `*.xy` prefix.
     *
     * @param {name} bidder - the bidder account to execute the request action for
     * @param {name} name - desired account name to create
     * @param {public_key} key - public key permission for both active & owner key
     *
     * @example
     *
     * cleos push action names.xy newaccount '["myaccount", "account.xy","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p myaccount
     */
    [[eosio::action]]
    void newaccount( const name bidder, const name name, const public_key key );

    /**
     * Notify contract when eosio.token deposits core token
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    using init_action = eosio::action_wrapper<"init"_n, &names::init>;

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
     * TABLE `claim`
     *
     * @param {name} name - account name claimed
     * @param {name} bidder - bidder account name
     * @param {asset} amount - amount paid for name
     * @param {time_point_sec} timestamp - bid timestamp
     *
     * @example
     * {
     *   "name": "myaccount",
     *   "bidder": "account.xy",
     *   "amount": "1.0000 EOS",
     *   "timestamp": "2019-08-07T18:37:37"
     * }
     */
    struct [[eosio::table("claim")]] claim_row {
        eosio::name     name;
        eosio::name     bidder;
        asset           amount;
        checksum256     trx_id;
        time_point_sec  timestamp;

        uint64_t primary_key() const { return name.value; }
    };

    /**
     * TABLE `sold`
     *
     * @param {name} name - account name created
     * @param {name} bidder - bidder account name
     * @param {asset} amount - amount paid for name
     * @param {time_point_sec} timestamp - name creation timestamp
     * @param {checksum256} trx_id - creation transaction id
     *
     * @example
     * {
     *   "name": "myaccount",
     *   "bidder": "account.xy",
     *   "amount": "1.0000 EOS",
     *   "bid_time": "2019-08-07T18:37:37",
     *   "trx_id": "3ab7022027117c4e901597bdfe6e078526f5228f806ca9e03ae729614641e5c2"
     * }
     */
    struct [[eosio::table("sold")]] sold_row {
        eosio::name     name;
        eosio::name     bidder;
        asset           amount;
        checksum256     trx_id;
        time_point_sec  timestamp;

        uint64_t primary_key() const { return name.value; }
    };

    // Tables
    typedef multi_index<"rates"_n, rates_row> rates_table;;
    typedef multi_index<"claim"_n, claim_row> claim_table;
    typedef multi_index<"sold"_n, sold_row> sold_table;

    // local instances of the multi indexes
    rates_table     _rates;
    claim_table     _claim;
    sold_table      _sold;

    // private helpers
    // ========================
    void send_fees( const asset quanity );
    name parse_memo( const string memo );
    authority public_key_to_authority( const public_key key );
    void claim_account( const name bidder, const name name, const asset quantity );
    void new_account( const name bidder, const name name, const public_key key );
    void set_rate( const uint8_t length, const asset quantity );
    checksum256 get_trx_id();
};
} /// namespace xy