//--------------------------------------------------------------------------------
// exmaple_hash_table.cpp
// 開番地法ハッシュテーブルテストテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_hash_table.h"//開番地法ハッシュテーブルテストテスト

//#include <gasha/hash_table.h>//開番地法ハッシュテーブルテスト

#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//ハッシュテーブル＆素数計算テスト
//--------------------------------------------------------------------------------

#include <chrono>//C++11 時間計測用
#include <unordered_map>//C++11 std::unordered_map
#include <map>//C++11 std::map
#include <string.h>//strcpy用

#if 0

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
//文字列操作関数
#ifdef USE_GCC
inline char* strcpy_s(char* dst, const std::size_t size, const char* src)
{
	return strcpy(dst, src);
}
inline char* strncpy_s(char* dst, const std::size_t size, const char* src, const std::size_t max)
{
	return strncpy(dst, src, max);
}
template<typename... Tx>
inline int sprintf_s(char* dst, const std::size_t size, const char* fmt, Tx... args)
{
	return sprintf(dst, fmt, args...);
}
#endif//USE_GCC

//----------------------------------------
//【ランタイム版】素数判定／算出テスト
void printPrime(const std::size_t min, const std::size_t max)
{
	if (max > min)
		printPrime(min, max - 1);
	printf("%6d is %s [prev=%6d(%6d), next=%6d(%6d)]\n", max, isPrime(max) ? "PRIME.    " : "NOT prime.", makePrimeLT(max), makePrimeLE(max), makePrimeGT(max), makePrimeGE(max));
}

//----------------------------------------
//【メタプログラミング版】素数判定／算出テスト
template<std::size_t N>
void _printPrimeCommon()
{
	printf("%6d is %s [prev=%6d(%6d), next=%6d(%6d)]\n", N, isStaticPrime<N>::value ? "PRIME.    " : "NOT prime.", makeStaticPrimeLT<N>::value, makeStaticPrimeLE<N>::value, makeStaticPrimeGT<N>::value, makeStaticPrimeGE<N>::value);
}
template<std::size_t MIN, std::size_t MAX>
struct printStaticPrime{
	void operator()()
	{
		printStaticPrime<MIN, MAX - 1>()();
		_printPrimeCommon<MAX>();
	}
};
template<std::size_t MIN>
struct printStaticPrime<MIN, MIN>{
	void operator()()
	{
		_printPrimeCommon<MIN>();
	}
};

//----------------------------------------
//ハッシュテーブル用テストデータ
struct data_t
{
	crc32_t m_key;//キー ※名前のハッシュ値
	int m_value;//値
	char m_name[20];//名前

	//コンストラクタ
	data_t(const char* name, const int value) :
		m_key(calcCRC32(name)),
		m_value(value)
	{
		strncpy_s(m_name, sizeof(m_name), name, sizeof(m_name) - 1);
		m_name[sizeof(m_name)-1] = '\0';
	#ifdef TEST_DATA_WATCH_CONSTRUCTOR
		printf("data_t::constructor(\"%s\", value)\n", name, value);
		//printf("    m_key=%d, m_name=[%s], m_value=%d\n", m_key, m_name, m_value);
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
	//デフォルトコンストラクタ
	data_t():
		m_key(0),
		m_value(0)
	{
		m_name[0] = '\0';
		printf("data_t::constructor\n");
	}
	//デストラクタ
	~data_t()
	{
		printf("data_t::destructor\n");
		//printf("  m_key=%d, m_name=[%s]\n", m_key, m_name);
	}
#endif//TEST_DATA_WATCH_CONSTRUCTOR
};

//----------------------------------------
//テストデータ操作クラス
#include <functional>//C++11 std::function用
#include <algorithm>//C++11 std::for_each用
struct ope_t : public hash_table::baseOpe_t<ope_t, crc32_t, data_t>
{
	//データ置換属性
	//※デフォルト（NEVER_REPLACE）のままとする
	//static const replace_attr_t REPLACE_ATTR = REPLACE;//キーが重複するデータは置換して登録する
	
