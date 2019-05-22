#!/usr/bin/env bash

eosc -u https://jungle.eosn.io tx create geojsonpoint create \
    '{"owner":"geojsonpoint","geo_id": "cityofottawa", "x": -75.70, "y": 45.42, "keys": ["name","admin.level","capital"], "values": ["Ottawa","6","yes"]}' -p geojsonpoint
