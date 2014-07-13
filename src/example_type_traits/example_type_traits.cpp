//--------------------------------------------------------------------------------
// exmaple_type_traits.cpp
// 型特性ユーティリティテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_type_traits.h"//型特性ユーティリティテスト

#include <gasha/type_traits.h>//型特性ユーティリティ：rankof(), sizeof(), sizeofelemof()

#include <type_traits>//std::rank, std::extent
#include <cstdio>//printf()

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

	//文字列化
	{
		printf("\n");
		printf("----- Test for toStr -----\n");
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
		printf("bool: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_bool), toNumStr(buff2, val_bool), toHexStr(buff3, val_bool), toByteStr(buff4, val_bool));
		printf("char: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_char), toNumStr(buff2, val_char), toHexStr(buff3, val_char), toByteStr(buff4, val_char));
		printf("unsigned char: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_uchar), toNumStr(buff2, val_uchar), toHexStr(buff3, val_uchar), toByteStr(buff4, val_uchar));
		printf("short: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_short), toNumStr(buff2, val_short), toHexStr(buff3, val_short), toByteStr(buff4, val_short));
		printf("unsigned short: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_ushort), toNumStr(buff2, val_ushort), toHexStr(buff3, val_ushort), toByteStr(buff4, val_ushort));
		printf("int: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_int), toNumStr(buff2, val_int), toHexStr(buff3, val_int), toByteStr(buff4, val_int));
		printf("unsigned int: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_uint), toNumStr(buff2, val_uint), toHexStr(buff3, val_uint), toByteStr(buff4, val_uint));
		printf("long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_long), toNumStr(buff2, val_long), toHexStr(buff3, val_long), toByteStr(buff4, val_long));
		printf("unsigned long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_ulong), toNumStr(buff2, val_ulong), toHexStr(buff3, val_ulong), toByteStr(buff4, val_ulong));
		printf("long long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_llong), toNumStr(buff2, val_llong), toHexStr(buff3, val_llong), toByteStr(buff4, val_llong));
		printf("unsigned long long: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_ullong), toNumStr(buff2, val_ullong), toHexStr(buff3, val_ullong), toByteStr(buff4, val_ullong));
		printf("float: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_float), toNumStr(buff2, val_float), toHexStr(buff3, val_float), toByteStr(buff4, val_float));
		printf("double: toStr()=\"%s\", toNumStr()=\"%s\", toHexStr()=\"%s\", toByteStr()=\"%s\"\n", toStr(buff1, val_double), toNumStr(buff2, val_double), toHexStr(buff3, val_double), toByteStr(buff4, val_double));
	}
	
	printf("\n");
	printf("- end -\n");
}

// End of file
