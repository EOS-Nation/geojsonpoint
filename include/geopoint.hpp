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
                _tag( get_self(), get_self().value ),
                _global( get_self(), get_self().value ),
                _bounds( get_self(), get_self().value )
        {}

        /**
         * Create node (longitude & latitude) with tags
         *
         * @param {name} owner - creator of the node
         * @param {node} node - {lat: float, lon: float}
         * @param {vector<tag>} tags - array of key & value tags
         */
        [[eosio::action]] void createnode(
            const name              owner,
            const node              node,
            const vector<tag>       tags
        );

        /**
         * Erase node and all associated tags
         *
         * @param {name} user - authenticated user
         * @param {vector<uint64_t>} ids - array of node identifiers
         */
        [[eosio::action]] void erasenodes(
            const name              user,
            const vector<uint64_t>  ids
        );

        /**
         * Move node to a new location
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {node} node - {lat: float, lon: float}
         */
        [[eosio::action]] void movenode(
            const name          user,
            const uint64_t      id,
            const node          node
        );

        /**
         * replace multiple tags from a node
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node/way identifier
         * @param {vector<tag>} tags - array of {key: "key", value: "value"} tags
         */
        [[eosio::action]] void replacetags(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * update multiple tags to a node
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node/way identifier
         * @param {vector<tag>} tags - array of {key: "key", value: "value"} tags
         */
        [[eosio::action]] void updatetags(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * erase all tags from a node
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node/way identifier
         */
        [[eosio::action]] void erasetags(
            const name          user,
            const uint64_t      id
        );

        /**
         * erase all specified keys from a node
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node/way identifier
         * @param {vector<name>} keys - specified keys to erase from node
         */
        [[eosio::action]] void erasekeys(
            const name          user,
            const uint64_t      id,
            const vector<name>  keys
        );

        /**
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
            uint64_t minlat = 0;
            uint64_t minlon = 0;
            uint64_t maxlat = 0;
            uint64_t maxlon = 0;
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

            uint64_t primary_key() const { return id; }
        };

        /**
         * Tag (properties) table
         */
        struct [[eosio::table("tag")]] tag_row {
            uint64_t        tag_id;
            uint64_t        id;
            name            key;
            string          value;

            uint64_t primary_key() const { return tag_id; }
            uint64_t by_id() const { return id; }
            uint128_t by_key() const { return compute_by_id_key( id, key ); }
        };

        // Singleton table
        typedef singleton<"global"_n, global_row> global_table;
        typedef singleton<"bounds"_n, bounds_row> bounds_table;

        // Multi-Index table
        typedef eosio::multi_index< "node"_n, node_row> node_table;
        typedef eosio::multi_index<
            "tag"_n, tag_row,
            indexed_by<"byid"_n, const_mem_fun<tag_row, uint64_t, &tag_row::by_id>>,
            indexed_by<"bykey"_n, const_mem_fun<tag_row, uint128_t, &tag_row::by_key>>
        > tag_table;

        // local instances of the multi indexes
        node_table          _node;
        tag_table           _tag;
        bounds_table        _bounds;
        global_table        _global;

        // tag - private helpers
        // =====================
        // emplace
        void emplace_tags( uint64_t id, vector<tag> tags );
        void emplace_tag( uint64_t id, tag tag );

        // modify
        void modify_tag( uint64_t id, tag tag );

        // replace
        void replace_tag( uint64_t id, tag tag );
        void replace_tags( uint64_t id, vector<tag> tags );

        // update
        void update_tag( uint64_t id, tag tag );
        void update_tags( uint64_t id, vector<tag> tags );

        // erase
        void erase_tags( uint64_t id );
        void erase_key( uint64_t id, name key );
        void erase_keys( uint64_t id, vector<name> keys );

        // validation
        void check_tag( tag tag );
        bool tag_exists( uint128_t key_id );
        void check_tag_exists( uint128_t key_id );

        // node - private helpers
        // =====================
        uint64_t emplace_node( name owner, node node );
        void erase_node( uint64_t id );
        void erase_nodes( vector<uint64_t> ids );
        void move_node( uint64_t id, node node );
        void update_node_version( name user, uint64_t id );
        bool node_exists( uint64_t id );
        void check_node_exists( uint64_t id );

        // primary keys
        // ============
        // generate unique key based on node id + key name
        static uint128_t compute_by_id_key( const uint64_t id, const name key ) {
            return ((uint128_t) key.value) << 64 | id;
        }

        // global index for node/way
        uint64_t global_available_primary_key() {
            uint64_t available_primary_key = _global.get_or_default().available_primary_key;
            _global.set(global_row{ available_primary_key + 1 }, get_self());
            return available_primary_key;
        }

        checksum256 get_trx_id() {
            size_t size = transaction_size();
            char buf[size];
            size_t read = read_transaction( buf, size );
            check( size == read, "read_transaction failed");
            return sha256( buf, read );
        }
};