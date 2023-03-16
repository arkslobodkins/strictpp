// Arkadijs Slobodkins, 2023

#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <numeric>
#include <type_traits>
#include <utility>

#include "strict_concepts.hpp"
#include "strict_error.hpp"
#include "strict_iterator.hpp"
#include "strict_val.hpp"
#include "strict_array_ops.hpp"

namespace strict_array {

class ArrayBase1D : protected Base {};
class ArrayExpr1D : protected Expr, protected ArrayBase1D {};
template<typename T> concept ArrayBaseType1D = BaseOf<ArrayBase1D, T>;
template<typename T> concept ArrayExprType1D = BaseOf<ArrayExpr1D, T>;

class SliceArrayBase1D : protected Base {};
class SliceArrayExpr1D : protected Expr, protected SliceArrayBase1D {};
template<typename T> concept SliceArrayBaseType1D = BaseOf<SliceArrayBase1D, T>;
template<typename T> concept SliceArrayExprType1D = BaseOf<SliceArrayExpr1D, T>;

template<typename T> concept OneDimBaseType = ArrayBaseType1D<T> || SliceArrayBaseType1D<T>;
template<typename T> concept OneDimFloatingBaseType = OneDimBaseType<T> && FloatingBaseType<T>;

// TODO: template size_type for Array and auxiliary objects

// Forward declarations
class slice;
class seq;

template<RealType T> class Low;
template<RealType T> class High;
template<RealType T> class Start;
template<RealType T> class End;
template<RealType T> class Incr;

class Size;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
class Array : private ArrayBase1D
{
public:
   using size_type = long long int;
   using value_type = StrictVal<T>;
   using real_type = T;
   using base_type = ArrayBase1D;
   using expr_base_type = ArrayExpr1D;

   // expr_type ind slice_type inform expression templates and SliceArrays
   // about how the object should be stored.
   using expr_type = const Array<T> &;
   using slice_type = Array<T> &;

   explicit Array();
   explicit Array(size_type size);
   explicit Array(size_type size, value_type val);
   explicit Array(std::initializer_list<value_type> list);
   Array(const Array & A);
   Array(Array && a) noexcept;

   Array & operator=(value_type val) &;
   Array & operator=(std::initializer_list<value_type> list) &;
   Array & operator=(const Array & A) &;
   Array & operator=(Array && A) & noexcept;

   // assign any one-dimensional type
   template<OneDimBaseType OneDimBaseT> Array & Assign(const OneDimBaseT & A) &;

   // constructor and assignment for Array expression templates
   template<ArrayExprType1D ArrayExprT1D> Array(const ArrayExprT1D & expr);
   template<ArrayExprType1D ArrayExprT1D> Array & operator=(const ArrayExprT1D & expr) &;

   ~Array();

   Array & operator+=(value_type val) &;
   Array & operator-=(value_type val) &;
   Array & operator*=(value_type val) &;
   Array & operator/=(value_type val) &;

   template<ArrayBaseType1D ArrayBaseT1D> Array & operator+=(const ArrayBaseT1D & A) &;
   template<ArrayBaseType1D ArrayBaseT1D> Array & operator-=(const ArrayBaseT1D & A) &;
   template<ArrayBaseType1D ArrayBaseT1D> Array & operator*=(const ArrayBaseT1D & A) &;
   template<ArrayBaseType1D ArrayBaseT1D> Array & operator/=(const ArrayBaseT1D & A) &;

   void swap(Array & A) noexcept;
   void resize(size_type size);
   template<ArrayBaseType1D ArrayBaseT1D> void resize_and_assign(const ArrayBaseT1D & A);

   [[nodiscard]] inline value_type & operator[](size_type i);
   [[nodiscard]] inline const value_type & operator[](size_type i) const;

   [[nodiscard]] inline auto operator[](seq s);
   [[nodiscard]] inline auto operator[](seq s) const;

   [[nodiscard]] value_type & first() { return elem[0]; }
   [[nodiscard]] value_type & last() { return elem[sz-1]; }
   [[nodiscard]] const value_type & first() const { return elem[0]; }
   [[nodiscard]] const value_type & last() const { return elem[sz-1]; }

   [[nodiscard]] size_type size() const { ASSERT_STRICT_DEBUG(sz > -1); return sz; }
   [[nodiscard]] bool empty() const { ASSERT_STRICT_DEBUG(sz > -1); return sz == 0; }

   [[nodiscard]] value_type* data() & { return !empty() ? &elem[0] : nullptr; }
   [[nodiscard]] const value_type* data() const & { return !empty() ? &elem[0] : nullptr; }

   template<RealType U> [[nodiscard]] Array<U> convert_type() const; // conversion chosen by the user;

   [[nodiscard]] auto begin() { return iterator{*this, 0}; }
   [[nodiscard]] auto end() { return iterator{*this, size()}; }
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }
   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   value_type* elem;
   size_type sz;

   template<typename F> void apply0(F f);
   template<ArrayBaseType1D ArrayBaseT1D, typename F>
      void apply1(const ArrayBaseT1D & A, F f);
};

