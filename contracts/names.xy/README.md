# XY.network::names

`*.xy` EOSIO account name marketplace

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## TABLE

- [`rates`](#rates-table)
- [`sold`](#sold-table)

## ON_NOTIFY `transfer`

Handles `newaccount` creation deposit fee

### example

`memo` format = `{name},{public_key},{referral}`

```bash
cleos transfer myaccount names.xy "5.0000 EOS" "account.xy,EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV,refaccount"
```

## TABLE `rates`

- `{uint8_t} length` - length of account name
- `{asset} amount` - quantity amount to claim name

### example

```json
{
  "length": 4,
  "amount": "100.0000 EOS"
}
```

## TABLE `sold`

- `{name} name` - account name created
- `{name} bidder` - bidder account name
- `{name} ref` - referral account name
- `{asset} amount` - amount paid for name
- `{checksum256} trx_id` - claim transaction id
- `{time_point_sec} timestamp` - bid timestamp

### example

```json
{
  "name": "account.xy",
  "bidder": "myaccount",
  "ref": "refaccount",
  "amount": "5.0000 EOS",
  "trx_id": "3ab7022027117c4e901597bdfe6e078526f5228f806ca9e03ae729614641e5c2",
  "timestamp": "2019-08-07T18:37:37"
}
```
