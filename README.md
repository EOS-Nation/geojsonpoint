# XY Geospatial Network EOSIO Smart Contracts

## Roadmap

- Mapbox `geometry.hpp` compatible with EOSIO Smart Contracts
- GeoJSON & Map Vector Tile Services
- UI map example for Block Explorers

## Map viewers

- [Leaflet](https://leafletjs.com/)
- [Leaflet Mapbox GL](https://github.com/mapbox/mapbox-gl-leaflet)

# API

## ACTION

- [`createnode`](#action-createnode)
- [`createway`](#action-createway)
- [`createrel`](#action-createrel)
- [`erase`](#action-erase)
- [`move`](#action-move)
- [`modify`](#action-modify)

## STRUCT

- [`tag`](#struct-tag)
- [`member`](#struct-tag)

## ACTION `createnode`

Create node (longitude & latitude) with tags

### params

- `{name} owner` - creator of the node
- `{point} node` - point{x, y}
- `{vector<tag>} tags` - array of key & value tags

### returns

`{uint64_t}` node id

## ACTION `createway`

Create way with tags

### params

- `{name} owner` - creator of the way
- `{vector<point>} way` - way
- `{vector<tag>} tags` - array of key & value tags
### returns

`{uint64_t}` way id

## ACTION `createrel`

Create relation with tags

### params

- `{name} owner` - creator of the way
- `{vector<member>} member` - array of member
- `{vector<tag>} tags` - array of key & value tags

### returns

`{uint64_t}` member id

## ACTION `erase`

Erase node and all associated tags

### params

- `{name} user` - authenticated user
- `{vector<uint64_t>} ids` - array of node identifiers

## ACTION `move`

Move node to a new location

### params

- `{name} user` - authenticated user
- `{uint64_t} id` - point identifier
- `{point} node` - point{x, y}

## ACTION `modify`

Modify tags from a node

### params

- `{name} user` - authenticated user
- `{uint64_t} id` - node identifier
- `{vector<tag>} tags` - array of key & value tags

## STRUCT `tag`

### params

- `{name} k` - key
- `{string} v` - value

### example

```json
{
    "k": "building",
    "v": "yes"
}
```

## STRUCT `member`

### params

- `{name} type` - type of member (way or node)
- `{uint64_t} ref` - ref id of way or node
- `{name} role` - role of member

### example

```json
{
    "type": "way",
    "ref": 123,
    "role": "outer"
}
```


