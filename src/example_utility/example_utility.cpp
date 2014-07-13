//--------------------------------------------------------------------------------
// exmaple_utility.cpp
// 汎用ユーティリティテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_utility.h"//汎用ユーティリティテスト

#include <gasha/utility.h>//汎用ユーティリティ

#include <cstdio>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//汎用ユーティリティテスト
void example_utility()
{
	{
		//min() / max()関数テスト
		printf("----- Test for min() / max() -----\n");
		#define MIN_MAX(T, FMT, ...) \
			{ const T var = GASHA_ min(__VA_ARGS__); printf("min(" #__VA_ARGS__ ") = " FMT "\n", var); } \
			{ const T var = GASHA_ max(__VA_ARGS__); printf("max(" #__VA_ARGS__ ") = " FMT "\n", var); }

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

		struct data_t
		{
			int val;
			bool operator<(const data_t& rhs) const { return val < rhs.val; }
			data_t(const int v) :
				val(v)
			{}
			data_t() :
				val(0)
			{}
		};
		{
			data_t a(1);
			data_t b(2);
			data_t c(3);
			data_t max_val = max(a, b, c);
			data_t min_val = min(a, b, c);
			data_t& max_obj = maxObj(a, b, c);
			data_t& min_obj = minObj(a, b, c);
			a.val += 10;
			c.val += 10;
			printf("max_val.val=%d\n", max_val.val);
			printf("min_val.val=%d\n", min_val.val);
			printf("max_obj.val=%d\n", max_obj.val);
			printf("min_obj.val=%d\n", min_obj.val);
		}
		{
			const data_t a(1);
			const data_t b(2);
			const data_t c(3);
			const data_t max_val = max(a, b, c);
			const data_t min_val = min(a, b, c);
			const data_t& max_obj = maxObj(a, b, c);
			const data_t& min_obj = minObj(a, b, c);
			//a.val += 10;
			//c.val += 10;
			printf("max_val.val=%d\n", max_val.val);
			printf("min_val.val=%d\n", min_val.val);
			printf("max_obj.val=%d\n", max_obj.val);
			printf("min_obj.val=%d\n", min_obj.val);
		}
		{
			data_t max_val = max(data_t(1), data_t(2), data_t(3));
			data_t min_val = min(data_t(1), data_t(2), data_t(3));
			data_t& max_obj = maxObj(data_t(1), data_t(2), data_t(3));
			data_t& min_obj = minObj(data_t(1), data_t(2), data_t(3));
			printf("max_val.val=%d\n", max_val.val);
			printf("min_val.val=%d\n", min_val.val);
			printf("max_obj.val=%d\n", max_obj.val);
			printf("min_obj.val=%d\n", min_obj.val);
		}
		{
			int a = 1;
			int b = 2;
			int c = 3;
			int max_val = max(a, b, c);
			int min_val = min(a, b, c);
			printf("max_val=%d\n", max_val);
			printf("min_val=%d\n", min_val);
		}
		{
			const int a = 1;
			const int b = 2;
			const int c = 3;
			const int max_val = max(a, b, c);
			const int min_val = min(a, b, c);
			printf("max_val=%d\n", max_val);
			printf("min_val=%d\n", min_val);
		}
		{
			const int max_val = max(1, 2, 3);
			const int min_val = min(1, 2, 3);
			printf("max_val=%d\n", max_val);
			printf("min_val=%d\n", min_val);
		}
		{
			const int max_val = max(1, 2.f, 3.);
			const int min_val = min(1, 2.f, 3.);
			printf("max_val=%d\n", max_val);
			printf("min_val=%d\n", min_val);
		}
	}
}

// End of file
