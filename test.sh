#!/usr/bin/env bash

eosc -u https://jungle.eosn.io tx create geojsonspec1 create \
    '{"owner":"geojsonspec1","geo_id": "cityofottawa", "x": -75.70, "y": 45.42, "keys": ["name","admin.level","capital"], "values": ["Ottawa","6","yes"]}' -p geojsonspec1
