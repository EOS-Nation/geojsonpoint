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
using std::string;

class [[eosio::contract("geojsonpoint")]] geojsonpoint : public eosio::contract {
    public:
        /**
         * Construct a new contract given the contract name
         *
         * @param receiver - The name of this contract
         * @param code - The code name of the action this contract is processing.
         * @param ds - The datastream used
         */
        geojsonpoint( name receiver, name code, datastream<const char*> ds )
        :contract(receiver, code, ds),
        points(_self, _self.value) {}

        [[eosio::action]]
        void create(
            const name  user,
            const float lat,
            const float lon,
            const string properties
        );

        [[eosio::action]]
        void clean();

    private:

        struct [[eosio::table]] point_row {
            uint64_t        id;
            name            user;
            float           lat;
            float           lon;
            string          properties;
            time_point_sec  timestamp;
            uint8_t         version;

            uint64_t primary_key() const { return id; }
         };

        typedef eosio::multi_index< "points"_n, point_row > points_table;

        points_table points;
};