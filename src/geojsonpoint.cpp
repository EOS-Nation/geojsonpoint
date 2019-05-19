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
    check( point_name.length() > 0, "point_name is empty");
    check( point_name.length() != 12, "point_name must be 12 characters in length");

    // Check if unique `point_name` already exists
    auto point_itr = _points.find( point_name.value );
    check( point_itr == _points.end(), "point_name already exists" );

    // Default attributes
    uint64_t uid = _points.available_primary_key();
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;
    bool is_public = true;

    // Set initial user as owner (point can have multiple or no owners)
    vector<name> owners;
    owners.push_back(owner);

    // Add geometry to `points` table
    _points.emplace( owner, [&]( auto & row ) {
        row.uid            = uid;
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
    check( point_name.length() > 0, "point_name is empty");

    // Find Point Unique Point Name
    auto point_itr = _points.find( point_name.value );
    check( point_itr != _points.end(), "No results found matching point_name" );

    // Update `points` table with new coordinates
    _points.modify( point_itr, user, [&](auto & row){
        row.lon = lon;
        row.lat = lat;
        row.version = row.version + 1;
        row.last_modified = user;
        row.modified_at = current_time_point();
    });
}

void geojsonpoint::update(
    const name            user,
    const name            point_name,
    const vector<name>    keys,
    const vector<string>  values
) {
    // Validate user input
    require_auth( user );
    check( point_name.length() > 0, "point_name is empty");

    // Find Point Unique Point Name
    auto point_itr = _points.find( point_name.value );
    check( point_itr != _points.end(), "No results found matching point_name" );

    // Update `points` table with new coordinates
    _points.modify( point_itr, user, [&](auto & row){
        row.keys = keys;
        row.values = values;
        row.version = row.version + 1;
        row.last_modified = user;
        row.modified_at = current_time_point();
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