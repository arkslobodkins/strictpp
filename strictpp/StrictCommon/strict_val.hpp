// Arkadijs Slobodkins, 2023


/// @file strict_val.hpp
/// @brief `Strict<T>` class, and related operators and conversion utilities.
/// @details Debug checks diagnose undefined behavior (UB) for integer operations and conversions.
/// Operations that produce unsigned wrapping, floating-point infinity and NaN, and conversion
/// losses that don't lead to UB are allowed.


#pragma once


#include "strict_numeric_checks.hpp"
#include "strict_traits.hpp"

#include <cstddef>


namespace spp {


/// @brief Type-safe wrapper for built-in type `T`.
/// @details Copying and assignment require the same `Strict<T>` type. Construction from built-in
/// values and conversion to built-in types require exactly `T` and are explicit. To convert it to
/// another `Strict<U>` or built-in type `U`, use the provided member functions or conversion
/// functions defined later in the file; for example, see `strict_cast`. Assignment, increment,
/// decrement, and compound assignment operators are only allowed for lvalues. Integer types
/// prevent undefined behavior in debug mode, such as protecting against division by 0.
template <Builtin T>
struct [[nodiscard]] alignas(T) Strict {
public:
   using value_type = T;

   constexpr Strict() = default;
   constexpr Strict(const Strict&) = default;
   constexpr explicit Strict(T x) noexcept : val_{x} {
   }
   constexpr Strict(auto) = delete;

   constexpr Strict& operator=(const Strict&) & = default;
   constexpr Strict& operator=(auto) & = delete;

   /// @brief Returns a value of the underlying built-in type.
   /// @note In general, prefer `val()` over the explicit conversion operator in non-templated
   /// contexts (e.g. `x.val()` over `double{x}`) and in expressions such as `x.get().val()`.
   /// Prefer explicit conversion in templated contexts when it makes expressions shorter,
   /// for example `T{x} >= T{y}`.
   [[nodiscard]] constexpr T val() const noexcept {
      return val_;
   }

   /// @copydoc val()
   [[nodiscard]] constexpr explicit operator T() const noexcept {
      return val_;
   }

   constexpr Strict operator+() const noexcept {
      return *this;
   }

   /// @pre For signed integers, the current value is not `std::numeric_limits<T>::min()`.
   constexpr Strict operator-() const noexcept(!SignedInteger<T>)
      requires(!UnsignedInteger<T>)
   {
      if constexpr(SignedInteger<T>) {
         detail::assert_valid_integer_negation(val_);
      }
      // Unary minus preserves T for the supported signed integer and floating-point types.
      return Strict{-val_};
   }

   constexpr Strict operator~() const noexcept
      requires(Integer<T>)
   {
      return Strict<T>{~val_};
   }

   /// @pre For signed integers, the current value is not `std::numeric_limits<T>::max()`.
   constexpr Strict& operator++() & noexcept(!SignedInteger<T>) {
      if constexpr(SignedInteger<T>) {
         detail::assert_valid_integer_increment(val_);
      }
      ++val_;
      return *this;
   }

   /// @pre For signed integers, the current value is not `std::numeric_limits<T>::min()`.
   constexpr Strict& operator--() & noexcept(!SignedInteger<T>) {
      if constexpr(SignedInteger<T>) {
         detail::assert_valid_integer_decrement(val_);
      }
      --val_;
      return *this;
   }

   /// @pre For signed integers, the current value is not `std::numeric_limits<T>::max()`.
   constexpr Strict operator++(int) & noexcept(!SignedInteger<T>) {
      Strict old{*this};
      ++*this;
      return old;
   }

   /// @pre For signed integers, the current value is not `std::numeric_limits<T>::min()`.
   constexpr Strict operator--(int) & noexcept(!SignedInteger<T>) {
      Strict old{*this};
      --*this;
      return old;
   }

   /// @pre For signed integers, the result is representable in `T`.
   constexpr Strict& operator+=(Strict x) & noexcept(!SignedInteger<T>) {
      if constexpr(SignedInteger<T>) {
         detail::assert_valid_integer_addition(val_, x.val_);
      }
      val_ += x.val_;
      return *this;
   }

