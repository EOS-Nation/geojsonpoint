#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

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
 *   "key": "prop0",
 *   "value": "value0"
 * }
 */
struct tag {
    name        key;
    string      value;
};