	//キーを取得
	inline static key_type getKey(const value_type& value){ return value.m_key; }

	//ロック型
	//※デフォルト（dummy_shared_lock）のままとする
	//typedef shared_spin_lock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//テストメイン
int main(const int argc, const char* argv[])
{
	//時間計測
	auto begin_time = std::chrono::system_clock::now();
	auto prev_time = begin_time;

	//処理時間計測
	auto calcElapsedTime = [](const std::chrono::system_clock::time_point& now_time, const std::chrono::system_clock::time_point& prev_time) -> double
	{
		const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now_time - prev_time);
		const auto elapsed_time = static_cast<double>(duration.count()) / 1000000000.;
		return elapsed_time;
	};
	auto getElapsedTime = [&calcElapsedTime](const std::chrono::system_clock::time_point& prev_time) -> double
	{
		return calcElapsedTime(std::chrono::system_clock::now(), prev_time);
	};

	//処理時間表示
	auto printElapsedTimeDirect = [&calcElapsedTime](const double elapsed_time, const bool is_preint) -> std::chrono::system_clock::time_point
	{
		if (is_preint)
			printf("*elapsed time=%.9llf sec.\n", elapsed_time);
		return std::chrono::system_clock::now();
	};
	auto printElapsedTime = [&calcElapsedTime, &printElapsedTimeDirect](const std::chrono::system_clock::time_point& prev_time, const bool is_print) -> std::chrono::system_clock::time_point
	{
		const auto now_time = std::chrono::system_clock::now();
		const auto elapsed_time = calcElapsedTime(now_time, prev_time);
		return printElapsedTimeDirect(elapsed_time, is_print);
	};

#if 0
	//--------------------
	//素数コンパイル時計算の再帰レベル限界チェック
	static const std::size_t x = 9999;
	printf("x=%d\n", x);
	printf("  isPrime=%s\n", isStaticPrime<x>::value ? "true" : "False");
	printf("  prev=%d\n", makeStaticPrimeLT<x>::value);
	printf("  next=%d\n", makeStaticPrimeGT<x>::value);
	printf("  equalPrev=%d\n", makeStaticPrimeLE<x>::value);
	printf("  equalNext=%d\n", makeStaticPrimeGE<x>::value);
#endif

#if 0
	//--------------------
	//素数計算のテスト
	static const std::size_t MIN = 0;
	static const std::size_t MAX = 10;
	
	printf("----- Check and Make Prime for Runtime -----\n");
	printPrime(MIN, MAX);
	
