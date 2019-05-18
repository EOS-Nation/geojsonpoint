#include "geojsonpoint.hpp"

void geojsonpoint::create(
    const name      user,
    const float     lat,
    const float     lon,
    const string    properties
) {
    require_auth( user );

    // Create unique key
    uint64_t id = _points.available_primary_key();

    // Add geometry to `points` table
    _points.emplace( _self, [&]( auto & row ) {
        row.id          = id;
        row.user        = user;
        row.lat         = lat;
        row.lon         = lon;
        row.timestamp   = current_time_point();
        row.version     = 1;
    });

    // Add properties to `properties` table
    _properties.emplace( _self, [&]( auto & row ) {
        row.id          = id;
        row.properties  = properties;
        row.timestamp   = current_time_point();
        row.version     = 1;
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

    // Remove all rows from `properties` table
    auto properties_itr = _properties.begin();
    while (properties_itr != _properties.end()){
        properties_itr = _properties.erase(properties_itr);
    }
}