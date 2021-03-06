﻿//--------------------------------------------------------------------------------
// exmaple_hash_table.cpp
// 開番地法ハッシュテーブルコンテナテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_hash_table.h"//開番地法ハッシュテーブルコンテナテスト

#include <gasha/hash_table.inl>//開番地法ハッシュテーブルコンテナ【インライン関数／テンプレート関数定義部】

#include <gasha/iterator.h>//イテレータ操作
#include <gasha/chrono.h>//時間処理ユーティリティ：nowTime(), calcElapsedTime()
#include <gasha/fast_string.h>//高速文字列処理：strncpy_fast()
#include <gasha/string.h>//文字列処理：spprintf()

#include <utility>//C++11 std::forward
#include <cstring>//std::memcpy()
#include <cstdio>//std::printf()

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <algorithm>//std::for_each()
#include <unordered_map>//C++11 std::unordered_map（比較用）
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//ハッシュテーブルテスト
//--------------------------------------------------------------------------------

//コンストラクタ
data_t::data_t(const char* name, const int value) :
	m_key(calcCRC32(name)),
	m_value(value)
{
	strncpy_fast(m_name, name, sizeof(m_name)-1);
	m_name[sizeof(m_name)-1] = '\0';
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	std::printf("data_t::constructor(\"%s\", value)\n", name, value);
	//std::printf("    m_key=%d, m_name=[%s], m_value=%d\n", m_key, m_name, m_value);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
//ムーブオペレータ
data_t& data_t::operator=(data_t&& rhs)
{
	std::memcpy(this, &rhs, sizeof(*this));
	std::printf("data_t::move_operator\n");
	return *this;
}
//コピーオペレータ
data_t& data_t::operator=(const data_t& rhs)
{
	std::memcpy(this, &rhs, sizeof(*this));
	std::printf("data_t::copy_operator\n");
	return *this;
}
//ムーブコンストラクタ
data_t::data_t(data_t&& src)
{
	std::memcpy(this, &src, sizeof(*this));
	std::printf("data_t::move_constructor\n");
}
//コピーコンストラクタ
data_t::data_t(const data_t& src)
{
	std::memcpy(this, &src, sizeof(*this));
	std::printf("data_t::copy_constructor\n");
}
//デフォルトコンストラクタ
data_t::data_t() :
	m_key(0),
	m_value(0)
{
	m_name[0] = '\0';
	std::printf("data_t::constructor\n");
}
//デストラクタ
data_t::~data_t()
{
	std::printf("data_t::destructor\n");
	//std::printf("  m_key=%d, m_name=[%s]\n", m_key, m_name);
}
#endif//TEST_DATA_WATCH_CONSTRUCTOR

//----------------------------------------
//テスト用補助関数
#ifdef PRINT_TEST_DATA_DETAIL
template<typename... Tx>
inline int printf_detail(const char* fmt, Tx&&... args)
{
	return std::printf(fmt, std::forward<Tx>(args)...);
}
#else//PRINT_TEST_DATA_DETAIL
inline int printf_detail(const char* fmt, ...){ return 0; }
#endif//PRINT_TEST_DATA_DETAIL

//----------------------------------------
//開番地法ハッシュテーブルコンテナテスト
void example_hash_table()
{
	//時間計測
	auto begin_time = nowTime();
	auto prev_time = begin_time;

	//処理時間表示
	auto printElapsedTimeDirect = [](const double elapsed_time, const bool is_preint) -> std::chrono::system_clock::time_point
	{
		if (is_preint)
			std::printf("*elapsed time=%.9lf sec.\n", elapsed_time);
		return nowTime();
	};
	auto printElapsedTime = [&printElapsedTimeDirect](const std::chrono::system_clock::time_point& prev_time, const bool is_print) -> std::chrono::system_clock::time_point
	{
		const auto elapsed_time = calcElapsedTime(prev_time);
		return printElapsedTimeDirect(elapsed_time, is_print);
	};

#if 0
	//--------------------
	//ハッシュテーブルのインデックス計算ロジックテスト
	//※全てのインデックスが、正確にテーブルサイズ（素数）回の再計算を行う事で、元のインデックスに戻ることを確認。
	{
		int ng_count = 0;
		const ope::key_type key_min = 0;// con.getKeyMin();
		const ope::key_type key_max = 100000;// con.getKeyMax();
		const ope::key_type key_step = 1;
		for (ope::key_type key = key_min; key <= key_max; key += key_step)
		{
			if (key % (1024 * 1024) == 0)
				std::printf("pass ... Key:%d\n", key);
			std::size_t count = 0;
			std::size_t first_index = con.calcIndex(key);
			std::size_t index = first_index;
			do
			{
				index = con.calcNextIndex(key, index);
				++count;
			} while (index != first_index && count < con.getTableSize());
			if (count != con.getTableSize())
			{
				std::printf("%llu is OUT! (count=%d)\n", static_cast<unsigned long long>(key), static_cast<int>(count));
				++ng_count;
			}
		}
		std::printf("Chek Hash Table: NG=%d/%d\n", static_cast<int>(ng_count), static_cast<int>(con.getTableSize()));
		for (std::size_t key = 10; key <= 30; key += 1)
		{
			std::printf("Key:%llu -> Index:%llu\n", static_cast<unsigned long long>(key), static_cast<unsigned long long>(con.calcIndex(key)));
		}
	}
#endif
	
	//--------------------
	//ハッシュテーブルテスト
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("Hash Table Test\n");
	std::printf("--------------------------------------------------------------------------------\n");
	typedef hash_table::container<ope> container_t;
	container_t* con = new container_t();

	//ハッシュテーブルの基本情報表示
	auto printTableParameter = [&con]()
	{
		std::printf("\n");
		std::printf("--- Table Parameter ---\n");
		std::printf(".max_size()=%d\n", static_cast<int>(con->max_size()));
		//std::printf(".capacity()=%d\n", static_cast<int>(con->capacity()));
		std::printf(".getOriginalTableSize()=%d\n", static_cast<int>(con->getOriginalTableSize()));
		std::printf(".getTableSize()=%d\n", static_cast<int>(con->getTableSize()));
		std::printf(".getTableSizeExtended()=%d\n", static_cast<int>(con->getTableSizeExtended()));
		std::printf(".getAutoRehashRatio()=%d\n", static_cast<int>(con->getAutoRehashRatio()));
		std::printf(".getAutoRehashSize()=%d\n", static_cast<int>(con->getAutoRehashSize()));
		std::printf(".getFindingCycleLimit()=%d\n", con->getFindingCycleLimit());
		std::printf(".getKeyMin()=%llu\n", static_cast<unsigned long long>(con->getKeyMin()));
		std::printf(".getKeyMax()=%llu\n", static_cast<unsigned long long>(con->getKeyMax()));
		std::printf(".getKeyRange()=%llu\n", static_cast<unsigned long long>(con->getKeyRange()));
	};
	printTableParameter();

	//テーブル状態表示
	auto printTableStatus = [&con]()
	{
		std::printf("\n");
		std::printf("--- Table Status ---\n");
		std::printf(".bucket_count()=%d\n", static_cast<int>(con->bucket_count()));
		std::printf(".max_bucket_count()=%d\n", static_cast<int>(con->max_bucket_count()));
		std::printf(".size()=%d\n", static_cast<int>(con->size()));
		std::printf(".empty()=%d\n", con->empty());
		std::printf(".getUsingCount()=%d\n", con->getUsingCount());
		std::printf(".getDeletedCount()=%d\n", con->getDeletedCount());
		std::printf(".getMaxFindingCycle()=%d\n", con->getMaxFindingCycle());
		std::printf(".getNotOptimizedCount()=%d\n", con->getNotOptimizedCount());
	};
	printTableStatus();

	//ハッシュテーブルへのデータ登録
	auto insertData = [&con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		std::printf("\n");
		std::printf("--- Insert Data ---\n");
		int insert_success = 0;
		int insert_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			GASHA_ spprintf(name, "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			data_t* obj = nullptr;
			//【推奨】【登録方法①】emplace()メソッドにキーとコンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーは発生しない
			#if TEST_USE_INSERT_TYPE == 1
			{
				obj = con->emplace(calcCRC32(name), name, i);//キーとコンストラクタパラメータを渡して登録
			}
			//【登録方法②】emplaceAuto()メソッドにコンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーが発生するので少し遅い
			#elif TEST_USE_INSERT_TYPE == 2
			{
				obj = con->emplaceAuto(name, i);//コンストラクタパラメータを渡して登録
			}
			//【登録方法③】insert()メソッドにキーとオブジェクトを渡して登録する方法
			//※オブジェクトのコピーが発生するので少し遅い
			//※操作用クラス baseOpe の派生クラスで、getKey() を実装する必要あり
			#elif TEST_USE_INSERT_TYPE == 3
			{
				data_t new_obj(name, i);
				obj = con->insert(calcCRC32(name), new_obj);
			}
			//【登録方法④】insertAuto()メソッドにオブジェクトを渡して登録する方法
			//※オブジェクトのコピーが発生するので少し遅い
			//※操作用クラス baseOpe の派生クラスで、getKey() を実装する必要あり
			#elif TEST_USE_INSERT_TYPE == 4
			{
				data_t new_obj(name, i);
				obj = con->insertAuto(new_obj);
			}
			//【登録方法⑤】assign()メソッドで先にキーの割り付けを行った後、データを書き込む
			//※コンストラクタのないオブジェクトを扱う場合に有効
			#elif TEST_USE_INSERT_TYPE == 5
			{
				const crc32_t key = calcCRC32(name);
				obj = con->assign(key);
				if(obj)
				{
					obj->m_key = key;
					strncpy_s(obj->m_name, sizeof(obj->m_name), name, sizeof(obj->m_name) - 1);
					obj->m_name[sizeof(obj->m_name) - 1] = '\0';
					obj->m_value = i;
				}
			}
			#endif//TEST_USE_INSERT_TYPE
			if (obj)
			{
				++insert_success;
				printf_detail("OK\n");
			}
			else
			{
				++insert_failure;
				printf_detail("NG\n");
			}
		}
		std::printf("success=%d, failure=%d\n", insert_success, insert_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	insertData(0, TEST_DATA_TABLE_SIZE / 2, 1);

	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	auto printTable = [&con, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- Print Table ---\n");
		iteratorForEach(*con, [&con](const container_t::iterator& ite)
			{
				printf_detail("%c[%6d](%6d) key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)%s\n", ite.isPrimaryIndex() ? ' ' : '*', ite.getIndex(), ite.getPrimaryIndex(), ite.getKey(), ite->m_name, ite->m_value, con->bucket(ite.getKey()), con->bucket_size(ite.getIndex()), ite.isDeleted() ? " <DELETED>" : "");
			}
		);
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	printTable();
	
#ifdef GASHA_HASH_TABLE_ENABLE_REVERSE_ITERATOR
	auto printTableReversed = [&con, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- Print Table (reverse) ---\n");
		iteratorReverseForEach(*con, [&con](const container_t::reverse_iterator& ite)
			{
				printf_detail("%c[%6d](%6d) key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)%s\n", ite.isPrimaryIndex() ? ' ' : '*', ite.getIndex(), ite.getPrimaryIndex(), ite.getKey(), ite->m_name, ite->m_value, con->bucket(ite.getKey()), con->bucket_size(ite.getIndex()), ite.isDeleted() ? " <DELETED>" : "");
			}
		);
	};
	printTableReversed();
#endif//GASHA_HASH_TABLE_ENABLE_REVERSE_ITERATOR

#if defined(GASHA_HASH_TABLE_ENABLE_RANDOM_ACCESS_INTERFACE) && defined(GASHA_HASH_TABLE_ENABLE_REVERSE_ITERATOR)
#ifdef TEST_ITERATOR_OPERATION
	{
		std::printf("\n");
		std::printf("--------------------[iterator operattion:begin]\n");
		std::printf("[constructor]\n");
		container_t::iterator ite = con->begin();
		container_t::reverse_iterator rite = con->rbegin();
		container_t::iterator ite_end = con->end();
		container_t::reverse_iterator rite_end = con->rend();
		container_t::iterator ite2 = con->rbegin();
		container_t::reverse_iterator rite2 = con->begin();
		container_t::iterator ite2_end = con->rend();
		container_t::reverse_iterator rite2_end = con->end();
		if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) std::printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', static_cast<int>(ite_end.getIndex()), static_cast<int>(ite_end.getPrimaryIndex()), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) std::printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', static_cast<int>(rite_end.getIndex()), static_cast<int>(rite_end.getPrimaryIndex()), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		if (ite2.isExist()) std::printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', static_cast<int>(ite2.getIndex()), static_cast<int>(ite2.getPrimaryIndex()), ite2->m_key, ite2->m_name, ite2->m_value);
		if (rite2.isExist()) std::printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', static_cast<int>(rite2.getIndex()), static_cast<int>(rite2.getPrimaryIndex()), rite2->m_key, rite2->m_name, rite2->m_value);
		if (ite2_end.isExist()) std::printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', static_cast<int>(ite2_end.getIndex()), static_cast<int>(ite2_end.getPrimaryIndex()), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		if (rite2_end.isExist()) std::printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', static_cast<int>(rite2_end.getIndex()), static_cast<int>(rite2_end.getPrimaryIndex()), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
		std::printf("ite_end - ite = %d\n", ite_end - ite);
		std::printf("ite - ite_end = %d\n", ite - ite_end);
		std::printf("rite_end - rite = %d\n", rite_end - rite);
		std::printf("rite - rite_end = %d\n", rite - rite_end);
		std::printf("ite2 - ite = %d\n", ite2 - ite);
		std::printf("ite - ite2 = %d\n", ite - ite2);
		std::printf("rite2 - rite = %d\n", rite2 - rite);
		std::printf("rite - rite2 = %d\n", rite - rite2);
		std::printf("[copy operator]\n");
		ite = con->begin();
		rite = con->rbegin();
		ite_end = con->end();
		rite_end = con->rend();
		ite2 = con->rbegin();
		rite2 = con->begin();
		ite2_end = con->rend();
		rite2_end = con->end();
		if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) std::printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', static_cast<int>(ite_end.getIndex()), static_cast<int>(ite_end.getPrimaryIndex()), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) std::printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', static_cast<int>(rite_end.getIndex()), static_cast<int>(rite_end.getPrimaryIndex()), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		if (ite2.isExist()) std::printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', static_cast<int>(ite2.getIndex()), static_cast<int>(ite2.getPrimaryIndex()), ite2->m_key, ite2->m_name, ite2->m_value);
		if (rite2.isExist()) std::printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', static_cast<int>(rite2.getIndex()), static_cast<int>(rite2.getPrimaryIndex()), rite2->m_key, rite2->m_name, rite2->m_value);
		if (ite2_end.isExist()) std::printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', static_cast<int>(ite2_end.getIndex()), static_cast<int>(ite2_end.getPrimaryIndex()), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		if (rite2_end.isExist()) std::printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', static_cast<int>(rite2_end.getIndex()), static_cast<int>(rite2_end.getPrimaryIndex()), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
		std::printf("[rite.base()]\n");
		ite2 = rite.base();
		ite2_end = rite_end.base();
		if (ite2.isExist()) std::printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', static_cast<int>(ite2.getIndex()), static_cast<int>(ite2.getPrimaryIndex()), ite2->m_key, ite2->m_name, ite2->m_value);
		if (ite2_end.isExist()) std::printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', static_cast<int>(ite2_end.getIndex()), static_cast<int>(ite2_end.getPrimaryIndex()), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		std::printf("[++ite,--ie_end]\n");
		++ite;
		++rite;
		--ite_end;
		--rite_end;
		if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) std::printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', static_cast<int>(ite_end.getIndex()), static_cast<int>(ite_end.getPrimaryIndex()), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) std::printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', static_cast<int>(rite_end.getIndex()), static_cast<int>(rite_end.getPrimaryIndex()), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		std::printf("[--ite,++ie_end]\n");
		--ite;
		--rite;
		++ite_end;
		++rite_end;
		if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) std::printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', static_cast<int>(ite_end.getIndex()), static_cast<int>(ite_end.getPrimaryIndex()), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) std::printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', static_cast<int>(rite_end.getIndex()), static_cast<int>(rite_end.getPrimaryIndex()), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		for (int i = 0; i < 3; ++i)
		{
			std::printf("[ite[%d]]\n", i);
			ite = ite[i];
			rite = rite[i];
			if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
			if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		}
		std::printf("[ite+=3]\n");
		ite += 3;
		rite += 3;
		if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		std::printf("[ite-=3]\n");
		ite -= 3;
		rite -= 3;
		if (ite.isExist()) std::printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', static_cast<int>(ite.getIndex()), static_cast<int>(ite.getPrimaryIndex()), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) std::printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', static_cast<int>(rite.getIndex()), static_cast<int>(rite.getPrimaryIndex()), rite->m_key, rite->m_name, rite->m_value);
		std::printf("ite_end - ite = %d\n", ite_end - ite);
		std::printf("ite - ite_end = %d\n", ite - ite_end);
		std::printf("rite_end - rite = %d\n", rite_end - rite);
		std::printf("rite - rite_end = %d\n", rite - rite_end);
		std::printf("[ite2-=2]\n");
		ite2 -= 2;
		rite2 -= 2;
		std::printf("ite2 - ite = %d\n", ite2 - ite);
		std::printf("ite - ite2 = %d\n", ite - ite2);
		std::printf("rite2 - rite = %d\n", rite2 - rite);
		std::printf("rite - rite2 = %d\n", rite - rite2);
		std::printf("[++ite_end]\n");
		++ite_end;
		++rite_end;
		std::printf("ite_end - ite = %d\n", ite_end - ite);
		std::printf("ite - ite_end = %d\n", ite - ite_end);
		std::printf("rite_end - rite = %d\n", rite_end - rite);
		std::printf("rite - rite_end = %d\n", rite - rite_end);
		std::printf("--------------------[iterator operattion:end]\n");
	}