	printf("----- Check and Make Prime for Meta-Programming -----\n");
	printStaticPrime<MIN, MAX>()();
#endif

#if 0
	//--------------------
	//ハッシュテーブルのインデックス計算ロジックテスト
	//※全てのインデックスが、正確にテーブルサイズ（素数）回の再計算を行う事で、元のインデックスに戻ることを確認。
	{
		int ng_count = 0;
		const ope_t::key_type key_min = 0;// con.getKeyMin();
		const ope_t::key_type key_max = 100000;// con.getKeyMax();
		const ope_t::key_type key_step = 1;
		for (ope_t::key_type key = key_min; key <= key_max; key += key_step)
		{
			if (key % (1024 * 1024) == 0)
				printf("pass ... Key:%d\n", key);
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
				printf("%u is OUT! (count=%d)\n", key, count);
				++ng_count;
			}
		}
		printf("Chek Hash Table: NG=%d/%d\n", ng_count, con.getTableSize());
		for (std::size_t key = 10; key <= 30; key += 1)
		{
			printf("Key:%u -> Index;%u\n", key, con.calcIndex(key));
		}
	}
#endif

#if 0
	//--------------------
	//ハッシュテーブルテスト（旧）
	{
		hash_table::container<ope_t, 20> con(hash_table::AUTO_WRITE_LOCK);
		
		data_t* result;
		printf("- emplace -\n");
		result = con.emplace(110, "123", 123);
		data_t* obj = con[110];
		obj->m_value = 1234567;
		result = con.emplace(220, "456", 456);
		result = con.emplace(330, "789", 789);
		result = con.emplace(110, "123", 123);
		for (auto& o : con)
		{
			printf("index=%d(%d), key=%08x, name=%s, value=%d, is_deleted=%s\n", o.m_index, con.calcIndex(o.m_key), o.m_key, o->m_name, o->m_value, o.m_isDeleted ? "TRUE" : "false");
		}
		printf(".size()=%d\n", con.size());
		printf(".getUsingCount()=%d\n", con.getUsingCount());
		printf(".getDeletedCount()=%d\n", con.getDeletedCount());
		printf(".getMaxFindingCycle()=%d\n", con.getMaxFindingCycle());
		printf("- erase -\n");
		con.erase(220);
		con.erase(330);
		for (auto& o : con)
		{
			printf("index=%d(%d), key=%08x, name=%s, value=%d, is_deleted=%s\n", o.m_index, con.calcIndex(o.m_key), o.m_key, o->m_name, o->m_value, o.m_isDeleted ? "TRUE" : "false");
		}
		printf(".getUsingCount()=%d\n", con.getUsingCount());
		printf(".getDeletedCount()=%d\n", con.getDeletedCount());
		printf(".getMaxFindingCycle()=%d\n", con.getMaxFindingCycle());
		printf("- emplace -\n");
		result = con.emplace(220, "456!", 4567);
		result = con.emplace("123", "123!!", 12345);
		result = con.emplaceAuto("456!!", 45679);
		result = con.emplace("789", "789!!", 78901);
		for (auto& o : con)
		{
			printf("index=%d(%d), key=%08x, name=%s, value=%d, is_deleted=%s\n", o.m_index, con.calcIndex(o.m_key), o.m_key, o->m_name, o->m_value, o.m_isDeleted ? "TRUE" : "false");
		}
		printf(".getUsingCount()=%d\n", con.getUsingCount());
		printf(".getDeletedCount()=%d\n", con.getDeletedCount());
		printf(".getMaxFindingCycle()=%d\n", con.getMaxFindingCycle());
		printf("- erase -\n");
		con.erase(220);
		con.erase(110);
		for (auto& o : con)
		{
			printf("index=%d(%d), key=%08x, name=%s, value=%d, is_deleted=%s\n", o.m_index, con.calcIndex(o.m_key), o.m_key, o->m_name, o->m_value, o.m_isDeleted ? "TRUE" : "false");
		}
		printf(".getUsingCount()=%d\n", con.getUsingCount());
		printf(".getDeletedCount()=%d\n", con.getDeletedCount());
		printf(".getMaxFindingCycle()=%d\n", con.getMaxFindingCycle());
		printf("- rehash -\n");
		con.rehash();
		for (auto& o : con)
		{
			printf("index=%d(%d), key=%08x, name=%s, value=%d, is_deleted=%s\n", o.m_index, con.calcIndex(o.m_key), o.m_key, o->m_name, o->m_value, o.m_isDeleted ? "TRUE" : "false");
		}
		printf(".getUsingCount()=%d\n", con.getUsingCount());
		printf(".getDeletedCount()=%d\n", con.getDeletedCount());
		printf(".getMaxFindingCycle()=%d\n", con.getMaxFindingCycle());
	}
#endif
	
	//--------------------
	//ハッシュテーブルテスト
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("Hash Table Test\n");
	printf("--------------------------------------------------------------------------------\n");
	typedef hash_table::container<ope_t, TEST_DATA_TABLE_SIZE, 0, 0> container_t;//自動リハッシュなし, 検索巡回回数制限なし
	container_t* con = new container_t();

