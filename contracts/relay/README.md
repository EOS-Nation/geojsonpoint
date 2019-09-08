# Relay - Liquidity Exchange

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
cleos push action relay enable '[true]' -p relay
```

## ACTION `setfee`

### params

- `{uint64_t} [fee=0]` - relay fee (ex: 500 = 0.5%)
- `{name} [account=""]` - account to redirect relay fee

### example

```bash
cleos push action relay setfee '[500, "myfees]' -p relay
```

## ACTION `setreserve`

add relay setreserve


- `{extended_symbol}` base - base symbol
- `{extended_symbol}` quote - quote symbol

### example

```bash
cleos push action relay setreserve '[{"contract": "token", "symbol": "4,SYS"}, {"contract": "eosio.token", "symbol": "4,EOS"}]'
```

## ON_NOTIFY `transfer`

On token transfer notification, tokens will be exchanged

### example

- `memo` format - ex: `"EOS"`

```bash
cleos transfer myaccount relay "1.0000 EOS" "SYS"
cleos transfer myaccount relay "1.0000 SYS" "EOS" --contract token
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
  "base": {"contract": "token", "symbol": "4,SYS"},
  "quote": {"contract": "eosio.token", "symbol": "4,EOS"}
}
```

