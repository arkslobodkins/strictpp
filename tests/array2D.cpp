#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>

#include "test.hpp"


using namespace spp;
using namespace spp::place;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void run_constr_default() {
   ASSERT(Array2D<T>{}.size() == 0_sl);
   ASSERT(Array2D<T>{}.rows() == 0_sl);
   ASSERT(Array2D<T>{}.cols() == 0_sl);
}


template <Builtin T>
void run_constr_size(ImplicitInt m, ImplicitInt n) {
   auto M = m.get(), N = n.get();
   ASSERT(Array2D<T>(M, N).size() == M * N);
   ASSERT(Array2D<T>(Rows{M}, Cols{N}).size() == M * N);
}


template <Builtin T>
void run_constr_size_value(ImplicitInt m, ImplicitInt n) {
   auto M = m.get(), N = n.get();

   Array2D<T> A1(M, N, One<T>);
   ASSERT(A1.size() == M * N);
   ASSERT(all_of(A1, One<T>, true_sb));

   Array2D<T> A2(Rows{M}, Cols{N}, Value{One<T>});
   ASSERT(A2.size() == M * N);
   ASSERT(all_of(A2, One<T>, true_sb));
}


template <Builtin T>
void run_constr_list() {
   auto x0 = Zero<T>;
   auto x1 = One<T>;
   Array2D<T> A{{x0, x0}, {x1, x1}};
   ASSERT(A(0, 0) == x0);
   ASSERT(A(0, 1) == x0);
   ASSERT(A(1, 0) == x1);
   ASSERT(A(1, 1) == x1);
   ASSERT(A.size() == 4_sl);
   ASSERT(A.rows() == 2_sl);
   ASSERT(A.cols() == 2_sl);
}


