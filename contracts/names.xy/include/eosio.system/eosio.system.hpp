#pragma once

#include <eosio/asset.hpp>
#include <eosio/privileged.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>

#include <eosio.system/exchange_state.hpp>
#include <eosio.system/native.hpp>

#include <deque>
#include <optional>
#include <string>
#include <type_traits>

namespace eosiosystem {

   using eosio::asset;
   using eosio::block_timestamp;
   using eosio::check;
   using eosio::const_mem_fun;
   using eosio::datastream;
   using eosio::indexed_by;
   using eosio::name;
   using eosio::same_payer;
   using eosio::symbol;
   using eosio::symbol_code;
   using eosio::time_point;
   using eosio::time_point_sec;
   using eosio::unsigned_int;

   /**
    * The EOSIO system contract.
    *
    * @details The EOSIO system contract governs ram market, voters, producers, global state.
    */
   class [[eosio::contract("eosio.system")]] system_contract : public native {
      public:
         /**
          * Delegate bandwidth and/or cpu action.
          *
          * @details Stakes SYS from the balance of `from` for the benefit of `receiver`.
          *
          * @param from - the account to delegate bandwidth from, that is, the account holding
          *    tokens to be staked,
          * @param receiver - the account to delegate bandwith to, that is, the account to
          *    whose resources staked tokens are added
          * @param stake_net_quantity - tokens staked for NET bandwidth,
          * @param stake_cpu_quantity - tokens staked for CPU bandwidth,
          * @param transfer - if true, ownership of staked tokens is transfered to `receiver`.
          *
          * @post All producers `from` account has voted for will have their votes updated immediately.
          */
         [[eosio::action]]
         void delegatebw( const name& from, const name& receiver,
                          const asset& stake_net_quantity, const asset& stake_cpu_quantity, bool transfer );

         /**
          * Undelegate bandwitdh action.
          *
          * @details Decreases the total tokens delegated by `from` to `receiver` and/or
          * frees the memory associated with the delegation if there is nothing
          * left to delegate.
          * This will cause an immediate reduction in net/cpu bandwidth of the
          * receiver.
          * A transaction is scheduled to send the tokens back to `from` after
          * the staking period has passed. If existing transaction is scheduled, it
          * will be canceled and a new transaction issued that has the combined
          * undelegated amount.
          * The `from` account loses voting power as a result of this call and
          * all producer tallies are updated.
          *
          * @param from - the account to undelegate bandwidth from, that is,
          *    the account whose tokens will be unstaked,
          * @param receiver - the account to undelegate bandwith to, that is,
          *    the account to whose benefit tokens have been staked,
          * @param unstake_net_quantity - tokens to be unstaked from NET bandwidth,
          * @param unstake_cpu_quantity - tokens to be unstaked from CPU bandwidth,
          *
          * @post Unstaked tokens are transferred to `from` liquid balance via a
          *    deferred transaction with a delay of 3 days.
          * @post If called during the delay period of a previous `undelegatebw`
          *    action, pending action is canceled and timer is reset.
          * @post All producers `from` account has voted for will have their votes updated immediately.
          * @post Bandwidth and storage for the deferred transaction are billed to `from`.
          */
         [[eosio::action]]
         void undelegatebw( const name& from, const name& receiver,
                            const asset& unstake_net_quantity, const asset& unstake_cpu_quantity );

         /**
          * Buy ram action.
          *
          * @details Increases receiver's ram quota based upon current price and quantity of
          * tokens provided. An inline transfer from receiver to system contract of
          * tokens will be executed.
          *
          * @param payer - the ram buyer,
          * @param receiver - the ram receiver,
          * @param quant - the quntity of tokens to buy ram with.
          */
         [[eosio::action]]
         void buyram( const name& payer, const name& receiver, const asset& quant );

         /**
          * Buy a specific amount of ram bytes action.
          *
          * @details Increases receiver's ram in quantity of bytes provided.
          * An inline transfer from receiver to system contract of tokens will be executed.
          *
          * @param payer - the ram buyer,
          * @param receiver - the ram receiver,
          * @param bytes - the quntity of ram to buy specified in bytes.
          */
         [[eosio::action]]
         void buyrambytes( const name& payer, const name& receiver, uint32_t bytes );

         using delegatebw_action = eosio::action_wrapper<"delegatebw"_n, &system_contract::delegatebw>;
         using buyrambytes_action = eosio::action_wrapper<"buyrambytes"_n, &system_contract::buyrambytes>;
         using buyram_action = eosio::action_wrapper<"buyram"_n, &system_contract::buyram>;
         using undelegatebw_action = eosio::action_wrapper<"undelegatebw"_n, &system_contract::undelegatebw>;
   };

   /** @}*/ // end of @defgroup eosiosystem eosio.system
} /// eosiosystem