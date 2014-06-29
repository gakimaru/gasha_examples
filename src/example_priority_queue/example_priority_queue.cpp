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

#include <algorithm>//std::for_each()
#include <chrono>//C++11 std::chrono
#include <list>//std::list（比較用）
#include <assert.h>//assert()

#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

GASHA_USING_NAMESPACE;//ネームスペース使用

#if 0

//--------------------------------------------------------------------------------
//プライオリティキューテスト
//--------------------------------------------------------------------------------

#include <memory.h>//memcpy用
#include <random>//C++11 std::random用
#include <algorithm>//std::for_each用
#include <queue>//std::priority_queue用※比較テスト用

//----------------------------------------
//テスト用補助関数
#ifdef PRINT_TEST_DATA_DETAIL
template<typename... Tx>
inline int printf_detail(const char* fmt, Tx... args)
{
	return printf(fmt, args...);
}
#else//PRINT_TEST_DATA_DETAIL
inline int printf_detail(const char* fmt, ...){ return 0; }
#endif//PRINT_TEST_DATA_DETAIL

//----------------------------------------
//テストデータ
enum PRIORITY : short
{
	HIGHEST = 5,
	HIGHER = 4,
	NORMAL = 3,
	LOWER = 2,
	LOWEST = 1,
};
struct data_t
{
	PRIORITY m_priority;//優先度
	unsigned int m_seqNo;//シーケンス番号
	int m_val;//データ
	int m_data[2];

	//コンストラクタ
	data_t(const PRIORITY priority, const int val) :
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
	data_t(const int val) :
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
	data_t& operator=(const data_t&& rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
		printf("data_t::move_operator\n");
		return *this;
	}
	//コピーオペレータ
	data_t& operator=(const data_t& rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
		printf("data_t::copy_operator\n");
		return *this;
	}
	//ムーブコンストラクタ
	data_t(const data_t&& src)
	{
		memcpy(this, &src, sizeof(*this));
		printf("data_t::move_constructor\n");
	}
	//コピーコンストラクタ
	data_t(const data_t& src)
	{
		memcpy(this, &src, sizeof(*this));
		printf("data_t::copy_constructor\n");
	}
#endif//TEST_DATA_WATCH_CONSTRUCTOR
	//デフォルトコンストラクタ
	data_t() :
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
	~data_t()
	{
	#ifdef TEST_DATA_WATCH_CONSTRUCTOR
		printf("data_t::destructor\n");
		//printf("    m_priority=%d, m_seqNo=%d, m_val=%d\n", m_priority, m_seqNo, m_val);
	#endif//TEST_DATA_WATCH_CONSTRUCTOR
	}
};
//----------------------------------------
//テストデータ操作クラス
struct heap_ope_t : public binary_heap::baseOpe<heap_ope_t, data_t>
{
	//キーを比較
	//※lhsの方が小さいければ true を返す
	//※派生クラスでの実装が必要
	inline static bool less(const node_type& lhs, const node_type& rhs)
	{
		return lhs.m_priority < rhs.m_priority;//優先度のみを比較
	}
	
	//ロック型
	//※デフォルト（dummy_lock）のままとする
	//typedef spin_lock lock_type;//ロックオブジェクト型
};
//----------------------------------------
//テストデータ操作クラス
struct ope : public priority_queue::baseOpe<ope, data_t, PRIORITY, int>
{
	//優先度を取得
	inline static priority_type getPriority(const node_type& node){ return node.m_priority; }
	//優先度を更新
	inline static void setPriority(node_type& node, const priority_type priority){ node.m_priority = priority; }
	
	//シーケンス番号を取得
	inline static seq_no_type getSeqNo(const node_type& node){ return node.m_seqNo; }
	//シーケンス番号を更新
	inline static void setSeqNo(node_type& node, const seq_no_type seq_no){ node.m_seqNo = seq_no; }