	//ハッシュテーブルの基本情報表示
	auto printTableParameter = [&con]()
	{
		printf("\n");
		printf("--- Table Parameter ---\n");
		printf(".max_size()=%u\n", con->max_size());
		//printf(".capacity()=%u\n", con->capacity());
		printf(".getOriginalTableSize()=%u\n", con->getOriginalTableSize());
		printf(".getTableSize()=%u\n", con->getTableSize());
		printf(".getTableSizeExtended()=%u\n", con->getTableSizeExtended());
		printf(".getAutoRehashRatio()=%u\n", con->getAutoRehashRatio());
		printf(".getAutoRehashSize()=%u\n", con->getAutoRehashSize());
		printf(".getFindingCycleLimit()=%u\n", con->getFindingCycleLimit());
		printf(".getKeyMin()=%u\n", con->getKeyMin());
		printf(".getKeyMax()=%u\n", con->getKeyMax());
		printf(".getKeyRange()=%u\n", con->getKeyRange());
	};
	printTableParameter();

	//テーブル状態表示
	auto printTableStatus = [&con]()
	{
		printf("\n");
		printf("--- Table Status ---\n");
		printf(".bucket_count()=%u\n", con->bucket_count());
		printf(".max_bucket_count()=%u\n", con->max_bucket_count());
		printf(".size()=%u\n", con->size());
		printf(".empty()=%u\n", con->empty());
		printf(".getUsingCount()=%u\n", con->getUsingCount());
		printf(".getDeletedCount()=%u\n", con->getDeletedCount());
		printf(".getMaxFindingCycle()=%u\n", con->getMaxFindingCycle());
		printf(".getNotOptimizedCount()=%u\n", con->getNotOptimizedCount());
	};
	printTableStatus();

