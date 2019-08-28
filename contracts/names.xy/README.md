# XY.network::names

`*.xy` EOSIO account name marketplace

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## ACTION

- [`claimaccount`](#action-claimaccount)

## TABLE

- [`rates`](#rates-table)
- [`claim`](#claim-table)
- [`sold`](#sold-table)

## ON_NOTIFY `transfer`

Handles `newaccount` creation deposit fee

### example

```bash
cleos transfer myaccount names.xy "1.0000 EOS" "account.xy"
```

## ACTION `claimaccount`

Account can create a new account using `*.xy` prefix.

### params

- `{name} bidder` - the bidder account to execute the request action for
- `{name} name` - desired account name to create
- `{public_key} public_key` - public key permission for both active & owner key

### example

```bash
cleos push action names.xy claimaccount '["myaccount", "account.xy","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p myaccount
```

## TABLE `rates`

- `{uint8_t} length` - length of account name
- `{asset} amount` - quantity amount to claim name

### example

```json
{
  "length": 4,
  "amount": "30.0000 EOS"
}
```

## TABLE `claim`

- `{name} name` - account name created
- `{name} bidder` - bidder account name
- `{asset} amount` - amount paid for name
- `{checksum256} trx_id` - claim transaction id
- `{time_point_sec} timestamp` - bid timestamp

### example

```json
{
  "name": "account.xy",
  "bidder": "myaccount",
  "amount": "1.0000 EOS",
  "trx_id": "3ab7022027117c4e901597bdfe6e078526f5228f806ca9e03ae729614641e5c2",
  "timestamp": "2019-08-07T18:37:37"
}
```

## TABLE `sold`

- `{name} name` - account name created
- `{name} bidder` - bidder account name
- `{asset} amount` - amount paid for name
- `{checksum256} trx_id` - creation transaction id
- `{time_point_sec} timestamp` - bid time

### example

```json
{
  "name": "account.xy",
  "bidder": "myaccount",
  "amount": "1.0000 EOS",
  "trx_id": "3ab7022027117c4e901597bdfe6e078526f5228f806ca9e03ae729614641e5c2",
  "timestamp": "2019-08-07T18:37:37"
}
```
