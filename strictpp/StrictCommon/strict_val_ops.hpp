// Arkadijs Slobodkins, 2023


#pragma once


#include "auxiliary_types.hpp"
#include "common_traits.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "strict_literals.hpp"
#include "strict_math.hpp"
#include "strict_traits.hpp"
#include "strict_val.hpp"
#include "use.hpp"

#include <algorithm>
#include <memory>


namespace spp {


namespace detail {

/// @brief Returns the absolute value of x as an unsigned long int.
/// @details If absolute value of the minimum long int were taken immediately as -x,
/// it would overflow. Calculation using unsigned long int is performed to avoid this edge case.
constexpr Strict<unsigned long int> unsigned_magnitude(const Strict<long int> x) {
   const auto magnitude = x.sul();
   return x < 0_sl ? 0_sul - magnitude : magnitude;
}

} // namespace detail


/// @brief Iteratively computes n!, with 0! = 1.
/// @pre n >= 0 and n! fits in long int (0 <= n <= 20 for 64-bit long int).
constexpr Strict<long int> factorial(const ImplicitInt n) {
   const StrictLong N = n.get();
   ASSERT_STRICT_DEBUG(N >= 0_sl);
   auto product = 1_sl;
   for(auto factor = 2_sl; factor <= N; ++factor) {
      ASSERT_STRICT_DEBUG_MSG(product <= constants::highest<long int> / factor,
                              "FACTORIAL RESULT DOES NOT FIT IN LONG INT!");
      product *= factor;
   }
   return product;
}


/// @brief Computes C(n, k), with C(n, 0) = C(n, n) = 1, in O(min(k, n - k)) iterations.
/// @pre 0 <= k <= n and the result fits in long int.
/// @details Cancels denominator factors before multiplication to avoid intermediate overflow.
constexpr Strict<long int> binom_coeff(const ImplicitInt n, const ImplicitInt k) {
   ASSERT_STRICT_DEBUG(k.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(n.get() >= k.get());
   const StrictLong count = mins(k.get(), n.get() - k.get());
   const StrictLong offset = n.get() - count;

   auto product = 1_sl;
   for(auto i = 1_sl; i <= count; ++i) {
      const StrictLong numerator = offset + i;
      const StrictLong divisor = gcds(numerator, i);
      const StrictLong factor = numerator / divisor;
      // The next binomial coefficient, product * factor / (i / divisor), is an integer.
      // Since i / divisor is coprime to factor, it must divide product exactly.
      product /= i / divisor;
      ASSERT_STRICT_DEBUG_MSG(product <= constants::highest<long int> / factor,
                              "BINOMIAL COEFFICIENT DOES NOT FIT IN LONG INT!");
      product *= factor;
   }
   return product;
}


/// @brief Returns x clamped to [low, high].
/// @pre low <= high; none of x, low, or high is NaN.
template <Real T>
constexpr Strict<T> clamps(const Strict<T> x, const Strict<T> low, const Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   if constexpr(Floating<T>) {
      // Use self-comparison instead of isnans to guarantee constexpr in C++20.
      ASSERT_STRICT_DEBUG(x == x);
      ASSERT_STRICT_DEBUG(low == low);
      ASSERT_STRICT_DEBUG(high == high);
   }
   return Strict{std::clamp(x.val(), low.val(), high.val())};
}


/// @copydoc clamps(Strict<T>, Strict<T>, Strict<T>)
/// @note Named-argument overload using Value, Low, and High wrappers.
template <Real T>
constexpr Strict<T> clamps(const Value<T> x, const Low<T> low, const High<T> high) {
   return clamps(x.get(), low.get(), high.get());
}


/// @brief Returns whether x is in the open interval (low, high).
/// @pre low <= high.
template <Real T>
constexpr StrictBool in_open(const Strict<T> x, const Strict<T> low, const Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return x > low && x < high;
}


/// @copydoc in_open(Strict<T>, Strict<T>, Strict<T>)
/// @note Named-argument overload using Value, Low, and High wrappers.
template <Real T>
constexpr StrictBool in_open(const Value<T> x, const Low<T> low, const High<T> high) {
   return in_open(x.get(), low.get(), high.get());
}


/// @brief Returns whether x is in the closed interval [low, high].
/// @pre low <= high.
template <Real T>
constexpr StrictBool in_closed(const Strict<T> x, const Strict<T> low, const Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return x >= low && x <= high;
}


/// @copydoc in_closed(Strict<T>, Strict<T>, Strict<T>)
/// @note Named-argument overload using Value, Low, and High wrappers.
template <Real T>
constexpr StrictBool in_closed(const Value<T> x, const Low<T> low, const High<T> high) {
   return in_closed(x.get(), low.get(), high.get());
}


/// @brief Computes base^p by squaring in the precision of base; returns 1 for p == 0.
/// @details Uses O(log(|p| + 1)) multiplications without converting p to floating point,
/// which preserves parity of p and hence the sign of the result for all p, unlike pows_int.
/// For negative exponents, inverts the base first to avoid overflow/underflow
/// from computing the positive power before taking its reciprocal.
/// Results may be less accurate than pows_int but in most cases are expected to have better
/// performance.
template <Floating T>
constexpr Strict<T> fast_pows_int(Strict<T> base, const ImplicitInt p) {
   Strict<T> result = One<T>;
   Strict<unsigned long int> remaining_exponent = detail::unsigned_magnitude(p.get());
   if(p.get() < 0_sl) {
      // For p < 0, evaluate (1 / base)^|p| directly, avoiding the
      // intermediate base^|p| and its potential overflow or underflow.
      // The final result may still overflow or underflow.
      base = invs(base);
   }
   for(;;) {
      if(strict_cast<bool>(remaining_exponent & 1_sul)) {
         result *= base;
      }
      remaining_exponent >>= 1_sul;
      if(!strict_cast<bool>(remaining_exponent)) {
         break;
      }
      base *= base;
   }
   return result;
}


/// @brief Computes base^p using the math library.
/// @details Bases of type float are promoted to double and compute base^p in double precision,
/// whereas other types compute base^p in base precision. Since the integer exponent is
/// converted to the floating type of base(or double if base is float), it may round if not exactly
/// representable. For example, if T is double using IEEE 754 binary64, every integer in
/// [-2^53, 2^53] is exactly representable. Outside that range, exponent conversion can lose parity
/// and change the sign for negative bases. Use fast_pows_int when the full integer exponent must
/// be preserved for large exponents.
template <Floating T>
STRICT_CONSTEXPR_2026 Strict<T> pows_int(const Strict<T> base, const ImplicitInt p) {
   if constexpr(SameAs<T, float>) {
      return pows(base.sd(), p.get().sd()).sf();
   } else {
      return pows(base, strict_cast<T>(p.get()));
   }
}


/// @brief Computes e^p in precision T.
template <Floating T>
STRICT_CONSTEXPR_2026 Strict<T> exps_int(const ImplicitInt p) {
   return exps(strict_cast<T>(p.get()));
}


/// @brief Computes a rounded sum and its rounding residual using the TwoSum algorithm.
/// @return A pair {r, s}, where r is the rounded value of x + y and s is the residual.
/// @details With finite inputs, round-to-nearest arithmetic, gradual underflow, and no
/// intermediate overflow, r + s equals the exact sum in real arithmetic.
template <Floating T>
[[nodiscard]] use::StrictPair<Strict<T>> two_sums(const Strict<T> x, const Strict<T> y) {
   volatile T r = x.val() + y.val();
   volatile T y_approx = r - x.val();
   volatile T x_approx = r - y_approx;
   volatile T y_error = y.val() - y_approx;
   volatile T x_error = x.val() - x_approx;
   T s = x_error + y_error;
   return use::StrictPair<Strict<T>>{r, s};
}


/// @brief Computes a rounded product and its rounding residual using fused multiply-add.
/// @return A pair {r, s}, where r is the rounded value of x * y and s = fmas(x, y, -r).
/// @details The exact residual is e = x * y - r, evaluated in real arithmetic.
/// With finite inputs, round-to-nearest arithmetic, gradual underflow, and no product
/// overflow, s is e rounded to T. If e is exactly representable in T, then s = e
/// and r + s equals the exact product in real arithmetic.
/// Underflow can prevent exact recovery of e.
template <Floating T>
[[nodiscard]] STRICT_CONSTEXPR_2023 use::StrictPair<Strict<T>> two_prods(const Strict<T> x,
                                                                         const Strict<T> y) {
   Strict<T> r = x * y;
   Strict<T> s = fmas(x, y, -r);
   return use::StrictPair<Strict<T>>{r, s};
}


/// @brief Computes x^p with a first-order correction for rounding error.
/// @pre p >= 0.
/// @return A pair {r, s}, where r is the computed power and s approximates x^p - r.
/// Returns {1, 0} for p == 0, including when x == 0.
/// @details Propagates multiplication residuals from two_prods, omitting second-order
/// error terms. The correction s is generally not an exact residual.
///
/// For fixed p, the expected error |(r + s) - x^p| in real arithmetic is O(u^2 * |x|^p),
/// where u is the unit roundoff of T and the error constant depends on p. This estimate
/// assumes the exactness conditions of two_prods and no overflow or underflow in the
/// correction arithmetic. It gives no relative-error guarantee for s itself.
///
/// @note Accumulate r and s separately using compensated summation, for example in
/// polynomial evaluation. Adding them in precision T can round away the correction.
///
/// @par Complexity
/// O(p) time and O(p) temporary storage for p >= 6; no allocation for smaller p.
template <Floating T>
[[nodiscard]] STRICT_CONSTEXPR_2023 use::StrictPair<Strict<T>> pow_prods(const Strict<T> x,
                                                                         const ImplicitInt p) {
   using Pair = use::StrictPair<Strict<T>>;

   ASSERT_STRICT_DEBUG(p.get() >= 0_sl);
   const long int exponent = p.get().val();

   if(exponent == 0) {
      return Pair{One<T>, Zero<T>};

   } else if(exponent == 1) {
      return Pair{x, Zero<T>};
   }

   const auto [sq, sq_res] = two_prods(x, x);
   if(exponent == 2) {
      return Pair{sq, sq_res};

   } else if(exponent == 3) {
      const auto [power, final_res] = two_prods(sq, x);
      return Pair{power, x * sq_res + final_res};

   } else if(exponent == 4) {
      const auto [fourth_power, fourth_res] = two_prods(sq, sq);
      return Pair{fourth_power, strict_cast<T>(2) * sq * sq_res + fourth_res};

   } else if(exponent == 5) {
      const auto [fourth_power, fourth_res] = two_prods(sq, sq);
      const auto [power, final_res] = two_prods(fourth_power, x);
      return Pair{power, strict_cast<T>(2) * x * sq * sq_res + x * fourth_res + final_res};

   } else {
      const long int half_exponent = exponent / 2;
      const long int residual_count = half_exponent - 1;

      // Allocate both temporary arrays in one buffer.
      const auto workspace = std::make_unique<Strict<T>[]>(to_size_t(2 * residual_count));
      // powers[i] approximates sq^i.
      Strict<T>* const powers = workspace.get();
      // residuals[j] is the local residual from multiplying the computed approximation
      // of sq^(j + 1) by sq to obtain that of sq^(j + 2); it excludes earlier errors.
      Strict<T>* const residuals = workspace.get() + residual_count;
      powers[0] = One<T>;
      powers[1] = sq;
      for(auto i = 2L; i < residual_count; ++i) {
         const auto [next_power, step_res] = two_prods(powers[i - 1], sq);
         powers[i] = next_power;
         residuals[i - 2] = step_res;
      }
      const auto [previous_power, previous_res] = two_prods(powers[residual_count - 1], sq);
      residuals[residual_count - 2] = previous_res;
      const auto [even_power, even_res] = two_prods(previous_power, sq);
      residuals[residual_count - 1] = even_res;

      // Weight each residual by the remaining power of sq: the residual at index
      // residual_count - 1 - i has i multiplications by sq remaining.
      Strict<T> correction_sum{};
      for(auto i = 0L; i < residual_count; ++i) {
         correction_sum += powers[i] * residuals[residual_count - 1 - i];
      }

      if(const auto even = exponent % 2 == 0; even) {
         return Pair{even_power,
                     strict_cast<T>(half_exponent) * previous_power * sq_res + correction_sum};
      } else {
         const auto [odd_power, final_res] = two_prods(even_power, x);
         return Pair{odd_power,
                     strict_cast<T>(half_exponent) * x * previous_power * sq_res
                        + x * correction_sum + final_res};
      }
   }
}


} // namespace spp
