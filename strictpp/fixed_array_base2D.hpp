// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "fixed_array_base1D.hpp"


namespace spp::detail {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
class STRICT_NODISCARD FixedArrayBase2D : private ReferenceBase2D, private TwoDimArrayBase {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // Important to test these, otherwise expressions like FixedArray2D<int, -2, -2>
   // would erroneously compile(-2 * -2 = 4, which is nonnegative).
   static_assert(M.get() > -1_sl);
   static_assert(N.get() > -1_sl);
   static_assert(semi_valid_row_col_sizes(M.get(), N.get()));

   // Constructors.
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D() = default;
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase2D(value_type x);
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase2D(Value<T> x);
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D(use::List2D<builtin_type> list);
   // Enforce parenthesis instead of braces.
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D(use::List1D<builtin_type> list) = delete;

   STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D(const FixedArrayBase2D& A) = default;
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D(FixedArrayBase2D&& A) noexcept = default;
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D(TwoDimBaseType auto const& A);

   // Assignments.
   STRICT_CONSTEXPR FixedArrayBase2D& operator=(value_type x);
   STRICT_CONSTEXPR FixedArrayBase2D& operator=(use::List2D<builtin_type> list);
   STRICT_CONSTEXPR FixedArrayBase2D& operator=(const FixedArrayBase2D& A) = default;
   STRICT_CONSTEXPR FixedArrayBase2D& operator=(FixedArrayBase2D&& A) noexcept = default;
   STRICT_CONSTEXPR FixedArrayBase2D& operator=(TwoDimBaseType auto const& A);

   STRICT_CONSTEXPR ~FixedArrayBase2D() = default;

   STRICT_CONSTEXPR_INLINE static index_t rows();
   STRICT_CONSTEXPR_INLINE static index_t cols();
   STRICT_CONSTEXPR_INLINE static index_t size();

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i, ImplicitInt j);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const;

   STRICT_NODISCARD_CONSTEXPR value_type* data() &;
   STRICT_NODISCARD_CONSTEXPR const value_type* data() const&;
   STRICT_NODISCARD_CONSTEXPR value_type* data() && = delete;
   STRICT_NODISCARD_CONSTEXPR const value_type* data() const&& = delete;

   // Converting to built-in types requires reinterpret_cast,
   // which cannot be done at compile time(not constexpr).
   STRICT_NODISCARD builtin_type* blas_data() &
      requires CompatibleBuiltin<T>;
   STRICT_NODISCARD const builtin_type* blas_data() const&
      requires CompatibleBuiltin<T>;

   STRICT_NODISCARD builtin_type* blas_data() &&
      requires CompatibleBuiltin<T>
   = delete;
   STRICT_NODISCARD const builtin_type* blas_data() const&&
      requires CompatibleBuiltin<T>
   = delete;

private:
   FixedArrayBase1D<T, M.get() * N.get(), AF> data1D_;
};


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D<T, M, N, AF>::FixedArrayBase2D(value_type x) {
   data1D_ = x;
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D<T, M, N, AF>::FixedArrayBase2D(Value<T> x)
    : FixedArrayBase2D(x.get()) {
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D<T, M, N, AF>::FixedArrayBase2D(
    use::List2D<builtin_type> list) {
   ASSERT_STRICT_DEBUG(valid_list2D(list));
   auto [nrows, ncols] = list2D_row_col_sizes(list);
   ASSERT_STRICT_DEBUG(this->rows() == nrows);
   ASSERT_STRICT_DEBUG(this->cols() == ncols);
   copy(list, *this);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase2D<T, M, N, AF>::FixedArrayBase2D(
    TwoDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_CONSTEXPR FixedArrayBase2D<T, M, N, AF>& FixedArrayBase2D<T, M, N, AF>::operator=(
    value_type x) {
   data1D_ = x;
   return *this;
}


// Handles empty initializer list case as well.
template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_CONSTEXPR FixedArrayBase2D<T, M, N, AF>& FixedArrayBase2D<T, M, N, AF>::operator=(
    use::List2D<T> list) {
   // Constructor of temp checks that list is valid and that it is of size M x N, same_size not
   // needed.
   FixedArrayBase2D temp(list);
   return *this = temp;
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_CONSTEXPR FixedArrayBase2D<T, M, N, AF>& FixedArrayBase2D<T, M, N, AF>::operator=(
    TwoDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t FixedArrayBase2D<T, M, N, AF>::rows() {
   return M.get();
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t FixedArrayBase2D<T, M, N, AF>::cols() {
   return N.get();
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t FixedArrayBase2D<T, M, N, AF>::size() {
   return M.get() * N.get();
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto FixedArrayBase2D<T, M, N, AF>::un(ImplicitInt i)
    -> value_type& {
   return data1D_.un(i);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto FixedArrayBase2D<T, M, N, AF>::un(ImplicitInt i) const
    -> const value_type& {
   return data1D_.un(i);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto FixedArrayBase2D<T, M, N, AF>::un(ImplicitInt i,
                                                                         ImplicitInt j)
    -> value_type& {
   return data1D_.un(i.get() * N.get() + j.get());
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto FixedArrayBase2D<T, M, N, AF>::un(ImplicitInt i,
                                                                         ImplicitInt j) const
    -> const value_type& {
   return data1D_.un(i.get() * N.get() + j.get());
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto FixedArrayBase2D<T, M, N, AF>::data() & -> value_type* {
   return data1D_.data();
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto FixedArrayBase2D<T, M, N, AF>::data() const& -> const value_type* {
   return data1D_.data();
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD auto FixedArrayBase2D<T, M, N, AF>::blas_data() & -> builtin_type*
   requires CompatibleBuiltin<T>
{
   return data1D_.blas_data();
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N, AlignmentFlag AF>
STRICT_NODISCARD auto FixedArrayBase2D<T, M, N, AF>::blas_data() const& -> const builtin_type*
   requires CompatibleBuiltin<T>
{
   return data1D_.blas_data();
}


}  // namespace spp::detail
