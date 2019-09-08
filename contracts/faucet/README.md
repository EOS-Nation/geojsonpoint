# XY.network::faucet

## ACTION

- [`request`](#action-request)

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## TABLE

- [`request`](#request-table)

## ACTION `request`

Account can request `XY` tokens from `faucet.xy`.

### params

- `{name} owner` - the owner account to execute the request action for

### example

```bash
cleos push action faucet.xy request '["myaccount"]' -p myaccount
```

## ON_NOTIFY `transfer`

Account can exchange `eosio.token` in exchange for `XY` tokens.

### example

```bash
cleos transfer myaccount faucet.xy "1.0000 EOS"
```

## TABLE `request`

- `{name} account` - account name
- `{time_point_sec} timestamp` - last time request occured

### example

```json
{
  "account": "myaccount",
  "timestamp": "2019-08-07T18:37:37"
}
```
