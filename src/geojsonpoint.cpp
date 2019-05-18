#include "geojsonpoint.hpp"

void geojsonpoint::create(
    const name           user,
    const float          lat,
    const float          lon,
    const vector<name>   keys,
    const vector<string> values,
    const string&        uid
) {
    // Validate user input
    require_auth( user );

    // Default attributes
    uint64_t id = _points.available_primary_key(); // create unique ID
    time_point_sec timestamp = current_time_point(); // current timestamp as of now
    uint8_t version = 1;
    bool is_public = true;

    // Set initial user as owner (point can have multiple or no owners)
    vector<name> owner;
    owner.push_back(user);

    // Add geometry to `points` table
    _points.emplace( user, [&]( auto & row ) {
        row.id          = id;
        row.uid         = uid;
        row.user        = user;
        row.owner       = owner;
        row.lat         = lat;
        row.lon         = lon;
        row.keys        = keys;
        row.values      = values;
        row.modified_at = timestamp;
        row.created_at  = timestamp;
        row.version     = version;
        row.is_public   = is_public;
    });
}

void geojsonpoint::clean()
{
    // Only `geojsonpoint` can call `clean` action
    require_auth(_self);

    // Remove all rows from `points` table
    auto points_itr = _points.begin();
    while (points_itr != _points.end()){
        points_itr = _points.erase(points_itr);
    }
}