#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>

#include <string>
#include <optional>

#include <structs.hpp>
#include <mapbox/geometry.hpp>

using namespace eosio;
using namespace std;
using namespace mapbox::geometry;

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
                _point( get_self(), get_self().value ),
                _linestring( get_self(), get_self().value ),
                _global( get_self(), get_self().value ),
                _bounds( get_self(), get_self().value )
        {}

        /**
         * ACTION create
         *
         * Create point (longitude & latitude) with tags
         *
         * @param {name} owner - creator of the point
         * @param {point<double>} pt - point{x, y}
         * @param {vector<tag>} tags - array of key & value tags
         * @returns {uint64_t} point id
         */
        [[eosio::action]] uint64_t create(
            const name              owner,
            const point<double>     pt,
            const vector<tag>       tags
        );

        /**
         * ACTION createline
         *
         * Create linestring with tags
         *
         * @param {name} owner - creator of the linestring
         * @param {linestring<double>} line - linestring
         * @param {vector<tag>} tags - array of key & value tags
         * @returns {uint64_t} linestring id
         */
        [[eosio::action]] uint64_t createline(
            const name                  owner,
            const linestring<double>    line,
            const vector<tag>           tags
        );

        /**
         * ACTION erase
         *
         * Erase node and all associated tags
         *
         * @param {name} user - authenticated user
         * @param {vector<uint64_t>} ids - array of node identifiers
         */
        [[eosio::action]] void erase(
            const name              user,
            const vector<uint64_t>  ids
        );

        /**
         * ACTION move
         *
         * Move point to a new location
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - point identifier
         * @param {point<double>} pt - point{x, y}
         */
        [[eosio::action]] void move(
            const name          user,
            const uint64_t      id,
            const point<double> pt
        );

        /**
         * ACTION modify
         *
         * Modify tags from a point
         *
         * @param {name} user - authenticated user
         * @param {uint64_t} id - point identifier
         * @param {vector<tag>} tags - array of key & value tags
         */
        [[eosio::action]] void modify(
            const name          user,
            const uint64_t      id,
            const vector<tag>   tags
        );

        /**
         * PRIVATE ACTION clean
         *
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
            float minlat = 0;
            float minlon = 0;
            float maxlat = 0;
            float maxlon = 0;
        };

        /**
         * point table
         */
        struct [[eosio::table("point")]] point_row {
            uint64_t        id;
            point<double>   point;

            // Version Control Attributes
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;
            checksum256     changeset;
            vector<tag>     tags;

            uint64_t primary_key() const { return id; }
        };

        /**
         * linestring table
         */
        struct [[eosio::table("linestring")]] linestring_row {
            uint64_t                id;
            linestring<double>      linestring;

            // Version Control Attributes
            name            user;
            uint32_t        version;
            time_point_sec  timestamp;
            checksum256     changeset;
            vector<tag>     tags;

            uint64_t primary_key() const { return id; }
        };

        // Singleton table
        typedef singleton<"global"_n, global_row> global_table;
        typedef singleton<"bounds"_n, bounds_row> bounds_table;

        // Multi-Index table
        typedef eosio::multi_index< "point"_n, point_row> point_table;
        typedef eosio::multi_index< "linestring"_n, linestring_row> linestring_table;

        // local instances of the multi indexes
        point_table         _point;
        linestring_table    _linestring;
        bounds_table        _bounds;
        global_table        _global;

        // tag - private helpers
        // =====================
        void modify_tags( uint64_t id, vector<tag> tags );
        void check_tag( tag tag );
        void check_tags( vector<tag> tags );

        // node - private helpers
        // =====================
        uint64_t emplace_point( name owner, point<double> pt, vector<tag> tags );
        void erase_point( uint64_t id );
        void erase_points( vector<uint64_t> ids );
        void move_point( uint64_t id, point<double> pt );
        void update_point_version( name user, uint64_t id );
        bool point_exists( uint64_t id );
        void check_point_exists( uint64_t id );

        // linestring - private helpers
        // ============================
        uint64_t emplace_linestring( name owner, linestring<double> line, vector<tag> tags );

        // bound - private helpers
        // =======================
        void update_bounds( point<double> pt );

        // global - private helpers
        // ========================
        uint64_t global_available_primary_key();
        checksum256 get_trx_id();
};