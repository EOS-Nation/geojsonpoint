#!/usr/bin/env bash

cd src
eosio-cpp geopoint.cpp -o ../geopoint.wasm -abigen -I ../include -I ./ -R ../resources