// construct Array from any one-dimensional type of the same real type
template<OneDimBaseType OneDimBaseT>
[[nodiscard]] auto ConstructArray(const OneDimBaseT & A);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// binary operations
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator+(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator-(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator*(const T1 & A, const T2 & B);
template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto operator/(const T1 & A, const T2 & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(StrictVal<U> val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, StrictVal<U> val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(U val, const T & B);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator+(const T & A, U val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator-(const T & A, U val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator*(const T & A, U val);
template<OneDimBaseType T, RealType U> [[nodiscard]] auto operator/(const T & A, U val);

// unary operations
template<OneDimBaseType T> [[nodiscard]] auto operator+(const T & A);
template<OneDimBaseType T> [[nodiscard]] auto operator-(const T & A);
template<OneDimBaseType T> [[nodiscard]] auto abs(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p);
template<OneDimFloatingBaseType T> [[nodiscard]] auto pow_int(const T & A, StrictVal<int> p);
template<OneDimFloatingBaseType T> [[nodiscard]] auto exp(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto log(const T & A);
template<OneDimFloatingBaseType T> [[nodiscard]] auto sqrt(const T & A);

template<OneDimBaseType T1, OneDimBaseType T2> [[nodiscard]] auto two_prod(const T1 & A, const T2 & B);
template<RealType T> [[nodiscard]] auto e_unit(long long int j, long long int size);
template<RealType T> [[nodiscard]] auto e_slice_unit(long long int j, long long int size);

template<RealType T>
[[nodiscard]] auto sequence(Size size, Start<T> start = Start<T>{}, Incr<T> incr = Incr<T>{T(1)});

template<RealType T>
[[nodiscard]] auto slice_sequence(Size size, Start<T> start = Start<T>{}, Incr<T> incr = Incr<T>{T(1)});

template<RealType T>
[[nodiscard]] auto linspace(Size size, Start<T> start = Start<T>{}, End<T> end = End<T>{T(1)});

template<RealType T>
[[nodiscard]] auto slice_linspace(Size size, Start<T> start = Start<T>{}, End<T> end = End<T>{T(1)});

template<IntegerType T>
[[nodiscard]] Array<T> array_random(SizeTypeOf<Array<T>> size, Low<T> low = Low<T>{}, High<T> high = High<T>{T(1)});

template<FloatingType T>
[[nodiscard]] Array<T> array_random(SizeTypeOf<Array<T>> size, Low<T> low = Low<T>{}, High<T> high = High<T>{T(1)});

#define STRICT_GENERATE_SMALL_TYPES(SmallObjectName)      \
template<RealType T>                                      \
class SmallObjectName                                     \
{                                                         \
public:                                                   \
   explicit SmallObjectName() = default;                  \
   explicit SmallObjectName(T x) : x{x} {}                \
   explicit SmallObjectName(StrictVal<T> x) : x{x} {}     \
   StrictVal<T> get() const { return x; }                 \
private:                                                  \
   StrictVal<T> x;                                        \
};

#define STRICT_GENERATE_SMALL_INT_TYPES(SmallObjectName)  \
class SmallObjectName                                     \
{                                                         \
public:                                                   \
   explicit SmallObjectName() : x{} {}                    \
   explicit SmallObjectName(long long int x) : x{x} {}    \
   long long int get() const { return x; }                \
private:                                                  \
   long long int x;                                       \
};

STRICT_GENERATE_SMALL_TYPES(Low)
STRICT_GENERATE_SMALL_TYPES(High)
STRICT_GENERATE_SMALL_TYPES(Start)
STRICT_GENERATE_SMALL_TYPES(End)
STRICT_GENERATE_SMALL_TYPES(Incr)

STRICT_GENERATE_SMALL_INT_TYPES(Size)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace internal {
   template<BaseType BaseT>
   bool valid_index(const BaseT & A, SizeTypeOf<BaseT> index) {
      return index > -1 && index < A.size();
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class slice
{
public:
   explicit slice(long long int start, long long int size, long long int stride)
      : m_start{start}, m_size{size}, m_stride{stride} {
      ASSERT_STRICT_DEBUG(start > -1);
      ASSERT_STRICT_DEBUG(size > 0);
      ASSERT_STRICT_DEBUG(stride > 0);
   }

   [[nodiscard]] long long int start() const { return m_start; }
   [[nodiscard]] long long int size() const { return m_size; }
   [[nodiscard]] long long int stride() const { return m_stride; }

   template<BaseType BaseT>
   bool valid(const BaseT & A) const
   {
      return internal::valid_index(A, m_start) &&
         internal::valid_index(A, m_start + m_stride * (m_size-1));
   }

private:
   long long int m_start;
   long long int m_size;
   long long int m_stride;
};

class seq
{
public:
   explicit seq(long long int first, long long int last, long long int stride = 1)
      : m_first{first}, m_last{last}, m_stride{stride}
   {
      ASSERT_STRICT_DEBUG(first > -1);
      ASSERT_STRICT_DEBUG(last >= first);
      ASSERT_STRICT_DEBUG(stride > 0);
   }

   slice to_slice() const { return slice{m_first, (m_last-m_first)/m_stride + 1, m_stride}; }

   [[nodiscard]] long long int first() const { return m_first; }
   [[nodiscard]] long long int last() const { return m_last; }
   [[nodiscard]] long long int stride() const { return m_stride; }

   template<BaseType BaseT>
   bool valid(const BaseT & A) const
   {
      return internal::valid_index(A, m_first) &&
         internal::valid_index(A, m_last);
   }

private :
   long long int m_first;
   long long int m_last;
   long long int m_stride;
};

//template<DirectBaseType DirectBaseT>
//class RandSliceArray : private SliceArrayBase1D
//{
//public:
//   using size_type = typename DirectBaseT::size_type;
//   using value_type = typename DirectBaseT::value_type;
//   using real_type = typename DirectBaseT::real_type;
//   using base_type = SliceArrayBase1D;
//   using expr_base_type = SliceArrayExpr1D;
//   using expr_type = const RandSliceArray<DirectBaseT>;
//   using slice_type = RandSliceArray<DirectBaseT>;
//
//   explicit inline RandSliceArray(DirectBaseT & A, const Array<long long int> & index_array);
//   RandSliceArray(const RandSliceArray & s);
//
//   RandSliceArray & operator=(const RandSliceArray & s);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator=(const SliceArrayBaseT1D & s);
//   RandSliceArray & operator=(StrictVal<real_type> s);
//   RandSliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);
//
//   // assign either Array, SliceArray, or their expression template
//   template<OneDimBaseType OneDimBaseT> RandSliceArray & Assign(const OneDimBaseT & A) &;
//
//   [[nodiscard]] auto & operator[](size_type i)
//      { return A[index_array[i]]; }
//   [[nodiscard]] const auto & operator[](size_type i) const
//      { return A[index_array[i]]; }
//
//   [[nodiscard]] inline auto sl(size_type first, size_type last);
//   [[nodiscard]] inline auto sl(size_type first, size_type last) const;
//   [[nodiscard]] inline auto sl(slice slice);
//   [[nodiscard]] inline auto sl(slice slice) const;
//
//   [[nodiscard]] size_type size() const { return index_array.size(); }
//   [[nodiscard]] bool empty() const { return A.empty(); }
//
//   RandSliceArray & operator+=(StrictVal<real_type> val);
//   RandSliceArray & operator-=(StrictVal<real_type> val);
//   RandSliceArray & operator*=(StrictVal<real_type> val);
//   RandSliceArray & operator/=(StrictVal<real_type> val);
//
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator+=(const SliceArrayBaseT1D & A);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator-=(const SliceArrayBaseT1D & A);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator*=(const SliceArrayBaseT1D & A);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D> RandSliceArray & operator/=(const SliceArrayBaseT1D & A);
//
//   [[nodiscard]] auto & first() { return (*this)[0]; }
//   [[nodiscard]] auto & last() { return (*this)[index_array.size()-1]; }
//   [[nodiscard]] auto & first() const { return (*this)[0]; }
//   [[nodiscard]] auto & last() const { return (*this)[index_array.size()-1]; }
//
//   [[nodiscard]] auto begin() { return iterator{*this, 0}; }
//   [[nodiscard]] auto end() { return iterator{*this, size()}; }
//   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
//   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
//   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
//   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }
//
//   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }
//   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }
//   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
//   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
//   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
//   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }
//
//private:
//   typename DirectBaseT::slice_type A;
//   Array<long long int> index_array;
//
//   template<typename F> void apply0(F f);
//   template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
//      void apply1(const SliceArrayBaseT1D & A, F f);
//};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
class SliceArray : private SliceArrayBase1D
{
public:
   using size_type = typename DirectBaseT::size_type;
   using value_type = typename DirectBaseT::value_type;
   using real_type = typename DirectBaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = const SliceArray<DirectBaseT>;
   using slice_type = SliceArray<DirectBaseT>;

   explicit inline SliceArray(DirectBaseT & A, slice sl);
   SliceArray(const SliceArray & s);

   SliceArray & operator=(const SliceArray & s);

   // assign other types of 1-D SliceArray
   template<SliceArrayBaseType1D SliceArrayBaseT1D>
      SliceArray & operator=(const SliceArrayBaseT1D & s);

   SliceArray & operator=(StrictVal<real_type> s);
   SliceArray & operator=(std::initializer_list<StrictVal<real_type>> list);

   // assign either Array, SliceArray, or their expression template
   template<OneDimBaseType OneDimBaseT> SliceArray & Assign(const OneDimBaseT & A);

   [[nodiscard]] auto & operator[](size_type i) {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return A[sl.start() + i*sl.stride()];
   }

   [[nodiscard]] const auto & operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return A[sl.start() + i*sl.stride()];
   }

   [[nodiscard]] inline auto operator[](seq s);
   [[nodiscard]] inline auto operator[](seq s) const;

   [[nodiscard]] size_type size() const { return sl.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   SliceArray & operator+=(StrictVal<real_type> val);
   SliceArray & operator-=(StrictVal<real_type> val);
   SliceArray & operator*=(StrictVal<real_type> val);
   SliceArray & operator/=(StrictVal<real_type> val);

   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator+=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator-=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator*=(const SliceArrayBaseT1D & A);
   template<SliceArrayBaseType1D SliceArrayBaseT1D> SliceArray & operator/=(const SliceArrayBaseT1D & A);

   [[nodiscard]] auto & first() { return (*this)[0]; }
   [[nodiscard]] auto & last() { return (*this)[sl.size()-1]; }
   [[nodiscard]] auto & first() const { return (*this)[0]; }
   [[nodiscard]] auto & last() const { return (*this)[sl.size()-1]; }

   [[nodiscard]] auto begin() { return iterator{*this, 0}; }
   [[nodiscard]] auto end() { return iterator{*this, size()}; }
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() { return std::reverse_iterator{end()}; }
   [[nodiscard]] auto rend() { return std::reverse_iterator{begin()}; }
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename DirectBaseT::slice_type A;
   slice sl;

   template<typename F> void apply0(F f);
   template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
      void apply1(const SliceArrayBaseT1D & A, F f);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
class ConstSliceArray : private SliceArrayBase1D
{
public:
   using size_type = typename BaseT::size_type;
   using value_type = typename BaseT::value_type;
   using real_type = typename BaseT::real_type;
   using base_type = SliceArrayBase1D;
   using expr_base_type = SliceArrayExpr1D;
   using expr_type = const ConstSliceArray<BaseT>;

   explicit inline ConstSliceArray(const BaseT & A, slice sl);
   ConstSliceArray(const ConstSliceArray & cs);
   ConstSliceArray & operator=(const ConstSliceArray &) = delete;

   [[nodiscard]] decltype(auto) operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return A[sl.start()+i*sl.stride()];
   }

   [[nodiscard]] inline auto operator[](seq s) const;

   [[nodiscard]] size_type size() const { return sl.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] decltype(auto) first() const { return (*this)[0]; }
   [[nodiscard]] decltype(auto) last() const { return (*this)[sl.size()-1]; }

   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }

   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; }
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

private:
   typename BaseT::expr_type A;
   slice sl;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T>::Array() :
   elem{},
   sz{}
{}

template<RealType T>
Array<T>::Array(size_type size) :
   elem{new StrictVal<T>[static_cast<std::size_t>(size)]},
   sz{size}
{ ASSERT_STRICT_DEBUG(sz > -1); }

template<RealType T>
Array<T>::Array(size_type size, StrictVal<T> val) :
   Array(size)
{
   ASSERT_STRICT_DEBUG(sz > 0);
   std::fill(begin(), end(), val);
}

template<RealType T>
Array<T>::Array(std::initializer_list<StrictVal<T>> list) :
   Array(static_cast<size_type>(list.size()))
{ std::copy(list.begin(), list.end(), begin()); }

template<RealType T> Array<T>::Array(const Array<T> & A) :
   Array(A.size())
{ std::copy(A.begin(), A.end(), begin()); }

template<RealType T>
Array<T>::Array(Array<T> && A) noexcept :
   elem{std::exchange(A.elem, {})},
   sz{std::exchange(A.sz, {})}
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T> & Array<T>::operator=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   std::fill(begin(), end(), val);
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(std::initializer_list<StrictVal<T>> list) &
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(const Array<T> & A) &
{
   if(this != &A) {
      ASSERT_STRICT_DEBUG(sz == A.sz);
      std::copy(A.begin(), A.end(), begin());
   }
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator=(Array<T> && A) & noexcept
{
   ASSERT_STRICT_DEBUG(sz == A.sz);
   Array<T> temp(std::move(A));
   swap(temp);
   return *this;
}

template<RealType T> template<OneDimBaseType OneDimBaseT>
Array<T> & Array<T>::Assign(const OneDimBaseT & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayExprType1D ArrayExprT1D>
Array<T>::Array(const ArrayExprT1D & expr) :
   Array(expr.size())
{ std::copy(expr.begin(), expr.end(), begin()); }

template<RealType T> template<ArrayExprType1D ArrayExprT1D>
Array<T> & Array<T>::operator=(const ArrayExprT1D & expr) &
{
   ASSERT_STRICT_DEBUG(sz == expr.size());
   std::copy(expr.begin(), expr.end(), begin());
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> Array<T>::~Array()
{ delete[] elem; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
Array<T> & Array<T>::operator+=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] += val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator-=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] -= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator*=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] *= val; } );
   return *this;
}

template<RealType T>
Array<T> & Array<T>::operator/=(StrictVal<T> val) &
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { elem[i] /= val; } );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator+=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] += A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator-=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] -= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator*=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] *= A[i]; });
   return *this;
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
Array<T> & Array<T>::operator/=(const ArrayBaseT1D & A) &
{
   ASSERT_STRICT_DEBUG(sz == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { elem[i] /= A[i]; });
   return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
void Array<T>::swap(Array<T> & A) noexcept
{
   std::swap(elem, A.elem);
   std::swap(sz, A.sz);
}

template<RealType T>
void Array<T>::resize(size_type size)
{
   Array<T> temp(size);
   std::copy(begin(), begin() + std::min(sz, size), temp.begin());
   swap(temp);
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D>
void Array<T>::resize_and_assign(const ArrayBaseT1D & A)
{
   resize(A.size());
   *this = A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
[[nodiscard]] inline StrictVal<T> & Array<T>::operator[](size_type i)
{
   #ifdef STRICT_DEBUG_ON
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
[[nodiscard]] inline const StrictVal<T> & Array<T>::operator[](size_type i) const
{
   #ifdef STRICT_DEBUG_ON
   if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
   #endif
   return elem[i];
}

template<RealType T>
[[nodiscard]] inline auto Array<T>::operator[](seq s)
{
   ASSERT_STRICT_DEBUG(s.valid(*this));
   return SliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
}

template<RealType T>
[[nodiscard]] inline auto Array<T>::operator[](seq s) const
{
   ASSERT_STRICT_DEBUG(s.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
[[nodiscard]] Array<U> Array<T>::convert_type() const
{
   Array<U> A(size());
   for(size_type i = 0; i < size(); ++i)
      A[i] = ((*this)[i]).template convert_type<U>();
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<typename F>
void Array<T>::apply0(F f)
{
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<RealType T> template<ArrayBaseType1D ArrayBaseT1D, typename F>
void Array<T>::apply1(const ArrayBaseT1D & A, F f)
{
   (void)A;
   for(size_type i = 0; i < sz; ++i)
      f(i);
}

template<OneDimBaseType OneDimBaseT>
[[nodiscard]] auto ConstructArray(const OneDimBaseT & A)
{
   Array<RealTypeOf<OneDimBaseT>> C(A.size());
   C.Assign(A);
   return C;
}

template<IntegerType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, Low<T> low, High<T> high)
{
   auto l = low.get();
   auto h = high.get();
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(h >= l);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   long int diff_range = (h - l).template convert_type<long int>() + 1;
   for(auto & x : A)
      x = l + T(std::rand() % diff_range);
   return A;
}

template<FloatingType T>
[[nodiscard]] Array<T> array_random(typename Array<T>::size_type size, Low<T> low, High<T> high)
{
   auto l = low.get();
   auto h = high.get();
   ASSERT_STRICT_DEBUG(size > 0);
   ASSERT_STRICT_DEBUG(h >= l);
   std::srand(static_cast<unsigned>((std::time(0))));
   Array<T> A(size);
   for(auto & x : A)
      x = l + (h - l) * T(std::rand()) / T(RAND_MAX);
   return A;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<DirectBaseType DirectBaseT>
inline SliceArray<DirectBaseT>::SliceArray(DirectBaseT & A, slice sl) : A{A}, sl{sl}
{ ASSERT_STRICT_DEBUG(sl.valid(A)); }

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT>::SliceArray(const SliceArray<DirectBaseT> & s) : A{s.A}, sl{s.sl}
{}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const SliceArray<DirectBaseT> & s)
{
   if(this != &s) {
      ASSERT_STRICT_DEBUG(size() == s.size());
      std::copy(s.begin(), s.end(), begin());
   }
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(const SliceArrayBaseT1D & s)
{
   static_assert(SameType<typename SliceArrayBaseT1D::real_type, real_type>);
   ASSERT_STRICT_DEBUG(size() == s.size());
      std::copy(s.begin(), s.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(StrictVal<real_type> val)
{
   std::fill(begin(), end(), val);
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator=(std::initializer_list<StrictVal<real_type>> list)
{
   ASSERT_STRICT_DEBUG(size() == static_cast<size_type>(list.size()));
   std::copy(list.begin(), list.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT> template<OneDimBaseType OneDimBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::Assign(const OneDimBaseT & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   std::copy(A.begin(), A.end(), begin());
   return *this;
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::operator[](seq s)
{
   ASSERT_STRICT_DEBUG(s.valid(*this));
   return SliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
}

template<DirectBaseType DirectBaseT>
[[nodiscard]] inline auto SliceArray<DirectBaseT>::operator[](seq s) const
{
   ASSERT_STRICT_DEBUG(s.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] += val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] -= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] *= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(StrictVal<real_type> val)
{
   ASSERT_STRICT_DEBUG(!empty());
   apply0([&](size_type i) { (*this)[i] /= val; } );
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator+=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] += A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator-=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] -= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator*=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] *= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D>
SliceArray<DirectBaseT> & SliceArray<DirectBaseT>::operator/=(const SliceArrayBaseT1D & A)
{
   ASSERT_STRICT_DEBUG(size() == A.size());
   ASSERT_STRICT_DEBUG(!empty());
   apply1(A, [&](size_type i) { (*this)[i] /= A[i]; });
   return *this;
}

template<DirectBaseType DirectBaseT> template<typename F>
void SliceArray<DirectBaseT>::apply0(F f)
{
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

template<DirectBaseType DirectBaseT> template<SliceArrayBaseType1D SliceArrayBaseT1D, typename F>
void SliceArray<DirectBaseT>::apply1(const SliceArrayBaseT1D & A, F f)
{
   (void)A;
   for(size_type i = 0; i < size(); ++i)
      f(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BaseType BaseT>
inline ConstSliceArray<BaseT>::ConstSliceArray(const BaseT & A, slice sl) : A{A}, sl{sl}
{ ASSERT_STRICT_DEBUG(sl.valid(A)); }

template<BaseType BaseT>
ConstSliceArray<BaseT>::ConstSliceArray(const ConstSliceArray<BaseT> & cs) : A{cs.A}, sl{cs.sl}
{}

template<BaseType BaseT>
[[nodiscard]] inline auto ConstSliceArray<BaseT>::operator[](seq s) const
{
   ASSERT_STRICT_DEBUG(s.valid(*this));
   return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UnaryPlus : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return +strict_val;
   }
};

struct UnaryMinus : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return -strict_val;
   }
};

struct UnaryAbs : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return abss(strict_val);
   }
};

template<RealType T>
struct UnaryPow : private UnaryOperation
{
   UnaryPow(StrictVal<T> p) : p{p} {}

   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return pows(strict_val, p);
   }
private:
   StrictVal<T> p;
};

struct UnaryPowInt : private UnaryOperation
{
   UnaryPowInt(StrictVal<int> p) : p{p} {}

   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return pows_int(strict_val, p);
   }
private:
   StrictVal<int> p;
};

struct UnaryExp : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return exps(strict_val);
   }
};

struct UnaryLog : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return logs(strict_val);
   }
};

struct UnarySqrt : private UnaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> strict_val) const {
      return sqrts(strict_val);
   }
};

struct Plus : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left + right;
   }
};

