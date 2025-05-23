#include <algorithm>
#include <cstdlib>
#include <utility>

#include "test.hpp"


using namespace spp;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_constr_default() {
   constexpr FixedArray2D<T, M, N> A;
   ASSERT(decltype(A)::size() == M.get() * N.get());
   ASSERT(decltype(A)::rows() == M.get());
   ASSERT(decltype(A)::cols() == N.get());
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_constr_value() {
   constexpr FixedArray2D<T, M, N> A1(One<T>);
   ASSERT(A1.size() == M.get() * N.get());
   ASSERT(all_of(A1, One<T>));

   constexpr FixedArray2D<T, M, N> A2(Value{One<T>});
   ASSERT(A2.size() == M.get() * N.get());
   ASSERT(all_of(A2, One<T>));
}


template <Builtin T>
consteval void run_constr_list() {
   constexpr auto x0 = Zero<T>;
   constexpr auto x1 = One<T>;
   constexpr FixedArray2D<T, 2, 2> A{{x0, x0}, {x1, x1}};
   static_assert(A(0, 0) == x0);
   static_assert(A(0, 1) == x0);
   static_assert(A(1, 0) == x1);
   static_assert(A(1, 1) == x1);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_constr_copy() {
   FixedArray2D<T, M, N> A1 = semi_random<T>(M, N);
   FixedArray2D<T, M, N> A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_constr_move() {
   FixedArray2D<T, M, N> A1 = semi_random<T>(M, N);
   FixedArray2D<T, M, N> A2 = A1;
   FixedArray2D<T, M, N> A3 = std::move(A1);
   ASSERT(A2 == A3);
   ASSERT(all_of(A1, Zero<T>));
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_constr_copy_other() {
   FixedArray2D<T, M, N> A1 = semi_random<T>(M, N);
   FixedArray2D<T, M, N> A2 = A1.view2D(M, N);
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_constr_list_fail() {
   REQUIRE_THROW((FixedArray2D<T, 2, 2>{{Zero<T>, Zero<T>}, {Zero<T>}}));
}


template <Builtin T>
void run_constr_copy_other_fail() {
   REQUIRE_THROW((FixedArray2D<T, 3, 2>(Array2D<T>(3, 3))));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_assign_value() {
   FixedArray2D<T, M, N> A;
   A = One<T>;
   ASSERT(all_of(A, One<T>));
}


template <Builtin T>
consteval void run_assign_list() {
   auto x0 = Zero<T>;
   auto x1 = One<T>;
   FixedArray2D<T, 2, 2> A;
   A = {{x0, x0}, {x1, x1}};
   ASSERT(A(0, 0) == x0);
   ASSERT(A(0, 1) == x0);
   ASSERT(A(1, 0) == x1);
   ASSERT(A(1, 1) == x1);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_assign_copy() {
   FixedArray2D<T, M, N> A1 = semi_random<T>(M, N);
   FixedArray2D<T, M, N> A2;
   A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_assign_move() {
   FixedArray2D<T, M, N> A1 = semi_random<T>(M, N);
   FixedArray2D<T, M, N> A2 = A1;
   FixedArray2D<T, M, N> A3;
   A3 = std::move(A1);

   ASSERT(A2 == A3);
   ASSERT(all_of(A1, Zero<T>));

   // self-move
   ASSERT(A2 == std::move(A2));
   ASSERT(A2 == A3);
}


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
consteval void run_assign_copy_other() {
   Array2D<T> A1 = semi_random<T>(M, N);
   FixedArray2D<T, M, N> A2;
   A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_assign_list_fail() {
   FixedArray2D<T, 2, 2> A;
   REQUIRE_THROW((A = {{Zero<T>, Zero<T>}, {Zero<T>}}));
}


template <Builtin T>
void run_assign_copy_other_fail() {
   Array2D<T> A1(3, 2);
   FixedArray2D<T, 2, 2> A2;
   REQUIRE_THROW(A2 = A1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void fixed_array_constructor() {
   constexpr index_t M = 4_sl, N = 3_sl;
   run_constr_default<T, M, N>();
   run_constr_value<T, M, N>();
   run_constr_list<T>();
   run_constr_copy<T, M, N>();
   run_constr_move<T, M, N>();
   run_constr_copy_other<T, M, N>();
   run_constr_list_fail<T>();
   run_constr_copy_other_fail<T>();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void fixed_array_assignment() {
   constexpr index_t M = 4_sl, N = 3_sl;
   run_assign_value<T, M, N>();
   run_assign_list<T>();
   run_assign_copy<T, M, N>();
   run_assign_move<T, M, N>();
   run_assign_copy_other<T, M, N>();

   run_assign_list_fail<T>();
   run_assign_copy_other_fail<T>();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
void fixed_array_data() {
   constexpr index_t M = 4_sl, N = 3_sl;
   FixedArray2D<T, M, N> A1(One<T>);
   const FixedArray2D<T, M, N> A2(One<T>);

   Strict<T>* a_ptr = A1.data();
   const Strict<T>* b_ptr = A2.data();
   ASSERT(std::all_of(a_ptr, a_ptr + (M * N).val(), [](auto x) { return x == One<T>; }));
   ASSERT(std::all_of(b_ptr, b_ptr + (M * N).val(), [](auto x) { return x == One<T>; }));

   T* a_blas_ptr = A1.blas_data();
   const T* b_blas_ptr = A2.blas_data();
   ASSERT(std::all_of(
       a_blas_ptr, a_blas_ptr + (M * N).val(), [](auto x) { return Strict{x} == One<T>; }));
   ASSERT(std::all_of(
       b_blas_ptr, b_blas_ptr + (M * N).val(), [](auto x) { return Strict{x} == One<T>; }));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_ALL_TYPES(fixed_array_constructor);
   TEST_ALL_TYPES(fixed_array_assignment);
   TEST_ALL_REAL_TYPES(fixed_array_data);

   return EXIT_SUCCESS;
}
