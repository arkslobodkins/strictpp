// Arkadijs Slobodkins, 2023


#pragma once


#include "common_traits.hpp"
#include "config.hpp"
#include "error.hpp"
#include "strict_traits.hpp"

#include <cstddef>
#include <limits>


namespace spp {


class ImplicitBool;


template <Builtin T>
struct [[nodiscard]] alignas(T) Strict {
public:
   using value_type = T;

   constexpr Strict() = default;

   constexpr Strict(const Strict&) = default;

   constexpr explicit Strict(T x) noexcept : val_{x} {
   }

   constexpr Strict(auto x) = delete;

   constexpr Strict& operator=(const Strict&) & = default;

   [[nodiscard]] constexpr T val() const noexcept {
      return val_;
   }

   [[nodiscard]] constexpr explicit operator T() const noexcept {
      return val_;
   }

   constexpr Strict operator+() const noexcept {
      return *this;
   }

   constexpr Strict operator-() const noexcept
      requires(!UnsignedInteger<T>)
   {
      // Unary minus preserves T for the supported signed integer and floating-point types.
      return Strict{-val_};
   }

   constexpr Strict operator~() const noexcept
      requires(Integer<T>)
   {
      return Strict<T>{~val_};
   }

   constexpr Strict& operator++() & noexcept {
      ++val_;
      return *this;
   }

   constexpr Strict& operator--() & noexcept {
      --val_;
      return *this;
   }

   constexpr Strict operator++(int) & noexcept {
      Strict old{val_};
      ++val_;
      return old;
   }

   constexpr Strict operator--(int) & noexcept {
      Strict old{val_};
      --val_;
      return old;
   }

   constexpr Strict& operator+=(Strict x) & noexcept {
      val_ += x.val_;
      return *this;
   }

   constexpr Strict& operator-=(Strict x) & noexcept {
      val_ -= x.val_;
      return *this;
   }

   constexpr Strict& operator*=(Strict x) & noexcept {
      val_ *= x.val_;
      return *this;
   }

   constexpr Strict& operator/=(Strict x) & {
      if constexpr(Integer<T>) {
         ASSERT_STRICT_DEBUG_MSG(x.val_ != 0, "Integer division by zero.");
         if constexpr(SignedInteger<T>) {
            ASSERT_STRICT_DEBUG_MSG(!(val_ == std::numeric_limits<T>::min() && x.val_ == T(-1)),
                                    "Integer division overflow.");
         }
      }

      val_ /= x.val_;
      return *this;
   }

   constexpr Strict& operator%=(Strict x) &
      requires(Integer<T>)
   {
      ASSERT_STRICT_DEBUG_MSG(x.val_ != 0, "Modulo division by zero.");
      if constexpr(SignedInteger<T>) {
         ASSERT_STRICT_DEBUG_MSG(!(val_ == std::numeric_limits<T>::min() && x.val_ == T(-1)),
                                 "Integer remainder overflow.");
      }
      val_ %= x.val_;
      return *this;
   }

   constexpr Strict& operator<<=(Strict x) &
      requires(Integer<T>)
   {
      if constexpr(SignedInteger<T>) {
         ASSERT_STRICT_DEBUG_MSG(val_ > -1, "First shift operand has a negative value.");
         ASSERT_STRICT_DEBUG_MSG(x.val_ > -1, "Second shift operand has a negative value.");
      }
      ASSERT_STRICT_DEBUG_MSG(
         x.val_ < static_cast<T>(std::numeric_limits<T>::digits + SignedInteger<T>),
         "Shift count is too large.");
      val_ <<= x.val_;
      return *this;
   }

   constexpr Strict& operator>>=(Strict x) &
      requires(Integer<T>)
   {
      if constexpr(SignedInteger<T>) {
         ASSERT_STRICT_DEBUG_MSG(val_ > -1, "First shift operand has a negative value.");
         ASSERT_STRICT_DEBUG_MSG(x.val_ > -1, "Second shift operand has a negative value.");
      }
      ASSERT_STRICT_DEBUG_MSG(
         x.val_ < static_cast<T>(std::numeric_limits<T>::digits + SignedInteger<T>),
         "Shift count is too large.");
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
   T val_{};
};


template <>
struct [[nodiscard]] alignas(bool) Strict<bool> {
public:
   using value_type = bool;

   constexpr Strict() = default;

   constexpr Strict(const Strict&) = default;

   constexpr explicit Strict(bool x) noexcept : val_{x} {
   }

