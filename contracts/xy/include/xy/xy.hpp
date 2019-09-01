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
 * STRUCT `tag`
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
 * STRUCT `member`
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
    name        ref;
    name        role;
};

/**
 * STRUCT `uid_object`
 *
 * @param {name} uid - unique identifier
 * @param {name} owner - owner name of xy object
 * @param {name} type - object type (node/way/relation)
 * @example
 *
 * {
 *   "uid": "mynode.xy",
 *   "owner": "myaccount",
 *   "type": "node"
 * }
 */
struct uid_object {
    name        uid;
    name        owner;
    name        type;
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
            _global( get_self(), get_self().value ),
            _uid( get_self(), get_self().value )
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
     * @returns {name} node id
     * @example
     *
     * cleos push action xy node '["myaccount", [45.0, 110.5], [{"k": "key", "v": "value"}], "mynode"]' -p myaccount
     */
    [[eosio::action]]
    name node( const name           owner,
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
     * @returns {name} way id
     * @example
     *
     * cleos push action xy way '["myaccount", [[45.0, 110.5], [25.0, 130.5]], [{"k": "key", "v": "value"}], "myway"]' -p myaccount
     */
    [[eosio::action]]
    name way( const name            owner,
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
     * @returns {name} member id
     * @example
     *
     * cleos push action xy relation '["myaccount", [{"type": "way", "ref": 1, "role": "outer"}], [{"k": "key", "v": "value"}], "myrelation"]' -p myaccount
     */
    [[eosio::action]]
    name relation( const name               owner,
                   const vector<member>     members,
                   const vector<tag>        tags,
                   const name               uid = name{""} );

    /**
     * ACTION `erase`
     *
     * Erase node and all associated tags
     *
     * @param {name} user - authenticated user
     * @param {vector<name>} uids - array of node identifiers
     * @example
     *
     * cleos push action xy erase '["myaccount", [0]]' -p myaccount
     */
    [[eosio::action]]
    void erase( const name              user,
                const vector<name>      uids );

    /**
     * ACTION `move`
     *
     * Move node to a new location
     *
     * @param {name} user - authenticated user
     * @param {name} uid - point identifier
     * @param {point} node - point{x, y}
     * @example
     *
     * cleos push action xy move '["myaccount", 0, [45.0, 110.5]]' -p myaccount
     */
    [[eosio::action]]
    void move( const name          user,
               const name          uid,
               const point         node );

    /**
     * ACTION `modify`
     *
     * Modify tags from a node
     *
     * @param {name} user - authenticated user
     * @param {name} uid - node identifier
     * @param {vector<tag>} tags - array of key & value tags
     * @example
     *
     * cleos push action xy modify '["myaccount", 0, [{"k": "key", "v": "value"}]]' -p myaccount
     */
    [[eosio::action]]
    void modify( const name          user,
                 const name          uid,
                 const vector<tag>   tags );

    /**
     * Clean tables
     */
    [[eosio::action]]
    void clean();

    /**
     * Check if unique identifier exists
     *
     * @param {name} uid - unique identifier
     * @returns {bool} true/false
     * @example
     *
     * uid_exists( "mynode"_n ); // => true/false
     */
    static bool uid_exists( const name uid )
    {
        uid_table _uid( "xy"_n, "xy"_n.value );
        return _uid.find( uid.value ) != _uid.end();
    }

    /**
     * Get uid object from unique identifier
     *
     * @param {name} uid - unique identifier
     * @returns {uid_object} uid object
     * @example
     *
     * get_uid( "mynode"_n );
     * // =>
     * // {
     * //   "id": 123,
     * //   "uid": "mynode.xy",
     * //   "owner": "myaccount",
     * //   "type": "node"
     * // }
     */
    static uid_object get_uid( const name uid )
    {
        uid_table _uid( "xy"_n, "xy"_n.value );
        auto row = _uid.get( uid.value, "uid does not exist" );
        return uid_object{ row.uid, row.owner, row.type };
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
     * @param {uint64_t} available_primary_key - global object identifier
     * @example
     *
     * {
     *   "available_primary_key": 123,
     * }
     */
    struct [[eosio::table("global")]] global_row {
        uint64_t available_primary_key = 0;
    };

    /**
     * TABLE `uid`
     *
     * @param {name} uid - unique identifier
     * @param {name} owner - owner object
     * @param {name} type - object type (node/way/relation)
     * @example
     *
     * {
     *   "uid": "mynode.xy",
     *   "owner": "myaccount",
     *   "type": "node"
     * }
     */
    struct [[eosio::table("uid"), eosio::contract("xy")]] uid_row {
        name        uid;
        name        owner;
        name        type;

        uint64_t primary_key() const { return uid.value; }
        uint64_t by_owner() const { return owner.value; }
    };

    /**
     * TABLE `node`
     *
     * @param {name} uid - unique identifier
     * @param {point} node - point{x, y} coordinate
     * @param {vector<tag>} tags - array of tags associated to object tag{key, value}
     * @param {name} owner - owner object
     * @param {uint32_t} version - amount of times object has been modified
     * @param {time_point_sec} timestamp - last time object was modified
     * @param {checksum256} changeset - transaction ID used to last modify object
     * @example
     *
     * {
     *   "uid": "mynode",
     *   "node": {"x": 45.0, "y": 110.5},
     *   "tags": [ { "k": "key", "v": "value" } ],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08"
     * }
     */
    struct [[eosio::table("node")]] node_row {
        name                uid;
        point               node;
        vector<tag>         tags;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;

        uint64_t primary_key() const { return uid.value; }
        uint64_t by_owner() const { return owner.value; }
    };

    /**
     * TABLE `way`
     *
     * @param {name} uid - unique identifier
     * @param {vector<uint64_t} refs - array of node ids
     * @param {vector<tag>} tags - array of tags associated to object tag{key, value}
     * @param {name} owner - owner object
     * @param {uint32_t} version - amount of times object has been modified
     * @param {time_point_sec} timestamp - last time object was modified
     * @param {checksum256} changeset - transaction ID used to last modify object
     * @example
     *
     * {
     *   "uid": "myway",
     *   "refs": [0, 1],
     *   "tags": [ { "k": "key", "v": "value" } ],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08"
     * }
     */
    struct [[eosio::table("way")]] way_row {
        name                uid;
        vector<name>        refs;
        vector<tag>         tags;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;

        uint64_t primary_key() const { return uid.value; }
        uint64_t by_owner() const { return owner.value; }
    };

    /**
     * TABLE `relation`
     *
     * @param {name} uid - unique identifier
     * @param {vector<member} members - array of member{type, ref, role}
     * @param {vector<tag>} tags - array of tags associated to object tag{key, value}
     * @param {name} owner - owner object
     * @param {uint32_t} version - amount of times object has been modified
     * @param {time_point_sec} timestamp - last time object was modified
     * @param {checksum256} changeset - transaction ID used to last modify object
     * @example
     *
     * {
     *   "uid": "myrel",
     *   "members": [{"type": "way", "ref": 1, "role": "outer"}],
     *   "tags": [ { "k": "key", "v": "value" } ],
     *   "owner": "myaccount",
     *   "version": 1,
     *   "timestamp": "2019-08-07T18:37:37",
     *   "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08"
     * }
     */
    struct [[eosio::table("relation")]] relation_row {
        name                uid;
        vector<member>      members;
        vector<tag>         tags;

        // Version Control Attributes
        name                owner;
        uint32_t            version;
        time_point_sec      timestamp;
        checksum256         changeset;

        uint64_t primary_key() const { return uid.value; }
        uint64_t by_owner() const { return owner.value; }
    };

    // Multi-Index Tables
    typedef eosio::multi_index< "node"_n, node_row,
        indexed_by<"byowner"_n, const_mem_fun<node_row, uint64_t, &node_row::by_owner>>
    > node_table;
    typedef eosio::multi_index< "way"_n, way_row,
        indexed_by<"byowner"_n, const_mem_fun<way_row, uint64_t, &way_row::by_owner>>
    > way_table;
    typedef eosio::multi_index< "relation"_n, relation_row,
        indexed_by<"byowner"_n, const_mem_fun<relation_row, uint64_t, &relation_row::by_owner>>
    > relation_table;
    typedef eosio::multi_index< "uid"_n, uid_row,
        indexed_by<"byowner"_n, const_mem_fun<uid_row, uint64_t, &uid_row::by_owner>>
    > uid_table;

    // Singleton Tables
    typedef eosio::singleton< "global"_n, global_row> global_table;

    // local instances of the multi indexes
    node_table          _node;
    way_table           _way;
    relation_table      _relation;
    global_table        _global;
    uid_table           _uid;

    // private helpers
    // ===============

    // tags
    // ====
    void modify_tags( const name owner, const name uid, const vector<tag> tags );
    void check_tag( const tag tag );
    void check_tags( const vector<tag> tags );

    // node
    // ====
    name emplace_node( const name owner, const point node, const vector<tag> tags, const name uid = name{""} );
    bool erase_node( const name uid );
    bool erase_nodes( const vector<name> uids );
    void move_node( const name uid, const point node );
    bool node_exists( const name uid );
    void check_node_exists( const name uid );

    // way
    // ===
    name emplace_way( const name owner, const vector<point> way, const vector<tag> tags, const name uid = name{""} );
    bool erase_way( name uid );
    bool erase_ways( const vector<name> uids );
    bool way_exists( const name uid );
    void check_way_exists( const name uid );
    void check_way( const vector<point> way );

    // relation
    // ========
    name emplace_relation( const name owner, const vector<member> member, const vector<tag> tags, const name uid = name{""} );
    bool erase_relation( name uid );
    bool erase_relations( const vector<name> uids );
    bool relation_exists( const name uid );
    void check_relation_exists( const name uid );

    // global
    // ======
    uint64_t global_available_primary_key();
    checksum256 get_trx_id();
    void update_version( name uid );
    uint64_t now();

    // consume
    // =======
    void consume_token( const name owner, const int64_t nodes, const int64_t tags, const string memo );
    int64_t calculate_consume( int64_t nodes, int64_t tags );
    void consume_modify_tags( const name user, const int64_t before, const int64_t after );

    // uid
    // ===
    name set_uid( const name owner, name uid, const name type );
};
