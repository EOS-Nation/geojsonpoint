#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>

#include <string>
#include <optional>

using eosio::check;
using eosio::current_time_point;
using eosio::time_point_sec;
using eosio::name;
using eosio::print;

using std::vector;
using std::string;
using std::optional;

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
                _owners( _self, _self.value ),
                _geometries( _self, _self.value ),
                _properties( _self, _self.value )
        {}

        /**
         * Create point (longitude & latitude) with properties
         *
         * @param {name} owner - Creator of the Point
         * @param {name} point_name - Unique Name Identifier
         * @param {float} x - Longitude (degrees)
         * @param {float} y - Latitude (degrees)
         * @param {vector<name>} keys - List of Keys
         * @param {vector<string>} values - List of Values
         */
        [[eosio::action]] void create(
            const name            owner,
            const name            point_name,
            const float           x,
            const float           y,
            const vector<name>    keys,
            const vector<string>  values
        );

        /**
         * Move point to new coordinates
         *
         * @param {name} user - User to modify the point
         * @param {name} point_name - Unique Name Identifier
         * @param {float} x - Longitude (degrees)
         * @param {float} y - Latitude (degrees)
         */
        [[eosio::action]] void move(
            const name   user,
            const name   point_name,
            const float  x,
            const float  y
        );

        /**
         * Update point properties
         *
         * @param {name} user - User to modify the point
         * @param {name} point_name - Unique Name Identifier
         * @param {vector<name>} keys - List of Keys
         * @param {vector<string>} values - List of Values
         */
        [[eosio::action]] void update(
            const name            user,
            const name            point_name,
            const vector<name>    keys,
            const vector<string>  values
        );

        /**
         * Clean - Removes all rows in table
         */
        [[eosio::action]] void clean();

    private:
        /**
         * Owners table
         */
        struct [[eosio::table]] owners_row {
            name            point_name;
            vector<name>    owners;
            time_point_sec  created_at;
            bool            is_public;
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;

            uint64_t primary_key() const { return point_name.value; }
        };

        /**
         * Geometries table
         */
        struct [[eosio::table]] geometries_row {
            name            point_name;
            float           x;
            float           y;
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;

            uint64_t primary_key() const { return point_name.value; }
        };

        /**
         * Properties table
         */
        struct [[eosio::table]] properties_row {
            name            point_name;
            vector<name>    keys;
            vector<string>  values;
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;

            uint64_t primary_key() const { return point_name.value; }
        };

        // Multi-Index table
        typedef eosio::multi_index< "owners"_n, owners_row> owners_table;
        typedef eosio::multi_index< "geometries"_n, geometries_row> geometries_table;
        typedef eosio::multi_index< "properties"_n, properties_row> properties_table;

        // Table aliases
        owners_table        _owners;
        geometries_table    _geometries;
        properties_table    _properties;
};