   constexpr Strict(auto x) = delete;

   constexpr Strict& operator=(const Strict&) & = default;

   [[nodiscard]] constexpr bool val() const noexcept {
      return val_;
   }

   // Intentionally implicit to satisfy the bool-conversion requirement of std::predicate
   // and standard-library ranges algorithms.
   [[nodiscard]] constexpr operator bool() const noexcept {
      return val_;
   }

   // ImplicitBool must be excluded from the deleted overload, otherwise this overload would be
   // an equally good candidate as templated constructor for ImplicitBool(StrictBool).
   template <typename T>
      requires(!SameAs<ImplicitBool, T>)
   constexpr operator T() const = delete;

   constexpr Strict operator!() const noexcept {
      return Strict{!val_};
   }

   // Function definitions must be implemented outside of struct definition to avoid incomplete
   // types.
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


// Conversion utilities for Strict types. Analogous to static_cast, but
// use concept constraints for additional safety.
template <Builtin T, Builtin U>
[[nodiscard]] constexpr T builtin_cast(U x) noexcept {
   return static_cast<T>(x);
}


template <Builtin T, Builtin U>
[[nodiscard]] constexpr T builtin_cast(Strict<U> x) noexcept {
   return builtin_cast<T>(x.val());
}


template <Real T, Real U>
[[nodiscard]] constexpr T real_cast(U x) noexcept {
   return static_cast<T>(x);
}


template <Real T, Real U>
[[nodiscard]] constexpr T real_cast(Strict<U> x) noexcept {
   return real_cast<T>(x.val());
}


template <Builtin T, Builtin U>
constexpr Strict<T> strict_cast(U x) noexcept {
   return Strict{builtin_cast<T>(x)};
}


template <Builtin T, Builtin U>
constexpr Strict<T> strict_cast(Strict<U> x) noexcept {
   return strict_cast<T>(x.val());
}


template <Builtin T>
[[nodiscard]] constexpr std::size_t to_size_t(T x) noexcept {
   return static_cast<std::size_t>(x);
}


template <Builtin T>
[[nodiscard]] constexpr std::size_t to_size_t(Strict<T> x) noexcept {
   return to_size_t(x.val());
}


template <Builtin T>
constexpr index_t to_index_t(T x) noexcept {
   return strict_cast<index_t::value_type>(x);
}


template <Builtin T>
constexpr index_t to_index_t(Strict<T> x) noexcept {
   return to_index_t(x.val());
}


template <Floating T, Integer U>
constexpr Strict<T> integer_to_floating(U x) noexcept {
   return strict_cast<T, U>(x);
}


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
constexpr Strict<int> Strict<T>::si() const noexcept {
   return strict_cast<int>(val_);
}


template <Builtin T>
constexpr Strict<long int> Strict<T>::sl() const noexcept {
   return strict_cast<long int>(val_);
}


template <Builtin T>
constexpr Strict<unsigned int> Strict<T>::sui() const noexcept {
   return strict_cast<unsigned int>(val_);
}


template <Builtin T>
constexpr Strict<unsigned long int> Strict<T>::sul() const noexcept {
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
template <Real T>
constexpr Strict<T> operator+(Strict<T> x, Strict<T> y) noexcept {
   return x += y;
}


template <Real T>
constexpr Strict<T> operator-(Strict<T> x, Strict<T> y) noexcept {
   return x -= y;
}


template <Real T>
constexpr Strict<T> operator*(Strict<T> x, Strict<T> y) noexcept {
   return x *= y;
}


template <Real T>
constexpr Strict<T> operator/(Strict<T> x, Strict<T> y) {
   return x /= y;
}


template <Integer T>
constexpr Strict<T> operator%(Strict<T> x, Strict<T> y) {
   return x %= y;
}


template <Integer T>
constexpr Strict<T> operator<<(Strict<T> x, Strict<T> y) {
   return x <<= y;
}


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


template <Boolean T>
constexpr Strict<T> operator^(Strict<T> x, Strict<T> y) noexcept {
   return strict_cast<bool>(bool{x} ^ bool{y});
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note Unlike the built-in `&&` operator, this overload does not short-circuit:
/// both operand expressions are evaluated before the function is called.
template <Boolean T>
constexpr StrictBool operator&&(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{bool{x} && bool{y}};
}


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


template <Builtin T>
constexpr StrictBool equal(Strict<T> x, Strict<T> y) noexcept {
   return StrictBool{T{x} == T{y}};
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
