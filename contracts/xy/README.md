# XY.network

# API

## ACTION

- [`node`](#action-node)
- [`way`](#action-way)
- [`relation`](#action-relation)
- [`erase`](#action-erase)
- [`move`](#action-move)
- [`modify`](#action-modify)

## STRUCT

- [`tag`](#struct-tag)
- [`member`](#struct-tag)

## TABLE

- [`node`](#node-table)
- [`way`](#way-table)
- [`relation`](#relation-table)
- [`global`](#global-table)

## ACTION `node`

Create node (longitude & latitude) with tags

### params

- `{name} owner` - creator of the node
- `{point} node` - point{x, y}
- `{vector<tag>} tags` - array of key & value tags

### returns

`{uint64_t}` node id

### example

```bash
cleos push action xy node '["myaccount", [45.0, 110.5], [{"k": "key", "v": "value"}]]'
```

## ACTION `way`

Create way with tags

### params

- `{name} owner` - creator of the way
- `{vector<point>} way` - way
- `{vector<tag>} tags` - array of key & value tags

### returns

`{uint64_t}` way id

## example

```bash
cleos push action xy way '["myaccount", [[45.0, 110.5], [25.0, 130.5]], [{"k": "key", "v": "value"}]]'
```

## ACTION `relation`

Create relation with tags

### params

- `{name} owner` - creator of the way
- `{vector<member>} member` - array of member
- `{vector<tag>} tags` - array of key & value tags

### returns

`{uint64_t}` member id

### example

```bash
cleos push action xy way '["myaccount", [{"type": "way", "ref": 1, "role": "outer"}], [{"k": "key", "v": "value"}]]'
```

## ACTION `erase`

Erase node and all associated tags

### params

- `{name} user` - authenticated user
- `{vector<uint64_t>} ids` - array of node identifiers

### example

```bash
cleos push action xy erase '["myaccount", [0]]'
```

## ACTION `move`

Move node to a new location

### params

- `{name} user` - authenticated user
- `{uint64_t} id` - point identifier
- `{point} node` - point{x, y}

```bash
cleos push action xy move '["myaccount", 0, [45.0, 110.5]]'
```

## ACTION `modify`

Modify tags from a node

### params

- `{name} user` - authenticated user
- `{uint64_t} id` - node identifier
- `{vector<tag>} tags` - array of key & value tags

### example

```bash
cleos push action xy modify '["myaccount", 0, [{"k": "key", "v": "value"}]]'
```

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

## TABLE `node`

- `{uint64_t} id` - object unique identifier
- `{point} node` - point{x, y} coordinate
- `{name} owner` - creator of object
- `{uint32_t} version` - amount of times object has been modified
- `{time_point_sec} timestamp` - last time object was modified
- `{checksum256} changeset` - transaction ID used to last modify object
- `{vector<tag>} tags` - array of tags associated to object tag{key, value}

### example

```json
{
  "id": 0,
  "node": {"x": 45.0, "y": 110.5},
  "owner": "myaccount",
  "version": 1,
  "timestamp": "2019-08-07T18:37:37",
  "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08",
  "tags": [ { "k": "key", "v": "value" } ]
}
```


## TABLE `way`

- `{uint64_t} id` - object unique identifier
- `{vector<uint64_t} refs` - array of node ids
- `{name} owner` - creator of object
- `{uint32_t} version` - amount of times object has been modified
- `{time_point_sec} timestamp` - last time object was modified
- `{checksum256} changeset` - transaction ID used to last modify object
- `{vector<tag>} tags` - array of tags associated to object tag{key, value}

### example

```json
{
  "id": 0,
  "refs": [0, 1],
  "owner": "myaccount",
  "version": 1,
  "timestamp": "2019-08-07T18:37:37",
  "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08",
  "tags": [ { "k": "key", "v": "value" } ]
}
```

## TABLE `relation`

- `{uint64_t} id` - object unique identifier
- `{vector<member} members` - array of member{type, ref, role}
- `{name} owner` - creator of object
- `{uint32_t} version` - amount of times object has been modified
- `{time_point_sec} timestamp` - last time object was modified
- `{checksum256} changeset` - transaction ID used to last modify object
- `{vector<tag>} tags` - array of tags associated to object tag{key, value}

### example

```json
{
  "id": 0,
  "members": [{"type": "way", "ref": 1, "role": "outer"}],
  "owner": "myaccount",
  "version": 1,
  "timestamp": "2019-08-07T18:37:37",
  "changeset": "0e90ad6152b9ba35500703bc9db858f6e1a550b5e1a8de05572f81cdcaae3a08",
  "tags": [ { "k": "key", "v": "value" } ]
}
```

## TABLE `global`

- `{uint64_t} available_primary_key` - global id for node/way/relation
