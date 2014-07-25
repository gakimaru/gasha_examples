#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_EXAMPLE_EXAMPLE_PRIORITY_QEUEUE_H
#define GASHA_INCLUDED_EXAMPLE_EXAMPLE_EXAMPLE_PRIORITY_QEUEUE_H

//--------------------------------------------------------------------------------
// exmaple_priority_queue.h
// 優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/binary_heap.h>//二分ヒープコンテナ【宣言部】
#include <gasha/priority_queue.h>//優先度付きキューコンテナアダプタ【宣言部】

#include <gasha/spin_lock.h>//スピンロック

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//二分ヒープテスト用設定とコンパイラスイッチ

#ifdef GASHA_OPTIMIZED

static const int TEST_DATA_PRIOR_MIN = 1;//テストデータの最小プライオリティ
static const int TEST_DATA_PRIOR_MAX = 5;//テストデータの最大プライオリティ
static const int TEST_DATA_MAX = 5000000;//テストデータの最大登録可能数
static const int TEST_DATA_REG_NUM = 5000000;//テストデータの登録数

//#define TEST_ITERATOR_OPERATION//イテレータ操作をテストする場合は、このマクロを有効にする
//#define TEST_LOCK_OPERATION//ロック操作をテストする場合は、このマクロを有効にする

//#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#else//GASHA_OPTIMIZED

static const int TEST_DATA_PRIOR_MIN = 1;//テストデータの最小プライオリティ
static const int TEST_DATA_PRIOR_MAX = 5;//テストデータの最大プライオリティ
static const int TEST_DATA_MAX = 128;//テストデータの最大登録可能数
static const int TEST_DATA_REG_NUM = 20;//テストデータの登録数

#define TEST_ITERATOR_OPERATION//イテレータ操作をテストする場合は、このマクロを有効にする
#define TEST_LOCK_OPERATION//ロック操作をテストする場合は、このマクロを有効にする

#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタ／代入演算子の動作を確認する場合、このマクロを有効化する

#endif//GASHA_OPTIMIZED

static const int TEST_DATA_TABLE_SIZE_FOR_POINTER = 100;//ポインター型テストデータテーブルサイズ

#define TEST_USE_ENQUEUE_TYPE 2//優先度付きキューのテストで使用するエンキュー方法
                               //1 ... オブジェクトを受け渡す方法
                               //2 ... 【推奨】コンストラクタパラメータを渡して登録する方法
                               //3 ... 新規キュー（オブジェクト）の参照を受け取って値をセットする方法

#define TEST_USE_DEQUEUE_TYPE 1//優先度付きキューのテストで使用するデキュー方法
                               //1 ... 【推奨】情報取得用のオブジェクトを受け渡す
                               //2 ... キュー（オブジェクト）の参照を受け取る方法

#define TEST_USE_PUSH_TYPE 2//二分ヒープのテストで使用するプッシュ方法
                            //1 ... オブジェクトを受け渡す方法
                            //2 ... 【推奨】コンストラクタパラメータを渡して登録する方法
                            //3 ... 新規キュー（オブジェクト）の参照を受け取って値をセットする方法

#define TEST_USE_POP_TYPE 1//二分ヒープのテストで使用するデキュー方法
                           //1 ... 【推奨】情報取得用のオブジェクトを受け渡す
                           //2 ... キュー（オブジェクト）の参照を受け取る方法

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
	data_t(const PRIORITY priority, const int val);
	data_t(const int val);
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	//ムーブオペレータ
	data_t& operator=(data_t&& rhs);
	//コピーオペレータ
	data_t& operator=(const data_t& rhs);
	//ムーブコンストラクタ
	data_t(data_t&& src);
	//コピーコンストラクタ
	data_t(const data_t& src);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
	//デフォルトコンストラクタ
	data_t();
	//デストラクタ
	~data_t();
};
//----------------------------------------
//テストデータ操作クラス：優先度付きキュー用
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

#ifdef TEST_LOCK_OPERATION
	//ロック型
	//※デフォルトは dummyLock
	typedef spinLock lock_type;//ロックオブジェクト型
#endif//TEST_LOCK_OPERATION
};
//----------------------------------------
//テストデータ操作クラス：優先度付きキュー用（ポインター操作用）
struct ptr_ope : public priority_queue::baseOpe<ptr_ope, data_t*, PRIORITY, int>
{
	inline static priority_type getPriority(const node_type& node){ return node->m_priority; }
	inline static void setPriority(node_type& node, const priority_type priority){ node->m_priority = priority; }
	inline static seq_no_type getSeqNo(const node_type& node){ return node->m_seqNo; }
	inline static void setSeqNo(node_type& node, const seq_no_type seq_no){ node->m_seqNo = seq_no; }

	//ロック型
	//※デフォルト（dummy_lock）のままとする
	//typedef spin_lock lock_type;//ロックオブジェクト型
};
//----------------------------------------
//テストデータ操作クラス：二分ヒープ用
struct heap_ope : public binary_heap::baseOpe<heap_ope, data_t>
{
	//キーを比較
	//※lhsの方が小さいければ true を返す
	//※派生クラスでの実装が必要
	struct less
	{
		bool operator()(const node_type& lhs, const node_type& rhs)
		{
			return lhs.m_priority < rhs.m_priority;//優先度のみを比較
		}
	};

#ifdef TEST_LOCK_OPERATION
	//ロック型
	//※デフォルトは dummyLock
	typedef spinLock lock_type;//ロックオブジェクト型
#endif//TEST_LOCK_OPERATION
};

//----------------------------------------
//優先度付きキューコンテナアダプタテスト
void example_priority_queue();

//----------------------------------------
//二分ヒープコンテナテスト
void example_binary_heap();

//----------------------------------------
//シンプル二優先度付きキューコンテナアダプタテスト
void example_simple_priority_queue();

//----------------------------------------
//シンプル二分ヒープコンテナテスト
void example_simple_binary_heap();

#endif//GASHA_INCLUDED_EXAMPLE_EXAMPLE_EXAMPLE_PRIORITY_QEUEUE_H

// End of file
