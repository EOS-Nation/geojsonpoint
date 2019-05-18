# GeoJSON Point EOSIO smart contract

## Compile

```bash
$ ./build.sh
```

## Quickstart

**Create a point**

```
eosc tx create geojsonpoint create '{"user":"<USER>", "lat": 45.123, "lon": 120.123, "properties": "{\"foo\":\"bar\"}"}' -p <USER>
```