	//ハッシュテーブルへのデータ登録
	auto insertData = [&con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		printf("\n");
		printf("--- Insert Data ---\n");
		int insert_success = 0;
		int insert_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			sprintf_s(name, sizeof(name), "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			data_t* obj = nullptr;
			#define USE_INSERT_TYPE 1
			//【推奨】【登録方法①】emplace()メソッドにキーとコンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーは発生しない
			#if USE_INSERT_TYPE == 1
			{
				obj = con->emplace(calcCRC32(name), name, i);//キーとコンストラクタパラメータを渡して登録
			}
			//【登録方法②】emplaceAuto()メソッドにコンストラクタパラメータを渡して登録する方法
			//※オブジェクトのコピーが発生するので少し遅い
			#elif USE_INSERT_TYPE == 2
			{
				obj = con->emplaceAuto(name, i);//コンストラクタパラメータを渡して登録
			}
			//【登録方法③】insert()メソッドにキーとオブジェクトを渡して登録する方法
			//※オブジェクトのコピーが発生するので少し遅い
			//※操作用クラス baseOpe_t の派生クラスで、getKey() を実装する必要あり
			#elif USE_INSERT_TYPE == 3
			{
				data_t new_obj(name, i);
				obj = con->insert(calcCRC32(name), new_obj);
			}
			//【登録方法④】insertAuto()メソッドにオブジェクトを渡して登録する方法
			//※オブジェクトのコピーが発生するので少し遅い
			//※操作用クラス baseOpe_t の派生クラスで、getKey() を実装する必要あり
			#elif USE_INSERT_TYPE == 4
			{
				data_t new_obj(name, i);
				obj = con->insertAuto(new_obj);
			}
			//【登録方法⑤】assign()メソッドでデータのポインタを取得し、そこに値を書き込む
			//※コンストラクタのないオブジェクトを扱う場合に有効
			#elif USE_INSERT_TYPE == 5
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
			#endif//USE_INSERT_TYPE
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
		printf("success=%d, failure=%d\n", insert_success, insert_failure);
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
		//for (container_t::set& set : *con)
		for (auto& set : *con)
		{
			printf_detail("%c[%6d](%6d) key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)%s\n", set.isPrimaryIndex() ? ' ' : '*', set.m_index, set.m_primaryIndex, set.m_key, set->m_name, set->m_value, con->bucket(set.m_key), con->bucket_size(set.m_index), set.m_isDeleted ? " <DELETED>" : "");
		}
		const bool is_print = false;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	printTable();

#if 0//イテレータとロック取得のテスト
	{
		printf_detail("--- Reverse Iterator ---\n");
		std::for_each(con->rbegin(), con->rend(), [&con](container_t::set& set)
			{
				printf_detail("%c[%6d](%6d) key=%08x, name=\"%s\", value=%d (bucket=%d, bucket_size=%d)%s\n", set.isPrimaryIndex() ? ' ' : '*', set.m_index, set.m_primaryIndex, set.m_key, set->m_name, set->m_value, con->bucket(set.m_key), con->bucket_size(set.m_index), set.m_isDeleted ? " <DELETED>" : "");
			}
		);
	}
	{
		shared_lock_guard<container_t::lock_type> lock(*con);
		container_t::iterator ite = con->begin();
		container_t::reverse_iterator rite = con->rbegin();
		container_t::iterator ite_end = con->end();
		container_t::reverse_iterator rite_end = con->rend();
		container_t::iterator ite2 = con->rbegin();
		container_t::reverse_iterator rite2 = con->begin();
		container_t::iterator ite2_end = con->rend();
		container_t::reverse_iterator rite2_end = con->end();
		printf("constructor\n");
		if (ite.isExist()) printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', ite.getIndex(), ite.getPrimaryIndex(), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', rite.getIndex(), rite.getPrimaryIndex(), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', ite_end.getIndex(), ite_end.getPrimaryIndex(), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', rite_end.getIndex(), rite_end.getPrimaryIndex(), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		if (ite2.isExist()) printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', ite2.getIndex(), ite2.getPrimaryIndex(), ite2->m_key, ite2->m_name, ite2->m_value);
		if (rite2.isExist()) printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', rite2.getIndex(), rite2.getPrimaryIndex(), rite2->m_key, rite2->m_name, rite2->m_value);
		if (ite2_end.isExist()) printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', ite2_end.getIndex(), ite2_end.getPrimaryIndex(), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		if (rite2_end.isExist()) printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', rite2_end.getIndex(), rite2_end.getPrimaryIndex(), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
		printf("copy operator\n");
		ite = con->begin();
		rite = con->rbegin();
		ite_end = con->end();
		rite_end = con->rend();
		ite2 = con->rbegin();
		rite2 = con->begin();
		ite2_end = con->rend();
		rite2_end = con->end();
		if (ite.isExist()) printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', ite.getIndex(), ite.getPrimaryIndex(), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', rite.getIndex(), rite.getPrimaryIndex(), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', ite_end.getIndex(), ite_end.getPrimaryIndex(), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', rite_end.getIndex(), rite_end.getPrimaryIndex(), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		if (ite2.isExist()) printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', ite2.getIndex(), ite2.getPrimaryIndex(), ite2->m_key, ite2->m_name, ite2->m_value);
		if (rite2.isExist()) printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', rite2.getIndex(), rite2.getPrimaryIndex(), rite2->m_key, rite2->m_name, rite2->m_value);
		if (ite2_end.isExist()) printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', ite2_end.getIndex(), ite2_end.getPrimaryIndex(), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		if (rite2_end.isExist()) printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', rite2_end.getIndex(), rite2_end.getPrimaryIndex(), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
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
			if (ite.isExist()) printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', ite.getIndex(), ite.getPrimaryIndex(), ite->m_key, ite->m_name, ite->m_value);
			if (rite.isExist()) printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', rite.getIndex(), rite.getPrimaryIndex(), rite->m_key, rite->m_name, rite->m_value);
			if (ite_end.isExist()) printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', ite_end.getIndex(), ite_end.getPrimaryIndex(), ite_end->m_key, ite_end->m_name, ite_end->m_value);
			if (rite_end.isExist()) printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', rite_end.getIndex(), rite_end.getPrimaryIndex(), rite_end->m_key, rite_end->m_name, rite_end->m_value);
			if (ite2.isExist()) printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', ite2.getIndex(), ite2.getPrimaryIndex(), ite2->m_key, ite2->m_name, ite2->m_value);
			if (rite2.isExist()) printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', rite2.getIndex(), rite2.getPrimaryIndex(), rite2->m_key, rite2->m_name, rite2->m_value);
			if (ite2_end.isExist()) printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', ite2_end.getIndex(), ite2_end.getPrimaryIndex(), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
			if (rite2_end.isExist()) printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', rite2_end.getIndex(), rite2_end.getPrimaryIndex(), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
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
		if (ite.isExist()) printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', ite.getIndex(), ite.getPrimaryIndex(), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', rite.getIndex(), rite.getPrimaryIndex(), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', ite_end.getIndex(), ite_end.getPrimaryIndex(), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', rite_end.getIndex(), rite_end.getPrimaryIndex(), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		if (ite2.isExist()) printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', ite2.getIndex(), ite2.getPrimaryIndex(), ite2->m_key, ite2->m_name, ite2->m_value);
		if (rite2.isExist()) printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', rite2.getIndex(), rite2.getPrimaryIndex(), rite2->m_key, rite2->m_name, rite2->m_value);
		if (ite2_end.isExist()) printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', ite2_end.getIndex(), ite2_end.getPrimaryIndex(), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		if (rite2_end.isExist()) printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', rite2_end.getIndex(), rite2_end.getPrimaryIndex(), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
		printf("-= 3\n");
		ite -= 3;
		rite -= 3;
		ite_end -= 3;
		rite_end -= 3;
		ite2 -= 3;
		rite2 -= 3;
		ite2_end -= 3;
		rite2_end -= 3;
		if (ite.isExist()) printf("ite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite.isDeleted() ? '*' : ' ', ite.getIndex(), ite.getPrimaryIndex(), ite->m_key, ite->m_name, ite->m_value);
		if (rite.isExist()) printf("rite:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite.isDeleted() ? '*' : ' ', rite.getIndex(), rite.getPrimaryIndex(), rite->m_key, rite->m_name, rite->m_value);
		if (ite_end.isExist()) printf("ite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite_end.isDeleted() ? '*' : ' ', ite_end.getIndex(), ite_end.getPrimaryIndex(), ite_end->m_key, ite_end->m_name, ite_end->m_value);
		if (rite_end.isExist()) printf("rite_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite_end.isDeleted() ? '*' : ' ', rite_end.getIndex(), rite_end.getPrimaryIndex(), rite_end->m_key, rite_end->m_name, rite_end->m_value);
		if (ite2.isExist()) printf("ite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2.isDeleted() ? '*' : ' ', ite2.getIndex(), ite2.getPrimaryIndex(), ite2->m_key, ite2->m_name, ite2->m_value);
		if (rite2.isExist()) printf("rite2:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2.isDeleted() ? '*' : ' ', rite2.getIndex(), rite2.getPrimaryIndex(), rite2->m_key, rite2->m_name, rite2->m_value);
		if (ite2_end.isExist()) printf("ite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", ite2_end.isDeleted() ? '*' : ' ', ite2_end.getIndex(), ite2_end.getPrimaryIndex(), ite2_end->m_key, ite2_end->m_name, ite2_end->m_value);
		if (rite2_end.isExist()) printf("rite2_end:%c[%d](%d) key=0x%08x, name=\"%s\", value=%d\n", rite2_end.isDeleted() ? '*' : ' ', rite2_end.getIndex(), rite2_end.getPrimaryIndex(), rite2_end->m_key, rite2_end->m_name, rite2_end->m_value);
	}
#endif

	//ハッシュテーブルアクセス
	auto findTable = [&con, &printElapsedTime, &prev_time]()
	{
		printf("\n");
		printf("--- Find Data ---\n");
		int find_success = 0;
		int find_failure = 0;
		for (int i = 0; i < TEST_DATA_TABLE_SIZE; ++i)
		{
			char name[20];
			sprintf_s(name, sizeof(name), "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			data_t* obj;
			crc32_t key;
			int index;
			int primary_index;
			bool is_primary_index;
			bool is_deleted;
			#define USE_FIND_TYPE 1
			//【推奨】【検索方法①】[]オペレータにキーを渡して検索する方法
			//※値を返す
			#if USE_FIND_TYPE == 1
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
			//※値を返す（[]オペレータと同じ）
			#elif USE_FIND_TYPE == 2
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
			#elif USE_FIND_TYPE == 3
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
			#endif//USE_FIND_TYPE
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
		printf("success=%d, failure=%d\n", find_success, find_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	findTable();

	//ハッシュテーブルからのデータ削除
	auto eraseData = [&con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		printf("\n");
		printf("--- Erase Data ---\n");
		int erase_success = 0;
		int erase_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			sprintf_s(name, sizeof(name), "Name_%06d", i);
			printf_detail("name=\"%s\" ... ", name);
			bool result = false;
			#define USE_ERASE_TYPE 1
			//【推奨】【削除方法①】erase()メソッドにキーを渡して削除する方法
			#if USE_ERASE_TYPE == 1
			{
				result = con->erase(calcCRC32(name));//キーを渡して削除
			}
			//【削除方法②】eraseAuto()メソッドにオブジェクトを渡して削除する方法
			//※操作用クラス baseOpe_t の派生クラスで、getKey() を実装する必要あり
			#elif USE_INSERT_TYPE == 2
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
		printf("success=%d, failure=%d\n", erase_success, erase_failure);
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
		printf("\n");
		printf("--- Rehash Table ---\n");
		con->rehash();
		printf("OK.\n");
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
		printf("\n");
		printf("--- Clear Table ---\n");
		con->clear();
		printf("OK.\n");
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

	printf("\n");
	printf("End\n");
	printf("============================================================\n");
	begin_time = printElapsedTime(begin_time, true);//処理時間表示

	//----------------------------------------------------------------------------------------------------
	//【比較用】【STL版】ハッシュテーブルテスト
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[STL] Hash Table Test\n");
	printf("--------------------------------------------------------------------------------\n");
	typedef std::unordered_map<crc32_t, data_t> stl_container_t;
	stl_container_t* stl_con = new stl_container_t();
	stl_con->reserve(TEST_DATA_TABLE_SIZE);

	//ハッシュテーブルの基本情報表示
	auto printSTLTableParameter = [&stl_con]()
	{
		printf("\n");
		printf("--- [STL] Table Parameter ---\n");
		printf(".max_size()=%u\n", stl_con->max_size());
	};
	printSTLTableParameter();

	//テーブル状態表示
	auto printSTLTableStatus = [&stl_con]()
	{
		printf("\n");
		printf("--- [STL] Table Status ---\n");
		printf(".size()=%u\n", stl_con->size());
		printf(".empty()=%u\n", stl_con->empty());
		printf(".bucket_count()=%u\n", stl_con->bucket_count());
		printf(".max_bucket_count()=%u\n", stl_con->max_bucket_count());
		printf(".load_factor()=%u\n", stl_con->load_factor());
		printf(".max_load_factor()=%u\n", stl_con->max_load_factor());
	};
	printSTLTableStatus();

	//ハッシュテーブルへのデータ登録
	auto insertSTLData = [&stl_con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		printf("\n");
		printf("--- [STL] Insert Data ---\n");
		int insert_success = 0;
		int insert_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			sprintf_s(name, sizeof(name), "Name_%06d", i);
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
		printf("success=%d, failure=%d\n", insert_success, insert_failure);
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
		printf("\n");
		printf("--- [STL] Find Data ---\n");
		int find_success = 0;
		int find_failure = 0;
		for (int i = 0; i < TEST_DATA_TABLE_SIZE; ++i)
		{
			char name[20];
			sprintf_s(name, sizeof(name), "Name_%06d", i);
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
		printf("success=%d, failure=%d\n", find_success, find_failure);
		const bool is_print = true;
		prev_time = printElapsedTime(prev_time, is_print);
	};
	findSTLTable();

	//ハッシュテーブルからのデータ削除
	auto eraseSTLData = [&stl_con, &printElapsedTime, &prev_time](const int begin, const int end, const int step)
	{
		printf("\n");
		printf("--- [STL] Erase Data ---\n");
		int erase_success = 0;
		int erase_failure = 0;
		for (int i = begin; i < end; i += step)
		{
			char name[20];
			sprintf_s(name, sizeof(name), "Name_%06d", i);
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
		printf("success=%d, failure=%d\n", erase_success, erase_failure);
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
		printf("\n");
		printf("--- [STL] Rehash Table ---\n");
		stl_con->rehash(TEST_DATA_TABLE_SIZE);
		printf("OK.\n");
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
		printf("\n");
		printf("--- [STL] Clear Table ---\n");
		stl_con->clear();
		printf("OK.\n");
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
	
	printf("\n");
	printf("End\n");
	printf("============================================================\n");
	printElapsedTime(begin_time, true);//処理時間表示

	//----------------------------------------------------------------------------------------------------
	//ポインタ型のハッシュテーブルテスト
	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("Hash Table Test for Pointer\n");
		printf("--------------------------------------------------------------------------------\n");

		//操作型
		struct p_ope_t : public hash_table::baseOpe_t<p_ope_t, int, data_t*>
		{
			//データ置換属性
			//※デフォルト（NEVER_REPLACE）のままとする
			//static const replace_attr_t REPLACE_ATTR = REPLACE;//キーが重複するデータは置換して登録する
			
			//キーを取得
			inline static key_type getKey(const value_type& value){ return value->m_key; }

			//ロック型
			//※デフォルト（dummy_shared_lock）のままとする
			//typedef shared_spin_lock lock_type;//ロックオブジェクト型
		};

		//ハッシュテーブル
		hash_table::container<p_ope_t, 100> p_con;

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
			printf("[%2d] ... ", key);
			data_t** p = p_con[key];
			if (p && *p)
				printf("OK  name=\"%s\", val=%d\n", (*p)->m_name, (*p)->m_value);
			else
				printf("NG\n");
		};
		printObj(10);
		printObj(20);
		printObj(30);
		printObj(40);

		//ロック取得テスト
		{
			shared_lock_guard<p_ope_t::lock_type> lock(p_con);//共有ロック（リードロック）取得（処理ブロックを抜ける時に自動開放）
			printObj(20);
		}
	}

	//----------------------------------------------------------------------------------------------------
	//関数型のハッシュテーブルテスト
	//※スクリプトなどから関数名（文字列）で関数を実行するような用途を想定
	{
		printf("\n");
		printf("--------------------------------------------------------------------------------\n");
		printf("Hash Table Test for Function\n");
		printf("--------------------------------------------------------------------------------\n");
		
		//操作型 ※単純な関数呼び出し用
		struct func_ope_t : public hash_table::baseOpe_t<func_ope_t, crc32_t, std::function<int(int, int)>>
		{
		};
		
		//オブジェクトメンバー関数呼び出しテスト用
		struct data_t
		{
			int calc(int a, int b)
			{
				return (a + b) * m_val;
			}
			data_t(const int val) :
				m_val(val)
			{}
		private:
			int m_val;
		};

		//操作型 ※オブジェクトメンバー関数呼び出し用
		struct obj_ope_t : public hash_table::baseOpe_t<func_ope_t, crc32_t, std::function<int(data_t&, int, int)>>
		{
		};

		//ハッシュテーブル
		hash_table::container<func_ope_t, 100> func_con;
		hash_table::container<obj_ope_t, 100> obj_con;

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
		obj_con.emplace("calc", std::mem_fn(&data_t::calc));//クラスメンバー関数の場合

		//検索して実行
		printf("50 + 4 = %d\n", (*func_con["+"])(50, 4));
		printf("50 - 4 = %d\n", (*func_con["-"])(50, 4));
		printf("50 * 4 = %d\n", (*func_con["*"])(50, 4));
		printf("50 / 4 = %d\n", (*func_con["/"])(0, 0));//std::bind()で事前にパラメータがセット済み
		data_t obj(3);
		printf("obj.calc(1, 2) = %d\n", (*obj_con["calc"])(obj, 1, 2));//クラスメンバー関数の場合、オブジェクトを渡す必要がある
	}

	return EXIT_SUCCESS;
}

//関数型ハッシュテーブルテスト用関数：通常関数
int func_add(int a, int b)
{
	return a + b;
};

#endif

//----------------------------------------
//開番地法ハッシュテーブルテストテスト
void example_hash_table()
{
}

// End of file
