#include "geojsonpoint.hpp"

void geojsonpoint::create(
    const name           owner,
    const name           point_name,
    const float          x,
    const float          y,
    const vector<name>   keys,
    const vector<string> values
) {
    // Validate user input
    require_auth( owner );
    check( point_name.length() > 0, "point_name is empty");

    // ** Premium Feature
    check( point_name.length() == 12, "point_name must be 12 characters in length");

    // Check if unique `point_name` already exists
    auto itr = _points.find( point_name.value );
    check( itr == _points.end(), "point_name already exists" );

    // Point attributes
    time_point_sec timestamp = current_time_point();
    uint32_t version = 1;

    // Point Public settings, if true any {user} can edit, if false only {owner} can edit
    // ** Premium Feature
    bool is_public = true;

    // Set initial user as owner (point can have multiple or no owners)
    vector<name> owners;
    owners.push_back(owner);

    // Add `owners` table
    _points.emplace( _self, [&]( auto & row ) {
        row.point_name     = point_name;
        row.owners         = owners;
        row.created_at     = timestamp;
        row.is_public      = is_public;
    });

    // Add `geometry` table
    _geometries.emplace( _self, [&]( auto & row ) {
        row.point_name     = point_name;
        row.x              = x;
        row.y              = y;
        row.user           = owner;
        row.version        = version;
        row.timestamp      = timestamp;
    });

    // Add `properties` table
    _properties.emplace( _self, [&]( auto & row ) {
        row.point_name     = point_name;
        row.keys           = keys;
        row.values         = values;
        row.user           = owner;
        row.version        = version;
        row.timestamp      = timestamp;
    });
}

void geojsonpoint::move(
    const name    user,
    const name    point_name,
    const float   x,
    const float   y
) {
    // Validate user input
    require_auth( user );
    check( point_name.length() > 0, "point_name is empty");

    // Find Point Unique Point Name
    auto itr = _geometries.find( point_name.value );
    check( itr != _geometries.end(), "No results found matching point_name" );

    // Update `points` table with new coordinates
    _geometries.modify( itr, _self, [&](auto & row){
        row.x = x;
        row.y = y;
        row.user = user;
        row.version = row.version + 1;
        row.timestamp = current_time_point();
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

    // Last modified timestamp
    time_point_sec timestamp = current_time_point();

    // Find Point Unique Point Name
    auto itr = _properties.find( point_name.value );
    check( itr != _properties.end(), "No results found matching point_name" );

    // Update `properties` table
    _properties.modify( itr, _self, [&](auto & row){
        row.keys = keys;
        row.values = values;
        row.user = user;
        row.version = row.version + 1;
        row.timestamp = current_time_point();
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

    // Remove all rows from `geometries` table
    auto geometries = _geometries.begin();
    while (geometries != _geometries.end()){
        geometries = _geometries.erase(geometries);
    }

    // Remove all rows from `geometries` table
    auto properties = _properties.begin();
    while (properties != _properties.end()){
        properties = _properties.erase(properties);
    }
}