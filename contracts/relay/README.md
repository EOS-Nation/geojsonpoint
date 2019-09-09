# Relay - Liquidity Exchange

## ACTION

- [`init`](#action-init)
- [`setreserve`](#action-setreserve)
- [`accountfee`](#action-accountfee)

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
cleos push action relay enable '[true]' -p relay
```

## ACTION `accountfee`

### params

- `{name} [account=""]` - account to redirect relay fee

### example

```bash
cleos push action relay accountfee '["myfees]' -p relay
```

## ACTION `setreserve`

add relay setreserve

- `{extended_symbol}` base - base symbol
- `{extended_symbol}` quote - quote symbol
- `{uint64_t} [fee=0]` - relay fee (ex: 50 = 0.5%) pips 1/100 of 1%

### example

```bash
cleos push action relay setreserve '[{"contract": "token.xy", "symbol": "4,XY"}, {"contract": "eosio.token", "symbol": "4,EOS"}, 50]' -p relay
```

## ON_NOTIFY `transfer`

On token transfer notification, tokens will be exchanged

### example

- `memo` format - ex: `"EOS"`

```bash
cleos transfer myaccount relay "1.0000 EOS" "XY"
cleos transfer myaccount relay "1.0000 XY" "EOS" --contract token.xy
```

## TABLE `settings`

- `{bool} [enabled=false]` - determine if relay is enabled or not
- `{name} [account_fee=""]` - account to redirect relay fee
- `{uint64_t} [max_fee=300]` - maximum fee (3%) pips 1/100 of 1%

### example

```json
{
  "enabled": false,
  "account_fee": "myfees",
  "max_fee": 300
}
```

## TABLE `reserves`

- `{uint64_t}` id - unique identifer
- `{extended_symbol}` base - base symbol
- `{extended_symbol}` quote - quote symbol
- `{uint64_t} [fee=0]` - relay fee (ex: 50 = 0.5%) pips 1/100 of 1%

### example

```json
{
  "id": 0,
  "base": {"contract": "token.xy", "symbol": "4,XY"},
  "quote": {"contract": "eosio.token", "symbol": "4,EOS"},
  "fee": 50
}
```

