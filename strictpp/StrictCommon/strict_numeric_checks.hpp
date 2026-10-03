// Arkadijs Slobodkins, 2023


/// @file strict_numeric_checks.hpp
/// @brief Internal debug checks for integer arithmetic, shift counts, and conversions.


#pragma once


#include "error.hpp"
#include "strict_traits.hpp"

#include <concepts>
#include <limits>
#include <type_traits>


namespace spp {


namespace detail {


template <SignedInteger T>
constexpr void assert_valid_integer_negation([[maybe_unused]] T x) {
   ASSERT_STRICT_DEBUG_MSG(x != std::numeric_limits<T>::min(), "Integer negation overflow.");
}


template <SignedInteger T>
constexpr void assert_valid_integer_increment([[maybe_unused]] T x) {
   ASSERT_STRICT_DEBUG_MSG(x != std::numeric_limits<T>::max(), "Integer increment overflow.");
}


template <SignedInteger T>
constexpr void assert_valid_integer_decrement([[maybe_unused]] T x) {
   ASSERT_STRICT_DEBUG_MSG(x != std::numeric_limits<T>::min(), "Integer decrement overflow.");
}


template <SignedInteger T>
constexpr void assert_valid_integer_addition([[maybe_unused]] T x, [[maybe_unused]] T y) {
   ASSERT_STRICT_DEBUG_MSG(y >= 0 ? x <= std::numeric_limits<T>::max() - y
                                  : x >= std::numeric_limits<T>::min() - y,
                           "Integer addition overflow.");
}


template <SignedInteger T>
constexpr void assert_valid_integer_subtraction([[maybe_unused]] T x, [[maybe_unused]] T y) {
   ASSERT_STRICT_DEBUG_MSG(y >= 0 ? x >= std::numeric_limits<T>::min() + y
                                  : x <= std::numeric_limits<T>::max() + y,
                           "Integer subtraction overflow.");
}


template <SignedInteger T>
constexpr void assert_valid_integer_multiplication([[maybe_unused]] T x, [[maybe_unused]] T y) {
#ifndef STRICT_DEBUG_OFF
   if(x == 0 || y == 0) {
      return;
   }
   constexpr T lowest = std::numeric_limits<T>::min();
   constexpr T highest = std::numeric_limits<T>::max();
   if(x > 0) {
      ASSERT_STRICT_DEBUG_MSG(y > 0 ? x <= highest / y : y >= lowest / x,
                              "Integer multiplication overflow.");
   } else {
      ASSERT_STRICT_DEBUG_MSG(y > 0 ? x >= lowest / y : x >= highest / y,
                              "Integer multiplication overflow.");
   }
#endif
}


template <bool remainder = false, Integer T>
constexpr void assert_valid_integer_division([[maybe_unused]] T x, [[maybe_unused]] T y) {
   ASSERT_STRICT_DEBUG_MSG(y != 0,
                           remainder ? "Modulo division by zero." : "Integer division by zero.");
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_DEBUG_MSG(!(x == std::numeric_limits<T>::min() && y == T(-1)),
                              remainder ? "Integer remainder overflow."
                                        : "Integer division overflow.");
   }
}


template <Integer T>
constexpr void assert_valid_shift_count([[maybe_unused]] T count) {
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_DEBUG_MSG(count >= 0, "Second shift operand has a negative value.");
   }
   ASSERT_STRICT_DEBUG_MSG(count
                              < static_cast<T>(std::numeric_limits<T>::digits + SignedInteger<T>),
                           "Shift count is too large.");
}


// std::integral instead of Integer is used to portably check conversions to std::size_t.
template <std::integral T, Floating U>
   requires(!Boolean<T>)
constexpr void assert_valid_integer_conversion([[maybe_unused]] U x) {
#ifndef STRICT_DEBUG_OFF
   constexpr U upper = static_cast<U>(std::numeric_limits<T>::max() / 2 + 1) * U(2);
   // The upper bound is an exact power of two, even when U cannot represent the integer maximum.
   if constexpr(std::is_signed_v<T>) {
      constexpr U lower = -upper;
      // At low precision, lower - 1 rounds to lower and no valid input lies below lower.
      constexpr U lower_exclusive = lower - U(1);
      ASSERT_STRICT_DEBUG_MSG(x < upper
                                 && (lower_exclusive == lower ? x >= lower : x > lower_exclusive),
                              "Floating-to-integer conversion is out of range or not finite.");
   } else {
      // Negative fractions greater than -1 truncate to zero and are valid unsigned inputs.
      ASSERT_STRICT_DEBUG_MSG(x > U(-1) && x < upper,
                              "Floating-to-integer conversion is out of range or not finite.");
   }
#endif
}


} // namespace detail


} // namespace spp