   /// @pre For signed integers, the result is representable in `T`.
   constexpr Strict& operator-=(Strict x) & noexcept(!SignedInteger<T>) {
      if constexpr(SignedInteger<T>) {
         detail::assert_valid_integer_subtraction(val_, x.val_);
      }
      val_ -= x.val_;
      return *this;
   }

   /// @pre For signed integers, the result is representable in `T`.
   constexpr Strict& operator*=(Strict x) & noexcept(!SignedInteger<T>) {
      if constexpr(SignedInteger<T>) {
         detail::assert_valid_integer_multiplication(val_, x.val_);
      }
      val_ *= x.val_;
      return *this;
   }

   /// @pre For integers, `x` is nonzero and the result is representable in `T`.
   constexpr Strict& operator/=(Strict x) & noexcept(!Integer<T>) {
      if constexpr(Integer<T>) {
         detail::assert_valid_integer_division(val_, x.val_);
      }

      val_ /= x.val_;
      return *this;
   }

   /// @pre `x` is nonzero and the corresponding quotient is representable in `T`.
   constexpr Strict& operator%=(Strict x) &
      requires(Integer<T>)
   {
      detail::assert_valid_integer_division<true>(val_, x.val_);
      val_ %= x.val_;
      return *this;
   }

   /// @pre The shift count is nonnegative and less than
   /// `std::numeric_limits<T>::digits + SignedInteger<T>`.
   /// Valid shift counts are `[0, 31]` for 32-bit integers and `[0, 63]` for 64-bit integers.
   /// @note Since C++20, negative signed left operands are allowed and bits shifted beyond the
   /// type's width are discarded. For a 32-bit `int`, `INT_MAX << 25` is defined and evaluates to
   /// `-33554432`.
   constexpr Strict& operator<<=(Strict x) &
      requires(Integer<T>)
   {
      detail::assert_valid_shift_count(x.val_);
      val_ <<= x.val_;
      return *this;
   }

   /// @pre The shift count is nonnegative and less than
   /// `std::numeric_limits<T>::digits + SignedInteger<T>`.
   /// Valid shift counts are `[0, 31]` for 32-bit integers and `[0, 63]` for 64-bit integers.
   /// @note Since C++20, negative signed left operands are allowed; signed right shift rounds
   /// toward negative infinity.
   constexpr Strict& operator>>=(Strict x) &
      requires(Integer<T>)
   {
      detail::assert_valid_shift_count(x.val_);
      val_ >>= x.val_;
      return *this;
   }

   constexpr Strict& operator&=(Strict x) & noexcept
      requires(Integer<T>)
   {
      val_ &= x.val_;
      return *this;
   }

   constexpr Strict& operator|=(Strict x) & noexcept
      requires(Integer<T>)
   {
      val_ |= x.val_;
      return *this;
   }

   constexpr Strict& operator^=(Strict x) & noexcept
      requires(Integer<T>)
   {
      val_ ^= x.val_;
      return *this;
   }

   constexpr Strict<bool> sb() const noexcept;
   constexpr Strict<int> si() const noexcept(!Floating<T>);
   constexpr Strict<long int> sl() const noexcept(!Floating<T>);
   constexpr Strict<unsigned int> sui() const noexcept(!Floating<T>);
   constexpr Strict<unsigned long int> sul() const noexcept(!Floating<T>);
   constexpr Strict<float> sf() const noexcept;
   constexpr Strict<double> sd() const noexcept;
   constexpr Strict<long double> sld() const noexcept;
#ifdef STRICT_QUAD_PRECISION
   constexpr Strict<float128> sq() const noexcept;
#endif

private:
   T val_{};
};


/// @brief Boolean specialization of `Strict<T>`. Unlike built-in `bool`, no arithmetic operations
/// are allowed.
/// @note Overloaded `&&` and `||` evaluate both operand expressions without short-circuiting.
/// To support interoperability with the standard library, conversion to `bool` is implicit.
template <>
struct [[nodiscard]] alignas(bool) Strict<bool> {
public:
   using value_type = bool;

