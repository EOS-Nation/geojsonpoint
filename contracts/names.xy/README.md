# XY.network::names

## ACTION

- [`newaccount`](#action-newaccount)

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## TABLE

- [`names`](#names-table)

## ACTION `newaccount`

Account can create a new account using `*.xy` prefix.

### params

- `{name} bidder` - the bidder account to execute the request action for
- `{name} name` - desired account name to create
- `{public_key} public_key` - public key permission for both active & owner key

### example

```bash
cleos push action names.xy request '["myaccount", "account.xy","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p myaccount
```

## ON_NOTIFY `transfer`

Handles `newaccount` deposit fee

### example

```bash
cleos transfer myaccount names.xy "1.0000 EOS" "account.xy"
```

## TABLE `rates`

- `{uint8_t} size` - account name size
- `{asset} amount` - rate amount paid for name

### example

```json
{
  "size": 4,
  "amount": "30.0000 EOS"
}
```

## TABLE `claimnames`

- `{name} name` - account name created
- `{name} bidder` - bidder account name
- `{asset} amount` - amount paid for name
- `{time_point_sec} timestamp` - bid timestamp

### example

```json
{
  "name": "myaccount",
  "bidder": "account.xy",
  "amount": "1.0000 EOS",
  "bid_time": "2019-08-07T18:37:37"
}
```

## TABLE `soldnames`

- `{name} name` - account name created
- `{name} bidder` - bidder account name
- `{asset} amount` - amount paid for name
- `{checksum256} trx_id` - creation transaction id
- `{time_point_sec} bid_time` - bid time

### example

```json
{
  "name": "myaccount",
  "bidder": "account.xy",
  "amount": "1.0000 EOS",
  "trx_id": "3ab7022027117c4e901597bdfe6e078526f5228f806ca9e03ae729614641e5c2",
  "bid_time": "2019-08-07T18:37:37"
}
```