struct Minus : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left - right;
   }
};

struct Mult : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left * right;
   }
};

struct Divide : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return left / right;
   }
};

struct BinaryTwoProdFirst : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return two_prod(left, right).first;
   }
};

struct BinaryTwoProdSecond : private BinaryOperation
{
   template<RealType T>
   StrictVal<T> operator()(StrictVal<T> left, StrictVal<T> right) const {
      return two_prod(left, right).second;
   }
};

#define STRICT_GENERATE_CONST_ITERATORS()                                       \
   [[nodiscard]] auto begin() const { return const_iterator{*this, 0}; }        \
   [[nodiscard]] auto end() const { return const_iterator{*this, size()}; }     \
   [[nodiscard]] auto cbegin() const { return const_iterator{*this, 0}; }       \
   [[nodiscard]] auto cend() const { return const_iterator{*this, size()}; }    \
                                                                                \
   [[nodiscard]] auto rbegin() const { return std::reverse_iterator{cend()}; }  \
   [[nodiscard]] auto rend() const { return std::reverse_iterator{cbegin()}; }  \
   [[nodiscard]] auto crbegin() const { return std::reverse_iterator{cend()}; } \
   [[nodiscard]] auto crend() const { return std::reverse_iterator{cbegin()}; }

#define STRICT_GENERATE_USING_EXPR_TYPES(OneDimObjectType)            \
   using size_type = typename OneDimObjectType::size_type;            \
   using value_type = typename OneDimObjectType::value_type;          \
   using real_type = typename OneDimObjectType::real_type;            \
   using base_type = typename OneDimObjectType::base_type;            \
   using expr_base_type = typename OneDimObjectType::expr_base_type;  \

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT>
class StandardUnitVectorExpr : private OneDimBaseT::expr_base_type
{
public:
   STRICT_GENERATE_CONST_ITERATORS();
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT);
   using expr_type = StandardUnitVectorExpr<OneDimBaseT>;

   explicit StandardUnitVectorExpr(size_type j, size_type size) : j{j}, sz{size} {
      ASSERT_STRICT_DEBUG(j > -1); ASSERT_STRICT_DEBUG(size > j);
   }

   StandardUnitVectorExpr(const StandardUnitVectorExpr &) = default;
   StandardUnitVectorExpr & operator=(const StandardUnitVectorExpr &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return j == i ? real_type{1} : real_type{0};
   }

   [[nodiscard]] size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return false; }

