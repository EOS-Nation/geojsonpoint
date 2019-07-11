#!/usr/bin/env bash

cd src
eosio-cpp geojsonpoint.cpp -o ../geojsonpoint.wasm -abigen -I ../include -I ./ -R ../resources
