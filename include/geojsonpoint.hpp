#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>

#include <string>
#include <optional>

using eosio::check;
using eosio::datastream;
using eosio::indexed_by;
using eosio::current_time_point;
using eosio::time_point_sec;
using eosio::name;
using eosio::print;
using eosio::contract;
using eosio::const_mem_fun;

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
        geojsonpoint( name receiver, name code, datastream<const char*> ds )
            : contract( receiver, code, ds ),
                _points( _self, _self.value )
        {}

        /**
         * Create point (longitude & latitude) with properties
         *
         * @param {name} owner - Creator of the Point
         * @param {name} point_name - Unique Name Identifier
         * @param {float} lon - Longitude (degrees)
         * @param {float} lat - Latitude (degrees)
         * @param {vector<name>} keys - List of Keys
         * @param {vector<string>} values - List of Values
         */
        [[eosio::action]] void create(
            const name            owner,
            const name            point_name,
            const float           lon,
            const float           lat,
            const vector<name>    keys,
            const vector<string>  values
        );

        /**
         * Move point to new coordinates
         *
         * @param {name} user - User to modify the point
         * @param {name} point_name - Unique Name Identifier
         * @param {float} lon - Longitude (degrees)
         * @param {float} lat - Latitude (degrees)
         */
        [[eosio::action]] void move(
            const name   user,
            const name   point_name,
            const float  lon,
            const float  lat
        );

        /**
         * Clean - Removes all rows in table
         */
        [[eosio::action]] void clean();

    private:
        /**
         * Points table
         */
        struct [[eosio::table]] point_row {
            name            point_name;
            vector<name>    owners;
            name            last_modified;
            float           lon;
            float           lat;
            vector<name>    keys;
            vector<string>  values;
            time_point_sec  created_at;
            time_point_sec  modified_at;
            uint8_t         version;
            bool            is_public;

            uint64_t primary_key() const { return point_name.value; }
        };

        // Multi-Index table
        typedef eosio::multi_index< "points"_n, point_row> points_table;

        // Table aliases
        points_table _points;
};