private:
   const size_type j;
   const size_type sz;
};

template<OneDimBaseType OneDimBaseT>
class SequenceExpr : private OneDimBaseT::expr_base_type
{
public:
   STRICT_GENERATE_CONST_ITERATORS();
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT);
   using expr_type = SequenceExpr<OneDimBaseT>;

   explicit SequenceExpr(value_type start, size_type size, value_type incr)
      : start{start}, sz{size}, incr{incr} {
      ASSERT_STRICT_DEBUG(size > 0);
   }
   SequenceExpr(const SequenceExpr &) = default;

   // assignment is deleted to stay consistent with other expression templates
   SequenceExpr & operator=(const SequenceExpr &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return start + incr * static_cast<real_type>(i);
   }

   [[nodiscard]] size_type size() const { return sz; }
   [[nodiscard]] bool empty() const { return false; }

private:
   value_type start;
   const size_type sz;
   value_type incr;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT, UnaryOperationType Op>
class UnaryExpr : private OneDimBaseT::expr_base_type
{
public:
   STRICT_GENERATE_CONST_ITERATORS();
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT);
   using expr_type = UnaryExpr<OneDimBaseT, Op>;

   explicit UnaryExpr(const OneDimBaseT & A, Op op) : A{A}, op{op} { ASSERT_STRICT_DEBUG(!A.empty()); }
   UnaryExpr(const UnaryExpr &) = default;
   UnaryExpr & operator=(const UnaryExpr &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return op(A[i]);
   }

   [[nodiscard]] auto operator[](seq s) const {
      ASSERT_STRICT_DEBUG(s.valid(*this));
      return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
   }

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] value_type first() const { return (*this)[0]; }
   [[nodiscard]] value_type last() const { return (*this)[size()-1]; }

