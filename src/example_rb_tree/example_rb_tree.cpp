﻿//--------------------------------------------------------------------------------
// exmaple_rb_tree.cpp
// 赤黒木テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_rb_tree.h"//赤黒木コンテナテスト

#include <gasha/rb_tree.inl>//赤黒木コンテナ【インライン関数／テンプレート関数定義部】

#include <gasha/iterator.h>//イテレータ操作

#include <utility>//C++11 std::forward
#include <random>//C++11 std::random
#include <chrono>//C++11 std::chrono

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <algorithm> <map> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::for_each()
//#include <map>//std::map（比較用）

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//赤黒木テスト
//--------------------------------------------------------------------------------

//----------------------------------------
//テストデータ

//コンストラクタ
data_t::data_t(const int key, const int val) :
	m_childS(nullptr),
	m_childL(nullptr),
	m_isBlack(false),
	m_key(key),
	m_val(val)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor(%d, %d)\n", key, val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
//デフォルトコンストラクタ
data_t::data_t() :
	m_childS(nullptr),
	m_childL(nullptr),
	m_isBlack(false),
	m_key(0),
	m_val(0)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor()\n");
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
//デストラクタ
data_t::~data_t()
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::destructor(): key=%d, val=%d\n", m_key, m_val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
//ムーブオペレータ
data_t::data_t& operator=(data_t&& rhs)
{
	memcpy(this, &rhs, sizeof(*this));
	printf("data_t::move_operator\n");
	return *this;
}
//コピーオペレータ
data_t::data_t& operator=(const data_t& rhs)
{
	memcpy(this, &rhs, sizeof(*this));
	printf("data_t::copy_operator\n");
	return *this;
}
//ムーブコンストラクタ
data_t::data_t(data_t&& src)
{
	memcpy(this, &src, sizeof(*this));
	printf("data_t::move_constructor\n");
}
//コピーコンストラクタ
data_t::data_t(const data_t& src)
{
	memcpy(this, &src, sizeof(*this));
	printf("data_t::copy_constructor\n");
}
#endif//TEST_DATA_WATCH_CONSTRUCTOR

//----------------------------------------
//テスト用補助関数
#ifdef PRINT_TEST_DATA_DETAIL
template<typename... Tx>
inline int printf_detail(const char* fmt, Tx&&... args)
{
	return printf(fmt, std::forward<Tx>(args)...);
}
#else//PRINT_TEST_DATA_DETAIL
inline int printf_detail(const char* fmt, ...){ return 0; }
#endif//PRINT_TEST_DATA_DETAIL
#ifdef PRINT_TEST_DATA_SEARCH
template<typename... Tx>
inline int printf_dbg_search(const char* fmt, Tx&&... args)
{
	return printf(fmt, std::forward<Tx>(args)...);
}
#else//PRINT_TEST_DATA_SEARCH
inline int printf_dbg_search(const char* fmt, ...){ return 0; }
#endif//PRINT_TEST_DATA_SEARCH