#endif//TEST_ITERATOR_OPERATION
#endif//GASHA_HASH_TABLE_ENABLE_RANDOM_ACCESS_INTERFACE, GASHA_HASH_TABLE_ENABLE_REVERSE_ITERATOR

#ifdef TEST_LOCK_OPERATION
	//ロック操作テスト
	std::printf("--------------------[lock operation:begin]\n");
	{
		auto lock(con->lockScoped());//lock_guard<container_t::lock_type> lock(*con);と同じ
		std::printf(".lockScoped() ... OK\n");
	}
	{
		auto lock(con->lockSharedScoped());//shared_lock_guard<container_t::lock_type> lock(*con);と同じ
		std::printf(".lockSharedScoped() ... OK\n");
	}
	{
		auto lock(con->lockUnique());//unique_shared_lock<container_t::lock_type> lock(*con);と同じ
		std::printf(".lockUnique() ... OK\n");
	}
	{
		auto lock(con->lockUnique(with_lock));//unique_shared_lock<container_t::lock_type> lock(*con, with_lock);と同じ
		std::printf(".lockUnique(with_lock) ... OK\n");
	}
	{
		auto lock(con->lockUnique(with_lock_shared));//unique_shared_lock<container_t::lock_type> lock(*con, with_lock_shared);と同じ
		std::printf(".lockUnique(with_lock_shared) ... OK\n");
	}
	{
		auto lock(con->lockUnique(try_to_lock));//unique_shared_lock<container_t::lock_type> lock(*con, try_to_lock);と同じ
		std::printf(".lockUnique(try_to_lock) ... OK\n");
	}
	{
		auto lock(con->lockUnique(try_to_lock_shared));//unique_shared_lock<container_t::lock_type> lock(*con, try_to_lock_shared);と同じ
		std::printf(".lockUnique(try_to_lock_shared) ... OK\n");
	}
	{
		container_t::lock_type& lock_obj = *con;
		lock_obj.lock();
		auto lock(con->lockUnique(adopt_lock));//unique_shared_lock<container_t::lock_type> lock(*con, adopt_lock);と同じ
		std::printf(".lockUnique(adopt_lock) ... OK\n");
	}
	{
		container_t::lock_type& lock_obj = *con;
		lock_obj.lock_shared();
		auto lock(con->lockUnique(adopt_shared_lock));//unique_shared_lock<container_t::lock_type> lock(*con, adopt_shared_lock);と同じ
		std::printf(".lockUnique(adopt_shared_lock) ... OK\n");
	}
	{
		auto lock(con->lockUnique(defer_lock));//unique_shared_lock<container_t::lock_type> lock(*con, defer_lock);と同じ
		std::printf(".lockUnique(defer_lock) ... OK\n");
	}
	std::printf("--------------------[lock operation:end]\n");
