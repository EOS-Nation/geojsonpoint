/**
 * Bancor Formula
 *
 * - token balance of EOS (eosio.token) in the relay: 778140638 EOS
 * - token balance of XY (token.xy) in the relay: 4295195539 XY
 *
 * ```js
 * Formula:
 * 10000 / (778140638 + 10000) * 4295195539
 * // => 55197.4814
 * ```
 *
 * @param {double} balance_from - from token balance in the relay
 * @param {double} balance_to - to token balance in the relay
 * @param {double} amount - amount to convert
 * @returns {double} computed amount
 * @example
 *
 * const double balance_from = 778140638 // EOS
 * const double balance_to = 4295195539 // XY
 * const double amount = 10000
 *
 * bancorx.bancorFormula(balance_from, balance_to, amount)
 * // => 55197.4814
 */
double relay::bancor_formula( double balance_from, double balance_to, double amount)
{
    return amount / (balance_from + amount) * balance_to;
}

/**
 * to_fixed
 *
 * formats a number to a fixed precision
 *
 * @param {double} num - number
 * @param {int} precision - precision
 * @example
 *
 * to_fixed(14.214212, 3) //=> 14.214
*/
double relay::to_fixed( double num, int precision )
{
    return (int)(num * pow(10, precision)) / pow(10, precision);
}