#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>

#include <string>
#include <optional>

#include <geometry.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("geojsonpoint")]] geojsonpoint : public eosio::contract {
    public:
        /**
         * Construct a new contract given the contract name
         *
         * @param {name} receiver - The name of this contract
         * @param {name} code - The code name of the action this contract is processing.
         * @param {datastream} ds - The datastream used
         */
        geojsonpoint( name receiver, name code, eosio::datastream<const char*> ds )
            : contract( receiver, code, ds ),
                _points( _self, _self.value ),
                _properties( _self, _self.value )
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
         * @param {name} geo_id - Unique Name Identifier
         * @param {float} x - Longitude (degrees)
         * @param {float} y - Latitude (degrees)
         */
        [[eosio::action]] void move(
            const name   user,
            const name   geo_id,
            const float  x,
            const float  y
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
         * Points table
         */
        struct [[eosio::table]] points_row {
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
         * Properties table
         */
        struct [[eosio::table]] properties_row {
            uint64_t        id;
            uint64_t        points_id;
            name            k;
            string          v;

            uint64_t primary_key() const { return id; }
            uint64_t by_points() const { return points_id; }
        };

        // Multi-Index table
        typedef eosio::multi_index< "points"_n, points_row> points_table;
        typedef eosio::multi_index<
            "properties"_n, properties_row,
            indexed_by<"bypoints"_n, const_mem_fun<properties_row, uint64_t, &properties_row::by_points>>
        > properties_table;

        // local instances of the multi indexes
        points_table            _points;
        properties_table        _properties;

        // Private helper methods used by other actions.
        void validate_properties( vector<tag> properties );
        void validate_geometry( point geometry );
        bool id_exists( name id );
};