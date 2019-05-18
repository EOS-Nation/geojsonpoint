#!/usr/bin/env bash

eosc -u https://kylin.eosn.io tx create geojsonpoint create \
    '{"user":"geojsonpoint", "lat": 45.123, "lon": 120.123, "keys": ["foo"], "values": ["bar"], "uid": "myid"}' -p geojsonpoint
