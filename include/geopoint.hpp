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
         * @param {name} owner - creator of the Point
         * @param {double} lat - latitude (degrees)
         * @param {double} lon - longitude (degrees)
         * @param {vector<tag>} properties - list of key & value tags
         */
        [[eosio::action]] void create(
            const name              owner,
            const double            lat,
            const double            lon,
            const vector<tag>       properties
         );

        /**
         * Move point to new coordinates
         *
         * @param {name} user - User to modify the point
         * @param {name} id - Points Identifier
         * @param {float} lat - Latitude (degrees)
         * @param {float} lon - Longitude (degrees)
         */
        [[eosio::action]] void move(
            const name      user,
            const uint64_t  id,
            const double    lat,
            const double    lon
        );

        /**
         * Update all properties
         *
         * @param {name} user - User to modify the point
         * @param {name} geo_id - Unique Name Identifier
         * @param {vector<name>} keys - List of Keys
         * @param {vector<string>} values - List of Values
         */
        [[eosio::action]] void update(
            const name            user,
            const name            geo_id,
            const vector<name>    keys,
            const vector<string>  values
        );

        /**
         * Clean - Removes all rows in table
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
            string          k;
            string          v;

            uint64_t primary_key() const { return tag_id; }
            uint64_t by_id() const { return id; }
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

        // Private helper methods used by other actions.
        void validate_properties( vector<tag> properties );
        bool id_exists( name id );
};