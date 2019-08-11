#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include <eosio/asset.hpp>

#include <string>
#include <optional>

#include <token.xy/token.xy.hpp>
#include <eosio.system/exchange_state.hpp>
#include <mapbox/geometry.hpp>

using namespace eosio;
using namespace std;
using namespace mapbox::geometry;

const name NETWORK_CONTRACT = "xy"_n;
const name TOKEN_CONTRACT = "token.xy"_n;
const name RELAY_CONTRACT = "relay.xy"_n;
const symbol TOKEN_SYMBOL = symbol{"XY", 4};

/**
 * STRUCT tag
 *
 * @param {name} k - key
 * @param {string} v - value
 *
 * @example
 * {
 *   "k": "building",
 *   "v": "yes"
 * }
 */
struct tag {
    name        k;
    string      v;
};

/**
 * STRUCT member
 *
 * @param {name} type - type of member (way or node)
 * @param {uint64_t} ref - ref id of way or node
 * @param {name} role - role of member
 *
 * @example
 * {
 *   "type": "way",
 *   "ref": 123,
 *   "role": "outer"
 * }
 */
struct member {
    name        type;
    uint64_t    ref;
    name        role;
};

class [[eosio::contract("xy")]] xy : public contract {
public:
    using contract::contract;

    /**
     * Construct a new contract given the contract name
     *
     * @param {name} receiver - The name of this contract
     * @param {name} code - The code name of the action this contract is processing.
     * @param {datastream} ds - The datastream used
     */
    xy( name receiver, name code, eosio::datastream<const char*> ds )
        : contract( receiver, code, ds ),
            _node( get_self(), get_self().value ),
            _way( get_self(), get_self().value ),
            _relation( get_self(), get_self().value ),
            _global( get_self(), get_self().value )
    {}

    /**
     * ACTION `node`
     *
     * Create node (longitude & latitude) with tags
     *
     * @param {name} owner - creator of the node
     * @param {point} node - point{x, y}
     * @param {vector<tag>} tags - array of key & value tags
     * @returns {uint64_t} node id
     * @example
     *
     * cleos push action xy node '["myaccount", [45.0, 110.5], [{"k": "key", "v": "value"}]]'
     */
    [[eosio::action]]
    uint64_t node( const name             owner,
                   const point            node,
                   const vector<tag>      tags );

    /**
     * ACTION `way`
     *
     * Create way with tags
     *
     * @param {name} owner - creator of the way
     * @param {vector<point>} way - way
     * @param {vector<tag>} tags - array of key & value tags
     * @returns {uint64_t} way id
     * @example
     *
     * cleos push action xy way '["myaccount", [[45.0, 110.5], [25.0, 130.5]], [{"k": "key", "v": "value"}]]'
     */
    [[eosio::action]]
    uint64_t way( const name            owner,
                  const vector<point>   way,
                  const vector<tag>     tags );

    /**
     * ACTION `relation`
     *
     * Create relation with tags
     *
     * @param {name} owner - creator of the way
     * @param {vector<member>} members - array of member
     * @param {vector<tag>} tags - array of key & value tags
     * @returns {uint64_t} member id
     * @example
     *
     * cleos push action xy relation '["myaccount", [{"type": "way", "ref": 1, "role": "outer"}], [{"k": "key", "v": "value"}]]'
     */
    [[eosio::action]]
    uint64_t relation( const name              owner,
                       const vector<member>    members,
                       const vector<tag>       tags );

    /**
     * ACTION `erase`
     *
     * Erase node and all associated tags
     *
     * @param {name} user - authenticated user
     * @param {vector<uint64_t>} ids - array of node identifiers
     * @example
     *
     * cleos push action xy erase '["myaccount", [0]]'
     */
    [[eosio::action]]
    void erase( const name              user,
                const vector<uint64_t>  ids );

    /**
     * ACTION `move`
     *
     * Move node to a new location
     *
     * @param {name} user - authenticated user
     * @param {uint64_t} id - point identifier
     * @param {point} node - point{x, y}
     * @example
     *
     * cleos push action xy move '["myaccount", 0, [45.0, 110.5]]'
     */
    [[eosio::action]]
    void move( const name          user,
               const uint64_t      id,
               const point         node );

    /**
     * ACTION `modify`
     *
     * Modify tags from a node
     *
     * @param {name} user - authenticated user
     * @param {uint64_t} id - node identifier
     * @param {vector<tag>} tags - array of key & value tags
     * @example
     *
     * cleos push action xy modify '["myaccount", 0, [{"k": "key", "v": "value"}]]'
     */
    [[eosio::action]]
    void modify( const name          user,
                 const uint64_t      id,
                 const vector<tag>   tags );

    /**
     * ACTION `setrate`
     *
     * Set rate of <chain>XY token
     *
     * @param {symbol} chain - chain value (ex: "EOS,4")
     * @example
     *
     * cleos push action xy setrate '["4,EOS"]'
     */
    [[eosio::action]]
    void setrate( const symbol chain );

    /**
     * Notify contract when eosio.token deposits core symbol
     *
     * Used for token swap
     */
    [[eosio::on_notify("eosio.token::transfer")]]
    void transfer( const name&    from,
                   const name&    to,
                   const asset&   quantity,
                   const string&  memo );

