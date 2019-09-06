# XY.network::relay

## ACTION

- [`init`](#action-init)
- [`setreserve`](#action-setreserve)

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## TABLE

- [`settings`](#settings-table)
- [`reserves`](#reserves-table)

## ACTION `enable`

### params

- `{bool} enabled` - activate relay

### example

```bash
cleos push action relay.xy enable '[true]' -p relay.xy
```

## ACTION `setreserve`

add relay setreserve


- `{extended_symbol}` base - base symbol
- `{extended_symbol}` quote - quote symbol

### example

```bash
cleos push action relay.xy setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}]'
```

## ON_NOTIFY `transfer`

On transfer notify `eosio.token` tokens will be exchanged for `XY` tokens.

On transfer notify `token.xy` tokens will be exchanged for `EOS` tokens.

### example

- `memo` format - ex: `"EOS"`

```bash
cleos transfer myaccount relay.xy "1.0000 EOS" "XY"
cleos transfer myaccount relay.xy "1.0000 XY" "EOS" --contract token.xy
```

## TABLE `settings`

- `{bool} [enabled=false]` - determine if relay is enabled or not

### example

```json
{
  "enabled": false,
}
```

## TABLE `reserves`

- `{uint64_t}` id - unique identifer
- `{extended_symbol}` base - base symbol
- `{extended_symbol}` quote - quote symbol

### example

```json
{
  "id": 0,
  "base": {"contract": "token.xy", "symbol": "4,XY"},
  "quote": {"contract": "eosio.token", "symbol": "4,EOS"}
}
```

