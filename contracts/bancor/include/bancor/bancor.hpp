#include <math.h>       /* pow */

namespace bancor {

/**
 * Bancor Formula
 *
 * @param {double} balance_from - from token balance in the relay
 * @param {double} balance_to - to token balance in the relay
 * @param {double} amount - amount to convert
 * @returns {double} computed amount
 * @example
 *
 * const double balance_from = 778140638; // EOS
 * const double balance_to = 4295195539; // BNT
 * const double amount = 10000;
 *
 * // Formula =>
 * // amount / (balance_from + amount) * balance_to
 *
 * bancor::convert( balance_from, balance_to, amount )
 * // => 55197.4814
 *
 */
double convert( double balance_from, double balance_to, double amount )
{
    return amount / (balance_from + amount) * balance_to;
}

/**
 * Bancor Inverse Formula
 *
 * @param {double} balance_from - from token balance in the relay
 * @param {double} balance_to - to token balance in the relay
 * @param {double} amount_desired - amount to desired
 * @returns {double} computed desired amount
 * @example
 *
 * const balance_from = 77814.0638; // EOS
 * const balance_to = 429519.5539120331; // BNT
 * const amount_desired = 1
 *
 * // Inverse Formula =>
 * // balance_from / (1.0 - amount_desired / balance_to) - balance_from
 *
 * bancor::inverse(balance_from, balance_to, amount_desired)
 * // => 0.1811657799
 */
double inverse( double balance_from, double balance_to, double amount_desired )
{
    return balance_from / (1.0 - amount_desired / balance_to) - balance_from;
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
double to_fixed( double num, int precision )
{
    return (int)(num * pow(10, precision)) / pow(10, precision);
}

} // namespace bancor