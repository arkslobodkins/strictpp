#include <cstdlib>
#include <utility>

#include "test.hpp"

using namespace spp;
using namespace spp::place;


// For these tests, sufficiently small array sizes should be used,
// otherwise constant evaluation will hit maximum step limit.
template <typename T>
consteval void array1D() {
   Array1D<T>{};
   Array1D<T>(2);
   Array1D<T>(Size{2});
   Array1D<T>(2, Zero<T>);
   Array1D<T>(Size{2}, Value{Zero<T>});
   Array1D<T>{Zero<T>};

   Array1D<T> A(2);
   Array1D<T> B = A;
   Array1D<T> C = const1D<T>(2, Zero<T>);
   Array1D<T> D(C.begin(), C.end());
   Array1D<T> E = std::move(A);

   E = Zero<T>;
   E = {Zero<T>, Zero<T>};
   E = B;
   E = const1D<T>(2, Zero<T>);
   E = std::move(B);

   E.swap(C);
   E.swap(std::move(C));
   E.resize_and_assign(C);
   E.resize_and_assign(std::move(C));
   E.resize(20);
   E.resize(20, false);

   E.remove(0);
   E.remove(0, 1);
   E.remove(Pos{0});
   E.remove(Pos{0}, Count{1});
   E.remove_front();
   E.remove_front(1);
   E.remove_front(Count{1});
   E.remove_back();
   E.remove_back(1);
   E.remove_back(Count{1});
   E.remove(last);
   E.remove({0, 2, 4});

   E.insert(0, Zero<T>);
   E.insert(Pos{0}, Value(Zero<T>));
   E.insert_front(Zero<T>);
   E.insert_front(Value{Zero<T>});
   E.insert_back(Zero<T>);
   E.insert_back(Value{Zero<T>});
   E.insert(0, D);
   E.insert(Pos{0}, D);
   E.insert_front(D);
   E.insert_back(D);

   E.size();
   E.un(0);
   E.data();
}


template <typename T>
consteval void fixed_array1D() {
   FixedArray1D<T, 2>{};
   FixedArray1D<T, 2>(Zero<T>);
   FixedArray1D<T, 2>(Value{Zero<T>});
   FixedArray1D<T, 2>{Zero<T>, Zero<T>};

   FixedArray1D<T, 2> A;
   FixedArray1D<T, 2> B = A;
   FixedArray1D<T, 2> C = const1D<T>(2, Zero<T>);
   FixedArray1D<T, 2> D(C.begin(), C.end());
   FixedArray1D<T, 2> E = std::move(A);

   E = Zero<T>;
   E = {Zero<T>, Zero<T>};
   E = B;
   E = const1D<T>(2, Zero<T>);
   E = std::move(B);

   E.size();
   E.un(0);
   E.data();
}


template <typename T>
consteval void array2D() {
   Array2D<T>{};
   Array2D<T>(2, 2);
   Array2D<T>(Rows{2}, Cols{2});
   Array2D<T>(2, 2, Zero<T>);
   Array2D<T>(Rows{2}, Cols{2}, Value{Zero<T>});
   Array2D<T>{{Zero<T>, Zero<T>}, {Zero<T>, Zero<T>}};

   Array2D<T> A(2, 2);
   Array2D<T> B = A;
   Array2D<T> C = const2D<T>(2, 2, Zero<T>);
   Array2D<T> D = std::move(A);

   D = Zero<T>;
   D = {{Zero<T>, Zero<T>}, {Zero<T>, Zero<T>}};
   D = B;
   D = const2D<T>(2, 2, Zero<T>);
   D = std::move(B);

   D.swap(C);
   D.swap(std::move(C));
   D.resize_and_assign(C);
   D.resize_and_assign(std::move(C));
   D.resize(20, 20);
   D.resize(20, 20, false);

   D.remove_rows(0, 1);
   D.remove_rows(Pos{0}, Count{1});
   D.remove_cols(0, 1);
   D.remove_cols(Pos{0}, Count{1});
   D.remove_row(0);
   D.remove_row(Pos{0});
   D.remove_row(last);
   D.remove_col(0);
   D.remove_col(Pos{0});
   D.remove_col(last);

   D.remove_rows_front(1);
   D.remove_rows_front(Count{1});
   D.remove_rows_back(1);
   D.remove_rows_back(Count{1});
   D.remove_cols_front(1);
   D.remove_cols_front(Count{1});
   D.remove_cols_back(1);
   D.remove_cols_back(Count{1});

   D.remove_rows({0, 2, 4});
   D.remove_cols({0, 2, 4});

   D.resize(2, D.cols());
   Array2D<T> E1(1, D.cols());
   D.insert_rows(0, E1);
   D.insert_rows(Pos{0}, E1);
   D.insert_rows_front(E1);
   D.insert_rows_back(E1);

   Array2D<T> E2(D.rows(), 1);
   D.insert_cols(0, E2);
   D.insert_cols(Pos{0}, E2);
   D.insert_cols_front(E2);
   D.insert_cols_back(E2);

   D.resize(2, D.cols());
   Array1D<T> x1(D.cols());
   D.insert_row(0, x1);
   D.insert_row(Pos{0}, x1);
   D.insert_row_front(x1);
   D.insert_row_back(x1);

   Array1D<T> x2(D.rows());
   D.insert_col(0, x2);
   D.insert_col(Pos{0}, x2);
   D.insert_col_front(x2);
   D.insert_col_back(x2);

   D.rows();
   D.cols();
   D.size();
   D.un(0);
   D.un(0, 0);
   D.data();
}


