//--------------------------------------------------------------------------------
// exmaple_sort_and_search.cpp
// ソート＆探索処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_sort_and_search.h"//ソート＆探索処理テスト

//【ソート処理】
#include <gasha/is_ordered.h>//整列状態確認
//分類：交換ソート
#include <gasha/bubble_sort.h>//バブルソート
#include <gasha/shaker_sort.h>//シェーカーソート
#include <gasha/odd_even_sort.h>//奇遇転置ソート
#include <gasha/shear_sort.h>//シェアソート
#include <gasha/comb_sort.h>//コムソート
#include <gasha/gnome_sort.h>//ノームソート
//分類：分割交換ソート
#include <gasha/quick_sort.h>//クイックソート
//選択ソート
#include <gasha/selection_sort.h>//選択ソート
#include <gasha/heap_sort.h>//ヒープソート
//分類：挿入ソート
#include <gasha/insertion_sort.h>//挿入ソート
#include <gasha/shell_sort.h>//シェルソート
//分類：マージソート
#include <gasha/inplace_merge_sort.h>//インプレースマージソート
//分類：混成ソート
#include <gasha/intro_sort.h>//イントロソート
//分類：分布ソート
#include <gasha/radix_sort.h>//基数ソート

//【探索処理】
#include <gasha/linear_search.h>//線形探索
#include <gasha/binary_search.h>//二分探索

#include <gasha/utility.h>//汎用ユーティリティ：nowTime(), calcElapsedTime()

#include <random>//C++11 std::random
#include <cstdint>//C++11 std::intptr_t

//【VC++】例外を無効化した状態で <algorithm> <functional> <bitset> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::sort(), std::for_each()
#include <functional>//C++11 std::function
#include <bitset>//std::bitset

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テスト用の比較処理

//※qsort用関数
inline int predicate_func_qsort(const void*lhs, const void*rhs)
{
	return reinterpret_cast<const data_t*>(lhs)->m_key - reinterpret_cast<const data_t*>(rhs)->m_key;
}

//※インライン関数
inline bool predicate_func_inline(const data_t& lhs, const data_t& rhs)
{
	return lhs.m_key < rhs.m_key;
}

//※関数オブジェクト
struct predicate_functor{
	inline bool operator()(const data_t& lhs, const data_t& rhs)
	{
		return lhs.m_key < rhs.m_key;
	}
};

//※ラムダ関数
auto predicate_lambda = [](const data_t& lhs, const data_t& rhs) -> bool
{
	return lhs.m_key < rhs.m_key;
};

//----------------------------------------
//less,greater,equalオペレータ（ソート用）

//※sort関数に比較関数の指定を省略した場合（つまりstd::less<T>()が指定された場合と同等）、このオペレータが使用される
inline bool operator<(const data_t& lhs, const data_t& rhs){ return lhs.m_key < rhs.m_key; }

//※sort関数にstd::greater<T>()を指定した場合、このオペレータが使用される
inline bool operator>(const data_t& lhs, const data_t& rhs){ return lhs.m_key > rhs.m_key; }

//----------------------------------------
//less,equalオペレータ（探索用）

//※linearSearch関数／binarySearch関数に比較関数の指定を省略した場合、このオペレータが使用される
inline bool operator<(const data_t& lhs, const int rhs){ return lhs.m_key < rhs; }

//※binarySearch関数に比較関数の指定を省略した場合、このオペレータが使用される
inline bool operator==(const data_t& lhs, const int rhs){ return lhs.m_key == rhs; }

//デフォルト
//#define predicate_default predicate_func
//#define predicate_default predicate_func_inline
//#define predicate_default predicate_functor()
#define predicate_default predicate_lambda