private:
   typename OneDimBaseT::expr_type A;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, OneDimBaseType OneDimBaseT2, BinaryOperationType Op>
class BinExpr : private OneDimBaseT1::expr_base_type
{
public:
   STRICT_GENERATE_CONST_ITERATORS();
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1);
   using expr_type = BinExpr<OneDimBaseT1, OneDimBaseT2, Op>;

   explicit BinExpr(const OneDimBaseT1 & A, const OneDimBaseT2 & B, Op op) : A{A}, B{B}, op{op} {
      // checking for the same size_type might be useful in the future
      static_assert(SameType<typename OneDimBaseT1::size_type, typename OneDimBaseT2::size_type>);
      static_assert(SameType<typename OneDimBaseT1::value_type, typename OneDimBaseT2::value_type>);
      static_assert(SameType<typename OneDimBaseT1::base_type, typename OneDimBaseT2::base_type>);
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(A.size() == B.size());
   }
   BinExpr(const BinExpr &) = default;
   BinExpr & operator=(const BinExpr &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return op(A[i], B[i]);
   }

   [[nodiscard]] auto operator[](seq s) const {
      ASSERT_STRICT_DEBUG(s.valid(*this));
      return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
   }

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] value_type first() const { return (*this)[0]; }
   [[nodiscard]] value_type last() const { return (*this)[size()-1]; }