   constexpr Strict() = default;
   constexpr Strict(const Strict&) = default;
   constexpr explicit Strict(bool x) noexcept : val_{x} {
   }
   constexpr Strict(auto) = delete;

   constexpr Strict& operator=(const Strict&) & = default;
   constexpr Strict& operator=(auto) & = delete;

   /// @brief Returns the underlying Boolean value.
   /// @see Strict<T>::val() for guidance on choosing between `val()` and conversion.
   [[nodiscard]] constexpr bool val() const noexcept {
      return val_;
   }


   /// @copybrief val()
   /// @details Implicit conversion lets comparison results satisfy the syntactic requirements
   /// of `boolean-testable` and supports standard library algorithms (e.g. `std::any_of`).
   /// Constraining the conversion to `Boolean` prevents implicit numeric conversions.
   /// @note `StrictBool` satisfies the compile-time checks of `boolean-testable`, but its
   /// overloaded `&&` and `||` do not provide the required short-circuiting behavior.
   /// @see Strict<T>::val() for guidance on choosing between `val()` and conversion.
   template <Boolean T>
   [[nodiscard]] constexpr operator T() const noexcept {
      return val_;
   }

   constexpr Strict operator!() const noexcept {
      return Strict{!val_};
   }

   constexpr Strict<bool> sb() const noexcept;
   constexpr Strict<int> si() const noexcept;
   constexpr Strict<long int> sl() const noexcept;
   constexpr Strict<unsigned int> sui() const noexcept;
   constexpr Strict<unsigned long int> sul() const noexcept;
   constexpr Strict<float> sf() const noexcept;
   constexpr Strict<double> sd() const noexcept;
   constexpr Strict<long double> sld() const noexcept;
#ifdef STRICT_QUAD_PRECISION
   constexpr Strict<float128> sq() const noexcept;
#endif

private:
   bool val_{};
};


// Conversion utilities for Strict types.

/// @brief Converts a supported built-in value of type `U` to `T` using `static_cast` and
/// detects conversions that cause UB in debug mode.
/// @pre For a floating-point source and an `Integer` destination, the input is finite and its
/// truncated value is representable in `T`.
/// @note Converting a larger signed or unsigned integer to a smaller signed integer does not
/// cause UB in C++20. These conversions are unchecked and may change the value or its sign.
template <Builtin T, Builtin U>
[[nodiscard]] constexpr T builtin_cast(U x) noexcept(!(Integer<T> && Floating<U>)) {
   if constexpr(Integer<T> && Floating<U>) {
      detail::assert_valid_integer_conversion<T>(x);
   }
   return static_cast<T>(x);
}


/// @copydoc builtin_cast(U)
template <Builtin T, Builtin U>
[[nodiscard]] constexpr T builtin_cast(Strict<U> x) noexcept(!(Integer<T> && Floating<U>)) {
   return builtin_cast<T>(x.val());
}


/// @brief Converts between supported real types.
/// @details Reuses `builtin_cast`.
template <Real T, Real U>
[[nodiscard]] constexpr T real_cast(U x) noexcept(!(Integer<T> && Floating<U>)) {
   return builtin_cast<T>(x);
}


/// @copydoc real_cast(U)
template <Real T, Real U>
[[nodiscard]] constexpr T real_cast(Strict<U> x) noexcept(!(Integer<T> && Floating<U>)) {
   return real_cast<T>(x.val());
}


/// @brief Converts a supported built-in value to `Strict<T>`.
/// @details Equivalent to `Strict<T>{builtin_cast<T>(x)}`.
template <Builtin T, Builtin U>
constexpr Strict<T> strict_cast(U x) noexcept(!(Integer<T> && Floating<U>)) {
   return Strict{builtin_cast<T>(x)};
}


/// @copydoc strict_cast(U)
template <Builtin T, Builtin U>
constexpr Strict<T> strict_cast(Strict<U> x) noexcept(!(Integer<T> && Floating<U>)) {
   return strict_cast<T>(x.val());
}


/// @brief Converts a supported built-in value of type `T` to `std::size_t` using `static_cast` and
/// detects conversions that cause UB in debug mode.
/// @pre Floating-point inputs are finite and their truncated value is representable in
/// `std::size_t`.
/// @note Negative integer inputs wrap. Floating-point inputs in `(-1, 0)` truncate to zero.
template <Builtin T>
[[nodiscard]] constexpr std::size_t to_size_t(T x) noexcept(!Floating<T>) {
   if constexpr(Floating<T>) {
      detail::assert_valid_integer_conversion<std::size_t>(x);
   }
   return static_cast<std::size_t>(x);
}


/// @copydoc to_size_t(T)
template <Builtin T>
[[nodiscard]] constexpr std::size_t to_size_t(Strict<T> x) noexcept(!Floating<T>) {
   return to_size_t(x.val());
}


/// @brief Converts a supported built-in value of type `T` to `index_t` and detects conversions that
/// cause UB in debug mode.
/// @pre Floating-point inputs are finite and their truncated value is representable in
/// `index_t::value_type`.
/// @details Uses the floating-to-integer debug checks of `strict_cast`.
/// @note Negative values and defined conversion losses are allowed.
template <Builtin T>
constexpr index_t to_index_t(T x) noexcept(!Floating<T>) {
   return strict_cast<index_t::value_type>(x);
}


/// @copydoc to_index_t(T)
template <Builtin T>
constexpr index_t to_index_t(Strict<T> x) noexcept(!Floating<T>) {
   return to_index_t(x.val());
}


/// @brief Converts an integer value to `Strict<T>` for a floating-point type `T`.
/// @note Uses unchecked `static_cast`; integer values not exactly representable in `T`
/// may be rounded.
template <Floating T, Integer U>
constexpr Strict<T> integer_to_floating(U x) noexcept {
   return strict_cast<T, U>(x);
}


/// @copydoc integer_to_floating(U)
template <Floating T, Integer U>
constexpr Strict<T> integer_to_floating(Strict<U> x) noexcept {
   return integer_to_floating<T>(x.val());
}


// Conversion members for primary template types.

template <Builtin T>
constexpr Strict<bool> Strict<T>::sb() const noexcept {
   return strict_cast<bool>(val_);
}


template <Builtin T>
constexpr Strict<int> Strict<T>::si() const noexcept(!Floating<T>) {
   return strict_cast<int>(val_);
}


template <Builtin T>
constexpr Strict<long int> Strict<T>::sl() const noexcept(!Floating<T>) {
   return strict_cast<long int>(val_);
}


template <Builtin T>
constexpr Strict<unsigned int> Strict<T>::sui() const noexcept(!Floating<T>) {
   return strict_cast<unsigned int>(val_);
}


template <Builtin T>
constexpr Strict<unsigned long int> Strict<T>::sul() const noexcept(!Floating<T>) {
   return strict_cast<unsigned long int>(val_);
}


template <Builtin T>
constexpr Strict<float> Strict<T>::sf() const noexcept {
   return strict_cast<float>(val_);
}


template <Builtin T>
constexpr Strict<double> Strict<T>::sd() const noexcept {
   return strict_cast<double>(val_);
}


template <Builtin T>
constexpr Strict<long double> Strict<T>::sld() const noexcept {
   return strict_cast<long double>(val_);
}


#ifdef STRICT_QUAD_PRECISION
template <Builtin T>
constexpr Strict<float128> Strict<T>::sq() const noexcept {
   return strict_cast<float128>(val_);
}
#endif


// Conversion members for boolean template specialization.

constexpr Strict<bool> Strict<bool>::sb() const noexcept {
   return strict_cast<bool>(val_);
}


constexpr Strict<int> Strict<bool>::si() const noexcept {
   return strict_cast<int>(val_);
}


constexpr Strict<long int> Strict<bool>::sl() const noexcept {
   return strict_cast<long int>(val_);
}


constexpr Strict<unsigned int> Strict<bool>::sui() const noexcept {
   return strict_cast<unsigned int>(val_);
}


constexpr Strict<unsigned long int> Strict<bool>::sul() const noexcept {
   return strict_cast<unsigned long int>(val_);
}


constexpr Strict<float> Strict<bool>::sf() const noexcept {
   return strict_cast<float>(val_);
}


constexpr Strict<double> Strict<bool>::sd() const noexcept {
   return strict_cast<double>(val_);
}


constexpr Strict<long double> Strict<bool>::sld() const noexcept {
   return strict_cast<long double>(val_);
}


#ifdef STRICT_QUAD_PRECISION
constexpr Strict<float128> Strict<bool>::sq() const noexcept {
   return strict_cast<float128>(val_);
}
#endif


// Arithmetic operators.

/// @pre For signed integers, the result is representable in `T`.
/// @details Reuses `Strict<T>::operator+=`.
template <Real T>
constexpr Strict<T> operator+(Strict<T> x, Strict<T> y) noexcept(!SignedInteger<T>) {
   return x += y;
}


/// @pre For signed integers, the result is representable in `T`.
/// @details Reuses `Strict<T>::operator-=`.
template <Real T>
constexpr Strict<T> operator-(Strict<T> x, Strict<T> y) noexcept(!SignedInteger<T>) {
   return x -= y;
}


/// @pre For signed integers, the result is representable in `T`.
/// @details Reuses `Strict<T>::operator*=`.
template <Real T>
constexpr Strict<T> operator*(Strict<T> x, Strict<T> y) noexcept(!SignedInteger<T>) {
   return x *= y;
}


/// @pre For integers, `y` is nonzero and the result is representable in `T`.
/// @details Reuses `Strict<T>::operator/=`.
template <Real T>
constexpr Strict<T> operator/(Strict<T> x, Strict<T> y) noexcept(!Integer<T>) {
   return x /= y;
}


/// @pre `y` is nonzero and the corresponding quotient is representable in `T`.
/// @details Reuses `Strict<T>::operator%=`.
template <Integer T>
constexpr Strict<T> operator%(Strict<T> x, Strict<T> y) {
   return x %= y;
}


/// @copydoc Strict<T>::operator<<=
/// @details Reuses `Strict<T>::operator<<=`.
template <Integer T>
constexpr Strict<T> operator<<(Strict<T> x, Strict<T> y) {
   return x <<= y;
}


/// @copydoc Strict<T>::operator>>=
/// @details Reuses `Strict<T>::operator>>=`.
template <Integer T>
constexpr Strict<T> operator>>(Strict<T> x, Strict<T> y) {
   return x >>= y;
}


template <Integer T>
constexpr Strict<T> operator&(Strict<T> x, Strict<T> y) noexcept {
   return x &= y;
}


template <Integer T>
constexpr Strict<T> operator|(Strict<T> x, Strict<T> y) noexcept {
   return x |= y;
}


template <Integer T>
constexpr Strict<T> operator^(Strict<T> x, Strict<T> y) noexcept {
   return x ^= y;
}


/// @brief Returns logical XOR as `StrictBool`.
template <Boolean T>
constexpr Strict<T> operator^(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} != T{y}};
}


