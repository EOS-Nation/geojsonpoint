#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

/**
 * {
 *   "type": "point"
 *   "coordinates": [110, 10]
 * }
 */
struct point {
    name                type;
    vector<double>      coordinates;
};

/**
 * {
 *   "lat": 10,
 *   "lon": 110
 * }
 */
struct node {
    double      lat;
    double      lon;
};

/**
 * {
 *   "k": "prop0",
 *   "v": "value0"
 * }
 */
struct tag {
    name        k;
    string      v;
};
