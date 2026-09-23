// Arkadijs Slobodkins, 2023


/// @file strict_literals.hpp
/// @brief User-defined literals for constructing `Strict<T>` values with range checks.
/// @details Provides additional safety compared to C++ numeric literals.
/// In particular, integer and standard floating-point literals reject out-of-range values
/// at compile time. The `_sf` and `_sd` literals also detect underflow to zero during
/// conversion from `long double` at compile time. The `_sq` literal performs its conversion
/// and range checks at runtime.


#pragma once


#include "error.hpp"
#include "strict_constants.hpp"
#include "strict_traits.hpp"
#include "strict_val.hpp"


#ifdef STRICT_QUAD_PRECISION
#include <algorithm>
#include <array>
#include <cerrno>
#include <cfenv>
#include <cstddef>
#include <iterator>
// Use <locale.h> for POSIX thread-local locale APIs, which <clocale> does not guarantee.
#include <locale.h>
#include <quadmath.h>
#include <string>
#include <string_view>
#endif


namespace spp::detail {


template <Integer T>
constexpr inline unsigned long long literal_max =
   static_cast<unsigned long long>(constants::highest<T>.val());


} // namespace spp::detail


namespace spp::inline literals {


/// @brief Converts an integer literal to `Strict<int>`.
/// @pre `x` is within the range of `int`.
/// @details Out-of-range values are rejected at compile time.
consteval Strict<int> operator""_si(unsigned long long x) {
   ASSERT_STRICT_ALWAYS_MSG(x <= detail::literal_max<int>, "int literal is out of range.");
   return Strict{static_cast<int>(x)};
}


/// @brief Converts an integer literal to `Strict<long int>`.
/// @pre `x` is within the range of `long int`.
/// @details Out-of-range values are rejected at compile time.
consteval Strict<long int> operator""_sl(unsigned long long x) {
   ASSERT_STRICT_ALWAYS_MSG(x <= detail::literal_max<long int>,
                            "long int literal is out of range.");
   return Strict{static_cast<long int>(x)};
}


/// @brief Converts an integer literal to `Strict<unsigned int>`.
/// @pre `x` is within the range of `unsigned int`.
/// @details Out-of-range values are rejected at compile time.
consteval Strict<unsigned int> operator""_sui(unsigned long long x) {
   ASSERT_STRICT_ALWAYS_MSG(x <= detail::literal_max<unsigned int>,
                            "unsigned int literal is out of range.");
   return Strict{static_cast<unsigned int>(x)};
}


/// @brief Converts an integer literal to `Strict<unsigned long int>`.
/// @pre `x` is within the range of `unsigned long int`.
/// @details Out-of-range values are rejected at compile time.
consteval Strict<unsigned long int> operator""_sul(unsigned long long x) {
   ASSERT_STRICT_ALWAYS_MSG(x <= detail::literal_max<unsigned long int>,
                            "unsigned long int literal is out of range.");
   return Strict{static_cast<unsigned long int>(x)};
}


/// @brief Converts a floating-point literal to `Strict<float>`.
/// @pre `x` is within the finite `float` range and conversion does not underflow to zero.
/// @details Out-of-range values and underflow to zero during conversion to `float` are
/// rejected at compile time.
/// @note The compiler first rounds the literal to `long double`;
/// underflow to zero during that initial conversion cannot be detected here.
consteval Strict<float> operator""_sf(long double x) {
   ASSERT_STRICT_ALWAYS_MSG(constants::lowest<float>.val() <= x
                               && x <= constants::highest<float>.val(),
                            "float literal is out of range.");
   const auto converted = static_cast<float>(x);
   ASSERT_STRICT_ALWAYS_MSG(!(converted == 0.0F && x != 0.0L),
                            "float literal underflowed to zero.");
   return Strict{converted};
}


/// @brief Converts a floating-point literal to `Strict<double>`.
/// @pre `x` is within the finite `double` range and conversion does not underflow to zero.
/// @details Out-of-range values and underflow to zero during conversion to `double` are
/// rejected at compile time.
/// @note The compiler first rounds the literal to `long double`;
/// underflow to zero during that initial conversion cannot be detected here.
consteval Strict<double> operator""_sd(long double x) {
   ASSERT_STRICT_ALWAYS_MSG(constants::lowest<double>.val() <= x
                               && x <= constants::highest<double>.val(),
                            "double literal is out of range.");
   const auto converted = static_cast<double>(x);
   ASSERT_STRICT_ALWAYS_MSG(!(converted == 0.0 && x != 0.0L),
                            "double literal underflowed to zero.");
   return Strict{converted};
}


/// @brief Converts a floating-point literal to `Strict<long double>`.
/// @pre `x` is finite.
/// @details Infinity and NaN are rejected at compile time.
consteval Strict<long double> operator""_sl(long double x) {
   ASSERT_STRICT_ALWAYS_MSG(constants::lowest<long double>.val() <= x
                               && x <= constants::highest<long double>.val(),
                            "long double literal must be finite.");
   return Strict{x};
}


} // namespace spp::inline literals


#ifdef STRICT_QUAD_PRECISION
namespace spp::detail {


struct ErrnoGuard {
   const int saved_errno = errno;

   ~ErrnoGuard() {
      errno = saved_errno;
   }
};


struct FloatingEnvironmentGuard {
   std::string_view literal;
   std::fenv_t saved_environment{};
   bool saved = false;

   ~FloatingEnvironmentGuard() {
      if(saved) {
         // Restoration failure must not throw, even during exception unwinding.
         ASSERT_STRICT_ABORT_ALWAYS_MSG(
            std::fesetenv(&saved_environment) == 0,
            "Could not restore the floating-point environment for float128 literal '"
               + std::string{literal} + "'.");
      }
   }
};


struct LocaleGuard {
   std::string_view literal;
   locale_t c_locale = nullptr;
   locale_t previous_locale = nullptr;

