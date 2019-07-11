#!/usr/bin/env bash

eosc -u https://jungle.eosn.io tx create geojsonspec2 create \
    '{"owner":"geojsonspec2","lat":10, "lon": 110,"properties":[{"k":"foo","v":"bar"}, {"k":"hello","v":"world"}]}' \
    -p geojsonspec2