private:
   typename OneDimBaseT1::expr_type A;
   typename OneDimBaseT2::expr_type B;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
class BinExprValLeft : private OneDimBaseT1::expr_base_type
{
public:
   STRICT_GENERATE_CONST_ITERATORS();
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1);
   using expr_type = const BinExprValLeft<OneDimBaseT1, T2, Op>;

   explicit BinExprValLeft(const OneDimBaseT1 & B, T2 val, Op op) : B{B}, val{val}, op{op} {
      static_assert(SameType<RealTypeOf<OneDimBaseT1>, T2>);
      ASSERT_STRICT_DEBUG(!B.empty());
   }
   BinExprValLeft(const BinExprValLeft &) = default;
   BinExprValLeft & operator=(const BinExprValLeft &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return op(val, B[i]);
   }

   [[nodiscard]] auto operator[](seq s) const {
      ASSERT_STRICT_DEBUG(s.valid(*this));
      return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
   }

   [[nodiscard]] size_type size() const { return B.size(); }
   [[nodiscard]] bool empty() const { return B.empty(); }

   [[nodiscard]] value_type first() const { return (*this)[0]; }
   [[nodiscard]] value_type last() const { return (*this)[size()-1]; }

private:
   typename OneDimBaseT1::expr_type B;
   StrictVal<T2> val;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType OneDimBaseT1, RealType T2, BinaryOperationType Op>
