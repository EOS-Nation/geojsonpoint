<h1 class="contract">close</h1>

{{owner}} agrees to close their zero quantity balance for the {{symbol_to_symbol_code symbol}} token.

RAM will be refunded to the RAM payer of the {{symbol_to_symbol_code symbol}} token balance for {{owner}}.

<h1 class="contract">create</h1>

{{$action.account}} agrees to create a new token with symbol {{asset_to_symbol_code maximum_supply}} to be managed by {{issuer}}.

This action will not result any any tokens being issued into circulation.

{{issuer}} will be allowed to issue tokens into circulation, up to a maximum supply of {{maximum_supply}}.

RAM will deducted from {{$action.account}}’s resources to create the necessary records.

<h1 class="contract">issue</h1>

The token manager agrees to issue {{quantity}} into circulation, and transfer it into {{to}}’s account.

{{#if memo}}There is a memo attached to the transfer stating:
{{memo}}
{{/if}}

If {{to}} does not have a balance for {{asset_to_symbol_code quantity}}, or the token manager does not have a balance for {{asset_to_symbol_code quantity}}, the token manager will be designated as the RAM payer of the {{asset_to_symbol_code quantity}} token balance for {{to}}. As a result, RAM will be deducted from the token manager’s resources to create the necessary records.

This action does not allow the total quantity to exceed the max allowed supply of the token.

<h1 class="contract">open</h1>

{{ram_payer}} agrees to establish a zero quantity balance for {{owner}} for the {{symbol_to_symbol_code symbol}} token.

If {{owner}} does not have a balance for {{symbol_to_symbol_code symbol}}, {{ram_payer}} will be designated as the RAM payer of the {{symbol_to_symbol_code symbol}} token balance for {{owner}}. As a result, RAM will be deducted from {{ram_payer}}’s resources to create the necessary records.

<h1 class="contract">retire</h1>

The token manager agrees to remove {{quantity}} from circulation, taken from their own account.

{{#if memo}} There is a memo attached to the action stating:
{{memo}}
{{/if}}

<h1 class="contract">transfer</h1>

{{from}} agrees to send {{quantity}} to {{to}}.

{{#if memo}}There is a memo attached to the transfer stating:
{{memo}}
{{/if}}

If {{from}} is not already the RAM payer of their {{asset_to_symbol_code quantity}} token balance, {{from}} will be designated as such. As a result, RAM will be deducted from {{from}}’s resources to refund the original RAM payer.

If {{to}} does not have a balance for {{asset_to_symbol_code quantity}}, {{from}} will be designated as the RAM payer of the {{asset_to_symbol_code quantity}} token balance for {{to}}. As a result, RAM will be deducted from {{from}}’s resources to create the necessary records.