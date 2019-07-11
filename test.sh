#!/usr/bin/env bash

eosc -u https://jungle.eosn.io tx create geojsonspec2 createnode \
    '{"owner":"geojsonspec2","lat":10, "lon": 110,"tags":[{"key":"foo","value":"bar"}, {"key":"hello","value":"world"}]}' \
    -p geojsonspec2
