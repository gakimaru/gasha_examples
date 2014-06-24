//--------------------------------------------------------------------------------
// exmaple_type_traits.cpp
// 型特性ユーティリティテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_type_traits.h"//型特性ユーティリティ

#include <gasha/type_traits.h>//rankof(), sizeof(), sizeofelemof()

#include <type_traits>//std::rank, std::extent
#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//型特性ユーティリティテスト
void example_type_traits()
{
	{
		printf("\n");
		printf("----- Test for rankof, elementof, sizeofelemof -----\n");
		int var0 = 0;
		int var1[1] = { 0 };
		int var2[2][3] = { 0 };
		int var3[4][5][6] = { 0 };
		int var4[7][8][9][10] = { 0 };
		printf("var0(Rank=%d, Total=%d, ElemtSize=%d)\n", rankof(var0), extentof(var0), sizeofelemof(var0));
		printf("var1[%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var1), rankof(var1), extentof(var1), sizeofelemof(var1));
		printf("var2[%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var2), extent2of(var2), rankof(var2), extentof(var2), sizeofelemof(var2));
		printf("var3[%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var3), extent2of(var3), extent3of(var3), rankof(var3), extentof(var3), sizeofelemof(var3));
		printf("var4[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var4), extent2of(var4), extent3of(var4), extent4of(var4), rankof(var4), extentof(var4), sizeofelemof(var4));
		printf("\n");
		printf("var0[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var0), extent2of(var0), extent3of(var0), extent4of(var0), rankof(var0), extentof(var0), sizeofelemof(var0));
		printf("var1[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var1), extent2of(var1), extent3of(var1), extent4of(var1), rankof(var1), extentof(var1), sizeofelemof(var1));
		printf("var2[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var2), extent2of(var2), extent3of(var2), extent4of(var2), rankof(var2), extentof(var2), sizeofelemof(var2));
		printf("var3[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var3), extent2of(var3), extent3of(var3), extent4of(var3), rankof(var3), extentof(var3), sizeofelemof(var3));
		printf("var4[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", extent1of(var4), extent2of(var4), extent3of(var4), extent4of(var4), rankof(var4), extentof(var4), sizeofelemof(var4));
	}
	{
		printf("\n");
		printf("----- Test for sizeofvalueof -----\n");
		struct data_t
		{
			char m_val[128];
		};
		data_t var1;
		data_t* var2;
		var1.m_val[0] = 0;
		var2 = &var1;
		printf("sizeof(var1)=%d, sizeof(var2)=%d\n", sizeof(var1), sizeof(var2));
		printf("sizeofvalueof(var1)=%d, sizeofvalueof(var2)=%d\n", sizeofvalueof(var1), sizeofvalueof(var2));
	}

	//比較用：std::rank, std::extent
	{
		printf("\n");
		printf("----- Test for std::rank, std::extent -----\n");
		typedef int arr0_t;
		typedef int arr1_t[1];
		typedef int arr2_t[2][3];
		typedef int arr3_t[4][5][6];
		typedef int arr4_t[7][8][9][10];
		printf("arr0_t(Rank=%d)\n", std::rank<arr0_t>::value);
		printf("arr1_t[%d](Rank=%d)\n", std::extent<arr1_t, 0>::value, std::rank<arr1_t>::value);
		printf("arr2_t[%d][%d](Rank=%d)\n", std::extent<arr2_t, 0>::value, std::extent<arr2_t, 1>::value, std::rank<arr2_t>::value);
		printf("arr3_t[%d][%d][%d](Rank=%d)\n", std::extent<arr3_t, 0>::value, std::extent<arr3_t, 1>::value, std::extent<arr3_t, 2>::value, std::rank<arr3_t>::value);
		printf("arr4_t[%d][%d][%d][%d](Rank=%d)\n", std::extent<arr4_t, 0>::value, std::extent<arr4_t, 1>::value, std::extent<arr4_t, 2>::value, std::extent<arr4_t, 3>::value, std::rank<arr4_t>::value);
		printf("\n");
		printf("arr0_t[%d][%d][%d][%d](Rank=%d)\n", std::extent<arr0_t, 0>::value, std::extent<arr0_t, 1>::value, std::extent<arr0_t, 2>::value, std::extent<arr0_t, 3>::value, std::rank<arr0_t>::value);
		printf("arr1_t[%d][%d][%d][%d](Rank=%d)\n", std::extent<arr1_t, 0>::value, std::extent<arr1_t, 1>::value, std::extent<arr1_t, 2>::value, std::extent<arr1_t, 3>::value, std::rank<arr1_t>::value);
		printf("arr2_t[%d][%d][%d][%d](Rank=%d)\n", std::extent<arr2_t, 0>::value, std::extent<arr2_t, 1>::value, std::extent<arr2_t, 2>::value, std::extent<arr2_t, 3>::value, std::rank<arr2_t>::value);
		printf("arr3_t[%d][%d][%d][%d](Rank=%d)\n", std::extent<arr3_t, 0>::value, std::extent<arr3_t, 1>::value, std::extent<arr3_t, 2>::value, std::extent<arr3_t, 3>::value, std::rank<arr3_t>::value);
		printf("arr4_t[%d][%d][%d][%d](Rank=%d)\n", std::extent<arr4_t, 0>::value, std::extent<arr4_t, 1>::value, std::extent<arr4_t, 2>::value, std::extent<arr4_t, 3>::value, std::rank<arr4_t>::value);
	}
}

// End of file
