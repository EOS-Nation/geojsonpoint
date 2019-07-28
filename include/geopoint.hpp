#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>

#include <string>
#include <optional>

#include <structs.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("geopoint")]] geopoint : public eosio::contract {
    public:
        /**
         * Construct a new contract given the contract name
         *
         * @param {name} receiver - The name of this contract
         * @param {name} code - The code name of the action this contract is processing.
         * @param {datastream} ds - The datastream used
         */
        geopoint( name receiver, name code, eosio::datastream<const char*> ds )
            : contract( receiver, code, ds ),
                _node( get_self(), get_self().value ),
                _global( get_self(), get_self().value ),
                _bounds( get_self(), get_self().value )
        {}

        /**
         * ACTION create
         *
         * Create node (longitude & latitude) with tags
         *
         * @param {name} owner - creator of the node
         * @param {float} lat - latitude
         * @param {float} lon - longitude
         * @param {vector<tag>} tags - array of key & value tags
         * @returns {uint64_t} node id
         */
        [[eosio::action]] uint64_t create(
            const name              owner,
            const float             lat,
            const float             lon,
            const vector<tag>       tags
        );

        /**
         * ACTION erase
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
         * ACTION move
         *
         * Move node to a new location
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {float} lat - latitude
         * @param {float} lon - longitude
         */
        [[eosio::action]] void move(
            const name          user,
            const uint64_t      id,
            const float         lat,
            const float         lon
        );

        /**
         * ACTION modify
         *
         * Modify tags from a node/way
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node/way identifier
         * @param {vector<tag>} tags - array of key & value tags
         */
        [[eosio::action]] void modify(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * PRIVATE ACTION clean
         *
         * Clean - Removes all rows in tables
         */
        [[eosio::action]] void clean();

    private:
        /**
         * Global table
         */
        struct [[eosio::table("global")]] global_row {
            uint64_t available_primary_key = 0;
        };

        /**
         * Bounds table
         */
        struct [[eosio::table("bounds")]] bounds_row {
            float minlat = 0;
            float minlon = 0;
            float maxlat = 0;
            float maxlon = 0;
        };

        /**
         * Node (point) table
         */
        struct [[eosio::table("node")]] node_row {
            uint64_t        id;
            float           lat;
            float           lon;

            // Version Control Attributes
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;
            checksum256     changeset;
            vector<tag>     tags;

            uint64_t primary_key() const { return id; }
        };

        // Singleton table
        typedef singleton<"global"_n, global_row> global_table;
        typedef singleton<"bounds"_n, bounds_row> bounds_table;

        // Multi-Index table
        typedef eosio::multi_index< "node"_n, node_row> node_table;

        // local instances of the multi indexes
        node_table          _node;
        bounds_table        _bounds;
        global_table        _global;

        // tag - private helpers
        // =====================
        void modify_tags( uint64_t id, vector<tag> tags );
        void check_tag( tag tag );
        void check_tags( vector<tag> tags );

        // node - private helpers
        // =====================
        uint64_t emplace_node( name owner, node node, vector<tag> tags );
        void erase_node( uint64_t id );
        void erase_nodes( vector<uint64_t> ids );
        void move_node( uint64_t id, node node );
        void update_node_version( name user, uint64_t id );
        bool node_exists( uint64_t id );
        void check_node_exists( uint64_t id );

        // bound - private helpers
        // =======================
        void update_bounds( node node );

        // global - private helpers
        // ========================
        uint64_t global_available_primary_key();
        checksum256 get_trx_id();
};