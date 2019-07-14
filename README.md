# Geo Point EOSIO smart contract

## Roadmap

- GeoPoint EOSIO Smart Contract standard
- GeoJSON & Map Vector Tile Services
- UI map example for Block Explorers

## Tables

### `node`

- `id`
- `lat`
- `lon`

### `tag`

- `id`
- `node_id`
- `key`
- `value`

## References

- [upgrading/1.5-to-1.6.md](https://github.com/EOSIO/eosio.cdt/blob/master/docs/upgrading/1.5-to-1.6.md)
- [singleton.hpp](https://github.com/EOSIO/eosio.cdt/blob/master/libraries/eosiolib/singleton.hpp)
- [multi_index.hpp](https://github.com/EOSIO/eosio.cdt/blob/master/libraries/eosiolib/multi_index.hpp)
- [Lambda Expressions](https://cmichel.io/cpp-guide-for-eos-development-iterators-lambda-expressions/)

## Compile

```bash
$ ./build.sh
```