#endif//TEST_LOCK_OPERATION

	//ハッシュテーブルアクセス
	auto findTable = [&con, &printElapsedTime, &prev_time]()
	{
		std::printf("\n");
		std::printf("--- Find Data ---\n");
		int find_success = 0;
		int find_failure = 0;
		for (int i = 0; i < TEST_DATA_TABLE_SIZE; ++i)
		{
			char name[20];
			GASHA_ spprintf(name, "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			data_t* obj;
			crc32_t key;
			std::size_t index;
			std::size_t primary_index;
			bool is_primary_index;
			bool is_deleted;
			//【推奨】【検索方法①】[]オペレータにキーを渡して検索する方法
			//※値を返す
			//※中身はfindValue()
			#if TEST_USE_FIND_TYPE == 1
			{
				obj = (*con)[name];
				if(obj)
				{
					key = obj->m_key;
					index = con->bucket(key);
					primary_index = con->calcIndex(key);
					is_primary_index = (index == primary_index);
					is_deleted = false;
				}
			}
			//【検索方法②】at()メソッドにキーを渡して検索する方法
			//※値を返す
			//※中身はfindValue() ※[]オペレータと同じ
			#elif TEST_USE_FIND_TYPE == 2
			{
				obj = con->at(name);
				if(obj)
				{
					key = obj->m_key;
					index = con->bucket(key);
					primary_index = con->calcIndex(key);
					is_primary_index = (index == primary_index);
					is_deleted = false;
				}
			}
			//【検索方法③】find()メソッドにキーを渡して検索する方法
			//※イテレータを返す
			#elif TEST_USE_FIND_TYPE == 3
			{
				container_t::iterator ite = con->find(name);
				obj = ite;
				if (obj)
				{
					key = ite.getKey();
					index = ite.getIndex();
					primary_index = ite.getPrimaryIndex();
					is_primary_index = ite.isPrimaryIndex();
					is_deleted = ite.isDeleted();
				}
			}
			#endif//TEST_USE_FIND_TYPE
			if (obj)
			{
				printf_detail("OK  %c[%6d](%6d) key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)%s\n", is_primary_index ? ' ' : '*', index, primary_index, key, obj->m_name, obj->m_value, con->bucket(key), con->bucket_size(index), is_deleted ? " <DELETED>" : "");
				++find_success;
			}
			else
			{
				printf_detail("NG\n");
				++find_failure;
			}
		}
		std::printf("success=%d, failure=%d\n", find_success, find_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	findTable();

	//ハッシュテーブルからのデータ削除
	auto eraseData = [&con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		std::printf("\n");
		std::printf("--- Erase Data ---\n");
		int erase_success = 0;
		int erase_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			GASHA_ spprintf(name, "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			bool result = false;
			#define USE_ERASE_TYPE 1
			//【推奨】【削除方法①】erase()メソッドにキーを渡して削除する方法
			#if USE_ERASE_TYPE == 1
			{
				result = con->erase(calcCRC32(name));//キーを渡して削除
			}
			//【削除方法②】eraseAuto()メソッドにオブジェクトを渡して削除する方法
			//※操作用クラス baseOpe の派生クラスで、getKey() を実装する必要あり
			#elif TEST_USE_INSERT_TYPE == 2
			{
				data_t obj(name, i);
				result = con->eraseAuto(obj);//オブジェクトを渡して削除
			}
			#endif//USE_ERASETYPE
			if (result)
			{
				++erase_success;
				printf_detail("OK\n");
			}
			else
			{
				++erase_failure;
				printf_detail("NG\n");
			}
		}
		std::printf("success=%d, failure=%d\n", erase_success, erase_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	eraseData(0, TEST_DATA_TABLE_SIZE, 2);
	
	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	printTable();

	//ハッシュテーブルアクセス
	findTable();

	//ハッシュテーブルへのデータ登録
	insertData(1, TEST_DATA_TABLE_SIZE, 2);

	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	printTable();

	//ハッシュテーブルアクセス
	findTable();

	//リハッシュ
	auto rehashTable = [&con, &printElapsedTime, &prev_time]()
	{
		std::printf("\n");
		std::printf("--- Rehash Table ---\n");
		con->rehash();
		std::printf("OK.\n");
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	rehashTable();

	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	printTable();

	//ハッシュテーブルアクセス
	findTable();

	//クリア
	auto clearTable = [&con, &printElapsedTime, &prev_time]()
	{
		std::printf("\n");
		std::printf("--- Clear Table ---\n");
		con->clear();
		std::printf("OK.\n");
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	clearTable();

	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	printTable();

	//ハッシュテーブルアクセス
	findTable();

	//ハッシュテーブルへのデータ登録
	insertData(0, TEST_DATA_TABLE_SIZE, 1);

	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	printTable();

	//ハッシュテーブルアクセス
	findTable();

	//ハッシュテーブルからのデータ削除
	eraseData(0, TEST_DATA_TABLE_SIZE, 1);

	//ハッシュテーブル状態表示
	printTableStatus();

	//ハッシュテーブル表示
	printTable();

	//ハッシュテーブルアクセス
	findTable();
	
	//--------------------
	//ハッシュテーブル削除
	delete con;
	con = nullptr;

	std::printf("\n");
	std::printf("End\n");
	std::printf("============================================================\n");
	begin_time = printElapsedTime(begin_time, true);//処理時間表示

	//----------------------------------------------------------------------------------------------------
	//【比較用】【STL版】ハッシュテーブルテスト
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[STL] Hash Table Test\n");
	std::printf("--------------------------------------------------------------------------------\n");
	typedef std::unordered_map<crc32_t, data_t> stl_container_t;
	stl_container_t* stl_con = new stl_container_t();
	stl_con->reserve(TEST_DATA_TABLE_SIZE);

	//ハッシュテーブルの基本情報表示
	auto printSTLTableParameter = [&stl_con]()
	{
		std::printf("\n");
		std::printf("--- [STL] Table Parameter ---\n");
		std::printf(".max_size()=%u\n", static_cast<int>(stl_con->max_size()));
	};
	printSTLTableParameter();

	//テーブル状態表示
	auto printSTLTableStatus = [&stl_con]()
	{
		std::printf("\n");
		std::printf("--- [STL] Table Status ---\n");
		std::printf(".size()=%d\n", static_cast<int>(stl_con->size()));
		std::printf(".empty()=%d\n", static_cast<int>(stl_con->empty()));
		std::printf(".bucket_count()=%d\n", static_cast<int>(stl_con->bucket_count()));
		std::printf(".max_bucket_count()=%d\n", static_cast<int>(stl_con->max_bucket_count()));
		std::printf(".load_factor()=%.3f\n", stl_con->load_factor());
		std::printf(".max_load_factor()=%.3f\n", stl_con->max_load_factor());
	};
	printSTLTableStatus();

	//ハッシュテーブルへのデータ登録
	auto insertSTLData = [&stl_con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		std::printf("\n");
		std::printf("--- [STL] Insert Data ---\n");
		int insert_success = 0;
		int insert_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			GASHA_ spprintf(name, "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			data_t obj(name, i);
			auto ite = stl_con->emplace(calcCRC32(name), obj);//キーとコンストラクタパラメータを渡して登録
			if (ite.second)
			{
				++insert_success;
				printf_detail("OK\n");
			}
			else
			{
				++insert_failure;
				printf_detail("NG\n");
			}
		}
		std::printf("success=%d, failure=%d\n", insert_success, insert_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	insertSTLData(0, TEST_DATA_TABLE_SIZE / 2, 1);

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	auto printSTLTable = [&stl_con, &printElapsedTime, &prev_time]()
	{
		printf_detail("\n");
		printf_detail("--- [STL] Print Table ---\n");
		//for (container_t::set& set : +con)
		for (auto& pair : *stl_con)
		{
			const crc32_t key = pair.first;
			const stl_container_t::size_type index = stl_con->bucket(key);
			printf_detail("[%6d] key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)\n", index, key, pair.second.m_name, pair.second.m_value, index, stl_con->bucket_size(index));
		}
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	printSTLTable();

	//ハッシュテーブルアクセス
	auto findSTLTable = [&stl_con, &printElapsedTime, &prev_time]()
	{
		std::printf("\n");
		std::printf("--- [STL] Find Data ---\n");
		int find_success = 0;
		int find_failure = 0;
		for (int i = 0; i < TEST_DATA_TABLE_SIZE; ++i)
		{
			char name[20];
			GASHA_ spprintf(name, "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			crc32_t key = calcCRC32(name);
			auto ite = stl_con->find(key);
			if (ite != stl_con->end())
			{
				const crc32_t key = ite->first;
				const stl_container_t::size_type index = stl_con->bucket(key);
				printf_detail("OK  key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)\n", key, ite->second.m_name, ite->second.m_value, index, stl_con->bucket_size(index));
				++find_success;
			}
			else
			{
				printf_detail("NG\n");
				++find_failure;
			}
		}
		std::printf("success=%d, failure=%d\n", find_success, find_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	findSTLTable();

	//ハッシュテーブルからのデータ削除
	auto eraseSTLData = [&stl_con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		std::printf("\n");
		std::printf("--- [STL] Erase Data ---\n");
		int erase_success = 0;
		int erase_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			GASHA_ spprintf(name, "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			crc32_t key = calcCRC32(name);
			bool result = stl_con->erase(key) == 1;
			if (result)
			{
				++erase_success;
				printf_detail("OK\n");
			}
			else
			{
				++erase_failure;
				printf_detail("NG\n");
			}
		}
		std::printf("success=%d, failure=%d\n", erase_success, erase_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	eraseSTLData(0, TEST_DATA_TABLE_SIZE, 2);

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	printSTLTable();

	//ハッシュテーブルアクセス
	findSTLTable();

	//ハッシュテーブルへのデータ登録
	insertSTLData(1, TEST_DATA_TABLE_SIZE, 2);

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	printSTLTable();

	//ハッシュテーブルアクセス
	findSTLTable();

	//リハッシュ
	auto rehashSTLTable = [&stl_con, &printElapsedTime, &prev_time]()
	{
		std::printf("\n");
		std::printf("--- [STL] Rehash Table ---\n");
		stl_con->rehash(TEST_DATA_TABLE_SIZE);
		std::printf("OK.\n");
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	rehashSTLTable();

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	printSTLTable();

	//ハッシュテーブルアクセス
	findSTLTable();

	//クリア
	auto clearSTLTable = [&stl_con, &printElapsedTime, &prev_time]()
	{
		std::printf("\n");
		std::printf("--- [STL] Clear Table ---\n");
		stl_con->clear();
		std::printf("OK.\n");
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	clearSTLTable();

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	printSTLTable();

	//ハッシュテーブルアクセス
	findSTLTable();

	//ハッシュテーブルへのデータ登録
	insertSTLData(0, TEST_DATA_TABLE_SIZE, 1);

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	printSTLTable();

	//ハッシュテーブルアクセス
	findSTLTable();

	//ハッシュテーブルからのデータ削除
	eraseSTLData(0, TEST_DATA_TABLE_SIZE, 1);

	//ハッシュテーブル状態表示
	printSTLTableStatus();

	//ハッシュテーブル表示
	printSTLTable();

	//ハッシュテーブルアクセス
	findSTLTable();

	//--------------------
	//ハッシュテーブル削除
	delete stl_con;
	stl_con = nullptr;
	
	std::printf("\n");
	std::printf("End\n");
	std::printf("============================================================\n");
	printElapsedTime(begin_time, true);//処理時間表示

	//----------------------------------------------------------------------------------------------------
	//ポインタ型のハッシュテーブルテスト
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Hash Table Test for Pointer\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//ハッシュテーブル
		hash_table::container<ptr_ope> p_con;

		//登録
		data_t obj1("0010", 123);
		data_t obj2("0020", 456);
		data_t obj3("0030", 789);
		p_con.insert(10, &obj1);
		p_con.insert(20, &obj2);
		p_con.insert(30, &obj3);

		//検索して表示
		auto printObj = [&p_con](const int key)
		{
			std::printf("[%2d] ... ", key);
			data_t** p = p_con[key];
			if (p && *p)
				std::printf("OK  name=\"%s\", val=%d\n", (*p)->m_name, (*p)->m_value);
			else
				std::printf("NG\n");
		};
		printObj(10);
		printObj(20);
		printObj(30);
		printObj(40);

		//ロック取得テスト
		{
			shared_lock_guard<ptr_ope::lock_type> lock(p_con);//共有ロック（リードロック）取得（処理ブロックを抜ける時に自動開放）
			printObj(20);
		}
	}

	//----------------------------------------------------------------------------------------------------
	//キーの範囲が狭いハッシュテーブルのテスト
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Hash Table Test for small-range key\n");
		std::printf("--------------------------------------------------------------------------------\n");

		//ハッシュテーブル
		typedef hash_table::container<narrow_range_key_ope> con_t;
		 con_t con;
		
		//登録
		//※範囲外のキーは登録に失敗する
		for (int key = -3, val = 1000; key <= 3; ++key, ++val)
			con.emplace(key, val);
		iteratorForEach(con, [](con_t::iterator& ite)
			{
				printf_detail("[%2d] key=%2d, value=%d\n", ite.getIndex(), ite.getKey(), *ite);
			}
		);
	}

	//----------------------------------------------------------------------------------------------------
	//関数型のハッシュテーブルテスト
	//※スクリプトなどから関数名（文字列）で関数を実行するような用途を想定
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Hash Table Test for Function\n");
		std::printf("--------------------------------------------------------------------------------\n");
		
		//ハッシュテーブル
		hash_table::container<func_ope> func_con;
		hash_table::container<obj_ope> obj_con;

		//関数型ハッシュテーブルテスト用関数：通常関数
		extern int func_add(int a, int b);

		//関数型ハッシュテーブルテスト用関数：関数オブジェクト
		struct func_sub{
			int operator()(int a, int b)
			{
				return a - b;
			};
		};

		//関数型ハッシュテーブルテスト用関数：ラムダ関数
		auto func_mul = [](int a, int b) -> int
		{
			return a * b;
		};

		//関数型ハッシュテーブルテスト用関数：ラムダ関数
		auto func_div = [](int a, int b) -> int
		{
			return a / b;
		};

		//登録
		func_con.emplace("+", func_add);
		func_con.emplace("-", func_sub());
		func_con.emplace("*", func_mul);
		func_con.emplace("/", std::bind(func_div, 50, 4));//std::bindを使用
		obj_con.emplace("calc", std::mem_fn(&calc_t::calc));//クラスメンバー関数の場合

		//検索して実行
		std::printf("50 + 4 = %d\n", (*func_con["+"])(50, 4));
		std::printf("50 - 4 = %d\n", (*func_con["-"])(50, 4));
		std::printf("50 * 4 = %d\n", (*func_con["*"])(50, 4));
		std::printf("50 / 4 = %d\n", (*func_con["/"])(0, 0));//std::bind()で事前にパラメータがセット済み
		calc_t obj(3);
		std::printf("obj.calc(1, 2) = %d\n", (*obj_con["calc"])(obj, 1, 2));//クラスメンバー関数の場合、オブジェクトを渡す必要がある
	}
}

//関数型ハッシュテーブルテスト用関数：通常関数
int func_add(int a, int b)
{
	return a + b;
};

// End of file
