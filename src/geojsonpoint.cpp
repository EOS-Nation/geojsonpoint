#include "geojsonpoint.hpp"

void geojsonpoint::create(
    const name      user,
    const float     lat,
    const float     lon,
    const string    properties
) {
    require_auth( user );

    points.emplace( _self, [&]( auto & row ) {
        row.id          = points.available_primary_key();
        row.user        = user;
        row.lat         = lat;
        row.lon         = lon;
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
    auto itr = points.begin();
    while (itr != points.end()){
        itr = points.erase(itr);
    }
}