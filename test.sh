#!/usr/bin/env bash

eosc -u https://jungle.eosn.io tx create geojsonspec2 createnode \
    '{"owner":"geojsonspec2","node": {"lat":10, "lon": 110}, "tags":[{"k":"foo","v":"bar"}, {"k":"hello","v":"world"}]}' \
    -p geojsonspec2
