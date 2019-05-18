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
            : contract(receiver, code, ds),
                _properties(_self, _self.value),
                _points(_self, _self.value)
        {}

        /**
         * Creates - Geographical point
         *
         * @param {name} user - Creator of the Point
         * @param {float} lat - Latitude (degrees)
         * @param {float} lon - Longitude (degrees)
         * @param {string} properties - Metadata properties of Point
         */
        [[eosio::action]] void create(
            const name  user,
            const float lat,
            const float lon,
            const string properties
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
            uint64_t        id;
            name            user;
            float           lat;
            float           lon;
            time_point_sec  timestamp;
            uint8_t         version;

            uint64_t primary_key() const { return id; }
        };

        /**
         * Properties table
         */
        struct [[eosio::table]] properties_row {
            uint64_t        id;
            string          properties;
            time_point_sec  timestamp;
            uint8_t         version;

            uint64_t primary_key() const { return id; }
        };

        /**
         * Multi Indexes
         */
        typedef eosio::multi_index< "points"_n, point_row > points_table;
        typedef eosio::multi_index< "properties"_n, properties_row > properties_table;

        /**
         * Table aliases
         */
        points_table _points;
        properties_table _properties;
};