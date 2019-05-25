#!/usr/bin/env bash

mkdir -p build
cd src
eosio-cpp -abigen geojsonpoint.cpp -o ../build/geojsonpoint.wasm -I ../include -R ../resources