template <typename T>
consteval void fixed_array2D() {
   FixedArray2D<T, 2, 2>{};
   FixedArray2D<T, 2, 2>(Zero<T>);
   FixedArray2D<T, 2, 2>(Value{Zero<T>});
   FixedArray2D<T, 2, 2>{{Zero<T>, Zero<T>}, {Zero<T>, Zero<T>}};

   FixedArray2D<T, 2, 2> A;
   FixedArray2D<T, 2, 2> B = A;
   [[maybe_unused]] FixedArray2D<T, 2, 2> C = const2D<T>(2, 2, Zero<T>);
   FixedArray2D<T, 2, 2> D = std::move(A);

   D = Zero<T>;
   D = {{Zero<T>, Zero<T>}, {Zero<T>, Zero<T>}};
   D = B;
   D = const2D<T>(2, 2, Zero<T>);
   D = std::move(B);

   D.size();
   D.un(0);
   D.data();
}


template <typename T>
consteval void array_ops() {
   Array2D<T> x(3, 2);

   sum(x);
   prod(x);
   mean(x);
   min(x);
   max(x);
   min_index(x);
   max_index(x);
   dot_prod(x, x);
   norm_inf(x);
   norm1(x);
   norm1_scaled(x);
   polynomial(x.view1D(), Zero<T>);

   has_zero(x);
   all_zeros(x);
   all_pos(x);
   all_neg(x);
   all_non_pos(x);
   all_non_neg(x);

   none_of(x, Zero<T>);
   any_of(x, Zero<T>);
   all_of(x, Zero<T>);
   none_of(x, []([[maybe_unused]] auto z) { return true_sb; });
   any_of(x, []([[maybe_unused]] auto z) { return true_sb; });
   all_of(x, []([[maybe_unused]] auto z) { return true_sb; });

   in_open_range(x.view1D(), Zero<T>, One<T>);
   in_closed_range(x.view1D(), Zero<T>, One<T>);
   in_cond_range(x.view1D(), []([[maybe_unused]] auto z) { return true_sb; });
   in_open_range(x.view1D(), Low{Zero<T>}, High{One<T>});
   in_closed_range(x.view1D(), Low{Zero<T>}, High{One<T>});

   for_each(x, []([[maybe_unused]] auto z) {});
   sort(x, [](auto z1, auto z2) { return z1 < z2; });
   sort_increasing(x);
   sort_decreasing(x);

#if __cplusplus >= 202302L
   all_finite(x);
   has_inf(x);
   has_nan(x);
   blas_array(x);
#endif
}


template <typename T>
consteval void expr_ops() {
   Array2D<T> x(3, 2, One<T>);

   if constexpr(!Boolean<T>) {
      +x;
      -x;
      abs(x);
      array_cast<int>(x);

      x + One<T>;
      x - One<T>;
      x* One<T>;
      x / One<T>;

      One<T> + x;
      One<T> - x;
      One<T>* x;
      One<T> / x;

      x + x;
      x - x;
      x* x;
      x / x;

      e_unit<T>(0, 2);
      identity<T>(2);

      if constexpr(Integer<T>) {
         ~x;
         x % One<T>;
         x << One<T>;
         x >> One<T>;
         x& One<T>;
         x | One<T>;
         x ^ One<T>;

         One<T> % x;
         One<T> << x;
         One<T> >> x;
         One<T>& x;
         One<T> | x;
         One<T> ^ x;

         x % x;
         x << x;
         x >> x;
         x & x;
         x | x;
         x ^ x;
      }

      if constexpr(Floating<T>) {
         fast_pow_int(x, 2);
         inv(x);
         linspace<T>(4, Zero<T>, One<T>);
         sequence<T>(4, Zero<T>, One<T>);
      }
   } else {
      !x;

      x&& true_sb;
      x || true_sb;
      x ^ true_sb;

      true_sb&& x;
      true_sb || x;
      true_sb ^ x;

      x&& x;
      x || x;
      x ^ x;
   }

   merge(x.view1D(), x.view1D(), x.view1D());
   merge(x.view1D(), One<T>, One<T>);
   merge(One<T>, One<T>, x.view1D());
   exclude(x.view1D(), 0);
   exclude(x.view1D(), last);
   const1D<T>(2, One<T>);
   const2D<T>(2, 2, One<T>);
}


