// Arkadijs Slobodkins, 2023


#pragma once


#include <type_traits>

#include "../ArrayCommon/array_auxiliary.hpp"
#include "../ArrayCommon/array_traits.hpp"
#include "../ArrayCommon/valid.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "exclude_last.hpp"
#include "types.hpp"
#include "unary.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
// Special operations.
template <OneDimBaseType Base, OneDimBaseType... BaseArgs>
STRICT_CONSTEXPR auto merge(const Base& A, const BaseArgs&... AArgs);


template <OneDimBaseType Base1, OneDimBaseType Base2>
STRICT_CONSTEXPR auto merge(const Base1& A1, const Base2& A2);


template <OneDimBaseType Base, StrictType Strict_t, StrictType... StrictArgs>
STRICT_CONSTEXPR auto merge(const Base& A, Strict_t x, StrictArgs... xargs);


template <OneDimBaseType Base, StrictType Strict_t>
STRICT_CONSTEXPR auto merge(const Base& A, Strict_t x);


// Cannot separate values from OneDimBaseType, so one must use StrictAndBase&...
template <StrictType Strict_t1, StrictType Strict_t2, typename... StrictAndBase>
   requires OneDimBaseType<detail::LastPack_t<StrictAndBase...>>
         && detail::AllStrictExceptLast<StrictAndBase...>
STRICT_CONSTEXPR auto merge(Strict_t1 x1, Strict_t2 x2, const StrictAndBase&... xargs_and_A);


template <StrictType Strict_t, OneDimBaseType Base>
STRICT_CONSTEXPR auto merge(Strict_t x, const Base& A);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimBaseType Base, TwoDimBaseType... BaseArgs>
STRICT_CONSTEXPR auto merge_horizontal(const Base& A, const BaseArgs&... AArgs);


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR auto merge_horizontal(const Base1& A1, const Base2& A2);


template <TwoDimBaseType Base, TwoDimBaseType... BaseArgs>
STRICT_CONSTEXPR auto merge_vertical(const Base& A, const BaseArgs&... AArgs);


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR auto merge_vertical(const Base1& A1, const Base2& A2);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, ImplicitInt pos, ImplicitInt count = 1);


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, Pos pos, Count count = Count{1});


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, [[maybe_unused]] Last lst);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_CONSTEXPR auto sequence(ImplicitInt size, Strict<T> start = Strict<T>{},
                               Strict<T> incr = Strict{T(1)});


template <Real T>
STRICT_CONSTEXPR auto sequence(Size size, Start<T> start = Start<T>{}, Incr<T> incr = Incr{T(1)});


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_CONSTEXPR auto linspace(ImplicitInt size, Strict<T> start, Strict<T> end);


template <Floating T>
STRICT_CONSTEXPR auto linspace(Size size, Start<T> start, End<T> end);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Not a template, so it must be inlined.
STRICT_CONSTEXPR_INLINE auto irange(ImplicitInt n);


// Overloads for irange do not need to be deleted if A is rvalue
// of ArrayType because it only stores information about its size.
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange(const Base& A);


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange_m1(const Base& A);


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_row(const Base& A);


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_row_m1(const Base& A);


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_col(const Base& A);


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_col_m1(const Base& A);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Generates standard unit vector on the fly.
// Involves redundant work if assigned to arrays that only contain zeros.
// Empty objects are not allowed(must be of at least size 1).
template <Real T>
STRICT_CONSTEXPR auto e_unit(ImplicitInt unit_index, ImplicitInt size);


template <Real T>
STRICT_CONSTEXPR auto e_unit(Index unit_index, Size size);


// Similarly, generates standard identity on the fly.
// Empty objects are allowed.
template <Real T>
STRICT_CONSTEXPR auto identity(ImplicitInt n);


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto transpose(const Base& A);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_CONSTEXPR auto const1D(ImplicitInt size, Strict<T> c);


template <Builtin T>
STRICT_CONSTEXPR auto const1D(Size size, Value<T> c);


template <Builtin T>
STRICT_CONSTEXPR auto const2D(ImplicitInt rows, ImplicitInt cols, Strict<T> c);


template <Builtin T>
STRICT_CONSTEXPR auto const2D(Rows rows, Cols cols, Value<T> c);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimBaseType Base, typename Op>
STRICT_CONSTEXPR auto row_reduce(const Base& A, Op op);


