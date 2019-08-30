# XY.network::relay

## ACTION

- [`init`](#action-init)

## ON_NOTIFY

- [`transfer`](#on-notify-transfer)

## TABLE

- [`settings`](#settings-table)

## ACTION `init`

### params

- `{bool} enabled` - activate relay

### example

```bash
cleos push action relay.xy init '[true]' -p relay.xy
```

## ON_NOTIFY `transfer`

On transfer notify `eosio.token` tokens will be exchanged for `XY` tokens.

On transfer notify `token.xy` tokens will be exchanged for `EOS` tokens.

### example

```bash
cleos transfer myaccount relay.xy "1.0000 EOS"
cleos transfer myaccount relay.xy "1.0000 XY" --contract token.xy
```

## TABLE `settings`

- `{bool} [enabled=false]` - determine if relay is activated
- `{symbol} [core_symbol="4,EOS"]` - core symbol

### example

```json
{
  "enabled": false,
  "core_symbol": {"symbol": "EOS", "precision": 4}
}
```
