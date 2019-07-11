#include "geojsonpoint.hpp"

#include "geometry.cpp"
#include "clean.cpp"
#include "validation.cpp"

// void geojsonpoint::create(
//     const name           owner,
//     const name           geo_id,
//     const float          x,
//     const float          y,
//     const vector<name>   keys,
//     const vector<string> values
// ) {
//     // Validate user input
//     require_auth( owner );
//     check( geo_id.length() > 0, "[geo_id] is empty");
//     check( keys.size() == values.size(), "[keys] & [values] must have the same size");

//     // ** Premium Feature
//     check( geo_id.length() == 12, "[geo_id] must be 12 characters in length");

//     // Check if unique `geo_id` already exists
//     auto itr = _points.find( geo_id.value );
//     check( itr == _points.end(), "[geo_id] already exists" );

//     // Point attributes
//     time_point_sec timestamp = current_time_point();
//     uint32_t version = 1;

//     // ***************
//     // Premium Feature
//     // ***************
//     // Point Public settings, if true any {user} can edit, if false only {owner} can edit
//     bool is_public = true;
//     uint32_t max_owners = 5;
//     uint32_t max_recipients = 5;
//     uint32_t max_properties = 10;

//     // Set initial user as owner (point can have multiple or no owners)
//     vector<name> owners;
//     owners.push_back(owner);

//     // Set notification recipients
//     vector<name> recipients;
//     recipients.push_back(owner);

    // // Add `owners` table
    // _points.emplace( _self, [&]( auto & row ) {
    //     row.geo_id         = geo_id;
    //     row.created_at     = timestamp;

    //     // Premium Features
    //     row.owners         = owners;
    //     row.recipients     = recipients;
    //     row.is_public      = is_public;
    //     row.max_owners     = max_owners;
    //     row.max_recipients = max_recipients;
    //     row.max_properties = max_properties;
    // });

//     // // Add `geometry` table
//     // _geometries.emplace( _self, [&]( auto & row ) {
//     //     row.geo_id         = geo_id;
//     //     row.x              = x;
//     //     row.y              = y;
//     //     row.user           = owner;
//     //     row.version        = version;
//     //     row.timestamp      = timestamp;
//     // });

//     // // Add `properties` table
//     // _properties.emplace( _self, [&]( auto & row ) {
//     //     row.geo_id         = geo_id;
//     //     row.keys           = keys;
//     //     row.values         = values;
//     //     row.user           = owner;
//     //     row.version        = version;
//     //     row.timestamp      = timestamp;
//     // });
// }

void geojsonpoint::move(
    const name    user,
    const name    geo_id,
    const float   x,
    const float   y
) {
    // // Validate user input
    // require_auth( user );
    // check( geo_id.length() > 0, "[geo_id] is empty");

    // // Find Point Unique Point Name
    // auto itr = _geometries.find( geo_id.value );
    // check( itr != _geometries.end(), "[geo_id] no matching results" );

    // // Update `points` table with new coordinates
    // _geometries.modify( itr, _self, [&](auto & row){
    //     row.x = x;
    //     row.y = y;

    //     // Version Control Attributes
    //     row.user = user;
    //     row.version = row.version + 1;
    //     row.timestamp = current_time_point();
    // });
}

void geojsonpoint::update(
    const name            user,
    const name            geo_id,
    const vector<name>    keys,
    const vector<string>  values
) {
    // // Validate user input
    // require_auth( user );
    // check( geo_id.length() > 0, "[geo_id] is empty");
    // check( keys.size() == values.size(), "[keys] & [values] must have the same size");

    // // Find Point Unique Point Name
    // auto itr = _properties.find( geo_id.value );
    // check( itr != _properties.end(), "[geo_id] no matching results" );

    // // Update `properties` table
    // _properties.modify( itr, _self, [&](auto & row){
    //     row.keys        = keys;
    //     row.values      = values;
    //     row.user        = user;
    //     row.version     = row.version + 1;
    //     row.timestamp   = current_time_point();
    // });
}
