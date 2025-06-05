// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "../derived2D.hpp"
#include "expr_traits.hpp"
#include "functors.hpp"
#include "special.hpp"
#include "types.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations.
template <BaseType Base1, BaseType Base2, typename F, bool copy_delete = false>
   requires expr::BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate(const Base1& A1, const Base2& A2, F f);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator&&(const Base1& A1, const Base2& A2);


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator||(const Base1& A1, const Base2& A2);


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base1, FloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base1, SignedIntegerBaseType Base2>
auto pow_prod(const Base1& A1, const Base2& A2);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
// Its seems that checking for the first set of conditions, namely:
// (RealBaseType<RemoveRef<Base1>> && RealBaseType<RemoveRef<Base2>>)
// is redundant, but removing it would lead to ambiguities instead of selecting
// a deleted overload in some cases, such as Array1D<double>(3) + 1._sd.


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator+(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator-(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator*(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator/(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator%(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator<<(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator>>(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator|(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::BooleanExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator&&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::BooleanExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator||(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::BooleanExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::FloatingExprDeleted<Base1, Base2>)
auto two_prod(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::FloatingExprDeleted<Base1, Base2>)
auto pow_prod(Base1&& A1, Base2&& A2) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations(scalars on the left).
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator&&(ValueTypeOf<Base> x, const Base& A);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator||(ValueTypeOf<Base> x, const Base& A);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&&(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator||(ValueTypeOf<Base> x, Base&& A) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, Base&& A) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations(scalars on the right).
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator&&(const Base& A, ValueTypeOf<Base> x);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator||(const Base& A, ValueTypeOf<Base> x);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator*(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator/(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator%(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator<<(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator>>(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator|(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&&(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator||(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(Base&& A, ValueTypeOf<Base> x) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto matvec_prod(const Base1& A, const Base2& x);


template <typename Base1, typename Base2>
   requires TwoDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>
             && (detail::ArrayRealTypeRvalue<Base1> || detail::ArrayRealTypeRvalue<Base2>)
STRICT_CONSTEXPR auto matvec_prod(Base1&& A1, Base2&& A2) = delete;


namespace detail {


template <BaseType Base>
STRICT_CONSTEXPR auto generate_const(const Base& A, ValueTypeOf<Base> x) {
   if constexpr(OneDimBaseType<Base>) {
      return const1D<BuiltinTypeOf<Base>>(A.size(), x);
   } else {
      return const2D<BuiltinTypeOf<Base>>(A.rows(), A.cols(), x);
   }
}


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base1, BaseType Base2, typename F, bool copy_delete>
   requires expr::BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate(const Base1& A1, const Base2& A2, F f) {
   using E = detail::BinaryExpr<Base1, Base2, F, copy_delete>;

   static_assert(same_dimension<Base1, Base2>());
   if constexpr(OneDimBaseType<Base1>) {
      return StrictArrayBase1D<E>(A1, A2, f);
   } else {
      return StrictArrayBase2D<E>(A1, A2, f);
   }
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryPlus{});
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryMinus{});
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryMult{});
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryDivide{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryModulo{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryRightShift{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryLeftShift{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBitwiseAnd{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBitwiseOr{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBitwiseXor{});
}


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator&&(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBooleanAnd{});
}


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator||(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBooleanOr{});
}


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBooleanXor{});
}


template <FloatingBaseType Base1, FloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate(A1, A2, expr::BinaryTwoProdFirst{}),
                    generate(A1, A2, expr::BinaryTwoProdSecond{})};
}


template <FloatingBaseType Base1, SignedIntegerBaseType Base2>
auto pow_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate(A1, A2, expr::BinaryPowProdFirst{}),
                    generate(A1, A2, expr::BinaryPowProdSecond{})};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryPlus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryMinus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryMult{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryDivide{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryModulo{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryRightShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryLeftShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryBitwiseAnd{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryBitwiseOr{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryBitwiseXor{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator&&(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryBooleanAnd{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator||(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryBooleanOr{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A) {
   return generate(detail::generate_const(A, x), A, expr::BinaryBooleanXor{});
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryPlus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryMinus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryMult{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryDivide{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryModulo{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryRightShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryLeftShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryBitwiseAnd{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryBitwiseOr{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryBitwiseXor{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator&&(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryBooleanAnd{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator||(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryBooleanOr{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, detail::generate_const(A, x), expr::BinaryBooleanXor{});
}


template <TwoDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto matvec_prod(const Base1& A, const Base2& x) {
   ASSERT_STRICT_DEBUG(A.cols() == x.size());
   // Re-implement dot product function here so that array_ops header does not need
   // to be included. It is also implemented slightly differently.
   auto dot = [](const auto& x1, const auto& x2) {
      ValueTypeOf<Base1> s{};
      for(index_t i = 0_sl; i < x1.size(); ++i) {
         s += x1[i] * x2[i];
      }
      return s;
   };
   return row_reduce(A, [&x, &dot](auto row) { return dot(row, x); });
}


}  // namespace spp
