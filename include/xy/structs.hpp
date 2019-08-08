#pragma once

#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

/**
 * STRUCT tag
 *
 * @param {name} k - key
 * @param {string} v - value
 *
 * @example
 * {
 *   "k": "building",
 *   "v": "yes"
 * }
 */
struct tag {
    eosio::name        k;
    std::string        v;
};

/**
 * STRUCT member
 *
 * @param {name} type - type of member (way or node)
 * @param {uint64_t} ref - ref id of way or node
 * @param {name} role - role of member
 *
 * @example
 * {
 *   "type": "way",
 *   "ref": 123,
 *   "role": "outer"
 * }
 */
struct member {
    eosio::name     type;
    uint64_t        ref;
    eosio::name     role;
};