template <Builtin T>
void run_constr_copy(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A1 = random<T>(m, n);
   Array2D<T> A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_constr_move(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A1 = random<T>(m, n);
   Array2D<T> A2 = A1;
   Array2D<T> A3 = std::move(A1);
   ASSERT(A2 == A3);
   ASSERT(A1.empty());
   ASSERT(A1.data() == nullptr);
}


template <Builtin T>
void run_constr_copy_other(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A1 = random<T>(m, n);
   Array2D<T> A2 = A1.view2D(m, n);
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_constr_size_fail() {
   REQUIRE_THROW(Array2D<T> A(-1, 1));
   REQUIRE_THROW(Array2D<T> A(1, -1));
   REQUIRE_THROW(Array2D<T> A(0, 1));
   REQUIRE_THROW(Array2D<T> A(1, 0));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void run_assign_value(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A(m, n);
   A = One<T>;
   ASSERT(all_of(A, One<T>));
}


template <Builtin T>
void run_assign_list() {
   auto x0 = Zero<T>;
   auto x1 = One<T>;
   Array2D<T> A(2, 2);
   A = {{x0, x0}, {x1, x1}};
   ASSERT(A(0, 0) == x0);
   ASSERT(A(0, 1) == x0);
   ASSERT(A(1, 0) == x1);
   ASSERT(A(1, 1) == x1);
   ASSERT(A.size() == 4_sl);
   ASSERT(A.rows() == 2_sl);
   ASSERT(A.cols() == 2_sl);
}


template <Builtin T>
void run_assign_copy(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A1 = random<T>(m, n);
   Array2D<T> A2(m, n);
   A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_assign_move(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A1 = random<T>(m, n);
   Array2D<T> A2 = A1;
   Array2D<T> A3(m, n);
   A3 = std::move(A1);

   ASSERT(A2 == A3);
   ASSERT(A1.empty());
   ASSERT(A1.data() == nullptr);

   // self-move
   ASSERT(A2 == std::move(A2));
   ASSERT(A2 == A3);
}


template <Builtin T>
void run_assign_copy_other(ImplicitInt m, ImplicitInt n) {
   Array2D<T> A1 = random<T>(m, n);
   Array2D<T> A2(m, n);
   A2 = A1.view2D(m, n);
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_assign_list_fail() {
   Array2D<T> A(2, 2);
   REQUIRE_THROW((A = {{Zero<T>, Zero<T>}, {Zero<T>}}));
}


template <Builtin T>
void run_assign_copy_fail() {
   Array2D<T> A1(3, 2), A2(2, 2);
   REQUIRE_THROW(A2 = A1);
}


template <Builtin T>
void run_assign_copy_other_fail() {
   Array2D<T> A1(3, 2), A2(2, 2);
   REQUIRE_THROW(A2 = A1(all, all));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_swap(auto X1, auto Y1) {
   auto X2 = X1;
   auto Y2 = Y1;

   X1.swap(Y1);
   ASSERT(X1 == Y2);
   ASSERT(Y1 == X2);

   // self-swap
   X1.swap(X1);
   ASSERT(X1 == Y2);
}


void run_swap_rvalue(auto X) {
   X.swap(decltype(X){});
   ASSERT(X.empty());
   ASSERT(X.size() == 0_sl);
}


void run_resize_assign(auto X1, auto X2) {
   X1.resize_and_assign(X2);
   ASSERT(X1 == X2);
}


void run_resize_assign_move(auto X1, auto X2) {
   auto X3 = X2;
   X1.resize_and_assign(std::move(X2));
   ASSERT(X1 == X3);
   ASSERT(X2.empty());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_remove_rows(auto X, Pos pos, Count count) {
   auto Y = X;
   X.remove_rows(pos, count);
   auto indexes = implicit_vector_sequence(Size{count.get()}, Start{pos.get()});
   ASSERT(X == Y(complement{indexes}, all));
}


void run_remove_cols(auto X, Pos pos, Count count) {
   auto Y = X;
   X.remove_cols(pos, count);
   auto indexes = implicit_vector_sequence(Size{count.get()}, Start{pos.get()});
   ASSERT(X == Y(all, complement{indexes}));
}


void run_remove_rows_front(auto X, Count count) {
   auto Y = X;
   X.remove_rows_front(count);
   ASSERT(X == Y(lastN{Y.rows() - count.get()}, all));
}


void run_remove_cols_front(auto X, Count count) {
   auto Y = X;
   X.remove_cols_front(count);
   ASSERT(X == Y(all, lastN{Y.cols() - count.get()}));
}


void run_remove_rows_back(auto X, Count count) {
   auto Y = X;
   X.remove_rows_back(count);
   ASSERT(X == Y(firstN{Y.rows() - count.get()}, all));
}


void run_remove_cols_back(auto X, Count count) {
   auto Y = X;
   X.remove_cols_back(count);
   ASSERT(X == Y(all, firstN{Y.cols() - count.get()}));
}


void run_remove_rows_vec(auto X, std::vector<ImplicitInt> indexes) {
   auto Y = X;
   X.remove_rows(indexes);
   ASSERT(X == Y(complement{indexes}, all));
}


void run_remove_cols_vec(auto X, std::vector<ImplicitInt> indexes) {
   auto Y = X;
   X.remove_cols(indexes);
   ASSERT(X == Y(all, complement{indexes}));
}


template <Builtin T>
void run_resize_fail() {
   Array2D<T> A;
   REQUIRE_THROW(A.resize(-1, -1));
   REQUIRE_THROW(A.resize(0, 5));
   REQUIRE_NOT_THROW(A.resize(0, 0));
}


template <Builtin T>
void run_remove_fail() {
   Array2D<T> A(10, 8);
   REQUIRE_THROW(A.remove_rows(Pos{0}, Count{0}));
   REQUIRE_THROW(A.remove_rows(Pos{-1}, Count{1}));
   REQUIRE_THROW(A.remove_rows(Pos{0}, Count{A.rows() + 1_sl}));
   REQUIRE_THROW(A.remove_cols(Pos{0}, Count{0}));
   REQUIRE_THROW(A.remove_cols(Pos{-1}, Count{1}));
   REQUIRE_THROW(A.remove_cols(Pos{0}, Count{A.cols() + 1_sl}));
   REQUIRE_NOT_THROW(A.remove_rows(Pos{0}, Count{A.rows()}));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_insert_rows(auto X, Pos pos, auto Y) {
   auto Z = X;
   Z.insert_rows(pos, Y);
   ASSERT(Z == merge_vertical(X(firstN{pos.get()}, all), Y, X(lastN{X.rows() - pos.get()}, all)));
}


void run_insert_cols(auto X, Pos pos, auto Y) {
   auto Z = X;
   Z.insert_cols(pos, Y);
   ASSERT(Z == merge_horizontal(X(all, firstN{pos.get()}), Y, X(all, lastN{X.cols() - pos.get()})));
}


void run_insert_row(auto X, Pos pos, auto Y) {
   auto Z = X;
   Z.insert_row(pos, Y);
   ASSERT(Z
          == merge_vertical(X(firstN{pos.get()}, all),
                            Y.view2D(1, Y.size()),
                            X(lastN{X.rows() - pos.get()}, all)));
}


void run_insert_col(auto X, Pos pos, auto Y) {
   auto Z = X;
   Z.insert_col(pos, Y);
   ASSERT(Z
          == merge_horizontal(X(all, firstN{pos.get()}),
                              Y.view2D(Y.size(), 1),
                              X(all, lastN{X.cols() - pos.get()})));
}


template <Builtin T>
void run_insert_fail() {
   Array2D<T> A(8, 10);
   REQUIRE_THROW(A.insert_rows(-1, A));
   REQUIRE_THROW(A.insert_cols(-1, A));
   REQUIRE_THROW(A.insert_rows(A.rows() + 1_sl, A));
   REQUIRE_THROW(A.insert_cols(A.cols() + 1_sl, A));
   REQUIRE_NOT_THROW(A.insert_rows(A.rows(), A));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_resize_and_assign_strong() {
   Array2D<int> A1{{2_si, 2_si}, {2_si, 2_si}};
   auto A2 = A1;
   auto A3 = A2;
   REQUIRE_THROW((A2.resize_and_assign(A1 / Array2D<int>{{2_si, 2_si}, {2_si, 0_si}}.lval())));
   ASSERT(A3 == A2);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void array_constructor() {
   constexpr index_t m = 10_sl, n = 8_sl;

   run_constr_default<T>();
   run_constr_size<T>(m, n);
   run_constr_size_value<T>(m, n);
   run_constr_list<T>();
   run_constr_copy<T>(m, n);
   run_constr_move<T>(m, n);
   run_constr_copy_other<T>(m, n);

   run_constr_size_fail<T>();
}


template <Builtin T>
void array_assignment() {
   constexpr index_t m = 10_sl, n = 8_sl;

   run_assign_value<T>(m, n);
   run_assign_list<T>();
   run_assign_copy<T>(m, n);
   run_assign_move<T>(m, n);
   run_assign_copy_other<T>(m, n);

   run_assign_list_fail<T>();
   run_assign_copy_fail<T>();
   run_assign_copy_other_fail<T>();
}


template <Builtin T>
void array_resize() {
   constexpr index_t m = 10_sl, n = 8_sl;
   const Array2D<T> A1 = random<T>(m, n);
   const Array2D<T> A2 = random<T>(2_sl * m, 2_sl * n);

   run_swap(A1, A2);
   run_swap_rvalue(A1);
   run_resize_assign(A1, A2);
   run_resize_assign_move(A1, A2);
}


template <Builtin T>
void array_remove() {
   constexpr index_t m = 10_sl, n = 8_sl;
   const Array2D<T> A = random<T>(m, n);

   run_remove_rows(A, Pos{0}, Count{1});
   run_remove_rows(A, Pos{0}, Count{m / 2_sl});
   run_remove_rows(A, Pos{0}, Count{m});
   run_remove_rows(A, Pos{m / 2_sl}, Count{1});
   run_remove_rows(A, Pos{m / 2_sl}, Count{m / 2_sl});
   run_remove_rows(A, Pos{m - 1_sl}, Count{1});

   run_remove_cols(A, Pos{0}, Count{1});
   run_remove_cols(A, Pos{0}, Count{n / 2_sl});
   run_remove_cols(A, Pos{0}, Count{n});
   run_remove_cols(A, Pos{n / 2_sl}, Count{1});
   run_remove_cols(A, Pos{n / 2_sl}, Count{n / 2_sl});
   run_remove_cols(A, Pos{n - 1_sl}, Count{1});

   run_remove_rows_front(A, Count{2_sl});
   run_remove_cols_front(A, Count{2_sl});
   run_remove_rows_back(A, Count{2_sl});
   run_remove_cols_back(A, Count{2_sl});

   run_remove_rows_vec(A, {0, 2, 4});
   run_remove_rows_vec(A, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
   run_remove_cols_vec(A, {0, 2, 4});
   run_remove_cols_vec(A, {0, 1, 2, 3, 4, 5, 6, 7});

   run_resize_fail<T>();
   run_remove_fail<T>();
}


template <Builtin T>
void array_insert() {
   constexpr index_t m = 10_sl, n = 8_sl;
   const Array2D<T> A1 = random<T>(m, n);
   const Array2D<T> A2 = random<T>(5, n);
   const Array2D<T> A3 = random<T>(m, 4);

   run_insert_rows(A1, Pos{0}, A2);
   run_insert_rows(A1, Pos{m / 2_sl}, A2);
   run_insert_rows(A1, Pos{m}, A2);

   run_insert_cols(A1, Pos{0}, A3);
   run_insert_cols(A1, Pos{n / 2_sl}, A3);
   run_insert_cols(A1, Pos{n}, A3);

   run_insert_row(A1, Pos{0}, Array1D<T>(n));
   run_insert_row(A1, Pos{m / 2_sl}, Array1D<T>(n));
   run_insert_row(A1, Pos{m}, Array1D<T>(n));

   run_insert_col(A1, Pos{0}, Array1D<T>(m));
   run_insert_col(A1, Pos{n / 2_sl}, Array1D<T>(m));
   run_insert_col(A1, Pos{n}, Array1D<T>(m));

   run_insert_fail<T>();
}


template <Real T>
void array_data() {
   constexpr index_t m = 10_sl, n = 8_sl;
   constexpr auto mn = m * n;
   Array2D<T> A1(m, n, One<T>);
   const Array2D<T> A2(m, n, One<T>);

   Strict<T>* a_ptr = A1.data();
   const Strict<T>* b_ptr = A2.data();
   ASSERT(std::all_of(a_ptr, a_ptr + mn.val(), [](auto x) { return x == One<T>; }));
   ASSERT(std::all_of(b_ptr, b_ptr + mn.val(), [](auto x) { return x == One<T>; }));

   T* a_blas_ptr = A1.blas_data();
   const T* b_blas_ptr = A2.blas_data();
   ASSERT(
       std::all_of(a_blas_ptr, a_blas_ptr + mn.val(), [](auto x) { return Strict{x} == One<T>; }));
   ASSERT(
       std::all_of(b_blas_ptr, b_blas_ptr + mn.val(), [](auto x) { return Strict{x} == One<T>; }));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void array_strong_guarantee() {
   run_resize_and_assign_strong();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_ALL_TYPES(array_constructor);
   TEST_ALL_TYPES(array_assignment);
   TEST_ALL_TYPES(array_resize);
   TEST_ALL_TYPES(array_remove);
   TEST_ALL_TYPES(array_insert);
   TEST_ALL_REAL_TYPES(array_data);
   TEST_NON_TYPE(array_strong_guarantee);

   return EXIT_SUCCESS;
}