//----------------------------------------
//ソート＆探索処理テスト
void example_sort_and_search()
{
	//時間計測
	auto begin_time = nowTime();
	auto prev_time = begin_time;

	//処理時間計測
	auto getElapsedTime = [](const std::chrono::system_clock::time_point& prev_time) -> double
	{
		return calcElapsedTime(prev_time);
	};

	//処理時間表示
	auto printElapsedTimeDirect = [](const double elapsed_time, const bool is_preint) -> std::chrono::system_clock::time_point
	{
		if (is_preint)
			printf("*elapsed time=%.9lf sec.\n", elapsed_time);
		return nowTime();
	};
	auto printElapsedTime = [&printElapsedTimeDirect](const std::chrono::system_clock::time_point& prev_time, const bool is_print) -> std::chrono::system_clock::time_point
	{
		const auto elapsed_time = calcElapsedTime(prev_time);
		return printElapsedTimeDirect(elapsed_time, is_print);
	};

	//配列状態表示
	auto showArrayCondition = [&prev_time, &printElapsedTime](const array_t* array)
	{
		const int ng = static_cast<int>(sumupUnordered(*array, predicate_default));
		if (ng == 0)
			printf("Array is ordered. [record(s)=%d]\n", array->size());
		else
			printf("[NG] Array is NOT ordered! [NG=%d / record(s)=%d]\n", ng, array->size());
		{
			std::bitset<TEST_DATA_COUNT> seq_no_map;
			const std::size_t size = array->size();
			const data_t* prev = &array->at(0);
			const data_t* now = prev + 1;
			bool array_is_not_stable = false;
			bool array_is_broken = false;
			seq_no_map[prev->m_seqNo] = true;
			for (std::size_t i = 1; i < size; ++i, ++now, ++prev)
			{
				seq_no_map[now->m_seqNo] = true;
				if (prev->m_key == now->m_key && prev->m_seqNo > now->m_seqNo)
					array_is_not_stable = true;
			}
			if (seq_no_map.count() != TEST_DATA_COUNT)
				array_is_broken = true;
			if (array_is_broken)
				printf("[NG] Array is BROKEN !! Sorting-program is mistaken !\n");
			if (array_is_not_stable)
				printf("Array is NOT STABLE.\n");
			else
				printf("Array is stable.\n");
		}
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};

	//配列初期化
	enum init_type_t
	{
		init_shuffle1 = 0,//シャッフルパターン1
		init_shuffle2,//シャッフルパターン2
		init_shuffle3,//シャッフルパターン3
		init_ordered,//整列済みパターン
		init_reversed,//逆順整列済みパターン
		init_ordered_without_both_ends,//整列済みパターン（ただし、始端と終端のみ入れ替え）
		init_hard_with_quick_sort,//クイックソートに厳しいパターン
		init_pattern_num//初期化パターン数
	};
	auto makeArray = [&prev_time, &printElapsedTime, &showArrayCondition](array_t*& array, const init_type_t type, const char* type_name)
	{
		printf("----- Make Array(%s) -----\n", type_name);
		array = new array_t();
		int key = 0;
		int same_key_num = 0;
		auto nextKey = [&key, &same_key_num]() -> int
		{
			const int now_key = key;
			++same_key_num;
			if (same_key_num % TEST_DATA_SAME_KEY_NUM == 0)
				++key;
			return now_key;
		};
		switch (type)
		{
		case init_shuffle1:
		case init_shuffle2:
		case init_shuffle3:
		case init_ordered:
		case init_ordered_without_both_ends:
			{
				for (data_t& obj : *array)
					obj.m_key = nextKey();
			}
			break;
		case init_reversed:
			{
				for_each(array->rbegin(), array->rend(), [&nextKey](data_t& obj)
					{
						obj.m_key = nextKey();
					}
				);
			}
			break;
		case init_hard_with_quick_sort:
			{
				std::mt19937 random_engine(type);
				const std::size_t size = array->size();
				const std::size_t top = 0;
				const std::size_t mid = size >> 1;
				const std::size_t btm = size - 1;
				(*array)[top].m_key = nextKey();
				if(size >= 3)
					(*array)[mid].m_key = nextKey();
				if (size >= 2)
					(*array)[btm].m_key = nextKey();
				if (size > 4)
				{
					data_t* obj_begin = &(*array)[mid + 1];
					data_t* obj_end = &(*array)[size - 1];
					for (data_t* obj = obj_begin; obj <= obj_end; ++obj)
						obj->m_key = nextKey();
					std::shuffle(obj_begin, obj_end, random_engine);
				}
				if (size > 3)
				{
					data_t* obj_begin = &(*array)[top + 1];
					data_t* obj_end = &(*array)[mid];
					for (data_t* obj = obj_begin; obj < obj_end; ++obj)
						obj->m_key = nextKey();
					std::shuffle(obj_begin, obj_end, random_engine);
				}
			}
			break;
		}
		switch (type)
		{
		case init_shuffle1:
		case init_shuffle2:
		case init_shuffle3:
			{
				std::mt19937 random_engine(type);
				std::shuffle(array->begin(), array->end(), random_engine);
			}
			break;
		case init_ordered_without_both_ends:
			{
				GASHA_ swapValues((*array)[0], (*array)[array->size() - 1]);
			}
			break;
		}
		int seq_no = 0;
		for (data_t& obj : *array)
			obj.m_seqNo = seq_no++;
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
		showArrayCondition(array);
	#if 0//配列情報表示（デバッグ確認用）
		const data_t* element0_p = &(*array)[0];
		const data_t* element1_p = &(*array)[1];
		const std::size_t data_size = sizeof(data_t);
		const std::size_t element_size = reinterpret_cast<std::intptr_t>(element1_p)-reinterpret_cast<std::intptr_t>(element0_p);
		const std::size_t array_size = sizeof(*array);
		const std::size_t element_count = array->size();
		printf("array_size=%d, element_count=%d, element_size=%d, data_size=%d, element0_p=%p, element1_p=%p\n", array_size, element_count, element_size, data_size, element0_p, element1_p);
	#endif
	};
	array_t* array_shuffle1 = nullptr;
	array_t* array_shuffle2 = nullptr;
	array_t* array_shuffle3 = nullptr;
	array_t* array_ordered = nullptr;
	array_t* array_reversed = nullptr;
	array_t* array_ordered_without_both_ends = nullptr;
	array_t* array_hard_with_quick_sort = nullptr;
	auto makeArraySet = [&makeArray, &array_shuffle1, &array_shuffle2, &array_shuffle3, &array_ordered, &array_reversed, &array_ordered_without_both_ends, &array_hard_with_quick_sort]()
	{
		#define PARAM(x) array_##x, init_##x, #x
		makeArray(PARAM(shuffle1));
		makeArray(PARAM(shuffle2));
		makeArray(PARAM(shuffle3));
		makeArray(PARAM(ordered));
		makeArray(PARAM(reversed));
		makeArray(PARAM(ordered_without_both_ends));
		makeArray(PARAM(hard_with_quick_sort));
		#undef PARAM
	};
	auto copyWorkArray = [&prev_time, &printElapsedTime, &array_shuffle1, &array_shuffle2, &array_shuffle3, &array_ordered, &array_reversed, &array_ordered_without_both_ends, &array_hard_with_quick_sort](array_t*& array, const init_type_t type, const char* type_name)
	{
	#ifdef TEST_DATA_WATCH_CONSTRUCTOR
		printf("----- Copy Work Array(%s) -----\n", type_name);
	#endif//TEST_DATA_WATCH_CONSTRUCTOR
		array = new array_t();
		const array_t* array_src = nullptr;
		switch (type)
		{
		case init_shuffle1:                   array_src = array_shuffle1; break;
		case init_shuffle2:                   array_src = array_shuffle2; break;
		case init_shuffle3:                   array_src = array_shuffle3; break;
		case init_ordered:                    array_src = array_ordered; break;
		case init_reversed:                   array_src = array_reversed; break;
		case init_ordered_without_both_ends:  array_src = array_ordered_without_both_ends; break;
		case init_hard_with_quick_sort:       array_src = array_hard_with_quick_sort; break;
		}
		memcpy(array, array_src,sizeof(array_t));
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};

	//配列削除
	auto deleteArray = [&prev_time, &printElapsedTime](array_t*& array, const char* type_name)
	{
		printf("----- Delete array(%s) -----\n", type_name);
		delete array;
		array = nullptr;
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	auto deleteArraySet = [&deleteArray, &array_shuffle1, &array_shuffle2, &array_shuffle3, &array_ordered, &array_reversed, &array_ordered_without_both_ends, &array_hard_with_quick_sort]()
	{
		#define PARAM(x) array_##x, #x
		deleteArray(PARAM(shuffle1));
		deleteArray(PARAM(shuffle2));
		deleteArray(PARAM(shuffle3));
		deleteArray(PARAM(ordered));
		deleteArray(PARAM(reversed));
		deleteArray(PARAM(ordered_without_both_ends));
		deleteArray(PARAM(hard_with_quick_sort));
		#undef PARAM
	};
	auto deleteWorkArray = [&prev_time, &printElapsedTime](array_t*& array, const char* type_name)
	{
	#ifdef TEST_DATA_WATCH_CONSTRUCTOR
		printf("----- Delete Work Array(%s) -----\n", type_name);
	#endif//TEST_DATA_WATCH_CONSTRUCTOR
		delete array;
		array = nullptr;
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};

	//ソート
	typedef std::function<std::size_t(array_t*)> sort_procedure;
	auto sort = [&prev_time, &getElapsedTime, &printElapsedTimeDirect, &showArrayCondition](array_t*& array, sort_procedure sort_proc, double& elapsed_time, std::size_t& swapped_count)
	{
	#ifdef TEST_DATA_WATCH_CONSTRUCTOR
		printf("----- Sort -----\n");
	#endif//TEST_DATA_WATCH_CONSTRUCTOR
		prev_time = nowTime();
		swapped_count = sort_proc(array);
		elapsed_time = getElapsedTime(prev_time);
		const bool is_print = true;
		prev_time = printElapsedTimeDirect(elapsed_time, is_print);
		showArrayCondition(array);
		if (swapped_count != 0xffffffff)
			printf("[swapped=%d count(s)]\n", swapped_count);
	};
	
	//計測
	auto measure = [&copyWorkArray, &sort, &deleteWorkArray](const init_type_t type, const char* type_name, sort_procedure sort_proc, double& elapsed_time, std::size_t& swapped_count)
	{
		printf("\n");
		printf("------------------------------\n");
		printf("Sort array(%s)\n", type_name);
		printf("------------------------------\n");
		array_t* array = nullptr;//配列
		copyWorkArray(array, type, type_name);//配列初期化
		sort(array, sort_proc, elapsed_time, swapped_count);//ソート
		deleteWorkArray(array, type_name);//配列削除
	};

	//ソート処理時間集計用構造体
	struct sum_t
	{
		int count;
		double elapsed_time_sum;
		double elapsed_time_avg;
		double elapsed_time_min;
		double elapsed_time_max;
		std::size_t swapped_count_sum;
		std::size_t swapped_count_avg;
		std::size_t swapped_count_min;
		std::size_t swapped_count_max;
		void add(const double elapsed_time, const std::size_t swapped_count)
		{
			++count;
			if (elapsed_time_sum == -1.)
				elapsed_time_sum = 0.;
			elapsed_time_sum += elapsed_time;
			elapsed_time_avg = elapsed_time_sum / static_cast<double>(count);
			if (elapsed_time_min == -1. || elapsed_time_min > elapsed_time)
				elapsed_time_min = elapsed_time;
			if (elapsed_time_max == -1. || elapsed_time_max < elapsed_time)
				elapsed_time_max = elapsed_time;
			if (swapped_count != 0xffffffff)
			{
				if (swapped_count_sum == 0xffffffff)
					swapped_count_sum = 0;
				swapped_count_sum += swapped_count;
				swapped_count_avg = swapped_count_sum / static_cast<std::size_t>(count);
				if (swapped_count_min == 0xffffffff || swapped_count_min > swapped_count)
					swapped_count_min = swapped_count;
				if (swapped_count_max == 0xffffffff || swapped_count_max < swapped_count)
					swapped_count_max = swapped_count;
			}
		}
		sum_t() :
			count(0),
			elapsed_time_sum(-1.),
			elapsed_time_avg(-1.),
			elapsed_time_min(-1.),
			elapsed_time_max(-1.),
			swapped_count_sum(0xffffffff),
			swapped_count_avg(0xffffffff),
			swapped_count_min(0xffffffff),
			swapped_count_max(0xffffffff)
		{}
	};
	
	//まとめて計測
	auto measureAll = [&measure](const char* sort_type_name, sort_procedure sort_proc) -> sum_t
	{
		printf("============================================================\n");
		printf("Sort type: [%s]\n", sort_type_name);
		printf("============================================================\n");

		#define PARAM(x) init_##x, #x

		//ソート処理時間集計
		sum_t sum;
		double elapsed_time;
		std::size_t swapped_count;

		//シャッフルパターン１
		measure(PARAM(shuffle1), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		//シャッフルパターン２
		measure(PARAM(shuffle2), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		//シャッフルパターン３
		measure(PARAM(shuffle3), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		//整列済みパターン
		measure(PARAM(ordered), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		//逆順整列済みパターン
		measure(PARAM(reversed), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		//整列済みパターン（ただし、始端と終端のみ入れ替え）
		measure(PARAM(ordered_without_both_ends), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		//クイックソートに厳しいパターン
		measure(PARAM(hard_with_quick_sort), sort_proc, elapsed_time, swapped_count);
		sum.add(elapsed_time, swapped_count);

		#undef PARAM

		printf("\n");
		printf("============================================================\n");
		printf("Sort type: [%s]\n", sort_type_name);
		printf("* Total time       = %.9lf sec.\n", sum.elapsed_time_sum);
		printf("* Average time     = %.9lf sec.\n", sum.elapsed_time_avg);
		printf("* Min time         = %.9lf sec.\n", sum.elapsed_time_min);
		printf("* Max time         = %.9lf sec.\n", sum.elapsed_time_max);
		if (sum.swapped_count_sum != 0xffffffff)
		{
			printf("* Total Swapped    = %9u count(s)\n", sum.swapped_count_sum);
			printf("* Average  Swapped = %9u count(s)\n", sum.swapped_count_avg);
			printf("* Min Swapped      = %9u count(s)\n", sum.swapped_count_min);
			printf("* Max Swapped      = %9u count(s)\n", sum.swapped_count_max);
		}
		printf("============================================================\n");

		return sum;
	};
	
	//----------------------------------------
	//測定開始
	printf("============================================================\n");
	printf("Initialize\n");
	printf("============================================================\n");
	makeArraySet();
	printf("\n");
	printf("\n");

	//----------------------------------------
	//標準ライブラリによるソート

#if 1
	//--------------------
	//Cライブラリ：qsort関数
	//アルゴリズム：クイックソート
	auto clib_qsort = [](array_t* array) -> std::size_t
	{
		qsort(&array->at(0), array->size(),sizeof(data_t), predicate_func_qsort);
		return 0xffffffff;
	};
	const sum_t sum_clib_qsort = measureAll("C-Library qsort", clib_qsort);
	printf("\n");
	printf("\n");

	//--------------------
	//STLソート1：通常関数で比較
	//アルゴリズム：イントロソート（クイックソート＋ヒープソート＋挿入ソート）
	auto stl_sort1 = [](array_t* array) -> std::size_t
	{
		std::sort(array->begin(), array->end(), predicate_func);
		return 0xffffffff;
	};
	const sum_t sum_stl1 = measureAll("STL std::sort(with function)", stl_sort1);
	printf("\n");
	printf("\n");

	//STLソート2：インライン関数で比較
	//アルゴリズム：（同上）
	auto stl_sort2 = [](array_t* array) -> std::size_t
	{
		std::sort(array->begin(), array->end(), predicate_func_inline);
		return 0xffffffff;
	};
	const sum_t sum_stl2 = measureAll("STL std::sort(with inline function)", stl_sort2);
	printf("\n");
	printf("\n");

	//STLソート3：関数オブジェクトで比較
	//アルゴリズム：（同上）
	auto stl_sort3 = [](array_t* array) -> std::size_t
	{
		std::sort(array->begin(), array->end(), predicate_functor());
		return 0xffffffff;
	};
	const sum_t sum_stl3 = measureAll("STL std::sort(with functor)", stl_sort3);
	printf("\n");
	printf("\n");

	//STLソート4：ラムダ関数で比較
	//アルゴリズム：（同上）
	auto stl_sort4 = [](array_t* array) -> std::size_t
	{
		std::sort(array->begin(), array->end(), predicate_lambda);
		return 0xffffffff;
	};
	const sum_t sum_stl4 = measureAll("STL std::sort(with lamda)", stl_sort4);
	printf("\n");
	printf("\n");

	//--------------------
	//STL安定ソート
	//アルゴリズム：マージソート ※外部メモリ使用あり
	auto stl_stable_sort = [](array_t* array) -> std::size_t
	{
		std::stable_sort(array->begin(), array->end(), predicate_func);
		return 0xffffffff;
	};
	const sum_t sum_stl_stable = measureAll("STL std::stable_sort *Not inplace", stl_stable_sort);
	printf("\n");
	printf("\n");

	//----------------------------------------
	//交換ソート

	//アルゴリズム：バブルソート
	auto bubble_sort = [](array_t* array) -> std::size_t
	{
		return bubbleSort(*array, predicate_default);
	};
	const sum_t sum_bubble = measureAll("Bubble sort", bubble_sort);
	printf("\n");
	printf("\n");
	
	//アルゴリズム：シェーカーソート
	auto shaker_sort = [](array_t* array) -> std::size_t
	{
		return shakerSort(*array, predicate_default);
	};
	const sum_t sum_shaker = measureAll("Shaker sort", shaker_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：奇遇転置ソート
	auto odd_even_sort = [](array_t* array) -> std::size_t
	{
		return oddEvenSort(*array, predicate_default);
	};
	const sum_t sum_odd_even = measureAll("Odd-Even sort", odd_even_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：シェアソート
	auto shear_sort = [](array_t* array) -> std::size_t
	{
		return shearSort(*array, predicate_default);
	};
	const sum_t sum_shear = measureAll("Shear sort", shear_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：コムソート
	auto comb_sort = [](array_t* array) -> std::size_t
	{
		return combSort(*array, predicate_default);
	};
	const sum_t sum_comb = measureAll("Comb sort", comb_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：ノームソート
	auto gnome_sort = [](array_t* array) -> std::size_t
	{
		return gnomeSort(*array, predicate_default);
	};
	const sum_t sum_gnome = measureAll("Gnome sort", gnome_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：クイックソート
	auto quick_sort = [](array_t* array) -> std::size_t
	{
		return quickSort(*array, predicate_default);
	};
	const sum_t sum_quick = measureAll("Quick sort", quick_sort);
	printf("\n");
	printf("\n");

	//----------------------------------------
	//選択ソート

	//アルゴリズム：選択ソート
	auto selection_sort = [](array_t* array) -> std::size_t
	{
		return selectionSort(*array, predicate_default);
	};
	const sum_t sum_selection = measureAll("Selection sort", selection_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：ヒープソート
	auto heap_sort = [](array_t* array) -> std::size_t
	{
		return heapSort(*array, predicate_default);
	};
	const sum_t sum_heap = measureAll("Heap sort", heap_sort);
	printf("\n");
	printf("\n");

	//----------------------------------------
	//挿入ソート

	//アルゴリズム：挿入ソート
	auto insertion_sort = [](array_t* array) -> std::size_t
	{
		return insertionSort(*array, predicate_default);
	};
	const sum_t sum_insertion = measureAll("Insertion sort", insertion_sort);
	printf("\n");
	printf("\n");

	//アルゴリズム：シェルソート
	auto shell_sort = [](array_t* array) -> std::size_t
	{
		return shellSort(*array, predicate_default);
	};
	const sum_t sum_shell = measureAll("Shell sort", shell_sort);
	printf("\n");
	printf("\n");

	//----------------------------------------
	//マージソート

	//アルゴリズム：インプレースマージソート
	auto inplace_merge_sort = [](array_t* array) -> std::size_t
	{
		return inplaceMergeSort(*array, predicate_default);
	};
	const sum_t sum_inplace_merge = measureAll("Inplace Merge sort", inplace_merge_sort);
	printf("\n");
	printf("\n");
#endif

	//----------------------------------------
	//混成ソート

	//アルゴリズム：イントロソート
	auto intro_sort = [](array_t* array) -> std::size_t
	{
		return introSort(*array, predicate_default);
	};
	const sum_t sum_intro = measureAll("Intro sort", intro_sort);
	printf("\n");
	printf("\n");

	//----------------------------------------
	//非比較ソート

	//アルゴリズム：基数ソート
	auto radix_sort = [](array_t* array) -> std::size_t
	{
		struct getKey{
			typedef int key_type;
			//typedef unsigned int key_type;
			//typedef unsigned char key_type;
			inline key_type operator()(const data_t& obj){ return obj.m_key; }
		};
		return radixSort(*array, getKey());
	};
	const sum_t sum_radix = measureAll("Radix sort", radix_sort);
	printf("\n");
	printf("\n");

	//----------------------------------------
	//測定終了

	//結果を表示
	auto printLine = [](const char* name, const sum_t& sum)
	{
		printf("- %-23s %13.9lf/%13.9lf/%13.9lf/%13.9lf",
			name,
			sum.elapsed_time_sum,
			sum.elapsed_time_avg,
			sum.elapsed_time_min,
			sum.elapsed_time_max);
		if (sum.swapped_count_sum != 0xffffffff)
		{
			printf("%11u/%11u/%11u/%11u",
				sum.swapped_count_sum,
				sum.swapped_count_avg,
				sum.swapped_count_min,
				sum.swapped_count_max);
		}
		printf("\n");
	};
	printf("============================================================\n");
	printf("Result(Summary)\n");
	printf("============================================================\n");
	printf("Array Element size = %d Bytes\n", sizeof(data_t));
	printf("Array Element(s)   = %d Count(s)\n", array_shuffle1->size());
	printf("Total Array size   = %d Bytes\n", sizeof(*array_shuffle1));
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("- Sort name:                Elapsed Time (Sum/Average/Min/Max) [sec.]             Swapped (Sum/Average/Min/Max) [count(s)]\n");
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
#if 1
	printf("[C-Library sort](Quick sort)\n");
	printLine("qsort(inline-function):", sum_clib_qsort);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[STL sort](Intro sort)\n");
	printLine("std::sort(function):", sum_stl1);
	printLine("std::sort(inline-func):", sum_stl2);
	printLine("std::sort(functor):", sum_stl3);
	printLine("std::sort(lambda):", sum_stl4);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[STL stable sort](Merge sort) *Not In-place sort\n");
	printLine("std::stable_sort<S>:", sum_stl_stable);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[Exchange sorts]\n");
	printLine("Bubble sort<S>:", sum_bubble);
	printLine("Shaker sort<S>:", sum_shaker);
	printLine("Odd-Even sort<S>:", sum_odd_even);
	printLine("Shear sort:", sum_shear);
	printLine("Comb sort:", sum_comb);
	printLine("Gnome sort<S>:", sum_gnome);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[Partition-exchange sorts]\n");
	printLine("Quick sort:", sum_quick);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[Selection sorts]\n");
	printLine("Selection sort:", sum_selection);
	printLine("Heap sort:", sum_heap);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[Insertion sorts]\n");
	printLine("Insertion sort<S>:", sum_insertion);
	printLine("Shell sort:", sum_shell);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[Merge sorts]\n");
	printLine("Inplace-Merge sort<S>:", sum_inplace_merge);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
#endif
	printf("[Hybrid sorts]\n");
	printLine("Intro sort:", sum_intro);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("[Distribution sorts] *Not In-place sort\n");
	printLine("Radix sort<S>:", sum_radix);
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("* <S> ... Stable sort algorithm.\n");
	printf("\n");

#if 1
	//----------------------------------------
	//バリエーションテスト

	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
	printf("Functoin variation test for sort and search.\n");
	printf("--------------------------------------------------------------------------------------------------------------------------------\n");
#if 1
	//----------------------------------------
	//ソートのバリエーション
	{
		printf("\n");
		printf("[Variation test for sort(intro-sort)]\n");
		//コンテナを直接渡してソートする方法
		array_t* arr = nullptr;//配列
		auto _begin = [&arr, &copyWorkArray](){ copyWorkArray(arr, init_shuffle1, "shuffle1"); };
		auto _end = [&arr, &deleteWorkArray](){ deleteWorkArray(arr, "shuffle1"); };
		_begin();  const std::size_t result = introSort(*arr, predicate_default); _end();
		//その他のソート指定方法
		_begin();  const std::size_t result1 = introSort(&arr->at(0), arr->size(), predicate_default); _end();
		//_begin(); const std::size_t result2 = introSort(arr->_Elems, predicate_default); _end();//VC++
		_begin(); const std::size_t result3 = introSort(&arr->at(0), &arr->at(arr->size() - 1) + 1, predicate_default); _end();
		_begin(); const std::size_t result4 = introSort(arr->begin(), arr->end(), predicate_default); _end();
		_begin(); const std::size_t result5 = introSort(*arr, predicate_default); _end();
		//比較関数を省略する方法 ※ operator<(const T&, const T&) が定義されている必要あり
		_begin(); const std::size_t result6 = introSort(&arr->at(0), arr->size()); _end();
		//_begin(); const std::size_t result7 = introSort(arr->_Elems); _end();//VC++
		_begin(); const std::size_t result8 = introSort(&arr->at(0), &arr->at(arr->size() - 1) + 1); _end();
		_begin(); const std::size_t result9 = introSort(arr->begin(), arr->end()); _end();
		_begin(); const std::size_t result10 = introSort(*arr); _end();
		//専用の比較関数を設けず、比較を逆転する方法 ※ operator>(const T&, const T&) が定義されている必要あり
		_begin(); const std::size_t result11 = introSort(*arr, std::greater<data_t>()); _end();
		printf("(result=%u)\n", result);
		printf("(result1=%u)\n", result1);
		//printf("(result2=%u)\n", result2);
		printf("(result3=%u)\n", result3);
		printf("(result4=%u)\n", result4);
		printf("(result5=%u)\n", result5);
		printf("(result6=%u)\n", result6);
		//printf("(result7=%u)\n", result7);
		printf("(result8=%u)\n", result8);
		printf("(result9=%u)\n", result9);
		printf("(result10=%u)\n", result10);
		printf("(result11=%u)\n", result11);
	}
#endif
	
#if 1
	//----------------------------------------
	//探索テスト
#if 1
	//線形探索
	{
		printf("\n");
		printf("[Variation test for linear-search]\n");
		int plus = 1;
		for (int target = -1; target < 300; target += plus, plus *= 2)
		{
			printf("target=%d\n", target);
			const data_t* data_def = linearSearchValue(*array_shuffle1, target);
			{
				array_t& arr = *array_shuffle1;
				const array_t& const_arr = *array_shuffle1;
				const data_t* data1_def = linearSearchValue(arr, target);
				const data_t* data2_def = linearSearchValue(const_arr, target);
				const data_t* data3_def = linearSearchValue(arr.begin(), arr.end(), target);
				const data_t* data4_def = linearSearchValue(arr.cbegin(), arr.cend(), target);
				const data_t* data5_def = linearSearchValue(const_arr.begin(), const_arr.end(), target);
				//const data_t* data6_def = linearSearchValue(arr._Elems, target);//VC++
				//const data_t* data7_def = linearSearchValue(const_arr._Elems, target);//VC++
				const data_t* data8_def = linearSearchValue(&arr.at(0), arr.size(), target);
				const data_t* data9_def = linearSearchValue(&const_arr.at(0), const_arr.size(), target);
				const data_t* data10_def = linearSearchValue(&arr.at(0), &arr.at(0) + arr.size(), target);
				const data_t* data11_def = linearSearchValue(&const_arr.at(0), &const_arr.at(0) + const_arr.size(), target);
				printf("  linearSearchValue(*array_shuffle1, target);\n");
				printf("    (data_def=%p)\n", data_def);
				printf("    (data1_def=%p)\n", data1_def);
				printf("    (data2_def=%p)\n", data2_def);
				printf("    (data3_def=%p)\n", data3_def);
				printf("    (data4_def=%p)\n", data4_def);
				printf("    (data5_def=%p)\n", data5_def);
				//printf("    (data6_def=%p)\n", data6_def);
				//printf("    (data7_def=%p)\n", data7_def);
				printf("    (data8_def=%p)\n", data8_def);
				printf("    (data9_def=%p)\n", data9_def);
				printf("    (data10_def=%p)\n", data10_def);
				printf("    (data11_def=%p)\n", data11_def);
			}
			auto search_predicate = [&target](const data_t& data) -> bool { return data.m_key == target; };
			const data_t* data = linearSearch(*array_shuffle1, search_predicate);
			{
				array_t& arr = *array_shuffle1;
				const array_t& const_arr = *array_shuffle1;
				const data_t* data1 = linearSearch(arr, search_predicate);
				const data_t* data2 = linearSearch(const_arr, search_predicate);
				const data_t* data3 = linearSearch(arr.begin(), arr.end(), search_predicate);
				const data_t* data4 = linearSearch(arr.cbegin(), arr.cend(), search_predicate);
				const data_t* data5 = linearSearch(const_arr.begin(), const_arr.end(), search_predicate);
				//const data_t* data6 = linearSearch(arr._Elems, search_predicate);//VC++
				//const data_t* data7 = linearSearch(const_arr._Elems, search_predicate);//VC++
				const data_t* data8 = linearSearch(&arr.at(0), arr.size(), search_predicate);
				const data_t* data9 = linearSearch(&const_arr.at(0), const_arr.size(), search_predicate);
				const data_t* data10 = linearSearch(&arr.at(0), &arr.at(0) + arr.size(), search_predicate);
				const data_t* data11 = linearSearch(&const_arr.at(0), &const_arr.at(0) + const_arr.size(), search_predicate);
				printf("  linearSearch(*array_shuffle1, search_predicate);\n");
				printf("    (data=%p)\n", data);
				printf("    (data1=%p)\n", data1);
				printf("    (data2=%p)\n", data2);
				printf("    (data3=%p)\n", data3);
				printf("    (data4=%p)\n", data4);
				printf("    (data5=%p)\n", data5);
				//printf("    (data6=%p)\n", data6);
				//printf("    (data7=%p)\n", data7);
				printf("    (data8=%p)\n", data8);
				printf("    (data9=%p)\n", data9);
				printf("    (data10=%p)\n", data10);
				printf("    (data11=%p)\n", data11);
			}
			auto search_predicate_val = [](const data_t& data, const int target) -> bool { return data.m_key == target; };
			const data_t* data_val = linearSearchValue(*array_shuffle1, target, search_predicate_val);
			{
				array_t& arr = *array_shuffle1;
				const array_t& const_arr = *array_shuffle1;
				const data_t* data1_val = linearSearchValue(arr, target, search_predicate_val);
				const data_t* data2_val = linearSearchValue(const_arr, target, search_predicate_val);
				const data_t* data3_val = linearSearchValue(arr.begin(), arr.end(), target, search_predicate_val);
				const data_t* data4_val = linearSearchValue(arr.cbegin(), arr.cend(), target, search_predicate_val);
				const data_t* data5_val = linearSearchValue(const_arr.begin(), const_arr.end(), target, search_predicate_val);
				//const data_t* data6_val = linearSearchValue(arr._Elems, target, search_predicate_val);//VC++
				//const data_t* data7_val = linearSearchValue(const_arr._Elems, target, search_predicate_val);//VC++
				const data_t* data8_val = linearSearchValue(&arr.at(0), arr.size(), target, search_predicate_val);
				const data_t* data9_val = linearSearchValue(&const_arr.at(0), const_arr.size(), target, search_predicate_val);
				const data_t* data10_val = linearSearchValue(&arr.at(0), &arr.at(0) + arr.size(), target, search_predicate_val);
				const data_t* data11_val = linearSearchValue(&const_arr.at(0), &const_arr.at(0) + const_arr.size(), target, search_predicate_val);
				printf("  linearSearchValue(*array_shuffle1, target, search_predicate_val);\n");
				printf("    (data_val=%p)\n", data_val);
				printf("    (data1_val=%p)\n", data1_val);
				printf("    (data2_val=%p)\n", data2_val);
				printf("    (data3_val=%p)\n", data3_val);
				printf("    (data4_val=%p)\n", data4_val);
				printf("    (data5_val=%p)\n", data5_val);
				//printf("    (data6_val=%p)\n", data6_val);
				//printf("    (data7_val=%p)\n", data7_val);
				printf("    (data8_val=%p)\n", data8_val);
				printf("    (data9_val=%p)\n", data9_val);
				printf("    (data10_val=%p)\n", data10_val);
				printf("    (data11_val=%p)\n", data11_val);
			}

			if (data_def)
				printf("target=%d, linearSearch=[%d, %d]\n", target, data_def->m_key, data_def->m_seqNo);
			else
				printf("target=%d, linearSearch=nullptr\n", target);
		}
	}
#endif

#if 1
	//二分探索
	{
		printf("\n");
		printf("[Variation test for binary-search]\n");
		auto sort_pred = [](const data_t& lhs, const data_t& rhs) -> bool { return lhs.m_key < rhs.m_key; };
		introSort(*array_shuffle1, sort_pred);
		int plus = 1;
		for (int target = -1; target < 300; target += plus, plus *= 2)
		{
			printf("target=%d\n", target);
			const data_t* data_def = binarySearchValue(*array_shuffle1, target);
			{
				array_t& arr = *array_shuffle1;
				const array_t& const_arr = *array_shuffle1;
				const data_t* data1_def = binarySearchValue(arr, target);
				const data_t* data2_def = binarySearchValue(const_arr, target);
				const data_t* data3_def = binarySearchValue(arr.begin(), arr.end(), target);
				const data_t* data4_def = binarySearchValue(arr.cbegin(), arr.cend(), target);
				const data_t* data5_def = binarySearchValue(const_arr.begin(), const_arr.end(), target);
				//const data_t* data6_def = binarySearchValue(arr._Elems, target);//VC++
				//const data_t* data7_def = binarySearchValue(const_arr._Elems, target);//VC++
				const data_t* data8_def = binarySearchValue(&arr.at(0), arr.size(), target);
				const data_t* data9_def = binarySearchValue(&const_arr.at(0), const_arr.size(), target);
				const data_t* data10_def = binarySearchValue(&arr.at(0), &arr.at(0) + arr.size(), target);
				const data_t* data11_def = binarySearchValue(&const_arr.at(0), &const_arr.at(0) + const_arr.size(), target);
				printf("  binarySearchValue(*array_shuffle1, target);\n");
				printf("    (data_def=%p)\n", data_def);
				printf("    (data1_def=%p)\n", data1_def);
				printf("    (data2_def=%p)\n", data2_def);
				printf("    (data3_def=%p)\n", data3_def);
				printf("    (data4_def=%p)\n", data4_def);
				printf("    (data5_def=%p)\n", data5_def);
				//printf("    (data6_def=%p)\n", data6_def);
				//printf("    (data7_def=%p)\n", data7_def);
				printf("    (data8_def=%p)\n", data8_def);
				printf("    (data9_def=%p)\n", data9_def);
				printf("   (data10_def=%p)\n", data10_def);
				printf("    (data11_def=%p)\n", data11_def);
			}
			auto comparisonForSearch = [&target](const data_t& data) -> int { return target - data.m_key; };
			const data_t* data = binarySearch(*array_shuffle1, comparisonForSearch);
			{
				array_t& arr = *array_shuffle1;
				const array_t& const_arr = *array_shuffle1;
				const data_t* data1 = binarySearch(arr, comparisonForSearch);
				const data_t* data2 = binarySearch(const_arr, comparisonForSearch);
				const data_t* data3 = binarySearch(arr.begin(), arr.end(), comparisonForSearch);
				const data_t* data4 = binarySearch(arr.cbegin(), arr.cend(), comparisonForSearch);
				const data_t* data5 = binarySearch(const_arr.begin(), const_arr.end(), comparisonForSearch);
				//const data_t* data6 = binarySearch(arr._Elems, comparisonForSearch);//VC++
				//const data_t* data7 = binarySearch(const_arr._Elems, comparisonForSearch);//VC++
				const data_t* data8 = binarySearch(&arr.at(0), arr.size(), comparisonForSearch);
				const data_t* data9 = binarySearch(&const_arr.at(0), const_arr.size(), comparisonForSearch);
				const data_t* data10 = binarySearch(&arr.at(0), &arr.at(0) + arr.size(), comparisonForSearch);
				const data_t* data11 = binarySearch(&const_arr.at(0), &const_arr.at(0) + const_arr.size(), comparisonForSearch);
				printf("  binarySearch(*array_shuffle1, comparisonForSearch);\n");
				printf("    (data=%p)\n", data);
				printf("    (data1=%p)\n", data1);
				printf("    (data2=%p)\n", data2);
				printf("    (data3=%p)\n", data3);
				printf("    (data4=%p)\n", data4);
				printf("    (data5=%p)\n", data5);
				//printf("    (data6=%p)\n", data6);
				//printf("    (data7=%p)\n", data7);
				printf("    (data8=%p)\n", data8);
				printf("    (data9=%p)\n", data9);
				printf("    (data10=%p)\n", data10);
				printf("    (data11=%p)\n", data11);
			}
			auto search_comparison_val = [](const data_t& data, const int target) -> int { return target - data.m_key; };
			const data_t* data_val = binarySearchValue(*array_shuffle1, target, search_comparison_val);
			{
				array_t& arr = *array_shuffle1;
				const array_t& const_arr = *array_shuffle1;
				const data_t* data1_val = binarySearchValue(arr, target, search_comparison_val);
				const data_t* data2_val = binarySearchValue(const_arr, target, search_comparison_val);
				const data_t* data3_val = binarySearchValue(arr.begin(), arr.end(), target, search_comparison_val);
				const data_t* data4_val = binarySearchValue(arr.cbegin(), arr.cend(), target, search_comparison_val);
				const data_t* data5_val = binarySearchValue(const_arr.begin(), const_arr.end(), target, search_comparison_val);
				//const data_t* data6_val = binarySearchValue(arr._Elems, target, search_comparison_val);//VC++
				//const data_t* data7_val = binarySearchValue(const_arr._Elems, target, search_comparison_val);//VC++
				const data_t* data8_val = binarySearchValue(&arr.at(0), arr.size(), target, search_comparison_val);
				const data_t* data9_val = binarySearchValue(&const_arr.at(0), const_arr.size(), target, search_comparison_val);
				const data_t* data10_val = binarySearchValue(&arr.at(0), &arr.at(0) + arr.size(), target, search_comparison_val);
				const data_t* data11_val = binarySearchValue(&const_arr.at(0), &const_arr.at(0) + const_arr.size(), target, search_comparison_val);
				printf("  binarySearchValue(*array_shuffle1, target, search_comparison_val);\n");
				printf("    (data_val=%p)\n", data_val);
				printf("    (data1_val=%p)\n", data1_val);
				printf("    (data2_val=%p)\n", data2_val);
				printf("    (data3_val=%p)\n", data3_val);
				printf("    (data4_val=%p)\n", data4_val);
				printf("    (data5_val=%p)\n", data5_val);
				//printf("    (data6_val=%p)\n", data6_val);
				//printf("    (data7_val=%p)\n", data7_val);
				printf("    (data8_val=%p)\n", data8_val);
				printf("    (data9_val=%p)\n", data9_val);
				printf("    (data10_val=%p)\n", data10_val);
				printf("    (data11_val=%p)\n", data11_val);
			}

			if (data_def)
				printf("target=%d, binarySearch=[%d, %d]\n", target, data_def->m_key, data_def->m_seqNo);
			else
				printf("target=%d, binarySearch=nullptr\n", target);
		}
	}
#endif
#endif
#endif

	//----------------------------------------
	//測定終了
	printf("============================================================\n");
	printf("Finalize\n");
	printf("============================================================\n");
	deleteArraySet();
	printf("\n");
	printf("\n");
	printf("============================================================\n");
	printf("End\n");
	printf("============================================================\n");
	printElapsedTime(begin_time, true);//処理時間表示

#if 0
	//ポインタ変数のソート処理動作確認
	{
		printf("\n");
		printf("\n");
		printf("--- Test for sort with pointer ---\n");
		static const std::size_t data_num = 10;
		std::array<data_t, data_num> d_arr;
		std::array<data_t*, data_num> p_arr;
		for (int i = 0; i < data_num; ++i)
		{
			d_arr[i].m_key = i;
			p_arr[data_num - i - 1] = &d_arr[i];
		}
		auto printData = [&p_arr]()
		{
			for (data_t* p : p_arr)
			{
				printf("[%d] ", p->m_key);
			}
			printf("\n");
		};
		auto predicate = [](data_t* val1, data_t* val2){ return val1->m_key < val2->m_key; };
		std::size_t unordered = sumupUnordered(p_arr, predicate);
		printf("[Before]\n");
		printData();
		printf("unordered=%d\n", unordered);
		introSort(p_arr, predicate);
		unordered = sumupUnordered(p_arr, predicate);
		printf("[After]\n");
		printData();
		printf("unordered=%d\n", unordered);
		printf("--- End ---\n");
	}
#endif
}

// End of file
