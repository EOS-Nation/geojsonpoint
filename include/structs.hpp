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
    float      lat;
    float      lon;
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