   ~LocaleGuard() {
      if(previous_locale != nullptr) {
         // Do not free a locale that may still be selected by this thread.
         ASSERT_STRICT_ABORT_ALWAYS_MSG(uselocale(previous_locale) != nullptr,
                                        "Could not restore the locale for float128 literal '"
                                           + std::string{literal} + "'.");
      }
      if(c_locale != nullptr) {
         freelocale(c_locale);
      }
   }
};


/// @param str Null-terminated literal text with digit separators removed.
/// @param literal Original literal text, including digit separators, used only for diagnostics.
inline float128 parse_float128_literal(const char* str, const std::string_view literal) {
   // Declare first so errno is restored after the other guards finish cleanup.
   ErrnoGuard errno_guard;
   FloatingEnvironmentGuard environment_guard{.literal = literal};

   // Save a backup before std::feholdexcept can partially change the environment and fail.
   environment_guard.saved = std::fegetenv(&environment_guard.saved_environment) == 0;
   ASSERT_STRICT_ALWAYS_MSG(environment_guard.saved,
                            "Could not save the floating-point environment for float128 literal '"
                               + std::string{literal} + "'.");
   // Suppress traps and discard flags raised by parsing or validation on scope exit.
   std::fenv_t held_environment{};
   const int hold_status = std::feholdexcept(&held_environment);
   ASSERT_STRICT_ALWAYS_MSG(hold_status == 0,
                            "Could not suppress floating-point traps for float128 literal '"
                               + std::string{literal} + "'.");
   const int rounding_status = std::fesetround(FE_TONEAREST);
   ASSERT_STRICT_ALWAYS_MSG(rounding_status == 0,
                            "Could not select round-to-nearest for float128 literal '"
                               + std::string{literal} + "'.");

   char* end = nullptr;
   float128 x{};
   bool range_error = false;
   {
      LocaleGuard locale_guard{literal};

      // Select the C numeric locale for this thread without changing the global locale.
      locale_guard.c_locale = newlocale(LC_NUMERIC_MASK, "C", nullptr);
      ASSERT_STRICT_ALWAYS_MSG(locale_guard.c_locale != nullptr,
                               "Could not create the C numeric locale for float128 literal '"
                                  + std::string{literal} + "'.");
      locale_guard.previous_locale = uselocale(locale_guard.c_locale);
      ASSERT_STRICT_ALWAYS_MSG(locale_guard.previous_locale != nullptr,
                               "Could not select the C numeric locale for float128 literal '"
                                  + std::string{literal} + "'.");
      errno = 0;
      x = strtoflt128(str, &end);
      range_error = errno == ERANGE;
   }
   // The caller's locale is restored before any validation can fail.
   ASSERT_STRICT_ALWAYS_MSG(end != str && *end == '\0',
                            "float128 literal '" + std::string{literal}
                               + "' must be parsed completely.");
   ASSERT_STRICT_ALWAYS_MSG(constants::lowest<float128>.val() <= x
                               && x <= constants::highest<float128>.val(),
                            "float128 literal '" + std::string{literal} + "' is out of range.");
   // ERANGE also reports rounding of subnormal values, which remain valid.
   ASSERT_STRICT_ALWAYS_MSG(!range_error || (x != 0 && x >= -FLT128_MIN && x <= FLT128_MIN),
                            "float128 literal '" + std::string{literal}
                               + "' overflowed or underflowed to zero.");
   return x;
}


} // namespace spp::detail


namespace spp::inline literals {


/// @brief Converts a floating-point literal to `Strict<float128>` at runtime.
/// @pre The literal is within the finite `float128` range and does not underflow to zero.
/// @details This operator is not `constexpr` and cannot be used in constant expressions.
/// Parses using `FE_TONEAREST` regardless of the caller's rounding mode. Parsing temporarily
/// disables floating-point traps. The caller's floating-point environment,
/// including rounding mode, exception flags, and trap settings, is restored on return
/// and when an exception is thrown. Out-of-range values and underflow to zero are rejected.
/// Failure to restore the locale or floating-point environment aborts even when exceptions
/// are enabled.
template <char... chars>
Strict<float128> operator""_sq() {
   static constexpr char literal_str[] = {chars..., '\0'};
   // Exclude C++ digit separators since strtoflt128 does not recognize them.
   static constexpr auto filtered_str = [] {
      // Include one character for the null terminator.
      constexpr std::size_t filtered_size = (std::size_t{1} + ... + (chars != '\''));
      std::array<char, filtered_size> result{};
      std::copy_if(std::begin(literal_str), std::end(literal_str), result.begin(), [](char c) {
         return c != '\'';
      });
      return result;
   }();

   // Hexadecimal 'e'/'E' are digits; hexadecimal floats require a 'p'/'P' exponent.
   constexpr bool is_hex = filtered_str.size() > 2 && filtered_str[0] == '0'
                        && (filtered_str[1] == 'x' || filtered_str[1] == 'X');
   constexpr bool has_floating_syntax = is_hex
                                         ? ((chars == 'p' || chars == 'P') || ...)
                                         : ((chars == '.' || chars == 'e' || chars == 'E') || ...);
   static_assert(has_floating_syntax, "A float128 literal requires floating-point syntax.");

   return Strict{detail::parse_float128_literal(filtered_str.data(),
                                                std::string_view{literal_str, sizeof...(chars)})};
}


} // namespace spp::inline literals
#endif
