#! /bin/bash

var=$1
mode=$2

if [ "$var" == "" ]; then
   var="clang"
fi

if [ "$mode" == "" ]; then
   mode="0"
fi

make all compiler=$var debug=$mode -j8

echo -e "\nRUNNING INFO"
./info.x

echo -e "\nRUNNING FIXED ARRAY 1D TESTS"
./fixed_array1D.x

echo -e "\nRUNNING FIXED ARRAY 2D TESTS"
./fixed_array2D.x

echo -e "\nRUNNING ARRAY 1D TESTS"
./array1D.x

echo -e "\nRUNNING ARRAY 2D TESTS"
./array2D.x

echo -e "\nRUNNING SLICEARRAY TESTS"
./slicearray.x

echo -e "\nRUNNING EXPRESSION TEMPLATE TESTS"
./expr.x

echo -e "\nRUNNING ITERATOR TESTS"
./iterator.x

echo -e "\nRUNNING ARRAY OPERATIONS TESTS"
./array_ops.x

echo -e "\nRUNNING ARRAY STABLE OPERATIONS TESTS"
./array_stable_ops.x

echo -e "\nRUNNING ERROR TOOLS TESTS"
./error_tools.x

echo -e "\nRUNNING Array 1D vs 2D TESTS"
./array_1Dvs2D.x

echo -e "\nRUNNING CONSTEXPR TESTS"
./constexpr.x

echo -e "\nRUNNING EMPTY TESTS"
./empty.x

echo -e ""
make clean
echo -e ""
