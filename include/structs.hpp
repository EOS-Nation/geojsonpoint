#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

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
