#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>

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
                _config( get_self(), get_self().value )
        {}

        /**
         * Create node (longitude & latitude) with tags
         *
         * @param {name} owner - creator of the node
         * @param {node} node - {lat: double, lon: double}
         * @param {vector<tag>} tags - array of key & value tags
         */
        [[eosio::action]] void createnode(
            const name              owner,
            const node              node,
            const vector<tag>       tags
        );

        /**
         * Move node to a new location
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {node} node - {lat: double, lon: double}
         */
        [[eosio::action]] void movenode(
            const name          user,
            const uint64_t      id,
            const node          node
        );

        /**
         * replace multiple tags from a node/way
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {vector<tag>} tags - array of {key: "key", value: "value"} tags
         */
        [[eosio::action]] void replacetags(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * erase single key from a node/way
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {name} key - key
         */
        [[eosio::action]] void erasekey(
            const name          user,
            const uint64_t      id,
            const name          key
        );

        /**
         * add multiple tags to a node/way
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {vector<tag>} tags - array of {key: "key", value: "value"} tags
         */
        [[eosio::action]] void addtags(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * erase all tags from a node/way
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         */
        [[eosio::action]] void erasetags(
            const name          user,
            const uint64_t      id
        );

        /**
         * erase all specified keys from a node/way
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {vector<name>} keys - specified keys to erase from node/way
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
            uint64_t last_id = 0;
        };

        /**
         * Config table
         */
        struct [[eosio::table("config")]] config_row {
            name authaccount = name{0};
        };

        /**
         * Node (point) table
         */
        struct [[eosio::table("node")]] node_row {
            uint64_t        id;
            double          lat;
            double          lon;

            // Version Control Attributes
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;

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
        typedef singleton<"config"_n, config_row> config_table;
        typedef singleton<"global"_n, global_row> global_table;

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
        global_table        _global;
        config_table        _config;

        // tag - private helpers
        void emplace_tags( uint64_t id, vector<tag> tags );
        void emplace_tag( uint64_t id, tag tag );
        void modify_tag( uint64_t id, tag tag );
        void replace_tag( uint64_t id, tag tag );
        void replace_tags( uint64_t id, vector<tag> tags );
        void erase_tags( uint64_t id );
        void erase_key( uint64_t id, name key );
        void erase_keys( uint64_t id, vector<name> keys );
        void add_tag( uint64_t id, tag tag );
        void add_tags( uint64_t id, vector<tag> tags );
        void check_tag( tag tag );
        bool tag_exists( uint128_t key_id );
        void check_tag_exists( uint128_t key_id );

        // node - private helpers
        uint64_t emplace_node( name owner, node node );
        void move_node( uint64_t id, node node );
        void update_node_version( name user, uint64_t id );
        bool node_exists( uint64_t id );
        void check_node_exists( uint64_t id );
        uint64_t global_available_primary_key();

        // static methods
        static uint128_t compute_by_id_key( const uint64_t id, const name key ) {
            return ((uint128_t) key.value) << 64 | id;
        }
};