class BinExprValRight : private OneDimBaseT1::expr_base_type
{
public:
   STRICT_GENERATE_CONST_ITERATORS();
   STRICT_GENERATE_USING_EXPR_TYPES(OneDimBaseT1);
   using expr_type = BinExprValRight<OneDimBaseT1, T2, Op>;

   explicit BinExprValRight(const OneDimBaseT1 & A, T2 val, Op op) : A{A}, val{val}, op{op} {
      static_assert(SameType<RealTypeOf<OneDimBaseT1>, T2>);
      ASSERT_STRICT_DEBUG(!A.empty());
   }
   BinExprValRight(const BinExprValRight &) = default;
   BinExprValRight & operator=(const BinExprValRight &) = delete;

   [[nodiscard]] value_type operator[](size_type i) const {
      #ifdef STRICT_DEBUG_ON
      if(!internal::valid_index(*this, i)) STRICT_THROW_OUT_OF_RANGE();
      #endif
      return op(A[i], val);
   }

   [[nodiscard]] auto operator[](seq s) const {
      ASSERT_STRICT_DEBUG(s.valid(*this));
      return ConstSliceArray<std::decay_t<decltype(*this)>> {*this, s.to_slice()};
   }

   [[nodiscard]] size_type size() const { return A.size(); }
   [[nodiscard]] bool empty() const { return A.empty(); }