template <typename T>
consteval void derived1D() {
   Array1D<T> x(10);
   x[0];
   x.at(0);
   x.empty();
   x.size_m1();

   x.begin();
   x.end();
   x.cbegin();
   x.cend();
   x.rbegin();
   x.rend();
   x.crbegin();
   x.crend();

   x.view1D();
   x.view2D(2, 5);
   x.dimension();

   std::move(x).lval();
   x.eval();
}


template <typename T>
consteval void derived2D() {
   Array2D<T> x(5, 2);
   x(0, 0);
   x.at(0, 0);
   x.empty();
   x.size_m1();
   x.rows();
   x.cols();

   x.begin();
   x.end();
   x.cbegin();
   x.cend();
   x.rbegin();
   x.rend();
   x.crbegin();
   x.crend();

   x.view1D();
   x.view2D(2, 5);
   x.dimension();

   std::move(x).lval();
   x.eval();
}


template <typename T>
consteval void slice_ops_1D() {
   Array1D<T> x(5);
   const Array1D<T> xc(5);

   x.view1D();
   xc.view1D();
   x.view2D(1, 5);
   xc.view2D(1, 5);

   x({0, 1, 2});
   xc({0, 1, 2});

   x(seq{0, 2});
   xc(seq{0, 2});

   x(seqN{0, 2});
   xc(seqN{0, 2});

   x(skipN{2});
   xc(skipN{2});

   x(firstN{2});
   xc(firstN{2});

   x(lastN{2});
   xc(lastN{2});

   x(complement({0}));
   xc(complement({0}));

   x(even);
   xc(even);

   x(odd);
   xc(odd);

   x(reverse);
   xc(reverse);
}


template <typename T>
consteval void slice_ops_2D() {
   Array2D<T> x(5, 5);
   const Array2D<T> xc(5, 5);

   x.view1D();
   xc.view1D();
   x.view2D(25, 1);
   xc.view2D(25, 1);

   x({0, 1, 2}, {0, 1, 2});
   xc({0, 1, 2}, {0, 1, 2});

   x(seq{0, 2}, seq{0, 2});
   xc(seq{0, 2}, seq{0, 2});

   x(seqN{0, 2}, seqN{0, 2});
   xc(seqN{0, 2}, seqN{0, 2});

   x(skipN{2}, skipN{2});
   xc(skipN{2}, skipN{2});

   x(firstN{2}, firstN{2});
   xc(firstN{2}, firstN{2});

   x(lastN{2}, lastN{2});
   xc(lastN{2}, lastN{2});

   x(complement{{0}}, complement{{0}});
   xc(complement{{0}}, complement{{0}});

   x(even, even);
   xc(even, even);

   x(odd, odd);
   xc(odd, odd);

   x(reverse, reverse);
   xc(reverse, reverse);

   x.row(0);
   xc.row(0);

   x.col(0);
   xc.col(0);

   x.diag();
   xc.diag();

   x.diag(1);
   xc.diag(1);

   x.rows({0, 1});
   x.cols({0, 1});

   x.block(0, 0, 2, 2);
   xc.block(0, 0, 2, 2);

   x.blockN(0, 0, 2, 2);
   xc.blockN(0, 0, 2, 2);
}


int main() {
   TEST_ALL_TYPES(array1D);
   TEST_ALL_TYPES(fixed_array1D);
   TEST_ALL_TYPES(array2D);
   TEST_ALL_TYPES(fixed_array2D);
   TEST_ALL_FLOAT_TYPES(array_ops);
   TEST_ALL_TYPES(expr_ops);
   TEST_ALL_TYPES(derived1D);
   TEST_ALL_TYPES(derived2D);
   TEST_ALL_TYPES(slice_ops_1D);
   TEST_ALL_TYPES(slice_ops_2D);
   return EXIT_SUCCESS;
}