template <TwoDimBaseType Base, typename Op>
STRICT_CONSTEXPR auto col_reduce(const Base& A, Op op);


namespace detail {


template <typename... Args> concept OneOfArrayOneDimTypeRvalue
    = (... || ArrayOneDimTypeRvalue<Args>);


template <typename... Args> concept OneOfArrayTwoDimTypeRvalue
    = (... || ArrayTwoDimTypeRvalue<Args>);


}  // namespace detail


// Deleted overloads.
template <typename... Args>
   requires detail::OneOfArrayOneDimTypeRvalue<Args...>
STRICT_CONSTEXPR auto merge(Args&&... args) = delete;


template <typename... Args>
   requires detail::OneOfArrayTwoDimTypeRvalue<Args...>
STRICT_CONSTEXPR auto merge_horizontal(Args&&... args) = delete;


template <typename... Args>
   requires detail::OneOfArrayTwoDimTypeRvalue<Args...>
STRICT_CONSTEXPR auto merge_vertical(Args&&... args) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::ArrayOneDimTypeRvalue<Base>
STRICT_CONSTEXPR auto exclude(Base&& A, ImplicitInt pos, ImplicitInt count = 1) = delete;


template <typename Base>
   requires detail::ArrayOneDimTypeRvalue<Base>
STRICT_CONSTEXPR auto exclude(Base&& A, Pos pos, Count count = Count{1}) = delete;


template <typename Base>
   requires detail::ArrayOneDimTypeRvalue<Base>
STRICT_CONSTEXPR auto exclude(Base&& A, Last lst) = delete;


template <typename Base>
   requires detail::ArrayTwoDimTypeRvalue<Base>
STRICT_CONSTEXPR auto transpose(Base&& A) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base, typename Op>
   requires detail::ArrayTwoDimTypeRvalue<Base>
STRICT_CONSTEXPR auto row_reduce(Base&& A, Op op) = delete;


template <typename Base, typename Op>
   requires detail::ArrayTwoDimTypeRvalue<Base>