    using node_action = eosio::action_wrapper<"node"_n, &xy::node>;
    using way_action = eosio::action_wrapper<"way"_n, &xy::way>;
    using relation_action = eosio::action_wrapper<"relation"_n, &xy::relation>;
    using erase_action = eosio::action_wrapper<"erase"_n, &xy::erase>;
    using modify_action = eosio::action_wrapper<"modify"_n, &xy::modify>;
    using move_action = eosio::action_wrapper<"move"_n, &xy::move>;
    using setrate_action = eosio::action_wrapper<"setrate"_n, &xy::setrate>;

private:
    /**
     * TABLE `global`
     *
     * @param {uint64_t} available_primary_key - global id for node/way/relation
     * @example
     *
     * {
     *   "available_primary_key": 0,
     *   "chain": "4,EOS"
     *   "rate": "0.1000 EOSXY",
     *   "rammarket": 0.000088812,
     * }
     */
    struct [[eosio::table("global")]] global_row {
        uint64_t available_primary_key = 0;
        symbol chain;
        asset rate;
    };

    /**
     * TABLE `node`
     *
     * @param {uint64_t} id - object unique identifier
     * @param {point} node - point{x, y} coordinate
     * @param {name} owner - creator of object
     * @param {uint32_t} version - amount of times object has been modified
     * @param {time_point_sec} timestamp - last time object was modified
     * @param {checksum256} changeset - transaction ID used to last modify object
     * @param {vector<tag>} tags - array of tags associated to object tag{key, value}
     * @example
     *
     * {
     *   "id": 0,
     *   "node": {"x": 45.0, "y": 110.5},
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08",
     *   "tags": [ { "k": "key", "v": "value" } ]
     * }
     */
    struct [[eosio::table("node")]] node_row {
        uint64_t            id;
        point               node;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;
        vector<tag>         tags;

        uint64_t primary_key() const { return id; }
    };

    /**
     * TABLE `way`
     *
     * @param {uint64_t} id - object unique identifier
     * @param {vector<uint64_t} refs - array of node ids
     * @param {name} owner - creator of object
     * @param {uint32_t} version - amount of times object has been modified
     * @param {time_point_sec} timestamp - last time object was modified
     * @param {checksum256} changeset - transaction ID used to last modify object
     * @param {vector<tag>} tags - array of tags associated to object tag{key, value}
     * @example
     *
     * {
     *   "id": 0,
     *   "refs": [0, 1],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08",
     *   "tags": [ { "k": "key", "v": "value" } ]
     * }
     */
    struct [[eosio::table("way")]] way_row {
        uint64_t            id;
        vector<uint64_t>    refs;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;
        vector<tag>         tags;

        uint64_t primary_key() const { return id; }
    };

    /**
     * TABLE `relation`
     *
     * @param {uint64_t} id - object unique identifier
     * @param {vector<member} members - array of member{type, ref, role}
     * @param {name} owner - creator of object
     * @param {uint32_t} version - amount of times object has been modified
     * @param {time_point_sec} timestamp - last time object was modified
     * @param {checksum256} changeset - transaction ID used to last modify object
     * @param {vector<tag>} tags - array of tags associated to object tag{key, value}
     * @example
     *
     * {
     *   "id": 0,
     *   "members": [{"type": "way", "ref": 1, "role": "outer"}],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08",
     *   "tags": [ { "k": "key", "v": "value" } ]
     * }
     */
    struct [[eosio::table("relation")]] relation_row {
        uint64_t            id;
        vector<member>      members;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;
        vector<tag>         tags;

        uint64_t primary_key() const { return id; }
    };

    // Singleton table
    typedef singleton<"global"_n, global_row> global_table;

    // Multi-Index table
    typedef eosio::multi_index< "node"_n, node_row> node_table;
    typedef eosio::multi_index< "way"_n, way_row> way_table;
    typedef eosio::multi_index< "relation"_n, relation_row> relation_table;

    // local instances of the multi indexes
    node_table          _node;
    way_table           _way;
    relation_table      _relation;
    global_table        _global;

    // tags - private helpers
    // ======================
    void modify_tags( name user, uint64_t id, vector<tag> tags );
    void check_tag( tag tag );
    void check_tags( vector<tag> tags );
    void consume_modify_tags( name user, int64_t before, int64_t after );

    // node - private helpers
    // ======================
    uint64_t emplace_node( name owner, point node, vector<tag> tags );
    bool erase_node( uint64_t id );
    bool erase_nodes( vector<uint64_t> ids );
    void move_node( uint64_t id, point node );
    bool node_exists( uint64_t id );
    void check_node_exists( uint64_t id );

    // way - private helpers
    // =====================
    uint64_t emplace_way( name owner, vector<point> way, vector<tag> tags );
    bool erase_way( uint64_t id );
    bool erase_ways( vector<uint64_t> ids );
    bool way_exists( uint64_t id );
    void check_way_exists( uint64_t id );
    void check_way( vector<point> way );

    // relation - private helpers
    // ==========================
    uint64_t emplace_relation( name owner, vector<member> member, vector<tag> tags );
    bool erase_relation( uint64_t id );
    bool erase_relations( vector<uint64_t> ids );
    bool relation_exists( uint64_t id );
    void check_relation_exists( uint64_t id );

    // utils - private helpers
    // =======================
    checksum256 get_trx_id();
    name get_owner( uint64_t id );
    void check_owner( name user, uint64_t id );
    void update_version( uint64_t id );
    uint64_t now();

    // global - private helpers
    // ========================
    uint64_t global_available_primary_key();
    int64_t get_rammarket( symbol chain );
    asset calculate_rate( symbol chain );
    void consume_token( name from, int64_t points, int64_t tags, string memo );
    int64_t calculate_consume( int64_t points, int64_t tags );
    void deferred_setrate( symbol chain );
};