/// @brief Returns logical AND as `StrictBool`.
/// @note Unlike the built-in `&&` operator, this overload does not short-circuit:
/// both operand expressions are evaluated before the function is called.
template <Boolean T>
constexpr StrictBool operator&&(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{bool{x} && bool{y}};
}


/// @brief Returns logical OR as `StrictBool`.
/// @note Unlike the built-in `||` operator, this overload does not short-circuit:
/// both operand expressions are evaluated before the function is called.
template <Boolean T>
constexpr StrictBool operator||(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{bool{x} || bool{y}};
}


// Comparison operators.

template <Builtin T>
constexpr StrictBool operator==(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} == T{y}};
}


/// @brief Named equality comparison, equivalent to `x == y`.
template <Builtin T>
constexpr StrictBool equal(Strict<T> x, Strict<T> y) noexcept {
   return x == y;
}


template <Builtin T>
constexpr StrictBool operator!=(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} != T{y}};
}


template <Builtin T>
constexpr StrictBool operator<(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} < T{y}};
}


template <Builtin T>
constexpr StrictBool operator<=(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} <= T{y}};
}


template <Builtin T>
constexpr StrictBool operator>(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} > T{y}};
}


template <Builtin T>
constexpr StrictBool operator>=(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} >= T{y}};
}


} // namespace spp