	//ロック型
	//※デフォルト（dummy_lock）のままとする
	//typedef spin_lock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//木を表示
template<class HEAP>
void showTree(const HEAP& heap)
{
	printf_detail("\n");
	printf_detail("--- Show tree (count=%d) ---\n", heap.size());
	//static const int depth_limit = 5;//最大でも5段階目までを表示（0段階目から数えるので最大で6段階表示される→最大：1+2+4+8+16+32=63個）
	static const int depth_limit = 4;//最大でも4段階目までを表示（0段階目から数えるので最大で5段階表示される→最大：1+2+4+8+16=31個）
	const int _depth_max = heap.depth_max();
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
			const data_t* node = heap.ref_top();
			int breath_tmp = breath;
			for (int depth_tmp = depth - 1; node; --depth_tmp)
			{
				if (depth_tmp < 0)
					break;
				node = heap.ref_child(node, (breath_tmp & (0x1 << depth_tmp)) != 0x0);
			}
			if (node)
			{
				{
					int c = 0;
					for (; c < print_indent / 2; ++c)
						printf_detail(" ");
					if (heap.ref_child_l(node) && c < print_indent)
					{
						printf_detail(".");
						++c;
					}
					for (; c < print_indent; ++c)
						printf_detail(heap.ref_child_l(node) ? "-" : " ");
				}
				printf_detail("%s%1d:%2d%s", heap.ref_child_l(node) ? "{" : "[", node->m_priority, node->m_val, heap.ref_child_r(node) ? "}" : "]");
				{
					int c = 0;
					for (; c < print_indent / 2; ++c)
						printf_detail(heap.ref_child_r(node) ? "-" : " ");
					if (heap.ref_child_r(node) && c < print_indent)
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
//テストメイン
int main(const int argc, const char* argv[])
{
	//プライオリティキューコンテナ生成
	typedef priority_queue::containerAdapter<ope, TEST_DATA_MAX> pqueue_t;
	pqueue_t* con = new pqueue_t();

	//処理時間計測開始
	const std::chrono::system_clock::time_point begin_time = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point prev_time = begin_time;

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
			#define USE_ENQUEUE_TYPE 2
			//【エンキュー方法①】オブジェクトを受け渡す方法
			//※オブジェクトのコピーが発生するので少し遅い。
			#if USE_ENQUEUE_TYPE == 1
			{
				data_t new_obj(priority, val);
				data_t* obj = con->enqueueCopying(new_obj);
				printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
			}
			//【推奨】【エンキュー方法②】コンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーは発生しない。
			//※コンストラクタが呼び出される。
			#elif USE_ENQUEUE_TYPE == 2
			{
				data_t* obj = con->enqueue(priority, val);//優先度とコンストラクタパラメータを渡して登録
				printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
			}
			//【エンキュー方法③】新規キュー（オブジェクト）の参照を受け取って値をセットする方法
			//※オブジェクトのコピーは発生しない。
			//※最初にコンストラクタが呼び出される。
			//※明示的に終了処理を呼び出し、ロックを解放しなければならない点に注意。
			//　（エンキュー／デキュー操作用クラスを使用することで、処理ブロックを抜ける時に自動敵にロックが解放される）
			//※エンキュー終了時にはポインタが変わる点にも注意。
			#elif USE_ENQUEUE_TYPE == 3
			{
				priority_queue::operation_guard<pqueue_t> ope(*con);//エンキュー／デキュー操作用クラス
				data_t* obj = ope.enqueueBegin(priority);//この時点で優先度とシーケンス番号がセットされ、ロックが取得される
				                                         //※戻り値は、処理ブロック内でしか（enqueueEnd/enqueueCancel呼び出しまでしか）有効ではないポインタなので注意
				obj->m_val = val;
				printf_detail("[%d:%2d(seq=%d)]\n", obj->m_priority, obj->m_val, obj->m_seqNo);
				//処理ブロックを抜ける時に自動的にデキューが終了し、ロックが解放される。
				//※受け取ったポインタを処理ブロックの外で参照すると、誤った情報を参照することになるので注意。
				//※明示的にエンキュー終了／取り消しを実行することも可能。
				//obj = ope.enqueueEnd();//明示的なエンキュー終了を行うと、正しいオブジェクトの参照を取得できる
				//ope.enqueueCancel();
			}
			#endif//USE_ENQUEUE_TYPE
		}
	};
	enqueue();

	//経過時間を表示
	auto printElapsedTime = [](const std::chrono::system_clock::time_point& prev_time, const bool is_show) -> std::chrono::system_clock::time_point
	{
		//最終経過時間表示
		const auto now_time = std::chrono::system_clock::now();
		const auto duration = now_time - prev_time;
		const double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000.;
		if (is_show)
			printf("*elapsed_time=%.9llf sec\n", elapsed_time);
		return now_time;
	};
	prev_time = printElapsedTime(prev_time, true);

#if 0
	//範囲に基づくforループテスト
	auto printNodesTest1 = [&con]()
	{
		printf("\n");
		printf("--- Test for C++11 for(:) ---\n");
		typedef pqueue_t::container_type container_t;
		container_t& heap = *con;
		for (const data_t& o : heap)
			printf("[%1d:%2d] ", o.m_priority, o.m_val);
		printf("\n");
	};
	printNodesTest1();
#endif

#if 0
	//std::for_eachテスト
	auto printNodesTest2 = [&con]()
	{
		printf("\n");
		printf("--- Test for std::for_each() ---\n");
		typedef pqueue_t::container_type container_t;
		container_t& heap = *con;
		printf("iterator:         ");
		std::for_each(heap.begin(), heap.end(), [](data_t& o)
			{
				printf("[%1d:%2d] ", o.m_priority, o.m_val);
			}
		);
		printf("\n");
		printf("reverse_iterator: ");
		std::for_each(heap.rbegin(), heap.rend(), [](data_t& o)
			{
				printf("[%1d:%2d] ", o.m_priority, o.m_val);
			}
		);
		printf("\n");
	};
	printNodesTest2();
#endif

#if 0//イテレータとロック取得のテスト
	{
		printf("\n");
		typedef pqueue_t::container_type container_t;
		container_t& heap = *con;
		lock_guard<container_t::lock_type> lock(heap);
		container_t::iterator ite = heap.begin();
		container_t::reverse_iterator rite = heap.rbegin();
		container_t::iterator ite_end = heap.end();
		container_t::reverse_iterator rite_end = heap.rend();
		container_t::iterator ite2 = heap.rbegin();
		container_t::reverse_iterator rite2 = heap.begin();
		container_t::iterator ite2_end = heap.rend();
		container_t::reverse_iterator rite2_end = heap.end();
		printf("constructor\n");
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		printf("copy operator\n");
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
		for (int i = 0; i <= 3; ++i)
		{
			printf("[%d]\n", i);
			ite = ite[i];
			rite = rite[i];
			ite_end = ite_end[i];
			rite_end = rite_end[i];
			ite2 = ite2[i];
			rite2 = rite2[i];
			ite2_end = ite2_end[i];
			rite2_end = rite2_end[i];
			if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
			if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
			if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
			if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		}
		printf("+= 3\n");
		ite += 3;
		rite += 3;
		ite_end += 3;
		rite_end += 3;
		ite2 += 3;
		rite2 += 3;
		ite2_end += 3;
		rite2_end += 3;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
		printf("-= 3\n");
		ite -= 3;
		rite -= 3;
		ite_end -= 3;
		rite_end -= 3;
		ite2 -= 3;
		rite2 -= 3;
		ite2_end -= 3;
		rite2_end -= 3;
		if (ite.isExist()) printf("ite:[%d] priority=%d, seqNo=%d, value=%d\n", ite.getIndex(), ite->m_priority, ite->m_seqNo, ite->m_val);
		if (rite.isExist()) printf("rite:[%d] priority=%d, seqNo=%d, value=%d\n", rite.getIndex(), rite->m_priority, rite->m_seqNo, rite->m_val);
		if (ite_end.isExist()) printf("ite_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite_end.getIndex(), ite_end->m_priority, ite_end->m_seqNo, ite_end->m_val);
		if (rite_end.isExist()) printf("rite_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite_end.getIndex(), rite_end->m_priority, rite_end->m_seqNo, rite_end->m_val);
		if (ite2.isExist()) printf("ite2:[%d] priority=%d, seqNo=%d, value=%d\n", ite2.getIndex(), ite2->m_priority, ite2->m_seqNo, ite2->m_val);
		if (rite2.isExist()) printf("rite2:[%d] priority=%d, seqNo=%d, value=%d\n", rite2.getIndex(), rite2->m_priority, rite2->m_seqNo, rite2->m_val);
		if (ite2_end.isExist()) printf("ite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_priority, ite2_end->m_seqNo, ite2_end->m_val);
		if (rite2_end.isExist()) printf("rite2_end:[%d] priority=%d, seqNo=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_priority, rite2_end->m_seqNo, rite2_end->m_val);
	}
#endif

	//木を表示
	showTree(con->getContainer());
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示

	//デキュー
	auto dequeue = [&con](const int pop_limit)
	{
		printf("\n");
		printf("--- Dequeue ---\n");
		for (int i = 0; i < pop_limit; ++i)
		{
			#define USE_DEQUEUE_TYPE 1
			//【推奨】【デキュー方法①】情報取得用のオブジェクトを受け渡す
			//※オブジェクトのコピーが発生する。
			//※デストラクタが呼び出される。（コピー後に実行）
			#if USE_DEQUEUE_TYPE == 1
			{
				data_t node;
				const bool result = con->dequeueCopying(node);
				if (!result)
					break;
				printf_detail("[%1d:%2d] ", node.m_priority, node.m_val);
			}
			//【デキュー方法②】キュー（オブジェクト）の参照を受け取る方法
			//※オブジェクトのコピーは発生しない。
			//※明示的に終了処理を呼び出し、ロックを解放しなければならない点に注意。
			//　（エンキュー／デキュー操作用クラスを使用することで、処理ブロックを抜ける時に自動敵にロックが解放される）
			//※最後にデストラクタが呼び出される。
			#elif USE_DEQUEUE_TYPE == 2
			{
				priority_queue::operation_guard<pqueue_t> ope(*con);//エンキュー／デキュー操作用クラス
				data_t* obj = ope.dequeueBegin();//この時点でロックが取得される
				                                 //※戻り値は、処理ブロック内でしか（dequeueEnd/dequeueCancel呼び出しまでしか）有効ではないポインタなので注意
				if (!obj)
					break;
				printf_detail("[%1d:%2d] ", obj->m_priority, obj->m_val);
				//処理ブロックを抜ける時に自動的にデキューが終了し、ロックが解放される。
				//※受け取ったポインタを処理ブロックの外で参照すると、誤った情報を参照することになるので注意。
				//※明示的にデキュー終了／取り消しを実行することも可能。
				//ope.dequeueEnd();
				//ope.dequeueCancel();
			}
			#endif//USE_DEQUEUE_TYPE
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
	//【挙動比較用】二分ヒープのテスト
	//※プライオリティキューと異なり、ポップ時に、プッシュ時（エンキュー時）の順序性が保証されていないことが確認できる
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[Test for binary_heap::container(Binary Heap)]\n");

	//ヒープコンテナ生成
	typedef binary_heap::container<heap_ope_t, TEST_DATA_MAX> heap_t;
	heap_t* heap = new heap_t();

	//二分ヒープでノードをプッシュ
	auto pushNodesBinHeap = [&heap]()
	{
		printf("\n");
		printf("--- Push nodes(Binary Heap) ---\n");
		std::mt19937 rand_engine;
		rand_engine.seed(0);
		std::uniform_int_distribution<int> rand_dist(TEST_DATA_PRIOR_MIN, TEST_DATA_PRIOR_MAX);
		{
			data_t* obj =heap->push(NORMAL, 0);
			printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
		}
		for (int val = 1; val < TEST_DATA_REG_NUM; ++val)
		{
			//※上記プライオリティキューで説明した、３種類のプッシュ方法が使える
			const PRIORITY priority = static_cast<PRIORITY>(rand_dist(rand_engine));
			#define USE_PUSH_TYPE 2
			//【プッシュ方法①】
			#if USE_PUSH_TYPE == 1
			{
				data_t new_obj(priority, val);
				data_t* obj = heap->pushCopying(new_obj);
				printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
			}
			//【推奨】【プッシュ方法②】
			#elif USE_PUSH_TYPE == 2
			{
				data_t* obj = heap->push(priority, val);
				printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
			}
			//【プッシュ方法③】
			#elif USE_PUSH_TYPE == 3
			{
				binary_heap::operation_guard<container_type> ope(*heap);
				data_t* obj = ope.pushBegin(priority, val);//※戻り値は、処理ブロック内でしか（pushEnd/pushCancel呼び出しまでしか）有効ではないポインタなので注意
				printf_detail("[%d:%2d]\n", obj->m_priority, obj->m_val);
				//obj = ope.popEnd();//明示的なポップ終了を行うと、正しいオブジェクトの参照を取得できる
			}
			#endif//USE_PUSH_TYPE
		}
	};
	pushNodesBinHeap();
	prev_time = printElapsedTime(prev_time, true);//経過時間を表示

	//木を表示
	showTree(*heap);
	prev_time = printElapsedTime(prev_time, false);//経過時間を表示
	
	//二分ヒープでノードをポップ
	auto popNodesBinHeap  = [&heap](const int pop_limit)
	{
		printf("\n");
		printf("--- Pop nodes(Binary Heap) ---\n");
		for (int i = 0; i < pop_limit; ++i)
		{
			//※上記プライオリティキューで説明した、２種類のポップ方法が使える
			#define USE_POP_TYPE 1
			//【推奨】【ポップ方法①】
			#if USE_POP_TYPE == 1
			{
				data_t node;
				const bool result = heap->popCopying(node);
				if (!result)
					break;
				printf_detail("[%1d:%2d] ", node.m_priority, node.m_val);
			}
			//【ポップ方法②】
			#elif USE_POP_TYPE == 2
			{
				binary_heap::operation_guard<heap_t> ope(*heap);
				data_t* obj = ope.popBegin();//※戻り値は、処理ブロック内でしか（popEnd/popCancel呼び出しまでしか）有効ではないポインタなので注意
				if (!obj)
					break;
				printf_detail("[%1d:%2d] ", obj->m_priority, obj->m_val);
			}
			#endif//USE_POP_TYPE
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
	typedef std::priority_queue<data_t, std::vector<data_t>, heap_ope_t> stl_container_type;
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

	//インスタンスを破棄
	delete con;
	con = nullptr;

	//--------------------
	//ポインタ変数をキューイングする場合のテスト
	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for priority_queue::containerAdapter(Priority Queue)] *Pointer\n");
		printf("\n");

		//操作型
		struct p_ope_t : public priority_queue::baseOpe<p_ope_t, data_t*, PRIORITY, int>
		{
			inline static priority_type getPriority(const node_type& node){ return node->m_priority; }
			inline static void setPriority(node_type& node, const priority_type priority){ node->m_priority = priority; }
			inline static seq_no_type getSeqNo(const node_type& node){ return node->m_seqNo; }
			inline static void setSeqNo(node_type& node, const seq_no_type seq_no){ node->m_seqNo = seq_no; }

			//ロック型
			//※デフォルト（dummy_lock）のままとする
			//typedef spin_lock lock_type;//ロックオブジェクト型
		};
		
		//プライオリティキュー
		priority_queue::containerAdapter<p_ope_t, 100> p_con;
		
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

	//終了
	printf("\n");
	printf("--- end ---\n");
	printElapsedTime(begin_time, true);//経過時間を表示

	return EXIT_SUCCESS;
}

#endif

//----------------------------------------
//優先度付きキュー／二分ヒープテスト
void example_priority_queue()
{
}

// End of file
