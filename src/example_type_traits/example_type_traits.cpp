﻿//--------------------------------------------------------------------------------
// exmaple_type_traits.cpp
// 型特性ユーティリティテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_type_traits.h"//型特性ユーティリティテスト

#include <gasha/type_traits.h>//型特性ユーティリティ：rankof(), sizeof()), static_cast<int>(sizeofelemof()

#include <type_traits>//std::rank, static_cast<int>(std::extent
#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//型特性ユーティリティテスト
void example_type_traits()
{
	{
		std::printf("\n");
		std::printf("----- Test for rankof, elementof, sizeofelemof -----\n");
		int var0 = 0;
		int var1[1] = { 0 };
		int var2[2][3] = { { 0 } };
		int var3[4][5][6] = { { { 0 } } };
		int var4[7][8][9][10] = { { { { 0 } } } };
		std::printf("var0(Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(rankof(var0)), static_cast<int>(extentof(var0)), static_cast<int>(sizeofelemof(var0)));
		std::printf("var1[%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var1)), static_cast<int>(rankof(var1)), static_cast<int>(extentof(var1)), static_cast<int>(sizeofelemof(var1)));
		std::printf("var2[%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var2)), static_cast<int>(extent2of(var2)), static_cast<int>(rankof(var2)), static_cast<int>(extentof(var2)), static_cast<int>(sizeofelemof(var2)));
		std::printf("var3[%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var3)), static_cast<int>(extent2of(var3)), static_cast<int>(extent3of(var3)), static_cast<int>(rankof(var3)), static_cast<int>(extentof(var3)), static_cast<int>(sizeofelemof(var3)));
		std::printf("var4[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var4)), static_cast<int>(extent2of(var4)), static_cast<int>(extent3of(var4)), static_cast<int>(extent4of(var4)), static_cast<int>(rankof(var4)), static_cast<int>(extentof(var4)), static_cast<int>(sizeofelemof(var4)));
		std::printf("\n");
		std::printf("var0[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var0)), static_cast<int>(extent2of(var0)), static_cast<int>(extent3of(var0)), static_cast<int>(extent4of(var0)), static_cast<int>(rankof(var0)), static_cast<int>(extentof(var0)), static_cast<int>(sizeofelemof(var0)));
		std::printf("var1[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var1)), static_cast<int>(extent2of(var1)), static_cast<int>(extent3of(var1)), static_cast<int>(extent4of(var1)), static_cast<int>(rankof(var1)), static_cast<int>(extentof(var1)), static_cast<int>(sizeofelemof(var1)));
		std::printf("var2[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var2)), static_cast<int>(extent2of(var2)), static_cast<int>(extent3of(var2)), static_cast<int>(extent4of(var2)), static_cast<int>(rankof(var2)), static_cast<int>(extentof(var2)), static_cast<int>(sizeofelemof(var2)));
		std::printf("var3[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var3)), static_cast<int>(extent2of(var3)), static_cast<int>(extent3of(var3)), static_cast<int>(extent4of(var3)), static_cast<int>(rankof(var3)), static_cast<int>(extentof(var3)), static_cast<int>(sizeofelemof(var3)));
		std::printf("var4[%d][%d][%d][%d](Rank=%d, Total=%d, ElemtSize=%d)\n", static_cast<int>(extent1of(var4)), static_cast<int>(extent2of(var4)), static_cast<int>(extent3of(var4)), static_cast<int>(extent4of(var4)), static_cast<int>(rankof(var4)), static_cast<int>(extentof(var4)), static_cast<int>(sizeofelemof(var4)));
	}
	{
	std::printf("\n");
	std::printf("----- Test for sizeofvalueof -----\n");
	struct data_t
	{
		char m_val[128];
	};
	data_t var1;
	data_t* var2;
	var1.m_val[0] = 0;
	var2 = &var1;
	std::printf("sizeof(var1)=%d, sizeof(var2)=%d\n", static_cast<int>(sizeof(var1)), static_cast<int>(sizeof(var2)));
	std::printf("sizeofvalueof(var1)=%d, sizeofvalueof(var2)=%d\n", static_cast<int>(sizeofvalueof(var1)), static_cast<int>(sizeofvalueof(var2)));
}

	//比較用：std::rank, static_cast<int>(std::extent
	{
		std::printf("\n");
		std::printf("----- Test for std::rank, static_cast<int>(std::extent -----\n");
		typedef int arr0_t;
		typedef int arr1_t[1];
		typedef int arr2_t[2][3];
		typedef int arr3_t[4][5][6];
		typedef int arr4_t[7][8][9][10];
		std::printf("arr0_t(Rank=%d)\n", static_cast<int>(std::rank<arr0_t>::value));
		std::printf("arr1_t[%d](Rank=%d)\n", static_cast<int>(std::extent<arr1_t, 0>::value), static_cast<int>(std::rank<arr1_t>::value));
		std::printf("arr2_t[%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr2_t, 0>::value), static_cast<int>(std::extent<arr2_t, 1>::value), static_cast<int>(std::rank<arr2_t>::value));
		std::printf("arr3_t[%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr3_t, 0>::value), static_cast<int>(std::extent<arr3_t, 1>::value), static_cast<int>(std::extent<arr3_t, 2>::value), static_cast<int>(std::rank<arr3_t>::value));
		std::printf("arr4_t[%d][%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr4_t, 0>::value), static_cast<int>(std::extent<arr4_t, 1>::value), static_cast<int>(std::extent<arr4_t, 2>::value), static_cast<int>(std::extent<arr4_t, 3>::value), static_cast<int>(std::rank<arr4_t>::value));
		std::printf("\n");
		std::printf("arr0_t[%d][%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr0_t, 0>::value), static_cast<int>(std::extent<arr0_t, 1>::value), static_cast<int>(std::extent<arr0_t, 2>::value), static_cast<int>(std::extent<arr0_t, 3>::value), static_cast<int>(std::rank<arr0_t>::value));
		std::printf("arr1_t[%d][%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr1_t, 0>::value), static_cast<int>(std::extent<arr1_t, 1>::value), static_cast<int>(std::extent<arr1_t, 2>::value), static_cast<int>(std::extent<arr1_t, 3>::value), static_cast<int>(std::rank<arr1_t>::value));
		std::printf("arr2_t[%d][%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr2_t, 0>::value), static_cast<int>(std::extent<arr2_t, 1>::value), static_cast<int>(std::extent<arr2_t, 2>::value), static_cast<int>(std::extent<arr2_t, 3>::value), static_cast<int>(std::rank<arr2_t>::value));
		std::printf("arr3_t[%d][%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr3_t, 0>::value), static_cast<int>(std::extent<arr3_t, 1>::value), static_cast<int>(std::extent<arr3_t, 2>::value), static_cast<int>(std::extent<arr3_t, 3>::value), static_cast<int>(std::rank<arr3_t>::value));
		std::printf("arr4_t[%d][%d][%d][%d](Rank=%d)\n", static_cast<int>(std::extent<arr4_t, 0>::value), static_cast<int>(std::extent<arr4_t, 1>::value), static_cast<int>(std::extent<arr4_t, 2>::value), static_cast<int>(std::extent<arr4_t, 3>::value), static_cast<int>(std::rank<arr4_t>::value));
	}

	//文字列化
	{
		std::printf("\n");
		std::printf("----- Test for toStr -----\n");
		bool val_bool = true;
		char val_char = 65;
		unsigned char val_uchar = 65;
		short val_short = (65 << 8) | 66;
		unsigned short val_ushort = (65 << 8) | 66;
		int val_int = (65 << 24) | (66 << 16) | (67 << 8) | 68;
		unsigned int val_uint = (65 << 24) | (66 << 16) | (67 << 8) | 68;
		long val_long = (65 << 24) | (66 << 16) | (67 << 8) | 68;
		unsigned long val_ulong = (65 << 24) | (66 << 16) | (67 << 8) | 68;
		long long val_llong = (65ll << 56) | (66ll << 48) | (67ll << 40) | (68ll << 32) | (69ll << 24) | (70ll << 16) | (71ll << 8) | 72ll;
		unsigned long long val_ullong = (65ll << 56) | (66ll << 48) | (67ll << 40) | (68ll << 32) | (69ll << 24) | (70ll << 16) | (71ll << 8) | 72ll;
		float val_float = 123.456f;
		double val_double = 123.456;
		char buff1[32], buff2[32], buff3[32], buff4[32];
		std::printf("bool: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_bool), toNumStr(buff2, val_bool), toHexStr(buff3, val_bool), toByteStr(buff4, val_bool));
		std::printf("char: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_char), toNumStr(buff2, val_char), toHexStr(buff3, val_char), toByteStr(buff4, val_char));
		std::printf("unsigned char: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_uchar), toNumStr(buff2, val_uchar), toHexStr(buff3, val_uchar), toByteStr(buff4, val_uchar));
		std::printf("short: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_short), toNumStr(buff2, val_short), toHexStr(buff3, val_short), toByteStr(buff4, val_short));
		std::printf("unsigned short: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_ushort), toNumStr(buff2, val_ushort), toHexStr(buff3, val_ushort), toByteStr(buff4, val_ushort));
		std::printf("int: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_int), toNumStr(buff2, val_int), toHexStr(buff3, val_int), toByteStr(buff4, val_int));
		std::printf("unsigned int: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_uint), toNumStr(buff2, val_uint), toHexStr(buff3, val_uint), toByteStr(buff4, val_uint));
		std::printf("long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_long), toNumStr(buff2, val_long), toHexStr(buff3, val_long), toByteStr(buff4, val_long));
		std::printf("unsigned long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_ulong), toNumStr(buff2, val_ulong), toHexStr(buff3, val_ulong), toByteStr(buff4, val_ulong));
		std::printf("long long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_llong), toNumStr(buff2, val_llong), toHexStr(buff3, val_llong), toByteStr(buff4, val_llong));
		std::printf("unsigned long long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_ullong), toNumStr(buff2, val_ullong), toHexStr(buff3, val_ullong), toByteStr(buff4, val_ullong));
		std::printf("float: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_float), toNumStr(buff2, val_float), toHexStr(buff3, val_float), toByteStr(buff4, val_float));
		std::printf("double: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_double), toNumStr(buff2, val_double), toHexStr(buff3, val_double), toByteStr(buff4, val_double));
	}
	
	std::printf("\n");
	std::printf("- end -\n");
}

// End of file
