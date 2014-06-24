//--------------------------------------------------------------------------------
// exmaple_utility.cpp
// 汎用ユーティリティテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_utility.h"//汎用ユーティリティ

#include <gasha/utility.h>//汎用ユーティリティ

#include <stdio.h>//printf()

USING_NAMESPACE_GASHA;//ネームスペース使用

//----------------------------------------
//汎用ユーティリティテスト
void example_utility()
{
	{
		//min() / max()関数テスト
		printf("----- Test for min() / max() -----\n");
		#define MIN_MAX(T, FMT, ...) \
			{ const T var = min(__VA_ARGS__); printf("min(" #__VA_ARGS__ ") = " FMT "\n", var); } \
			{ const T var = max(__VA_ARGS__); printf("max(" #__VA_ARGS__ ") = " FMT "\n", var); }

		MIN_MAX(int, "%d", 1, 2);
		MIN_MAX(int, "%d", 2, 1);
		MIN_MAX(int, "%d", 1, 2, 3);
		MIN_MAX(int, "%d", 3, 2, 1);
		MIN_MAX(int, "%d", 2, 3, 1);
		MIN_MAX(int, "%d", 3, 1, 2);

		MIN_MAX(float, "%.1f", 1.1f, 2.2f);
		MIN_MAX(float, "%.1f", 2.2f, 1.1f);
		MIN_MAX(float, "%.1f", 1.1f, 2.2f, 3.3f);
		MIN_MAX(float, "%.1f", 3.3f, 2.2f, 1.1f);
		MIN_MAX(float, "%.1f", 2.2f, 3.3f, 1.1f);
		MIN_MAX(float, "%.1f", 3.3f, 1.1f, 2.2f);

		MIN_MAX(int, "%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
		MIN_MAX(int, "%d", 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
		MIN_MAX(int, "%d", 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1);
	}
}

// End of file
