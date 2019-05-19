#!/usr/bin/env bash

eosc -u https://kylin.eosn.io tx create geojsonpoint create \
    '{"owner":"geojsonpoint","point_name": "myuniqueid11", "lon": 120.123, "lat": 45.123, "keys": ["foo"], "values": ["bar"]}' -p geojsonpoint
