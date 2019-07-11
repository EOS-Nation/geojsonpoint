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
                _tag( get_self(), get_self().value )
        {}

        /**
         * Create point (longitude & latitude) with properties
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
         * Move point to new coordinates
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

        // /**
        //  * update single tag
        //  *
        //  * @param {name} user - authenticated user
        //  * @param {uint64_t} id - node identifier
        //  * @param {tag} tag - {k: "key", v: "value"} tag
        //  */
        // [[eosio::action]] void updatetag(
        //     const name          user,
        //     const uint64_t      id,
        //     const tag           tag
        // );

        /**
         * replace multiple tags
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {vector<tag>} tags - array of {k: "key", v: "value"} tags
         */
        [[eosio::action]] void replacetags(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * delete single tag
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         * @param {name} k - key
         */
        [[eosio::action]] void deletetag(
            const name          user,
            const uint64_t      id,
            const name          k
        );

        /**
         * delete multiple tags
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - node identifier
         */
        [[eosio::action]] void deletetags(
            const name          user,
            const uint64_t      id
        );

        /**
         * Clean - Removes all rows in tables
         */
        [[eosio::action]] void clean();

    private:
        /**
         * Node (point) table
         */
        struct [[eosio::table]] node_row {
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
        struct [[eosio::table]] tag_row {
            uint64_t        tag_id;
            uint64_t        id;
            name            k;
            string          v;

            uint64_t primary_key() const { return tag_id; }
            uint64_t by_id() const { return id; }
            // uint64_t by_key() const { return key; }
        };

        // Multi-Index table
        typedef eosio::multi_index< "node"_n, node_row> node_table;
        typedef eosio::multi_index<
            "tag"_n, tag_row,
            indexed_by<"byid"_n, const_mem_fun<tag_row, uint64_t, &tag_row::by_id>>
        > tag_table;

        // local instances of the multi indexes
        node_table          _node;
        tag_table           _tag;

        // void compute_by_id_key(const uint64_t id, const name key);

        // tag - private helpers
        void create_tags( uint64_t id, vector<tag> tags );
        void create_tag( uint64_t id, tag tag );
        void modify_tag( uint64_t tag_id, tag tag );
        void replace_tag( uint64_t tag_id, tag tag );
        void delete_tags( uint64_t id );
        void delete_tag( uint64_t id, name k );
        void check_tag( tag tag );
        bool tag_exists( uint64_t tag_id );
        void check_tag_exists( uint64_t tag_id );

        // node - private helpers
        uint64_t create_node( name owner, node node );
        void move_node( uint64_t id, node node );
        void update_node_version( name user, uint64_t id );
        bool node_exists( uint64_t id );
        void check_node_exists( uint64_t id );
};