   [[nodiscard]] value_type first() const { return (*this)[0]; }
   [[nodiscard]] value_type last() const { return (*this)[size()-1]; }

private:
   typename OneDimBaseT1::expr_type A;
   StrictVal<T2> val;
   Op op;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator+(const T1 & A, const T2 & B)
{ return BinExpr{A, B, Plus{}}; }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator-(const T1 & A, const T2 & B)
{ return BinExpr{A, B, Minus{}}; }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator*(const T1 & A, const T2 & B)
{ return BinExpr{A, B, Mult{}}; }

template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto operator/(const T1 & A, const T2 & B)
{ return BinExpr{A, B, Divide{}}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(StrictVal<U> val, const T & B)
{ return BinExprValLeft{B, U{val}, Plus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(StrictVal<U> val, const T & B)
{ return BinExprValLeft{B, U{val}, Minus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(StrictVal<U> val, const T & B)
{ return BinExprValLeft{B, U{val}, Mult{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(StrictVal<U> val, const T & B)
{ return BinExprValLeft{B, U{val}, Divide{}}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(const T & A, StrictVal<U> val)
{ return BinExprValRight{A, U{val}, Plus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(const T & A, StrictVal<U> val)
{ return BinExprValRight{A, U{val}, Minus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(const T & A, StrictVal<U> val)
{ return BinExprValRight{A, U{val}, Mult{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(const T & A, StrictVal<U> val)
{ return BinExprValRight{A, U{val}, Divide{}}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(U val, const T & B)
{ return BinExprValLeft{B, val, Plus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(U val, const T & B)
{ return BinExprValLeft{B, val, Minus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(U val, const T & B)
{ return BinExprValLeft{B, val, Mult{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(U val, const T & B)
{ return BinExprValLeft{B, val, Divide{}}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator+(const T & A, U val)
{ return BinExprValRight{A, val, Plus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator-(const T & A, U val)
{ return BinExprValRight{A, val, Minus{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator*(const T & A, U val)
{ return BinExprValRight{A, val, Mult{}}; }

template<OneDimBaseType T, RealType U>
[[nodiscard]] auto operator/(const T & A, U val)
{ return BinExprValRight{A, val, Divide{}}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T>
[[nodiscard]] auto operator+(const T & A)
{ return UnaryExpr{A, UnaryPlus{}}; }

template<OneDimBaseType T>
[[nodiscard]] auto operator-(const T & A)
{ return UnaryExpr{A, UnaryMinus{}}; }

template<OneDimBaseType T>
[[nodiscard]] auto abs(const T & A)
{ return UnaryExpr{A, UnaryAbs{}}; }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow(const T & A, ValueTypeOf<T> p)
{ return UnaryExpr{A, UnaryPow{p}}; }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto pow_int(const T & A, StrictVal<int> p)
{ return UnaryExpr{A, UnaryPowInt{p}}; }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto exp(const T & A)
{ return UnaryExpr{A, UnaryExp{}}; }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto log(const T & A)
{ return UnaryExpr{A, UnaryLog{}}; }

template<OneDimFloatingBaseType T>
[[nodiscard]] auto sqrt(const T & A)
{ return UnaryExpr{A, UnarySqrt{}}; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<OneDimBaseType T1, OneDimBaseType T2>
[[nodiscard]] auto two_prod(const T1 & A, const T2 & B)
{ return std::pair{BinExpr{A, B, BinaryTwoProdFirst{}}, BinExpr{A, B, BinaryTwoProdSecond{}}}; }

template<RealType T>
[[nodiscard]] auto e_unit(long long int j, long long int size)
{ return StandardUnitVectorExpr<Array<T>>{j, size}; }

template<RealType T>
[[nodiscard]] auto e_slice_unit(long long int j, long long int size)
{ return StandardUnitVectorExpr<SliceArray<Array<T>>>{j, size}; }

template<RealType T>
[[nodiscard]] auto sequence(Size size, Start<T> start, Incr<T> incr)
{ return SequenceExpr<Array<T>>{start.get(), size.get(), incr.get()}; }

template<RealType T>
[[nodiscard]] auto slice_sequence(Size size, Start<T> start, Incr<T> incr)
{ return SequenceExpr<SliceArray<Array<T>>>{start.get(), size.get(), incr.get()}; }

template<RealType T>
[[nodiscard]] auto linspace(Size size, Start<T> start, End<T> end)
{
   auto sz = size.get();
   ASSERT_STRICT_DEBUG(sz > 1);
   return SequenceExpr<Array<T>>{start.get(), sz, (end.get()-start.get())/strict_cast<T>(sz-1)};
}

template<RealType T>
[[nodiscard]] auto slice_linspace(Size size, Start<T> start, End<T> end)
{
   auto sz = size.get();
   ASSERT_STRICT_DEBUG(sz > 1);
   return SequenceExpr<SliceArray<Array<T>>>{start.get(), sz, (end.get()-start.get())/strict_cast<T>(sz-1)};
}

}

#endif
