// Arkadijs Slobodkins, 2023


#pragma once


#include "strict_traits.hpp"
#include "strict_val.hpp"

#include <limits>
#include <numbers>

#ifdef STRICT_QUAD_PRECISION
#include <quadmath.h>
#endif


namespace spp::constants {


/// @brief Difference between 1 and the next larger representable value of T.
/// @note For binary round-to-nearest arithmetic, unit roundoff is epsilon<T> / 2.
template <Floating T>
constexpr inline Strict<T> epsilon{std::numeric_limits<T>::epsilon()};
#ifdef STRICT_QUAD_PRECISION
// Document float128 specializations explicitly: @copydoc references to the
// primary template were not resolved in CLion's documentation popup.
/// @brief Difference between 1 and the next larger representable float128 value.
/// @note For binary round-to-nearest arithmetic, unit roundoff is epsilon<float128> / 2.
template <>
constexpr inline Strict<float128> epsilon<float128>{FLT128_EPSILON};
#endif

/// @copydoc epsilon
constexpr inline auto epsilon_sf = epsilon<float>;
/// @copydoc epsilon
constexpr inline auto epsilon_sd = epsilon<double>;
/// @copydoc epsilon
constexpr inline auto epsilon_sld = epsilon<long double>;
#ifdef STRICT_QUAD_PRECISION
/// @copydoc epsilon
constexpr inline auto epsilon_sq = epsilon<float128>;
#endif


/// @brief Pi in precision T.
template <Floating T>
constexpr inline Strict<T> pi{std::numbers::pi_v<T>};
#ifdef STRICT_QUAD_PRECISION
/// @brief Pi in float128 precision.
template <>
constexpr inline Strict<float128> pi<float128>{M_PIq};
#endif

/// @copydoc pi
constexpr inline auto pi_sf = pi<float>;
/// @copydoc pi
constexpr inline auto pi_sd = pi<double>;
/// @copydoc pi
constexpr inline auto pi_sld = pi<long double>;
#ifdef STRICT_QUAD_PRECISION
/// @copydoc pi
constexpr inline auto pi_sq = pi<float128>;
#endif


/// @brief Euler's number e, the base of natural logarithms, in precision T.
template <Floating T>
constexpr inline Strict<T> e{std::numbers::e_v<T>};
#ifdef STRICT_QUAD_PRECISION
/// @brief Euler's number e, the base of natural logarithms, in float128 precision.
template <>
constexpr inline Strict<float128> e<float128>{M_Eq};
#endif

/// @copydoc e
constexpr inline auto e_sf = e<float>;
/// @copydoc e
constexpr inline auto e_sd = e<double>;
/// @copydoc e
constexpr inline auto e_sld = e<long double>;
#ifdef STRICT_QUAD_PRECISION
/// @copydoc e
constexpr inline auto e_sq = e<float128>;
#endif


/// @brief Square root of 2 in precision T.
template <Floating T>
constexpr inline Strict<T> sqrt2{std::numbers::sqrt2_v<T>};
#ifdef STRICT_QUAD_PRECISION
/// @brief Square root of 2 in float128 precision.
template <>
constexpr inline Strict<float128> sqrt2<float128>{M_SQRT2q};
#endif

/// @copydoc sqrt2
constexpr inline auto sqrt2_sf = sqrt2<float>;
/// @copydoc sqrt2
constexpr inline auto sqrt2_sd = sqrt2<double>;
/// @copydoc sqrt2
constexpr inline auto sqrt2_sld = sqrt2<long double>;
#ifdef STRICT_QUAD_PRECISION
/// @copydoc sqrt2
constexpr inline auto sqrt2_sq = sqrt2<float128>;
#endif


/// @brief Lowest finite representable value of T.
/// @note Zero for unsigned integers; the most negative value for signed integers;
/// -highest<T> for floating-point types.
template <Real T>
constexpr inline Strict<T> lowest{std::numeric_limits<T>::lowest()};
#ifdef STRICT_QUAD_PRECISION
/// @brief Lowest finite representable float128 value, equal to -highest<float128>.
template <>
constexpr inline Strict<float128> lowest<float128>{-FLT128_MAX};
#endif


/// @brief Highest finite representable value of T.
template <Real T>
constexpr inline Strict<T> highest{std::numeric_limits<T>::max()};
#ifdef STRICT_QUAD_PRECISION
/// @brief Highest finite representable float128 value.
template <>
constexpr inline Strict<float128> highest<float128>{FLT128_MAX};
#endif


} // namespace spp::constants
