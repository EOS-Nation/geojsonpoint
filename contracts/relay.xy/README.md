# XY.network::relay

## ACTION

- [`init`](#action-init)
- [`setreserve`](#action-setreserve)
- [`setfee`](#action-setfee)

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## TABLE

- [`settings`](#settings-table)
- [`reserves`](#reserves-table)

## ACTION `enable`

### params

- `{bool} [enabled=false]` - determine if relay is enabled or not

### example

```bash
cleos push action relay.xy enable '[true]' -p relay.xy
```

## ACTION `setfee`

### params

- `{uint64_t} [fee=0]` - relay fee (ex: 500 = 0.5%)
- `{name} [account=""]` - account to redirect relay fee

### example

```bash
cleos push action relay.xy setfee '[500, "myfees]' -p relay.xy
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
- `{uint64_t} [fee=0]` - relay fee (ex: 500 = 0.5%)
- `{name} [account=""]` - account to redirect relay fee
- `{uint64_t} [max_fee=30000]` - maximum fee (3%)

### example

```json
{
  "enabled": false,
  "fee": 500,
  "fee_account": "myfees",
  "max_fee": 30000
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

