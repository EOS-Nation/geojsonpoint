#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/crypto.hpp>
#include <eosio/asset.hpp>

#include <string>
#include <optional>

#include <token.xy/token.xy.hpp>
#include <mapbox/geometry.hpp>

using namespace eosio;
using namespace std;
using namespace mapbox::geometry;

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
     * @param {name} [uid=""] - unique identifier
     * @returns {uint64_t} node id
     * @example
     *
     * cleos push action xy node '["myaccount", [45.0, 110.5], [{"k": "key", "v": "value"}], "mynode"]' -p myaccount
     */
    [[eosio::action]]
    uint64_t node( const name           owner,
                   const point          node,
                   const vector<tag>    tags,
                   const name           uid = name{""} );

    /**
     * ACTION `way`
     *
     * Create way with tags
     *
     * @param {name} owner - creator of the way
     * @param {vector<point>} way - way
     * @param {vector<tag>} tags - array of key & value tags
     * @param {name} [uid=""] - unique identifier
     * @returns {uint64_t} way id
     * @example
     *
     * cleos push action xy way '["myaccount", [[45.0, 110.5], [25.0, 130.5]], [{"k": "key", "v": "value"}], "myway"]' -p myaccount
     */
    [[eosio::action]]
    uint64_t way( const name            owner,
                  const vector<point>   way,
                  const vector<tag>     tags,
                  const name            uid = name{""} );

    /**
     * ACTION `relation`
     *
     * Create relation with tags
     *
     * @param {name} owner - creator of the way
     * @param {vector<member>} members - array of member
     * @param {vector<tag>} tags - array of key & value tags
     * @param {name} [uid=""] - unique identifier
     * @returns {uint64_t} member id
     * @example
     *
     * cleos push action xy relation '["myaccount", [{"type": "way", "ref": 1, "role": "outer"}], [{"k": "key", "v": "value"}], "myrelation"]' -p myaccount
     */
    [[eosio::action]]
    uint64_t relation( const name               owner,
                       const vector<member>     members,
                       const vector<tag>        tags,
                       const name               uid = name{""} );

    /**
     * ACTION `erase`
     *
     * Erase node and all associated tags
     *
     * @param {name} user - authenticated user
     * @param {vector<uint64_t>} ids - array of node identifiers
     * @example
     *
     * cleos push action xy erase '["myaccount", [0]]' -p myaccount
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
     * cleos push action xy move '["myaccount", 0, [45.0, 110.5]]' -p myaccount
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
     * cleos push action xy modify '["myaccount", 0, [{"k": "key", "v": "value"}]]' -p myaccount
     */
    [[eosio::action]]
    void modify( const name          user,
                 const uint64_t      id,
                 const vector<tag>   tags );

    /**
     * Clean tables
     */
    [[eosio::action]]
    void clean();

    /**
     * Convert unique identifier name to global object identifier
     *
     * @param {name} uid - unique identifier name
     * @returns {uint64_t} id - global object identifier
     * @example
     *
     * uid_to_id("mynode"_n); // => 123
     */
    static uint64_t uid_to_id( const name uid )
    {
        global_table global( "xy"_n, "xy"_n.value );
        auto index = global.get_index<"byuid"_n>();
        return index.get( uid.value, "uid does not exist" ).id;
    }

    /**
     * Check if unique identifier exists
     *
     * @param {name} uid - unique identifier
     * @returns {bool} true/false
     * @example
     *
     * uid_exists("mynode"); // => true/false
     */
    static uint64_t uid_exists( const name uid )
    {
        global_table global( "xy"_n, "xy"_n.value );
        auto index = global.get_index<"byuid"_n>();
        return index.find( uid.value ) != index.end();
    }

    using node_action = eosio::action_wrapper<"node"_n, &xy::node>;
    using way_action = eosio::action_wrapper<"way"_n, &xy::way>;
    using relation_action = eosio::action_wrapper<"relation"_n, &xy::relation>;
    using erase_action = eosio::action_wrapper<"erase"_n, &xy::erase>;
    using modify_action = eosio::action_wrapper<"modify"_n, &xy::modify>;
    using move_action = eosio::action_wrapper<"move"_n, &xy::move>;