//----------------------------------------
//赤黒木テスト
void example_rb_tree()
{
	//型
	typedef rb_tree::container<ope> container_t;
	typedef container_t::iterator iterator;
	typedef container_t::const_iterator const_iterator;
	typedef container_t::reverse_iterator reverse_iterator;
	typedef container_t::const_reverse_iterator const_reverse_iterator;
	
	//赤黒木コンテナ
	container_t con;

	//処理時間計測開始
	const std::chrono::system_clock::time_point begin_time = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point prev_time = begin_time;

	//データを登録
	auto regList = [&con]()
	{
		printf("--- Make table ---\n");
		auto insert = [&con](const int key, const int val)
		{
			data_t* new_node = new data_t(key, val);
			printf_detail("[%2d:%d] ", new_node->m_key, new_node->m_val);
			con.insert(*new_node);
			rb_tree::printf_dbg_add("\n");
		};
	#ifdef REGIST_TEST_DATA_SEQUENTIALLY
		//シーケンシャルにデータ登録
		for (int i = 0; i < TEST_DATA_REG_NUM; ++i)
		{
			const int key = TEST_DATA_KEY_MIN + i % (TEST_DATA_KEY_MAX - TEST_DATA_KEY_MIN + 1);
			insert(key, 1000 + i);
		}
	#else//REGIST_TEST_DATA_SEQUENTIALLY
	#ifndef TEST_DATA_REGISTRATION_LIST
		//ランダムにデータ登録
		std::mt19937 rand_engine;
		rand_engine.seed(0);
		std::uniform_int_distribution<int> rand_dist(TEST_DATA_KEY_MIN, TEST_DATA_KEY_MAX);
		for (int i = 0; i < TEST_DATA_REG_NUM; ++i)
			insert(rand_dist(rand_engine), 1000 + i);
	#else//TEST_DATA_REGISTRATION_LIST
		//固定順にデータ登録
		const int key_list[] = TEST_DATA_REGISTRATION_LIST;
		{
			int i = 0;
			for (int key : key_list)
				insert(key, 1000 + (i++));
		}
	#endif//TEST_DATA_REGISTRATION_LIST
	#endif//REGIST_TEST_DATA_SEQUENTIALLY
		printf_detail("\n");
		printf("%d registered.\n", TEST_DATA_REG_NUM);
	};
	regList();

	//経過時間を表示
	auto printElapsedTime = [](const std::chrono::system_clock::time_point& prev_time) -> std::chrono::system_clock::time_point
	{
		//最終経過時間表示
		const auto now_time = std::chrono::system_clock::now();
		const auto duration = now_time - prev_time;
		const double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000.;
		printf("*elapsed_time=%.9lf sec\n", elapsed_time);
		return now_time;
	};
	prev_time = printElapsedTime(prev_time);

	//木を表示
	auto showTree = [&con]()
	{
		printf("--- Show tree (count=%d) ---\n", con.size());
		//static const int depth_limit = 5;//最大でも5段階目までを表示（0段階目から数えるので最大で6段階表示される→最大：1+2+4+8+16+32=63個）
		static const int depth_limit = 4;//最大でも4段階目までを表示（0段階目から数えるので最大で5段階表示される→最大：1+2+4+8+16=31個）
		const int _depth_max = con.maxDepth();
		printf("depth_max=%d (limit for showing=%d)\n", _depth_max, depth_limit);
	#ifdef PRINT_TEST_DATA_TREE
		const int depth_max = _depth_max <= depth_limit ? _depth_max : depth_limit;
		const int width_max = depth_max < 0 ? 0 : 1 << depth_max;//static_cast<int>(std::pow(2, depth_max));
		for (int depth = 0; depth <= depth_max; ++depth)
		{
			const int width = 1 << depth;//static_cast<int>(std::pow(2, depth));
			static const int label_len = 6;
			const int print_width = (width_max * label_len) / width;
			const int print_indent = (print_width - label_len) / 2;
			for (int breath = 0; breath < width; ++breath)
			{
				const data_t* node = con.root();
				int breath_tmp = breath;
				for (int depth_tmp = depth - 1; node; --depth_tmp)
				{
					if (depth_tmp < 0)
						break;
					node = ope::getChild(*node, (breath_tmp & (0x1 << depth_tmp)) != 0x0);
				}
				if (node)
				{
					{
						int c = 0;
						for (; c < print_indent / 2; ++c)
							printf(" ");
						if (ope::getChildS(*node) && c < print_indent)
						{
							printf(".");
							++c;
						}
						for (; c < print_indent; ++c)
							printf(ope::getChildS(*node) ? "-" : " ");
					}
					printf("%s%2d:%c%s", ope::getChildS(*node) ? "{" : "[", node->m_key, ope::isBlack(*node) ? 'B' : 'R', ope::getChildL(*node) ? "}" : "]");
					{
						int c = 0;
						for (; c < print_indent / 2; ++c)
							printf(ope::getChildL(*node) ? "-" : " ");
						if (ope::getChildL(*node) && c < print_indent)
						{
							printf(".");
							++c;
						}
						for (; c < print_indent; ++c)
							printf(" ");
					}
				}
				else
				{
					for (int c = 0; c < print_width; ++c)
						printf(" ");
				}
			}
			printf("\n");
		}
	#endif//PRINT_TEST_DATA_TREE
	};
	showTree();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//各枝までのノード数を表示
	//※条件③と条件④違反確認
	auto showNodesCount = [&con]()
	{
		printf("--- Show nodes count (count=%d) ---\n", con.size());
		const int depth_max = con.maxDepth();
		const unsigned long long width_max = depth_max < 0 ? 0llu : 1llu << depth_max;//static_cast<long long>(std::pow(2, static_cast<long long>(depth_max)));
		printf("depth_max=%d, width_max=%llu\n", depth_max, width_max);
		if(depth_max > 63)
		{
			printf("'depth_max' is over 63. aborting therefor.\n");
			return;
		}
	#ifdef PRINT_TEST_DATA_COLOR_COUNT
		const data_t* prev_node = nullptr;
		int blacks_min = -1;
		int blacks_max = -1;
		int reds_min = -1;
		int reds_max = -1;
		int total_min = -1;
		int total_max = -1;
		int total_illegal_connects = 0;
		for (unsigned long long breath = 0; breath < width_max; ++breath)
		{
			int blacks = 0;
			int reds = 0;
			int illegal_connects = 0;
			const data_t* last_node = nullptr;
			const data_t* parent_node = nullptr;
			const data_t* node = con.root();
			long long breath_tmp = breath;
			for (long long depth_tmp = depth_max - 1; node; --depth_tmp)
			{
				last_node = node;
				if (ope::isBlack(*node))
					++blacks;
				else//if (ope::isRed(*node))
				{
					++reds;
					if (parent_node && ope::isRed(*parent_node))
					{
						++illegal_connects;
					}
				}
				if (depth_tmp < 0)
					break;
				parent_node = node;
				node = ope::getChild(*node, (breath_tmp & (0x1ll << depth_tmp)) != 0x0ll);
			}
			int total = blacks + reds;
			blacks_min = blacks_min > blacks || blacks_min == -1 ? blacks : blacks_min;
			blacks_max = blacks_max < blacks || blacks_max == -1 ? blacks : blacks_max;
			reds_min = reds_min > reds || reds_min == -1 ? reds : reds_min;
			reds_max = reds_max < reds || reds_max == -1 ? reds : reds_max;
			total_min = total_min > total || total_min == -1 ? total : total_min;
			total_max = total_max < total || total_max == -1 ? total : total_max;
			total_illegal_connects += illegal_connects;
			if (prev_node != last_node)
			{
			#ifdef PRINT_TEST_DATA_DETAIL
				printf("%5lld:[%2d] blacks=%d, reds=%d, total=%d (illegal=%d)\n", breath, last_node->m_key, blacks, reds, total, illegal_connects);
			#endif//PRINT_TEST_DATA_DETAIL
			}
			prev_node = last_node;
		}
		blacks_min = blacks_min >= 0 ? blacks_min : 0;
		blacks_max = blacks_max >= 0 ? blacks_max : 0;
		reds_min = reds_min >= 0 ? reds_min : 0;
		reds_max = reds_max >= 0 ? reds_max : 0;
		total_min = total_min >= 0 ? total_min : 0;
		total_max = total_max >= 0 ? total_max : 0;
		printf("max: blacks=%d, reds=%d, total=%d\n", blacks_max, reds_max, total_max);
		printf("min: blacks=%d, reds=%d, total=%d\n", blacks_min, reds_min, total_min);
		printf("diff:blacks=%d, reds=%d, total=%d (illegal=%d)\n", blacks_max - blacks_min, reds_max - reds_min, total_max - total_min, total_illegal_connects);
	#endif//PRINT_TEST_DATA_COLOR_COUNT
	};
	showNodesCount();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//一番小さいノードから昇順に全ノードをリストアップ
	auto showListAsc = [&con]()
	{
		printf("--- Show nodes ascending (count=%d) ---\n", con.size());
		bool is_found = false;
		for (const data_t& obj : con)
		{
			if (!is_found)
				is_found = true;
			printf_detail("[%2d:%4d] ", obj.m_key, obj.m_val);
		}
		if (is_found)
			printf_detail("\n");
		else
			printf("(nothing)\n");
	};
	showListAsc();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//一番大きいノードから降順に全ノードをリストアップ
	auto showListDesc = [&con]()
	{
		printf("--- Show nodes descending (count=%d) ---\n", con.size());
		bool is_found = false;
		std::for_each(con.rbegin(), con.rend(),//リバースイテレータ
			[&is_found](const data_t& obj)
			{
				if (!is_found)
					is_found = true;
				printf_detail("[%2d:%4d] ", obj.m_key, obj.m_val);
			}
		);
		if (is_found)
			printf_detail("\n");
		else
			printf("(nothing)\n");
	};
	showListDesc();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	#ifdef TEST_ITERATOR_OPERATION
	{
		printf("\n");
		printf("--------------------[iterator operattion:begin]\n");
		printf("[constructor]\n");
		container_t::iterator ite = con.begin();
		container_t::reverse_iterator rite = con.rbegin();
		container_t::iterator ite_end = con.end();
		container_t::reverse_iterator rite_end = con.rend();
		container_t::iterator ite2 = con.rbegin();
		container_t::reverse_iterator rite2 = con.begin();
		container_t::iterator ite2_end = con.rend();
		container_t::reverse_iterator rite2_end = con.end();
		printf("constructor\n");
		if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
		if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:key=%d, value=%d\n", ite2->m_key, ite2->m_val);
		if (rite2.isExist()) printf("rite2:key=%d, value=%d\n", rite2->m_key, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
		printf("ite_end - ite = %d\n", ite_end - ite);
		printf("ite - ite_end = %d\n", ite - ite_end);
		printf("rite_end - rite = %d\n", rite_end - rite);
		printf("rite - rite_end = %d\n", rite - rite_end);
		printf("ite2 - ite = %d\n", ite2 - ite);
		printf("ite - ite2 = %d\n", ite - ite2);
		printf("rite2 - rite = %d\n", rite2 - rite);
		printf("rite - rite2 = %d\n", rite - rite2);
		printf("[copy operator]\n");
		ite = con.begin();
		rite = con.rbegin();
		ite_end = con.end();
		rite_end = con.rend();
		ite2 = con.rbegin();
		rite2 = con.begin();
		ite2_end = con.rend();
		rite2_end = con.end();
		if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
		if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:key=%d, value=%d\n", ite2->m_key, ite2->m_val);
		if (rite2.isExist()) printf("rite2:key=%d, value=%d\n", rite2->m_key, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
		printf("[rite.base()]\n");
		ite2 = rite.base();
		ite2_end = rite_end.base();
		if (ite2.isExist()) printf("ite2: key=%d, value=%d\n", ite2->m_key, ite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end: key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
		printf("[++ite,--ie_end]\n");
		++ite;
		++rite;
		--ite_end;
		--rite_end;
		if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
		if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
		printf("[--ite,++ite_end]\n");
		--ite;
		--rite;
		++ite_end;
		++rite_end;
		if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
		if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
		for (int i = 0; i < 3; ++i)
		{
			printf("[ite[%d]]\n", i);
			ite = ite[i];
			rite = rite[i];
			if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		}
		printf("[ite+=3]\n");
		ite += 3;
		rite += 3;
		if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
		if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		printf("[ite-=3]\n");
		ite -= 3;
		rite -= 3;
		if (ite.isExist()) printf("ite:key=%d, value=%d\n", ite->m_key, ite->m_val);
		if (rite.isExist()) printf("rite:key=%d, value=%d\n", rite->m_key, rite->m_val);
		printf("ite_end - ite = %d\n", ite_end - ite);
		printf("ite - ite_end = %d\n", ite - ite_end);
		printf("rite_end - rite = %d\n", rite_end - rite);
		printf("rite - rite_end = %d\n", rite - rite_end);
		printf("[ite2-=2]\n");
		ite2 -= 2;
		rite2 -= 2;
		printf("ite2 - ite = %d\n", ite2 - ite);
		printf("ite - ite2 = %d\n", ite - ite2);
		printf("rite2 - rite = %d\n", rite2 - rite);
		printf("rite - rite2 = %d\n", rite - rite2);
		printf("[++ite_end]\n");
		++ite_end;
		++rite_end;
		printf("ite_end - ite = %d\n", ite_end - ite);
		printf("ite - ite_end = %d\n", ite - ite_end);
		printf("rite_end - rite = %d\n", rite_end - rite);
		printf("rite - rite_end = %d\n", rite - rite_end);
		printf("--------------------[iterator operattion:end]\n");
	}
#endif

	//指定のキーのノードを検索し、同じキーのノードをリストアップ
	auto searchData = [&con]()
	{
		printf("--- Search each key nodes ---\n");
		for (int search_key = TEST_DATA_KEY_MIN; search_key <= TEST_DATA_KEY_MAX; ++search_key)
		{
			static const int print_count_limit = 10;
			int print_count = 0;
			bool is_found = false;
			std::for_each(con.find(search_key), con.equal_range(search_key),
				[&con, &is_found, &search_key, &print_count](const data_t& obj)
				{
					if (!is_found)
					{
						printf_dbg_search("%2d(%d):", search_key, con.count(search_key));
						is_found = true;
					}
					if (print_count < print_count_limit)
						printf_dbg_search("[%2d] ", obj.m_key);
					else if (print_count == print_count_limit)
						printf_dbg_search("...");
					++print_count;
				}
			);
			if (is_found)
				printf_dbg_search("\n");
		}
	};
	searchData();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//指定のキーのノードを検索
	//※一致ノードは表示を省略
	//※最近ノードから数ノードを表示
	auto searchNearestData = [&con](const rb_tree::match_type_t search_type)
	{
		printf("--- Search each nearest key nodes for %s ---\n", search_type == rb_tree::FOR_NEAREST_SMALLER ? "smaller" : search_type == rb_tree::FOR_NEAREST_LARGER ? "larger" : "same");
		for (int search_key = TEST_DATA_KEY_MIN; search_key <= TEST_DATA_KEY_MAX; ++search_key)
		{
			bool is_found = false;
			const iterator ite(con.find(search_key, search_type));
			const iterator end(con.end());
			for (int i = 0; ite != end && ite->m_key != search_key && i < 4; ++i, ++ite)
			{
				const data_t& obj = *ite;
				if (!is_found)
				{
					printf_dbg_search("%2d:", search_key);
					is_found = true;
				}
				printf_dbg_search("[%2d] ", obj.m_key);
			}
			if (is_found)
				printf_dbg_search("\n");
		}
	};
	//指定のキーと同じか内輪で一番近いノードを検索
	searchNearestData(rb_tree::FOR_NEAREST_SMALLER);
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//指定のキーと同じかそれより大きい最近ノードを検索
	searchNearestData(rb_tree::FOR_NEAREST_LARGER);
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//ノードを削除
	//※特定のキーを削除
	auto removeNodes = [&con]()
	{
		printf("--- Remove nodes ---\n");
		auto erase = [&con](const int remove_key) -> bool
		{
			data_t* removed_node = con.erase(remove_key);
			if (removed_node)
			{
				delete removed_node;
				printf_detail("[%2d] ", remove_key);
				rb_tree::printf_dbg_remove("\n");
				return true;
			}
			return false;
		};
		int removed_count = 0;
	#ifndef TEST_DATA_REMOVING_LIST
		//ランダムにデータ削除
		std::mt19937 rand_engine;
		rand_engine.seed(1);
		std::uniform_int_distribution<int> rand_dist(TEST_DATA_KEY_MIN, TEST_DATA_KEY_MAX);
		const int removed_count_max = TEST_DATA_REG_NUM / 4;
		while (removed_count < removed_count_max)
		{
			const bool result = erase(rand_dist(rand_engine));
			if (result)
				++removed_count;
		}
	#else//TEST_DATA_REMOVING_LIST
		//固定順にデータ削除
		const int key_list[] = TEST_DATA_REMOVING_LIST;
		for (int key : key_list)
		{
			const bool result = erase(key);
			if (result)
				++removed_count;
		}
	#endif//TEST_DATA_REMOVING_LIST
		rb_tree::printf_dbg_remove("\n");
		printf("%d removed.\n", removed_count);
	};
	removeNodes();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//木を表示
	showTree();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//各枝までのノード数を表示
	showNodesCount();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//一番小さいノードから昇順に全ノードをリストアップ
	showListAsc();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//ノードを削除
	//※すべての値のキーを一つずつ削除
	auto removeEachKeyNodes = [&con]()
	{
		printf("--- Remove each key nodes ---\n");
		int removed_count = 0;
		for (int remove_key = TEST_DATA_KEY_MIN; remove_key <= TEST_DATA_KEY_MAX; ++remove_key)
		{
			iterator ite(con.find(remove_key));
			data_t* removed_node = con.erase(ite);
			if (removed_node)
			{
				delete removed_node;
				++removed_count;
				printf_detail("[%2d] ", remove_key);
				rb_tree::printf_dbg_remove("\n");
			}
		}
		rb_tree::printf_dbg_remove("\n");
		printf("%d removed.\n", removed_count);
	};
	removeEachKeyNodes();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//木を表示
	showTree();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//各枝までのノード数を表示
	showNodesCount();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//一番小さいノードから昇順に全ノードをリストアップ
	showListAsc();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//ノードを全削除
	//※すべての値のキーに対して、削除が失敗するまで削除を実行
	auto removeAllNodes = [&con]()
	{
		printf("--- Remove all nodes ---\n");
		int removed_count = 0;
		for (int remove_key = TEST_DATA_KEY_MIN; remove_key <= TEST_DATA_KEY_MAX;)
		{
			iterator ite(con.find(remove_key));
			data_t* removed_node = con.erase(ite);
			if (removed_node)
			{
				delete removed_node;
				++removed_count;
				printf_detail("[%2d] ", remove_key);
				rb_tree::printf_dbg_remove("\n");
			}
			else
				++remove_key;
		}
		rb_tree::printf_dbg_remove("\n");
		printf("%d removed.\n", removed_count);
	};
	removeAllNodes();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//木を表示
	showTree();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//各枝までのノード数を表示
	showNodesCount();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//一番小さいノードから昇順に全ノードをリストアップ
	showListAsc();
	prev_time = printElapsedTime(prev_time);//経過時間を表示

	//終了
	printf("--- end ---\n");
	printElapsedTime(begin_time);//経過時間を表示
}

// End of file