STRICT_CONSTEXPR auto col_reduce(Base&& A, Op op) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace detail {


STRICT_CONSTEXPR_INLINE auto irange2D(ImplicitInt m, ImplicitInt n) {
   auto op = [n](index_t i, index_t j) { return i * n.get() + j; };
   using E = detail::IndexExpr2D<long int, decltype(op)>;
   return StrictArrayBase2D<E>{m.get(), n.get(), op};
}


STRICT_CONSTEXPR_INLINE auto horizontal_op(auto const& A1E, auto const& A2E) {
   return [A1E, A2E](auto i) {
      auto row_pos = i / (A1E.cols() + A2E.cols());
      auto col_pos = i % (A1E.cols() + A2E.cols());
      auto i1 = row_pos;
      auto i2 = row_pos;
      auto j1 = col_pos;
      auto j2 = col_pos - A1E.cols();
      return (col_pos < A1E.cols()) ? A1E.un(i1 * A1E.cols() + j1) : A2E.un(i2 * A2E.cols() + j2);
   };
}


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base, OneDimBaseType... BaseArgs>
STRICT_CONSTEXPR auto merge(const Base& A, const BaseArgs&... AArgs) {
   auto AE = generate(A, [](auto x) { return x; });
   auto ArgsE = merge(AArgs...);
   auto op = [AE, ArgsE](auto i) { return i < AE.size() ? AE.un(i) : ArgsE.un(i - AE.size()); };
   return generate(irange(AE.size() + ArgsE.size()), op);
}


template <OneDimBaseType Base1, OneDimBaseType Base2>
STRICT_CONSTEXPR auto merge(const Base1& A1, const Base2& A2) {
   auto A1E = generate(A1, [](auto x) { return x; });
   auto A2E = generate(A2, [](auto x) { return x; });
   auto op = [A1E, A2E](auto i) { return i < A1E.size() ? A1E.un(i) : A2E.un(i - A1E.size()); };
   return generate(irange(A1.size() + A2.size()), op);
}


template <OneDimBaseType Base, StrictType Strict_t, StrictType... StrictArgs>
STRICT_CONSTEXPR auto merge(const Base& A, Strict_t x, StrictArgs... xargs) {
   return merge(merge(A, x), xargs...);
}


template <OneDimBaseType Base, StrictType Strict_t>
STRICT_CONSTEXPR auto merge(const Base& A, Strict_t x) {
   auto AE = generate(A, [](auto z) { return z; });
   auto op = [AE, x](auto i) { return i == AE.size() ? x : AE.un(i); };
   return generate(irange(AE.size() + 1_sl), op);
}


template <StrictType Strict_t1, StrictType Strict_t2, typename... StrictAndBase>
   requires OneDimBaseType<detail::LastPack_t<StrictAndBase...>>
         && detail::AllStrictExceptLast<StrictAndBase...>
STRICT_CONSTEXPR auto merge(Strict_t1 x1, Strict_t2 x2, const StrictAndBase&... xargs_and_A) {
   return merge(x1, merge(x2, xargs_and_A...));
}


template <StrictType Strict_t, OneDimBaseType Base>
STRICT_CONSTEXPR auto merge(Strict_t x, const Base& A) {
   auto AE = generate(A, [](auto z) { return z; });
   auto op = [AE, x](auto i) { return i == 0_sl ? x : AE.un(i - 1_sl); };
   return generate(irange(AE.size() + 1_sl), op);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimBaseType Base, TwoDimBaseType... BaseArgs>
STRICT_CONSTEXPR auto merge_horizontal(const Base& A, const BaseArgs&... AArgs) {
   auto AE = generate(A, [](auto x) { return x; });
   auto ArgsE = merge_horizontal(AArgs...);

   if(!AE.empty() && !ArgsE.empty()) {
      ASSERT_STRICT_DEBUG(AE.rows() == ArgsE.rows());
   }
   auto ERows = !AE.empty() ? AE.rows() : ArgsE.rows();
   return generate(detail::irange2D(ERows, AE.cols() + ArgsE.cols()),
                   detail::horizontal_op(AE, ArgsE));
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR auto merge_horizontal(const Base1& A1, const Base2& A2) {
   auto A1E = generate(A1, [](auto x) { return x; });
   auto A2E = generate(A2, [](auto x) { return x; });

   if(!A1E.empty() && !A2E.empty()) {
      ASSERT_STRICT_DEBUG(A1.rows() == A2.rows());
   }
   auto ERows = !A1E.empty() ? A1E.rows() : A2E.rows();
   return generate(detail::irange2D(ERows, A1E.cols() + A2E.cols()),
                   detail::horizontal_op(A1E, A2E));
}


template <TwoDimBaseType Base, TwoDimBaseType... BaseArgs>
STRICT_CONSTEXPR auto merge_vertical(const Base& A, const BaseArgs&... AArgs) {
   auto AE = generate(A, [](auto x) { return x; });
   auto ArgsE = merge_vertical(AArgs...);

   if(!AE.empty() && !ArgsE.empty()) {
      ASSERT_STRICT_DEBUG(AE.cols() == ArgsE.cols());
   }
   auto ECols = !AE.empty() ? AE.cols() : ArgsE.cols();
   auto op = [AE, ArgsE](auto i) { return i < AE.size() ? AE.un(i) : ArgsE.un(i - AE.size()); };
   return generate(detail::irange2D(AE.rows() + ArgsE.rows(), ECols), op);
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR auto merge_vertical(const Base1& A1, const Base2& A2) {
   auto A1E = generate(A1, [](auto x) { return x; });
   auto A2E = generate(A2, [](auto x) { return x; });

   if(!A1E.empty() && !A2E.empty()) {
      ASSERT_STRICT_DEBUG(A1.cols() == A2.cols());
   }
   auto ECols = !A1E.empty() ? A1E.cols() : A2E.cols();
   auto op = [A1E, A2E](auto i) { return i < A1E.size() ? A1E.un(i) : A2E.un(i - A1E.size()); };
   return generate(detail::irange2D(A1E.rows() + A2E.rows(), ECols), op);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, ImplicitInt pos, ImplicitInt count) {
   ASSERT_STRICT_DEBUG(count.get() > 0_sl);
   ASSERT_STRICT_DEBUG(detail::valid_index(A, pos.get()));
   ASSERT_STRICT_DEBUG(detail::valid_index(A, pos.get() + count.get() - 1_sl));
   auto AE = generate(A, [](auto x) { return x; });
   return generate(irange(A.size() - count.get()), [pos, count, AE](auto j) {
      return j < pos.get() ? AE.un(j) : AE.un(j + count.get());
   });
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, Pos pos, Count count) {
   return exclude(A, pos.get(), count.get());
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto exclude(const Base& A, [[maybe_unused]] Last lst) {
   return exclude(A, A.size_m1());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_CONSTEXPR auto sequence(ImplicitInt size, Strict<T> start, Strict<T> incr) {
   return StrictArrayBase1D<detail::SequenceExpr1D<T>>{start, size.get(), incr};
}


template <Real T>
STRICT_CONSTEXPR auto sequence(Size size, Start<T> start, Incr<T> incr) {
   return sequence<T>(size.get(), start.get(), incr.get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_CONSTEXPR auto linspace(ImplicitInt size, Strict<T> start, Strict<T> end) {
   ASSERT_STRICT_DEBUG(size.get() > 0_sl);
   auto sz = size.get();
   return StrictArrayBase1D<detail::SequenceExpr1D<T>>{
       start, sz, (end - start) / strict_cast<T>(sz - 1_sl)};
}


template <Floating T>
STRICT_CONSTEXPR auto linspace(Size size, Start<T> start, End<T> end) {
   return linspace<T>(size.get(), start.get(), end.get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
STRICT_CONSTEXPR_INLINE auto irange(ImplicitInt n) {
   return StrictArrayBase1D<detail::SequenceExpr1D<long int>>{0_sl, n.get(), 1_sl};
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange(const Base& A) {
   return irange(A.size());
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto irange_m1(const Base& A) {
   return irange(A.size_m1());
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_row(const Base& A) {
   return irange(A.rows());
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_row_m1(const Base& A) {
   return irange(A.rows() - 1_sl);
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_col(const Base& A) {
   return irange(A.cols());
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto irange_col_m1(const Base& A) {
   return irange(A.cols() - 1_sl);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_CONSTEXPR auto e_unit(ImplicitInt unit_index, ImplicitInt size) {
   ASSERT_STRICT_DEBUG(unit_index.get() > -1_sl);
   ASSERT_STRICT_DEBUG(size.get() > unit_index.get());
   return generate(irange(size),
                   [unit_index](auto i) { return i == unit_index.get() ? One<T> : Zero<T>; });
}


template <Real T>
STRICT_CONSTEXPR auto e_unit(Index unit_index, Size size) {
   return e_unit<T>(unit_index.get(), size.get());
}


template <Real T>
STRICT_CONSTEXPR auto identity(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);
   auto op = [](index_t i, index_t j) { return i == j ? One<T> : Zero<T>; };
   using E = detail::IndexExpr2D<T, decltype(op)>;
   return StrictArrayBase2D<E>{n.get(), n.get(), op};
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto transpose(const Base& A) {
   auto op = [&A](index_t i, index_t j) { return A.un(j, i); };
   using E = detail::IndexExpr2D<BuiltinTypeOf<Base>, decltype(op)>;
   return StrictArrayBase2D<E>{A.cols(), A.rows(), op};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_CONSTEXPR auto const1D(ImplicitInt size, Strict<T> c) {
   ASSERT_STRICT_DEBUG(size.get() > -1_sl);
   return generate(irange(size), [c]([[maybe_unused]] auto i) { return c; });
}


template <Builtin T>
STRICT_CONSTEXPR auto const1D(Size size, Value<T> c) {
   return const1D<T>(size.get(), c.get());
}


template <Builtin T>
STRICT_CONSTEXPR auto const2D(ImplicitInt rows, ImplicitInt cols, Strict<T> c) {
   ASSERT_STRICT_DEBUG(rows.get() > -1_sl);
   ASSERT_STRICT_DEBUG(cols.get() > -1_sl);
   ASSERT_STRICT_DEBUG(detail::semi_valid_row_col_sizes(rows.get(), cols.get()));
   return generate(detail::irange2D(rows, cols), [c]([[maybe_unused]] auto i) { return c; });
}


template <Builtin T>
STRICT_CONSTEXPR auto const2D(Rows rows, Cols cols, Value<T> c) {
   return const2D<T>(rows.get(), cols.get(), c.get());
}


template <TwoDimBaseType Base, typename Op>
STRICT_CONSTEXPR auto row_reduce(const Base& A, Op op) {
   using E = detail::ReduceExpr<Base, Op, true>;
   return StrictArrayBase1D<E>{A, op};
}


template <TwoDimBaseType Base, typename Op>
STRICT_CONSTEXPR auto col_reduce(const Base& A, Op op) {
   using E = detail::ReduceExpr<Base, Op, false>;
   return StrictArrayBase1D<E>{A, op};
}


}  // namespace spp

