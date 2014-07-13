//--------------------------------------------------------------------------------
// exmaple_priority_queue.cpp
// 優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_priority_queue.h"//優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト

#include <gasha/binary_heap.inl>//二分ヒープコンテナ【インライン関数／テンプレート関数定義部】
#include <gasha/priority_queue.inl>//優先度付きキューコンテナアダプタ【インライン関数／テンプレート関数定義部】

#include <gasha/iterator.h>//イテレータ操作
#include <gasha/chrono.h>//時間処理ユーティリティ：nowTime(), calcElapsedTime()

#include <utility>//C++11 std::forward
#include <cstring>//memcpy()
#include <cstdio>//printf()
#include <cassert>//assert()

//【VC++】例外を無効化した状態で <random> <algorithm> <queue> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <random>//C++11 std::random
#include <algorithm>//std::for_each()
#include <queue>//std::priority_queue（比較用）

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト
//--------------------------------------------------------------------------------

//コンストラクタ
data_t::data_t(const PRIORITY priority, const int val) :
	m_priority(priority),
	m_seqNo(0),
	m_val(val)
{
	m_data[0] = 99;
	m_data[1] = 99;
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor(%d, %d)\n", priority, val);
	//printf("    m_priority=%d, m_seqNo=%d, m_val=%d\n", m_priority, m_seqNo, m_val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
data_t::data_t(const int val) :
	m_priority(NORMAL),
	m_seqNo(0),
	m_val(val)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor(%d)\n", val);
	//printf("    m_priority=%d, m_seqNo=%d, m_val=%d\n", m_priority, m_seqNo, m_val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
//ムーブオペレータ
data_t& data_t::operator=(data_t&& rhs)
{
	std::memcpy(this, &rhs, sizeof(*this));
	printf("data_t::move_operator\n");
	return *this;
}
//コピーオペレータ
data_t& data_t::operator=(const data_t& rhs)
{
	std::memcpy(this, &rhs, sizeof(*this));
	printf("data_t::copy_operator\n");
	return *this;
}
//ムーブコンストラクタ
data_t::data_t(data_t&& src)
{
	std::memcpy(this, &src, sizeof(*this));
	printf("data_t::move_constructor\n");
}
//コピーコンストラクタ
data_t::data_t(const data_t& src)
{
	std::memcpy(this, &src, sizeof(*this));
	printf("data_t::copy_constructor\n");
}
#endif//TEST_DATA_WATCH_CONSTRUCTOR
//デフォルトコンストラクタ
data_t::data_t() :
	m_priority(NORMAL),
	m_seqNo(0),
	m_val(0)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor\n");
	//printf("    m_priority=%d, m_seqNo=%d, m_val=%d\n", m_priority, m_seqNo, m_val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
//デストラクタ
data_t::~data_t()
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::destructor\n");
	//printf("    m_priority=%d, m_seqNo=%d, m_val=%d\n", m_priority, m_seqNo, m_val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}

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

//----------------------------------------
//木を表示
template<class HEAP>
void showTree(const HEAP& heap)
{
	printf_detail("\n");
	printf_detail("--- Show tree (count=%d) ---\n", heap.size());
	//static const int depth_limit = 5;//最大でも5段階目までを表示（0段階目から数えるので最大で6段階表示される→最大：1+2+4+8+16+32=63個）
	static const int depth_limit = 4;//最大でも4段階目までを表示（0段階目から数えるので最大で5段階表示される→最大：1+2+4+8+16=31個）
	const int _depth_max = heap.maxDepth();
	printf_detail("depth_max=%d (limit for showing=%d)\n", _depth_max, depth_limit);
	const int depth_max = _depth_max <= depth_limit ? _depth_max : depth_limit;
	const int width_max = static_cast<int>(std::pow(2, depth_max));
	for (int depth = 0; depth <= depth_max; ++depth)
	{
		const int width = static_cast<int>(std::pow(2, depth));
		static const int label_len = 6;
		const int print_width = (width_max * label_len) / width;
		const int print_indent = (print_width - label_len) / 2;
		for (int breath = 0; breath < width; ++breath)
		{
			const data_t* node = heap.refTop();
			int breath_tmp = breath;
			for (int depth_tmp = depth - 1; node; --depth_tmp)
			{
				if (depth_tmp < 0)
					break;
				node = heap.refChild(node, (breath_tmp & (0x1 << depth_tmp)) != 0x0);
			}
			if (node)
			{
				{
					int c = 0;
					for (; c < print_indent / 2; ++c)
						printf_detail(" ");
					if (heap.refChildL(node) && c < print_indent)
					{
						printf_detail(".");
						++c;
					}
					for (; c < print_indent; ++c)
						printf_detail(heap.refChildL(node) ? "-" : " ");
				}
				printf_detail("%s%1d:%2d%s", heap.refChildL(node) ? "{" : "[", node->m_priority, node->m_val, heap.refChildR(node) ? "}" : "]");
				{
					int c = 0;
					for (; c < print_indent / 2; ++c)
						printf_detail(heap.refChildR(node) ? "-" : " ");
					if (heap.refChildR(node) && c < print_indent)
					{
						printf_detail(".");
						++c;
					}
					for (; c < print_indent; ++c)
						printf_detail(" ");
				}
			}
			else
			{
				for (int c = 0; c < print_width; ++c)
					printf_detail(" ");
			}
		}
		printf_detail("\n");
	}
};

//----------------------------------------
//優先度付きキューコンテナアダプタテスト
void example_priority_queue()
{
	//プライオリティキューコンテナ生成
	typedef priority_queue::container<ope, TEST_DATA_MAX> pqueue_t;
	pqueue_t* con = new pqueue_t();

	//処理時間計測開始
	const auto begin_time = nowTime();
	auto prev_time = begin_time;

	//--------------------
	//プライオリティキューのテスト
	printf("--------------------------------------------------------------------------------\n");
	printf("[Test for priority_queue::containerAdapter(Priority Queue)]\n");

	//エンキュー
	auto enqueue = [&con]()
	{
		printf("\n");
		printf("--- Enqueue ---\n");
		std::mt19937 rand_engine;
		rand_engine.seed(0);
		std::uniform_int_distribution<int> rand_dist(TEST_DATA_PRIOR_MIN, TEST_DATA_PRIOR_MAX);
		{
			data_t* obj = con->enqueue(NORMAL, 0);
			printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
		}
		for (int val = 1; val < TEST_DATA_REG_NUM; ++val)
		{
			const PRIORITY priority = static_cast<PRIORITY>(rand_dist(rand_engine));
			//【エンキュー方法①】オブジェクトを受け渡す方法
			//※オブジェクトのコピーが発生するので少し遅い。
			#if TEST_USE_ENQUEUE_TYPE == 1
			{
				data_t new_obj(priority, val);
				data_t* obj = con->enqueueCopying(new_obj);
				printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
			}
			//【推奨】【エンキュー方法②】コンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーは発生しない。
			//※コンストラクタが呼び出される。
			#elif TEST_USE_ENQUEUE_TYPE == 2
			{
				data_t* obj = con->enqueue(priority, val);//優先度とコンストラクタパラメータを渡して登録
				printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
			}
			//【エンキュー方法③】新規キュー（オブジェクト）の参照を受け取って値をセットする方法
			//※オブジェクトのコピーは発生しない。
			//※最初にコンストラクタ呼び出しを行うが、その時点でエンキューを完了せず、オブジェクトを操作できる。
			//※単一操作オブジェクトを使用することで、誤った操作を防ぎ（二重エンキューやエンキュー中のデキューなど）、かつ、
			//　処理ブロックを抜ける時に自動敵にエンキューを完了する。
			//※明示的にエンキューを終了または取り消しすることも可能。
			//※エンキューを開始してから完了するまでの間、ロックが取得される。
			//※エンキュー終了時にはオブジェクトのポインタが変わる点に注意。
			#elif TEST_USE_ENQUEUE_TYPE == 3
			{
				auto ope = con->operationUnique();//単一操作オブジェクト ※pqueue_t::uniqueOperation ope(*con); と書くのと同じ
				data_t* obj = ope.enqueueBegin(priority, val);//この時点で優先度とシーケンス番号がセットされ、ロックが取得される
				//※エンキュー途中の状態。この状態では、単一操作オブジェクトの他の操作が禁止される。
				//※戻り値は、処理ブロック内でしか（enqueueEnd/enqueueCancel呼び出しまでしか）有効ではないポインタなので注意
				obj->m_val = val;
				printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
				//処理ブロックを抜ける時に自動的にデキューが終了し、ロックが解放される。
				//※受け取ったポインタを処理ブロックの外で参照すると、誤った情報を参照することになるので注意。
				//※明示的にエンキュー終了／取り消しを実行することも可能。
				//obj = ope.enqueueEnd();//明示的なエンキュー終了を行うと、正しいオブジェクトの参照を取得できる ※ロックが解除されるので、他のスレッドでデキューされる可能性がある事に注意
				//ope.enqueueCancel();//エンキュー取り消し
			}
			#endif//TEST_USE_ENQUEUE_TYPE
		}
	};
	enqueue();

	//経過時間を表示
	auto printElapsedTime = [](const std::chrono::system_clock::time_point& prev_time, const bool is_show) -> std::chrono::system_clock::time_point
	{
		//最終経過時間表示
		auto now_time = nowTime();
		const double elapsed_time = calcElapsedTime(prev_time, now_time);
		if (is_show)
			printf("*elapsed_time=%.9lf sec\n", elapsed_time);
		return now_time;
	};
	prev_time = printElapsedTime(prev_time, true);

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//キューのリストを表示
	auto printTable = [&con, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- Print Queue List ---\n");
		typedef pqueue_t::container_type container_t;
		forEach(static_cast<container_t&>(*con), [&con](const data_t& obj)
			{
				printf_detail(" [%d,%d:%d]", obj.m_priority, obj.m_seqNo, obj.m_val);
			}
		);
		printf_detail("\n");
		prev_time = printElapsedTime(prev_time, false);
	};
	printTable();

#ifdef GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR
	auto printTableReversed = [&con, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- Print Queue List (reverse) ---\n");
		typedef pqueue_t::container_type container_t;
		reverseForEach(static_cast<container_t&>(*con), [&con](const data_t& obj)
			{
				printf_detail(" [%d,%d:%d]", obj.m_priority, obj.m_seqNo, obj.m_val);
			}
		);
		printf_detail("\n");
		prev_time = printElapsedTime(prev_time, false);
	};
	printTableReversed();
#endif//GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR

#if defined(GASHA_BINARY_HEAP_ENABLE_RANDOM_ACCESS_INTERFACE) && defined(GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR)
#ifdef TEST_ITERATOR_OPERATION
	{
		printf("\n");
		typedef pqueue_t::container_type container_t;
		container_t& heap = *con;
		printf("--------------------[iterator operattion:begin]\n");
		printf("[constructor]\n");
		container_t::iterator ite = heap.begin();
		container_t::reverse_iterator rite = heap.rbegin();
		container_t::iterator ite_end = heap.end();
		container_t::reverse_iterator rite_end = heap.rend();
		container_t::iterator ite2 = heap.rbegin();
		container_t::reverse_iterator rite2 = heap.begin();
		container_t::iterator ite2_end = heap.rend();
		container_t::reverse_iterator rite2_end = heap.end();
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		printf("ite_end - ite = %d\n", ite_end - ite);
		printf("ite - ite_end = %d\n", ite - ite_end);
		printf("rite_end - rite = %d\n", rite_end - rite);
		printf("rite - rite_end = %d\n", rite - rite_end);
		printf("ite2 - ite = %d\n", ite2 - ite);
		printf("ite - ite2 = %d\n", ite - ite2);
		printf("rite2 - rite = %d\n", rite2 - rite);
		printf("rite - rite2 = %d\n", rite - rite2);
		printf("[copy operator]\n");
		ite = heap.begin();
		rite = heap.rbegin();
		ite_end = heap.end();
		rite_end = heap.rend();
		ite2 = heap.rbegin();
		rite2 = heap.begin();
		ite2_end = heap.rend();
		rite2_end = heap.end();
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		printf("[rite.base()]\n");
		ite2 = rite.base();
		ite2_end = rite_end.base();
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		printf("[++ite,--ie_end]\n");
		++ite;
		++rite;
		--ite_end;
		--rite_end;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		printf("[--ite,++ie_end]\n");
		--ite;
		--rite;
		++ite_end;
		++rite_end;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		for (int i = 0; i < 3; ++i)
		{
			printf("[ite[%d]]\n", i);
			ite = ite[i];
			rite = rite[i];
			if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		}
		printf("[ite+=3]\n");
		ite += 3;
		rite += 3;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		printf("[ite-=3]\n");
		ite -= 3;
		rite -= 3;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
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
#endif//TEST_ITERATOR_OPERATION
#endif//GASHA_BINARY_HEAP_ENABLE_RANDOM_ACCESS_INTERFACE, GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR

#ifdef TEST_LOCK_OPERATION
	//ロック操作テスト
	printf("--------------------[lock operation:begin]\n");
	{
		auto lock(con->lockScoped());//lock_guard<container_t::lock_type> lock(*con);と同じ
		printf(".lockScoped() ... OK\n");
	}
	{
		auto lock(con->lockUnique());//unique_shared_lock<container_t::lock_type> lock(*con);と同じ
		printf(".lockUnique() ... OK\n");
	}
	{
		auto lock(con->lockUnique(with_lock));//unique_shared_lock<container_t::lock_type> lock(*con, with_lock);と同じ
		printf(".lockUnique(with_lock) ... OK\n");
	}
	{
		auto lock(con->lockUnique(try_to_lock));//unique_shared_lock<container_t::lock_type> lock(*con, try_to_lock);と同じ
		printf(".lockUnique(try_to_lock) ... OK\n");
	}
	{
		pqueue_t::lock_type& lock_obj = *con;
		lock_obj.lock();
		auto lock(con->lockUnique(adopt_lock));//unique_shared_lock<container_t::lock_type> lock(*con, adopt_lock);と同じ
		printf(".lockUnique(adopt_lock) ... OK\n");
	}
	{
		auto lock(con->lockUnique(defer_lock));//unique_shared_lock<container_t::lock_type> lock(*con, defer_lock);と同じ
		printf(".lockUnique(defer_lock) ... OK\n");
	}
	printf("--------------------[lock operation:end]\n");
#endif//TEST_LOCK_OPERATION

	//デキュー
	auto dequeue = [&con](const int pop_limit)
	{
		printf("\n");
		printf("--- Dequeue ---\n");
		for (int i = 0; i < pop_limit; ++i)
		{
			//【推奨】【デキュー方法①】情報取得用のオブジェクトを受け渡す
			//※オブジェクトのコピーが発生する。
			//※デストラクタが呼び出される。（コピー後に実行）
			#if TEST_USE_DEQUEUE_TYPE == 1
			{
				data_t node;
				const bool result = con->dequeueCopying(node);
				if (!result)
					break;
				printf_detail("[%1d:%2d] ", node.m_priority, node.m_val);
			}
			//【デキュー方法②】キュー（オブジェクト）の参照を受け取る方法
			//※オブジェクトのコピーは発生しない。
			//※単一操作オブジェクトを使用することで、誤った操作を防ぎ（二重デキューやデキュー中のエンキューなど）、かつ、
			//　処理ブロックを抜ける時に自動敵にデキューを完了する。
			//※明示的にデキューを終了または取り消しすることも可能。
			//※デキューを開始してから完了するまでの間、ロックが取得される。
			//※最後にデストラクタが呼び出される。
			#elif TEST_USE_DEQUEUE_TYPE == 2
			{
				auto ope = con->operationUnique();//単一操作オブジェクト ※pqueue_t::uniqueOperation ope(*con); と書くのと同じ
				data_t* obj = ope.dequeueBegin();//この時点でロックが取得される
				//※デキュー途中の状態。この状態では、単一操作オブジェクトの他の操作が禁止される。
				//※戻り値は、処理ブロック内でしか（dequeueEnd/dequeueCancel呼び出しまでしか）有効ではないポインタなので注意
				if (!obj)
					break;
				printf_detail("[%1d:%2d] ", obj->m_priority, obj->m_val);
				//処理ブロックを抜ける時に自動的にデキューが終了し、ロックが解放される。
				//※受け取ったポインタを処理ブロックの外で参照すると、誤った情報を参照することになるので注意。
				//※明示的にデキュー終了／取り消しを実行することも可能。
				//ope.dequeueEnd();//デキュー完了
				//ope.dequeueCancel();//デキュー取り消し
			}
			#endif//TEST_USE_DEQUEUE_TYPE
		}
		printf_detail("\n");
	};
	dequeue(3);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//先頭（根）ノードの優先度を変更
	auto changePriorityOnTop = [&con](const PRIORITY new_priority)
	{
		printf("\n");
		printf("--- Change Priority ---\n");
		const data_t* node = con->top();//先頭ノードを取得（ポップされない）
		printf("[%1d:%2d(seq=%d)]", node->m_priority, node->m_val, node->m_seqNo);
		node = con->changePriorityOnTop(new_priority);//優先度を変更（変更後、キューが再配置される）
		printf(" -> [%1d:%2d(seq=%d)]\n", node->m_priority, node->m_val, node->m_seqNo);
	};
	changePriorityOnTop(HIGHEST);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	changePriorityOnTop(LOWER);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	changePriorityOnTop(HIGHER);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//デキュー
	dequeue(TEST_DATA_REG_NUM / 2);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//デキュー
	dequeue(TEST_DATA_REG_NUM);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//--------------------
	//プライオリティキューのクリアのテスト
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[Test for priority_queue::containerAdapter(Priority Queue)] *Clear\n");

	//エンキュー
	enqueue();
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	printf("\n");
	printf("--- Clear ---\n");

	//クリア
	con->clear();
	printf("OK\n");
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//--------------------
	//ポインタ変数をキューイングする場合のテスト
	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for priority_queue::containerAdapter(Priority Queue)] *Pointer\n");
		printf("\n");

		//プライオリティキュー
		priority_queue::container<ptr_ope, TEST_DATA_TABLE_SIZE_FOR_POINTER> p_con;

		//エンキュー
		data_t obj1(NORMAL, 1);
		data_t obj2(HIGHER, 2);
		data_t obj3(LOWER, 3);
		data_t obj4(HIGHEST, 4);
		data_t obj5(LOWEST, 5);
		p_con.enqueueCopying(&obj1);
		p_con.enqueueCopying(&obj2);
		p_con.enqueueCopying(&obj3);
		p_con.enqueueCopying(&obj4);
		p_con.enqueueCopying(&obj5);

		//デキュー
		auto dequeuObj = [&p_con]()
		{
			data_t* obj_p = nullptr;
			p_con.dequeueCopying(obj_p);
			printf_detail("pop: [%1d:%2d](seq=%d)\n", obj_p->m_priority, obj_p->m_val, obj_p->m_seqNo);
		};
		dequeuObj();
		dequeuObj();
		dequeuObj();
		dequeuObj();
		dequeuObj();
	}

	//--------------------
	//【挙動比較用】プライオリティキューの再テスト
	//※上記の二分ヒープ／STLのテストと同一の流れのテストを実施
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[Test for priority_queue::containerAdapter(Priority Queue)] *Second time\n");

	//エンキュー
	enqueue();
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//デキュー
	dequeue(TEST_DATA_REG_NUM / 2);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//デキュー
	dequeue(TEST_DATA_REG_NUM);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//インスタンスを破棄
	delete con;
	con = nullptr;

	//終了
	printf("\n");
	printf("--- end ---\n");
	printElapsedTime(begin_time, true);//経過時間を表示
}

