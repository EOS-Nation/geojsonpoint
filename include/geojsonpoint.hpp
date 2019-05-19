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
                _points( _self, _self.value ),
                _global( _self, _self.value )
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
         * Points table
         */
        struct [[eosio::table]] point_row {
            uint64_t        uid;
            name            point_name;
            vector<name>    owners;
            name            last_modified;
            float           lon;
            float           lat;
            vector<name>    keys;
            vector<string>  values;
            time_point_sec  created_at;
            time_point_sec  modified_at;
            uint32_t        version;
            bool            is_public;

            uint64_t primary_key() const { return point_name.value; }
            uint64_t by_uid() const { return uid; }
        };


        /**
         * Global table
         */
        struct [[eosio::table]] global_state {
            uint64_t        last_uid;
            time_point_sec  last_created_at;
            time_point_sec  last_modified_at;
        };

        // Multi-Index table
        typedef eosio::multi_index< "points"_n, point_row,
            eosio::indexed_by<"byuid"_n, eosio::const_mem_fun<point_row, uint64_t, & point_row::by_uid> >
        > points_table;

        typedef eosio::singleton< "global"_n, global_state > global_state_singleton;

        // Table aliases
        points_table            _points;
        global_state_singleton  _global;
};