#include "geojsonpoint.hpp"

void geojsonpoint::create(
    const name           owner,
    const name           point_name,
    const float          lon,
    const float          lat,
    const vector<name>   keys,
    const vector<string> values
) {
    // Validate user input
    require_auth( owner );

    // Check if unique `point_name` already exists
    auto point_itr = _points.find( point_name.value );
    check( point_itr == _points.end(), "point_name already exists" );

    // Default attributes
    time_point_sec timestamp = current_time_point(); // current timestamp as of now
    uint8_t version = 1;
    bool is_public = true;

    // Set initial user as owner (point can have multiple or no owners)
    vector<name> owners;
    owners.push_back(owner);

    // Set Unique Identifier as blank if `point_name` does not exists
    // if (point_name.length() == 0) point_name = name("");

    // Add geometry to `points` table
    _points.emplace( owner, [&]( auto & row ) {
        row.point_name     = point_name;
        row.owners         = owners;
        row.last_modified  = owner;
        row.lon            = lon;
        row.lat            = lat;
        row.keys           = keys;
        row.values         = values;
        row.created_at     = timestamp;
        row.modified_at    = timestamp;
        row.version        = version;
        row.is_public      = is_public;
    });
}

void geojsonpoint::move(
    const name    user,
    const name    point_name,
    const float   lon,
    const float   lat
) {
    // Validate user input
    require_auth( user );

    // Find Point Unique Point Name
    auto point_itr = _points.find( point_name.value );
    check( point_itr != _points.end(), "No results found matching point_name" );

    // Update `points` table with new coordinates
    _points.modify( point_itr, user, [&](auto & row){
        row.lon = lon;
        row.lat = lat;
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