private:
    /**
     * TABLE `global`
     *
     * @param {uint64_t} id - global id for node/way/relation
     * @param {name} uid - unique id defined by owner
     * @param {name} type - object type (node/way/relation)
     * @example
     *
     * {
     *   "id": 0,
     *   "uid": "mynode",
     *   "owner": "myaccount",
     *   "type": "node"
     * }
     */
    struct [[eosio::table("global")]] global_row {
        uint64_t id;
        name uid;
        name owner;
        name type;

        uint64_t primary_key() const { return id; }
        uint64_t by_uid() const { return uid.value; }
        uint64_t by_owner() const { return owner.value; }
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
     *   "tags": [ { "k": "key", "v": "value" } ],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08"
     * }
     */
    struct [[eosio::table("node")]] node_row {
        uint64_t            id;
        point               node;
        vector<tag>         tags;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;

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
     *   "tags": [ { "k": "key", "v": "value" } ],
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08"
     * }
     */
    struct [[eosio::table("way")]] way_row {
        uint64_t            id;
        vector<uint64_t>    refs;
        vector<tag>         tags;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;

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
     *   "tags": [ { "k": "key", "v": "value" } ],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08"
     * }
     */
    struct [[eosio::table("relation")]] relation_row {
        uint64_t            id;
        vector<member>      members;
        vector<tag>         tags;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;

        uint64_t primary_key() const { return id; }
    };

    // Multi-Index table
    typedef eosio::multi_index< "node"_n, node_row> node_table;
    typedef eosio::multi_index< "way"_n, way_row> way_table;
    typedef eosio::multi_index< "relation"_n, relation_row> relation_table;
    typedef eosio::multi_index< "global"_n, global_row,
        indexed_by<"byuid"_n, const_mem_fun<global_row, uint64_t, &global_row::by_uid>>,
        indexed_by<"byowner"_n, const_mem_fun<global_row, uint64_t, &global_row::by_owner>>
    > global_table;

    // local instances of the multi indexes
    node_table                  _node;
    way_table                   _way;
    relation_table              _relation;
    global_table                _global;

    // private helpers
    // ===============

    // tags
    // ====
    void modify_tags( name user, uint64_t id, vector<tag> tags );
    void check_tag( tag tag );
    void check_tags( vector<tag> tags );
    void consume_modify_tags( name user, int64_t before, int64_t after );

    // node
    // ====
    uint64_t emplace_node( const name owner, const point node, const vector<tag> tags, const name uid = name{""} );
    bool erase_node( uint64_t id );
    bool erase_nodes( vector<uint64_t> ids );
    void move_node( uint64_t id, point node );
    bool node_exists( uint64_t id );
    void check_node_exists( uint64_t id );

    // way
    // ===
    uint64_t emplace_way( name owner, vector<point> way, vector<tag> tags, const name uid = name{""} );
    bool erase_way( uint64_t id );
    bool erase_ways( vector<uint64_t> ids );
    bool way_exists( uint64_t id );
    void check_way_exists( uint64_t id );
    void check_way( vector<point> way );

    // relation
    // ========
    uint64_t emplace_relation( name owner, vector<member> member, vector<tag> tags, const name uid = name{""} );
    bool erase_relation( uint64_t id );
    bool erase_relations( vector<uint64_t> ids );
    bool relation_exists( uint64_t id );
    void check_relation_exists( uint64_t id );

    // utils
    // =====
    checksum256 get_trx_id();
    name get_owner( uint64_t id );
    void check_owner( name user, uint64_t id );
    void update_version( uint64_t id );
    uint64_t now();

    // global
    // ======
    uint64_t global_available_primary_key( const name owner, const name type, const name uid = name{""} );

    // consume
    // =======
    void consume_token( name from, int64_t nodes, int64_t tags, string memo );
    int64_t calculate_consume( int64_t nodes, int64_t tags );
};
