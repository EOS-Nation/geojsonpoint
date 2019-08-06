#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>

#include <string>
#include <optional>

#include <structs.hpp>
#include <mapbox/geometry.hpp>

using namespace eosio;
using namespace std;
using namespace mapbox::geometry;

class [[eosio::contract("xy")]] xy : public eosio::contract {
    public:
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
                _bounds( get_self(), get_self().value )
        {}

        /**
         * ACTION `createnode`
         *
         * Create node (longitude & latitude) with tags
         *
         * @param {name} owner - creator of the node
         * @param {point} node - point{x, y}
         * @param {vector<tag>} tags - array of key & value tags
         * @returns {uint64_t} node id
         */
        [[eosio::action]] uint64_t createnode(
            const name              owner,
            const point             node,
            const vector<tag>       tags
        );

        /**
         * ACTION `createway`
         *
         * Create way with tags
         *
         * @param {name} owner - creator of the way
         * @param {vector<point>} way - way
         * @param {vector<tag>} tags - array of key & value tags
         * @returns {uint64_t} way id
         */
        [[eosio::action]] uint64_t createway(
            const name                  owner,
            const vector<point>         way,
            const vector<tag>           tags
        );

        /**
         * ACTION `createrel`
         *
         * Create relation with tags
         *
         * @param {name} owner - creator of the way
         * @param {vector<member>} members - array of member
         * @param {vector<tag>} tags - array of key & value tags
         * @returns {uint64_t} member id
         */
        [[eosio::action]] uint64_t createrel(
            const name                  owner,
            const vector<member>        members,
            const vector<tag>           tags
        );

        /**
         * ACTION `erase`
         *
         * Erase node and all associated tags
         *
         * @param {name} user - authenticated user
         * @param {vector<uint64_t>} ids - array of node identifiers
         */
        [[eosio::action]] void erase(
            const name              user,
            const vector<uint64_t>  ids
        );

        /**
         * ACTION `move`
         *
         * Move node to a new location
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - point identifier
         * @param {point} node - point{x, y}
         */
        [[eosio::action]] void move(
            const name          user,
            const uint64_t      id,
            const point         node
        );

        /**
         * ACTION `modify`
         *
         * Modify tags from a node
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {vector<tag>} tags - array of key & value tags
         */
        [[eosio::action]] void modify(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * PRIVATE ACTION `clean`
         *
         * Removes all rows in tables
         */
        [[eosio::action]] void clean();

    private:
        /**
         * TABLE `global`
         */
        struct [[eosio::table("global")]] global_row {
            uint64_t available_primary_key = 0;
        };

        /**
         * TABLE `bounds`
         */
        struct [[eosio::table("bounds")]] bounds_row {
            float min_x;
            float min_y;
            float max_x;
            float max_y;
        };

        /**
         * TABLE `node`
         */
        struct [[eosio::table("node")]] node_row {
            uint64_t        id;
            point           node;

            // Version Control Attributes
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;
            checksum256     changeset;
            vector<tag>     tags;

            uint64_t primary_key() const { return id; }
        };

        /**
         * TABLE `way`
         */
        struct [[eosio::table("way")]] way_row {
            uint64_t            id;
            vector<uint64_t>    refs;

            // Version Control Attributes
            name                user;
            uint32_t            version;
            time_point_sec      timestamp;
            checksum256         changeset;
            vector<tag>         tags;

            uint64_t primary_key() const { return id; }
        };

        /**
         * TABLE `relation`
         */
        struct [[eosio::table("relation")]] relation_row {
            uint64_t            id;
            vector<member>      members;

            // Version Control Attributes
            name                user;
            uint32_t            version;
            time_point_sec      timestamp;
            checksum256         changeset;
            vector<tag>         tags;

            uint64_t primary_key() const { return id; }
        };

        // Singleton table
        typedef singleton<"global"_n, global_row> global_table;
        typedef singleton<"bounds"_n, bounds_row> bounds_table;

        // Multi-Index table
        typedef eosio::multi_index< "node"_n, node_row> node_table;
        typedef eosio::multi_index< "way"_n, way_row> way_table;
        typedef eosio::multi_index< "relation"_n, relation_row> relation_table;

        // local instances of the multi indexes
        node_table          _node;
        way_table           _way;
        relation_table      _relation;
        bounds_table        _bounds;
        global_table        _global;

        // properties - private helpers
        // ============================
        void modify_tags( uint64_t id, vector<tag> tags );
        void check_tag( tag tag );
        void check_tags( vector<tag> tags );

        // node - private helpers
        // ======================
        uint64_t emplace_node( name owner, point node, vector<tag> tags );
        bool erase_node( uint64_t id );
        bool erase_nodes( vector<uint64_t> ids );
        void move_node( uint64_t id, point node );
        void update_node_version( name user, uint64_t id );
        bool node_exists( uint64_t id );
        void check_node_exists( uint64_t id );

        // way - private helpers
        // =====================
        uint64_t emplace_way( name owner, vector<point> way, vector<tag> tags );
        bool erase_way( uint64_t id );
        bool erase_ways( vector<uint64_t> ids );
        bool way_exists( uint64_t id );
        void check_way_exists( uint64_t id );

        // relation - private helpers
        // ==========================
        uint64_t emplace_relation( name owner, vector<member> member, vector<tag> tags );
        bool erase_relation( uint64_t id );
        bool erase_relations( vector<uint64_t> ids );
        bool relation_exists( uint64_t id );
        void check_relation_exists( uint64_t id );

        // bound - private helpers
        // =======================
        void update_bounds( point node );

        // global - private helpers
        // ========================
        uint64_t global_available_primary_key();
        checksum256 get_trx_id();
};