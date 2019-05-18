#!/usr/bin/env bash

cd src
eosio-cpp -abigen geojsonpoint.cpp -o ../geojsonpoint.wasm -I ../include -I ./ -R ../resources