//----------------------------------------
//二分ヒープコンテナテスト
void example_binary_heap()
{
	//ヒープコンテナ生成
	typedef binary_heap::container<heap_ope, TEST_DATA_MAX> heap_t;
	heap_t* heap = new heap_t();

	//処理時間計測開始
	const auto begin_time = nowTime();
	auto prev_time = begin_time;
	
	//--------------------
	//【挙動比較用】二分ヒープのテスト
	//※プライオリティキューと異なり、ポップ時に、プッシュ時（エンキュー時）の順序性が保証されていないことが確認できる
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[Test for binary_heap::container(Binary Heap)]\n");

	//二分ヒープでノードをプッシュ
	auto pushNodesBinHeap = [&heap]()
	{
		printf("\n");
		printf("--- Push nodes(Binary Heap) ---\n");
		std::mt19937 rand_engine;
		rand_engine.seed(0);
		std::uniform_int_distribution<int> rand_dist(TEST_DATA_PRIOR_MIN, TEST_DATA_PRIOR_MAX);
		{
			data_t* obj = heap->push(NORMAL, 0);
			printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
		}
		for (int val = 1; val < TEST_DATA_REG_NUM; ++val)
		{
			//※上記プライオリティキューで説明した、３種類のプッシュ方法が使える
			const PRIORITY priority = static_cast<PRIORITY>(rand_dist(rand_engine));
			//【プッシュ方法①】オブジェクトを受け渡す方法
			//※オブジェクトのコピーが発生するので少し遅い。
			#if TEST_USE_PUSH_TYPE == 1
			{
				data_t new_obj(priority, val);
				data_t* obj = heap->pushCopying(new_obj);
				printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
			}
			//【推奨】【プッシュ方法②】コンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーは発生しない。
			//※コンストラクタが呼び出される。
			#elif TEST_USE_PUSH_TYPE == 2
			{
				data_t* obj = heap->push(priority, val);
				printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
			}
			//【プッシュ方法③】新規キュー（オブジェクト）の参照を受け取って値をセットする方法
			//※オブジェクトのコピーは発生しない。
			//※最初にコンストラクタ呼び出しを行うが、その時点でプッシュを完了せず、オブジェクトを操作できる。
			//※単一操作オブジェクトを使用することで、誤った操作を防ぎ（二重プッシュやプッシュ中のポップなど）、かつ、
			//　処理ブロックを抜ける時に自動敵にプッシュを完了する。
			//※明示的にプッシュを終了または取り消しすることも可能。
			//※プッシュを開始してから完了するまでの間、ロックが取得される。
			//※プッシュ終了時にはオブジェクトのポインタが変わる点に注意。
			#elif TEST_USE_PUSH_TYPE == 3
			{
				auto ope = heap->operationUnique();//単一操作オブジェクト ※heap_t::uniqueOperation ope(*heap); と書くのと同じ
				data_t* obj = ope.pushBegin(priority, val);//この時点でロックが取得される
				//※プッシュ途中の状態。この状態では、単一操作オブジェクトの他の操作が禁止される。
				//※戻り値は、処理ブロック内でしか（pushEnd/pushCancel呼び出しまでしか）有効ではないポインタなので注意
				printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
				//obj = ope.pushEnd();//明示的なプッシュ終了を行うと、正しいオブジェクトの参照を取得できる ※ロックが解除されるので、他のスレッドでポップされる可能性がある事に注意
				//ope.pushCancel();//プッシュ取り消し
			}
			#endif//TEST_USE_PUSH_TYPE
		}
	};
	pushNodesBinHeap();

	//経過時間を表示
	auto printElapsedTime = [](const std::chrono::system_clock::time_point& prev_time, const bool is_show) -> std::chrono::system_clock::time_point
	{
		//最終経過時間表示
		auto now_time = nowTime();
		const double elapsed_time = calcElapsedTime(prev_time, now_time);
		if (is_show)
			printf("*elapsed_time=%.9lf sec\n", elapsed_time);
		return now_time;
	};
	prev_time = printElapsedTime(prev_time, true);

	//木を表示
	showTree(*heap);
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//キューのリストを表示
	auto printTable = [&heap, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- Print Queue List ---\n");
		forEach(*heap, [&heap](const data_t& obj)
			{
				printf_detail(" [%d,%d:%d]", obj.m_priority, obj.m_seqNo, obj.m_val);
			}
		);
		printf_detail("\n");
		prev_time = printElapsedTime(prev_time, false);
	};
	printTable();

#ifdef GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR
	auto printTableReversed = [&heap, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- Print Queue List (reverse) ---\n");
		reverseForEach(*heap, [&heap](const data_t& obj)
			{
				printf_detail(" [%d,%d:%d]", obj.m_priority, obj.m_seqNo, obj.m_val);
			}
		);
		printf_detail("\n");
		prev_time = printElapsedTime(prev_time, false);
	};
	printTableReversed();
#endif//GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR

#if defined(GASHA_BINARY_HEAP_ENABLE_RANDOM_ACCESS_INTERFACE) && defined(GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR)
#ifdef TEST_ITERATOR_OPERATION
	{
		printf("\n");
		printf("--------------------[iterator operattion:begin]\n");
		printf("[constructor]\n");
		heap_t::iterator ite = heap->begin();
		heap_t::reverse_iterator rite = heap->rbegin();
		heap_t::iterator ite_end = heap->end();
		heap_t::reverse_iterator rite_end = heap->rend();
		heap_t::iterator ite2 = heap->rbegin();
		heap_t::reverse_iterator rite2 = heap->begin();
		heap_t::iterator ite2_end = heap->rend();
		heap_t::reverse_iterator rite2_end = heap->end();
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		printf("ite_end - ite = %d\n", ite_end - ite);
		printf("ite - ite_end = %d\n", ite - ite_end);
		printf("rite_end - rite = %d\n", rite_end - rite);
		printf("rite - rite_end = %d\n", rite - rite_end);
		printf("ite2 - ite = %d\n", ite2 - ite);
		printf("ite - ite2 = %d\n", ite - ite2);
		printf("rite2 - rite = %d\n", rite2 - rite);
		printf("rite - rite2 = %d\n", rite - rite2);
		printf("[copy operator]\n");
		ite = heap->begin();
		rite = heap->rbegin();
		ite_end = heap->end();
		rite_end = heap->rend();
		ite2 = heap->rbegin();
		rite2 = heap->begin();
		ite2_end = heap->rend();
		rite2_end = heap->end();
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		printf("[rite.base()]\n");
		ite2 = rite.base();
		ite2_end = rite_end.base();
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		printf("[++ite,--ie_end]\n");
		++ite;
		++rite;
		--ite_end;
		--rite_end;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		printf("[--ite,++ie_end]\n");
		--ite;
		--rite;
		++ite_end;
		++rite_end;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		for (int i = 0; i < 3; ++i)
		{
			printf("[ite[%d]]\n", i);
			ite = ite[i];
			rite = rite[i];
			if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		}
		printf("[ite+=3]\n");
		ite += 3;
		rite += 3;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		printf("[ite-=3]\n");
		ite -= 3;
		rite -= 3;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
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
#endif//TEST_ITERATOR_OPERATION
#endif//GASHA_BINARY_HEAP_ENABLE_RANDOM_ACCESS_INTERFACE, GASHA_BINARY_HEAP_ENABLE_REVERSE_ITERATOR

#ifdef TEST_LOCK_OPERATION
	//ロック操作テスト
	printf("--------------------[lock operation:begin]\n");
	{
		auto lock(heap->lockScoped());//lock_guard<container_t::lock_type> lock(*heap);と同じ
		printf(".lockScoped() ... OK\n");
	}
	{
		auto lock(heap->lockUnique());//unique_shared_lock<container_t::lock_type> lock(*heap);と同じ
		printf(".lockUnique() ... OK\n");
	}
	{
		auto lock(heap->lockUnique(with_lock));//unique_shared_lock<container_t::lock_type> lock(*heap, with_lock);と同じ
		printf(".lockUnique(with_lock) ... OK\n");
	}
	{
		auto lock(heap->lockUnique(try_to_lock));//unique_shared_lock<container_t::lock_type> lock(*heap, try_lock);と同じ
		printf(".lockUnique(try_to_lock) ... OK\n");
	}
	{
		heap_t::lock_type& lock_obj = *heap;
		lock_obj.lock();
		auto lock(heap->lockUnique(adopt_lock));//unique_shared_lock<container_t::lock_type> lock(*heap, adopt_lock);と同じ
		printf(".lockUnique(adopt_lock) ... OK\n");
	}
	{
		auto lock(heap->lockUnique(defer_lock));//unique_shared_lock<container_t::lock_type> lock(*heap, defer_lock);と同じ
		printf(".lockUnique(defer_lock) ... OK\n");
	}
	printf("--------------------[lock operation:end]\n");
#endif//TEST_LOCK_OPERATION

	//二分ヒープでノードをポップ
	auto popNodesBinHeap  = [&heap](const int pop_limit)
	{
		printf("\n");
		printf("--- Pop nodes(Binary Heap) ---\n");
		for (int i = 0; i < pop_limit; ++i)
		{
			//※上記プライオリティキューで説明した、２種類のポップ方法が使える
			//【推奨】【ポップ方法①】情報取得用のオブジェクトを受け渡す
			//※オブジェクトのコピーが発生する。
			//※デストラクタが呼び出される。（コピー後に実行）
			#if TEST_USE_POP_TYPE == 1
			{
				data_t node;
				const bool result = heap->popCopying(node);
				if (!result)
					break;
				printf_detail("[%1d:%2d] ", node.m_priority, node.m_val);
			}
			//【ポップ方法②】キュー（オブジェクト）の参照を受け取る方法
			//※オブジェクトのコピーは発生しない。
			//※単一操作オブジェクトを使用することで、誤った操作を防ぎ（二重ポップやポップ中のプッシュなど）、かつ、
			//　処理ブロックを抜ける時に自動敵にポップを完了する。
			//※明示的にポップを終了または取り消しすることも可能。
			//※ポップを開始してから完了するまでの間、ロックが取得される。
			//※最後にデストラクタが呼び出される。
			#elif TEST_USE_POP_TYPE == 2
			{
				auto ope = heap->operationUnique();//単一操作オブジェクト ※heap_t::uniqueOperation ope(*heap); と書くのと同じ
				data_t* obj = ope.popBegin();//この時点でロックが取得される
				//※ポップ途中の状態。この状態では、単一操作オブジェクトの他の操作が禁止される。
				//※戻り値は、処理ブロック内でしか（popEnd/popCancel呼び出しまでしか）有効ではないポインタなので注意
				if (!obj)
					break;
				printf_detail("[%1d:%2d] ", obj->m_priority, obj->m_val);
				//ope.popEnd();//ポップ終了
				//ope.popCancel();//ポップ取り消し
		}
			#endif//TEST_USE_POP_TYPE
		}
		printf_detail("\n");
	};
	popNodesBinHeap(TEST_DATA_REG_NUM / 2);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(*heap);
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//ノードをポップ
	popNodesBinHeap(TEST_DATA_REG_NUM);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(*heap);
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//インスタンスを破棄
	delete heap;
	heap = nullptr;

	//--------------------
	//【挙動比較用】STLのstd::priority_queueテスト
	//※プライオリティキューと異なり、ポップ時に、プッシュ時（エンキュー時）の順序性が保証されていないことが確認できる
	//※STLと二分ヒープの挙動（ポップ順序）が全く同じことを確認できる
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[Test for std::priority_queue(STL)]\n");
	
	//優先度付きキューコンテナ生成
	typedef std::priority_queue<data_t, std::vector<data_t>, typename heap_ope::less> stl_container_type;
	//typedef std::priority_queue<data_t, std::vector<data_t>, ope> stl_container_type;
	stl_container_type* stl_heap = new stl_container_type();

	//STLでノードをプッシュ
	auto pushNodesSTL = [&stl_heap]()
	{
		printf("\n");
		printf("--- Push nodes(STL) ---\n");
		std::mt19937 rand_engine;
		rand_engine.seed(0);
		std::uniform_int_distribution<int> rand_dist(TEST_DATA_PRIOR_MIN, TEST_DATA_PRIOR_MAX);
		{
			data_t obj(NORMAL, 0);
			stl_heap->push(obj);
			printf_detail("[%d:%2d]\n", obj.m_priority, obj.m_val);
		}
		for (int val = 1; val < TEST_DATA_REG_NUM; ++val)
		{
			const PRIORITY priority = static_cast<PRIORITY>(rand_dist(rand_engine));
			data_t obj(priority, val);
			stl_heap->push(obj);
			printf_detail("[%d:%2d]\n", obj.m_priority, obj.m_val);
		}
	};
	pushNodesSTL();
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//STLでノードをポップ
	auto popNodesSTL = [&stl_heap](const int pop_limit)
	{
		printf("\n");
		printf("--- Pop nodes(STL) ---\n");
		for (int i = 0; i < pop_limit && !stl_heap->empty(); ++i)
		{
			data_t node = stl_heap->top();
			printf_detail("[%1d:%2d] ", node.m_priority, node.m_val);
			stl_heap->pop();
		}
		printf_detail("\n");
	};
	popNodesSTL(TEST_DATA_REG_NUM / 2);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示
	
	popNodesSTL(TEST_DATA_REG_NUM);
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//インスタンスを破棄
	delete stl_heap;
	stl_heap = nullptr;

	//終了
	printf("\n");
	printf("--- end ---\n");
	printElapsedTime(begin_time, true);//経過時間を表示
}

// End of file
