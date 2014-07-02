//--------------------------------------------------------------------------------
// exmaple_linked_list.cpp
// 双方向連結リストコンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_linked_list.h"//双方向連結リストコンテナテスト

#include <gasha/linked_list.inl>//双方向連結リストコンテナ【インライン関数／テンプレート関数定義部】

#include <gasha/iterator.h>//イテレータ操作

#include <utility>//C++11 std::move, std::forward
#include <chrono>//C++11 std::chrono
#include <stdio.h>//printf()

#include <assert.h>//assert()

//【VC++】例外を無効化した状態で <algorithm> <list> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::for_each()
#include <list>//std::list（比較用）

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//双方向連結リストコンテナテスト
//--------------------------------------------------------------------------------

//----------------------------------------
//テストデータ

//コンストラクタ
data_t::data_t(const int key, const int val) :
	m_next(nullptr),
	m_prev(nullptr),
	m_key(key),
	m_val(val)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor(%d, %d)\n", key, val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
//デフォルトコンストラクタ
data_t::data_t() :
	m_next(nullptr),
	m_prev(nullptr),
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
data_t& data_t::operator=(data_t&& rhs)
{
	memcpy(this, &rhs, sizeof(*this));
	printf("data_t::move_operator\n");
	return *this;
}
//コピーオペレータ
data_t& data_t::operator=(const data_t& rhs)
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
//双方向連結リストコンテナテスト
void example_linked_list()
{
	//--------------------
	//テスト①：基本ロジックテスト
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for linked_list::container(User defined type)]\n");

		//双方向連結リストコンテナ生成
		typedef linked_list::container<ope> container_t;
		container_t con;

		//データを表示
		auto printAll = [&con]()
		{
			printf("size=%d\n", con.size());
			printf("list=");
			if (con.empty())
			{
				printf("(empty)\n");
				return;
			}
			for (auto& data : con)
			{
				printf(" [%d:%d]", data.m_key, data.m_val);
			}
			printf("\n");
		};

		//データを逆順に表示
		auto printReverse = [&con]()
		{
			printf("size=%d\n", con.size());
			printf("list(reverse)=");
			if (con.empty())
			{
				printf("(empty)\n");
				return;
			}
			std::for_each(con.rbegin(), con.rend(),
				[](data_t& data)
				{
					printf(" [%d:%d]", data.m_key, data.m_val);
				}
			);
			printf("\n");
		};

		//末尾に連続プッシュ(1)
		auto continuous_push_back = [&con](const int num)
		{
			printf("\n");
			printf("[push_back * %d]\n", num);
			for (int i = 0; i < num; ++i)
			{
				const int key = i;
				const int val = 100 + i;
				printf("push_back(%d:%d) ... ", key, val);
				data_t* data = new data_t(key, val);
				if (con.push_back(*data))
					printf("OK\n");
				else
				{
					delete data;
					printf("NG!\n");
				}
			}
		};
		continuous_push_back(10);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//先頭から連続ポップ(1)
		auto continuous_pop_front = [&con](const int num)
		{
			printf("\n");
			printf("[pop_front * %d]\n", num);
			for (int i = 0; i < num; ++i)
			{
				printf("pop_front() ... ");
				data_t* data = con.pop_front();
				if (data)
				{
					printf("OK [%d:%d]\n", data->m_key, data->m_val);
					delete data;
				}
				else
					printf("NG!\n");
			}
		};
		continuous_pop_front(5);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//末尾に連続プッシュ(2)
		continuous_push_back(20);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//先頭から連続ポップ(2)
		continuous_pop_front(25);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//先頭に連続プッシュ(1)
		auto continuous_push_front = [&con](const int num)
		{
			printf("\n");
			printf("[push_front * %d]\n", num);
			for (int i = 0; i < num; ++i)
			{
				const int key = 1000 + i;
				const int val = 100 + i;
				printf("push_front(%d:%d) ... ", key, val);
				data_t* data = new data_t(key, val);
				if (con.push_front(*data))
					printf("OK\n");
				else
				{
					delete data;
					printf("NG!\n");
				}
			}
		};
		continuous_push_front(10);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//末尾から連続ポップ(1)
		auto continuous_pop_back = [&con](const int num)
		{
			printf("\n");
			printf("[pop_back * %d]\n", num);
			for (int i = 0; i < num; ++i)
			{
				printf("pop_back() ... ");
				data_t* data = con.pop_back();
				if (data)
				{
					printf("OK [%d:%d]\n", data->m_key, data->m_val);
					delete data;
				}
				else
					printf("NG!\n");
			}
		};
		continuous_pop_back(5);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//先頭に連続プッシュ(2)
		continuous_push_front(20);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//末尾から連続ポップ(2)
		continuous_pop_back(25);
		printAll();//全件表示
		//printReverse();//全件逆順表示

		//データ登録：push_back()メソッド
		printf("\n");
		printf("[push_back(1)]\n");
		auto push_back = [&con](const int key, const int val)
		{
			data_t* data = new data_t(key, val);
			con.push_back(*data);
		};
		push_back(5, 101);
		push_back(8, 102);
		push_back(3, 103);
		push_back(1, 104);
		push_back(7, 105);
		push_back(4, 106);
		push_back(13, 107);
		push_back(10, 108);
		push_back(5, 109);
		
		//データを表示
		printAll();

		//データを逆順に表示
		printReverse();

		//ソ―ト
		printf("\n");
		printf("[sort]\n");
		con.sort();//通常ソート
		printAll();//全件表示

		//逆順にソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom sort]\n");
		auto reverse_pred = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key > rhs.m_key; };
		con.sort(reverse_pred);//通常ソート
		printAll();//全件表示

	#ifdef GASHA_LINKED_LIST_ENABLE_STABLE_SORT
		//安定ソ―ト
		printf("\n");
		printf("[stable_sort]\n");
		con.stableSort();//安定ソート
		printAll();//全件表示

		//逆順にソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom stable_sort]\n");
		//auto reverse_pred = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key > rhs.m_key; };
		con.stableSort(reverse_pred);//安定ソート
		printAll();//全件表示
	#endif//GASHA_LINKED_LIST_ENABLE_STABLE_SORT

	#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
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
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2: key=%d, value=%d\n", ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2: key=%d, value=%d\n", rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end: key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end: key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
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
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2: key=%d, value=%d\n", ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2: key=%d, value=%d\n", rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end: key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end: key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
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
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
			printf("[--ite,++ie_end]\n");
			--ite;
			--rite;
			++ite_end;
			++rite_end;
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
			for (int i = 0; i < 3; ++i)
			{
				printf("[ite[%d]]\n", i);
				ite = ite[i];
				rite = rite[i];
				if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
				if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			}
			printf("[ite+=3]\n");
			ite += 3;
			rite += 3;
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			printf("[ite-=3]\n");
			ite -= 3;
			rite -= 3;
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
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
	#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE

		//線形探索
		printf("\n");
		printf("[find]\n");
		printAll();//全件表示
		auto find = [&con](const int key)
		{
			printf("findValue(key=%d)=", key);
		#ifdef USE_STL_ALGORITM
			auto ite = std::find(con.begin(), con.end(), key);//線形探索(STL版)
		#else//USE_STL_ALGORITM
			auto ite = con.findValue(key);//線形探索
		#endif//USE_STL_ALGORITM
			if (ite.isExist())
			{
				printf(" [%d:%d]", ite->m_key, ite->m_val);
				++ite;
				if (ite.isExist())
					printf(" next=[%d:%d]", ite->m_key, ite->m_val);
			}
			else
				printf("(not found)");
			printf("\n");
		};
		find(5);
		find(6);
		find(7);

	#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
		//二分探索（ソート前）
		printf("\n");
		printf("[binary search(before sort)]\n");
		printAll();//全件表示
		auto binary_search = [&con](const int key)
		{
			printf("binarySearchValue(key=%d)=", key);
		#ifdef USE_STL_ALGORITM
			if (std::binary_search(con.begin(), con.end(), key))//二分探索(STL版)
			{
				auto ite = std::lower_bound(con.begin(), con.end(), key);
		#else//USE_STL_ALGORITM
			auto ite = con.binarySearchValue(key);//二分探索
			if (ite.isExist())
			{
		#endif//USE_STL_ALGORITM
				printf(" [%d:%d]", ite->m_key, ite->m_val);
				--ite;
				if (ite.isExist())
					printf(" prev=[%d:%d]", ite->m_key, ite->m_val);
			}
			else
				printf("(not found)");
			printf("\n");
		};
		binary_search(5);
		binary_search(6);
		binary_search(7);

		//二分探索（ソート後）
		printf("\n");
		printf("[binary search(after sort)]\n");
		con.sort();//ソート済み状態にする
		printAll();//全件表示
		binary_search(5);
		binary_search(6);
		binary_search(7);
		
		//※二分探索前の状態に戻す
		con.sort(reverse_pred);//通常ソート
	#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH

		//削除１：削除ノードで指定
		printf("\n");
		printf("[remove]\n");
		{
			auto ite = con.begin();
		#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
			ite += 2;
		#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE
			data_t* del = con.remove(*ite);//前から3つ目のノードを削除
			if (del)
				delete del;
			printAll();//全件表示
		}

		//削除２：イテレータで指定
		printf("\n");
		printf("[erase(1)]\n");
		{
			auto ite = con.end();
		#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
			ite -= 4;
		#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE
			data_t* del = con.erase(ite);//後ろから4つ目の位置のノードを削除
			if (del)
				delete del;
			printAll();//全件表示
		}

		//削除３：イテレータの範囲で指定
		printf("\n");
		printf("[erase(2)]\n");
		{
			auto start = con.end();
		#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
			start -= 4;
		#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE
			auto end = start;
		#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
			end += 2;
		#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE
			const data_t* del = con.erase(start, end);//後ろから4つ目の位置から2つのノードを削除
			while (del)
			{
				const data_t* next = del->m_next;
				delete del;
				del = next;
			}
			printAll();//全件表示
		}

		//削除４：イテレータの範囲で指定（末尾まで削除）
		printf("\n");
		printf("[erase(3)]\n");
		{
			auto start = con.end();
		#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
			start -= 2;
		#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE
			auto end = con.end();
			const data_t* del = con.erase(start, end);//後ろから2つ目の位置から末尾までのノードを削除
			while (del)
			{
				const data_t* next = del->m_next;
				delete del;
				del = next;
			}
			printAll();//全件表示
		}

		//挿入１：insert()メソッド
		printf("\n");
		printf("[insert]\n");
		{
			auto ite = con.begin();
			++ite;
			data_t* data = new data_t(99, 999);
			con.insert(ite, *data);//先頭から2つ目の位置に挿入
			printAll();//全件表示
		}

		//挿入２：insert_before()メソッド
		printf("\n");
		printf("[insert_before]\n");
		{
			auto ite = con.begin();
		#if defined(GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE)
			ite += 3;
		#endif//GASHA_LINKED_LIST_ENABLE_RANDOM_ACCESS_INTERFACE
			data_t* data = new data_t(88, 888);
			con.insert_before(ite, *data);//先頭から4つ目の位置の前に挿入
			printAll();//全件表示
		}

		//クリア
		printf("\n");
		printf("[clear]\n");
		{
			const data_t* del = con.clear();
			while (del)
			{
				const data_t* next = del->m_next;
				delete del;
				del = next;
			}
			printAll();//全件表示
			//printReverse();//全件逆順表示
		}
	}

	//--------------------
	//テスト②：ソート、探索の設定を変える
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for linked_list::container(User defined type with custom operator type)]\n");

		//双方向連結リストコンテナ生成
		typedef linked_list::container<another_ope> container_t;
		container_t con;

		//データ登録１：push_back()メソッド
		printf("\n");
		printf("[push_back]\n");
		auto push_back = [&con](const int key, const int val)
		{
			data_t* data = new data_t(key, val);
			con.push_back(*data);
		};
		push_back(1, 105);
		push_back(1, 108);
		push_back(2, 103);
		push_back(2, 101);
		push_back(3, 107);
		push_back(3, 104);
		push_back(4, 113);
		push_back(4, 111);
		push_back(5, 105);
		push_back(5, 106);

		//データを表示
		auto printAll = [&con]()
		{
			printf("size=%d\n", con.size());
			printf("list=");
			if (con.empty())
			{
				printf("(empty)\n");
				return;
			}
			for (auto& data : con)
			{
				printf(" [%d:%d]", data.m_key, data.m_val);
			}
			printf("\n");
		};
		printAll();

		//ソ―ト
		printf("\n");
		printf("[sort]\n");
		con.sort();//通常ソート
		//con.stableSort();//安定ソート
		printAll();//全件表示

		//線形探索
		printf("\n");
		printf("[find]\n");
		auto find = [&con](const int value)
		{
			printf("findValue(value=%d)=", value);
			auto ite = con.findValue(value);//線形探索
			if (ite.isExist())
				printf(" [%d:%d]", ite->m_key, ite->m_val);
			else
				printf("(not found)");
			printf("\n");
		};
		find(101);
		find(110);
		find(103);

	#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
		//二分探索
		printf("\n");
		printf("[binary search]\n");
		auto binary_search = [&con](const int value)
		{
			printf("binarySearchValue(value=%d)=", value);
			auto ite = con.binarySearchValue(value);//二分探索
			if (ite.isExist())
				printf(" [%d:%d]", ite->m_key, ite->m_val);
			else
				printf("(not found)");
			printf("\n");
		};
		binary_search(101);
		binary_search(110);
		binary_search(103);
	#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
		
		//カスタムソート
		{
			printf("\n");
			printf("[sort with custom predicate]\n");
			auto predicate = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key < rhs.m_key ? true : lhs.m_key == rhs.m_key ? lhs.m_val > rhs.m_val : false; };
			con.sort(predicate);//通常ソート
			//con.stableSort(reverse_pred);//安定ソート
			printAll();//全件表示
		}

		//カスタム線形探索(1)
		printf("\n");
		printf("[find with custom predicate(1)]\n");
		auto custom_find1 = [&con](const int key, const int value)
		{
			printf("find(key=%d, value=%d)=", key, value);
			auto predicate = [&key, &value](const data_t& lhs) -> bool { return lhs.m_key == key && lhs.m_val == value; };
			auto ite = con.find(predicate);//線形探索
			if (ite.isExist())
				printf(" [%d:%d]", ite->m_key, ite->m_val);
			else
				printf("(not found)");
			printf("\n");
		};
		custom_find1(2, 101);
		custom_find1(2, 102);
		custom_find1(2, 103);

	#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
		//カスタム二分探索(1)
		printf("\n");
		printf("[binary search with custom comparison(1)]\n");
		auto custom_binary_search1 = [&con](const int key, const int value)
		{
			printf("binarySearchValue(key=%d, value=%d)=", key, value);
			auto comparison = [&key, &value](const data_t& lhs) -> int { return key == lhs.m_key ? lhs.m_val - value : key > lhs.m_key ? 1 : -1; };
			auto ite = con.binarySearch(comparison);//二分探索
			if (ite.isExist())
				printf(" [%d:%d]", ite->m_key, ite->m_val);
			else
				printf("(not found)");
			printf("\n");
		};
		custom_binary_search1(2, 101);
		custom_binary_search1(2, 102);
		custom_binary_search1(2, 103);
	#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH

		//カスタム線形探索(2)
		printf("\n");
		printf("[find with custom predicate(2)]\n");
		auto custom_find2 = [&con](const int key)
		{
			printf("find(key=%d)=", key);
			auto predicate = [](const data_t& lhs, const int key) -> bool { return lhs.m_key == key; };
			auto ite = con.findValue(key, predicate);//線形探索
			if (ite.isExist())
				printf(" [%d:%d]", ite->m_key, ite->m_val);
			else
				printf("(not found)");
			printf("\n");
		};
		custom_find2(2);
		custom_find2(7);
		custom_find2(3);

	#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
		//カスタム二分探索(2)
		printf("\n");
		printf("[binary search with custom comparison(2)]\n");
		auto custom_binary_search2 = [&con](const int key)
		{
			printf("binarySearchValue(key=%d)=", key);
			auto comparison = [](const data_t& lhs, const int key) -> int { return key - lhs.m_key; };
			auto ite = con.binarySearchValue(key, comparison);//二分探索
			if (ite.isExist())
				printf(" [%d:%d]", ite->m_key, ite->m_val);
			else
				printf("(not found)");
			printf("\n");
		};
		custom_binary_search2(2);
		custom_binary_search2(7);
		custom_binary_search2(3);
	#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
	}

	//--------------------
	//テスト③：大量登録テスト
	{
		//経過時間を表示
		auto printElapsedTime = [](const std::chrono::system_clock::time_point& prev_time, const bool is_show) -> std::chrono::system_clock::time_point
		{
			//最終経過時間表示
			const auto now_time = std::chrono::system_clock::now();
			const auto duration = now_time - prev_time;
			const double elapsed_time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000000.;
			if (is_show)
				printf("*elapsed_time=%.9lf sec\n", elapsed_time);
			return now_time;
		};

		{
			printf("--------------------------------------------------------------------------------\n");
			printf("[Test for performance linked_list]\n");

			const std::chrono::system_clock::time_point begin_time = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point prev_time = begin_time;

			//データを初期化
			printf("\n");
			printf("[create container]\n");
			typedef linked_list::container<ope> container_t;
			container_t* con = new container_t;//コンテナ生成
			prev_time = printElapsedTime(prev_time, true);

			//データを登録
			printf("\n");
			printf("[push_back() * %d]\n", TEST_DATA_NUM);
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; ++i)
				{
					data_t* data = new data_t(i, 10000000 + i);
					con->push_back(*data);
					++num;
				}
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

			//イテレータ(1)
			printf("\n");
			printf("[iterator(1)]\n");
			{
				printf_detail("size=%d\n", con->size());
				printf_detail("list=");
				if (con->empty())
					printf_detail("(empty)");
				int num = 0;
				for (const data_t& value : *con)
				{
					printf_detail(" [%d:%d]", value.m_key, value.m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef ENABLE_SORT_TEST
			//逆順ソート
			printf("\n");
			printf("[reverse sort]\n");
			auto reverse_sort = [](const data_t& lhs, const data_t& rhs){return lhs.m_key > rhs.m_key; };
			con->sort(reverse_sort);
			assert(con->isOrdered(reverse_sort));
			prev_time = printElapsedTime(prev_time, true);
		#endif//ENABLE_SORT_TEST

			//イテレータ(2)
			printf("\n");
			printf("[iterator(2)]\n");
			{
				printf_detail("size=%d\n", con->size());
				printf_detail("list=");
				if (con->empty())
					printf_detail("(empty)");
				int num = 0;
				forEach(*con, [&num](const data_t& value)
					{
						printf_detail(" [%d:%d]", value.m_key, value.m_val);
						++num;
					}
				);
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef ENABLE_SORT_TEST
			//正順ソート
			printf("\n");
			printf("[sort]\n");
			con->sort();
			assert(con->isOrdered());
			prev_time = printElapsedTime(prev_time, true);
		#endif//ENABLE_SORT_TEST

			//リバースイテレータ
			printf("\n");
			printf("[reverse_iterator]\n");
			{
				printf_detail("size=%d\n", con->size());
				printf_detail("list=");
				if (con->empty())
					printf_detail("(empty)");
				int num = 0;
				reverseForEach(*con, [&num](const data_t& value)
					{
						printf_detail(" [%d:%d]", value.m_key, value.m_val);
						++num;
					}
				);
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef ENABLE_STABLE_SORT_TEST
		#ifdef GASHA_LINKED_LIST_ENABLE_STABLE_SORT
			//逆順安定ソート
			printf("\n");
			printf("[reverse stable sort]\n");
			con->stableSort(reverse_sort);
			assert(con->isOrdered(reverse_sort));
			prev_time = printElapsedTime(prev_time, true);

			//正順安定ソート
			printf("\n");
			printf("[stable sort]\n");
			con->stableSort();
			assert(con->isOrdered());
			prev_time = printElapsedTime(prev_time, true);
		#endif//GASHA_LINKED_LIST_ENABLE_STABLE_SORT
		#endif//ENABLE_STABLE_SORT_TEST

			//線形探索
			printf("\n");
			printf("[findValue]\n");
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; i += TEST_DATA_FIND_STEP)
				{
					container_t::iterator ite = std::move(con->findValue(i));
					printf_detail(" [%d:%d]", ite->m_key, ite->m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
			//二分探索
			printf("\n");
			printf("[binarySearchValue]\n");
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; i += TEST_DATA_BINARY_SEARCH_STEP)
				{
					container_t::iterator ite = std::move(con->binarySearchValue(i));
					printf_detail(" [%d:%d]", ite->m_key, ite->m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);
		#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH

			//データを破棄
			printf("\n");
			printf("[delete container]\n");
			{
				const data_t* data = con->clear();
				while (data)
				{
					const data_t* next = data->m_next;
					delete data;
					data = next;
				}
			}
			delete con;//コンテナを破棄
			con = nullptr;
			prev_time = printElapsedTime(prev_time, true);

			//総時間
			printf("\n");
			printf("[finish]\n");
			printElapsedTime(begin_time, true);
		}

		{
			printf("--------------------------------------------------------------------------------\n");
			printf("[Test for performance std::list]\n");

			const std::chrono::system_clock::time_point begin_time = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point prev_time = begin_time;

			//データを初期化
			printf("\n");
			printf("[create container]\n");
			typedef std::list<data_t> container_t;
			container_t* con = new container_t();//std::listコンテナを生成
			prev_time = printElapsedTime(prev_time, true);

			//データを登録
			printf("\n");
			printf("[push_back() * %d]\n", TEST_DATA_NUM);
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; ++i)
				{
					data_t value(i, 10000000 + i);
					con->push_back(std::move(value));
					++num;
				}
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

			//イテレータ(1)
			printf("\n");
			printf("[iterator(1)]\n");
			{
				printf_detail("size=%d, max_size=%d\n", con->size(), con->max_size());
				printf_detail("array=");
				if (con->empty())
					printf_detail("(empty)");
				int num = 0;
				for (const data_t& value : *con)
				{
					printf_detail(" [%d:%d]", value.m_key, value.m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef ENABLE_SORT_TEST
			//逆順ソート
			printf("\n");
			printf("[reverse sort]\n");
			auto reverse_sort = [](const data_t& lhs, const data_t& rhs){return lhs.m_key > rhs.m_key; };
			con->sort(reverse_sort);
			prev_time = printElapsedTime(prev_time, true);
		#endif//ENABLE_SORT_TEST

			//イテレータ(2)
			printf("\n");
			printf("[iterator(2)]\n");
			{
				printf_detail("size=%d, max_size=%d\n", con->size(), con->max_size());
				printf_detail("array=");
				if (con->empty())
					printf_detail("(empty)");
				int num = 0;
				std::for_each(con->begin(), con->end(), [&num](const data_t& value)
					{
						printf_detail(" [%d:%d]", value.m_key, value.m_val);
						++num;
					}
				);
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef ENABLE_SORT_TEST
			//正順ソート
			printf("\n");
			printf("[sort]\n");
			con->sort();
			prev_time = printElapsedTime(prev_time, true);
		#endif//ENABLE_SORT_TEST

			//リバースイテレータ
			printf("\n");
			printf("[reverse_iterator]\n");
			{
				printf_detail("size=%d, max_size=%d\n", con->size(), con->max_size());
				printf_detail("array=");
				if (con->empty())
					printf_detail("(empty)");
				int num = 0;
				std::for_each(con->rbegin(), con->rend(), [&num](const data_t& value)
					{
						printf_detail(" [%d:%d]", value.m_key, value.m_val);
						++num;
					}
				);
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef ENABLE_STABLE_SORT_TEST
		#ifdef GASHA_LINKED_LIST_ENABLE_STABLE_SORT
			//逆順安定ソート
			printf("\n");
			printf("[reverse stable sort] ... is not available.\n");

			//正順安定ソート
			printf("\n");
			printf("[stable sort] ... is not available.\n");
		#endif//GASHA_LINKED_LIST_ENABLE_STABLE_SORT
		#endif//ENABLE_STABLE_SORT_TEST

			//線形探索
			printf("\n");
			printf("[findValue]\n");
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; i += TEST_DATA_FIND_STEP)
				{
					container_t::iterator ite = std::find(con->begin(), con->end(), i);
					printf_detail(" [%d:%d]", ite->m_key, ite->m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

		#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
			//二分探索
			printf("\n");
			printf("[binarySearchValue]\n");
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; i += TEST_DATA_BINARY_SEARCH_STEP)
				{
					container_t::iterator ite = std::lower_bound(con->begin(), con->end(), i);
					printf_detail(" [%d:%d]", ite->m_key, ite->m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);
		#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH

			//データを破棄
			printf("\n");
			printf("[delete container]\n");
			delete con;//std::listコンテナを破棄
			con = nullptr;
			prev_time = printElapsedTime(prev_time, true);

			//総時間
			printf("\n");
			printf("[finish]\n");
			printElapsedTime(begin_time, true);
		}
	}

	printf("\n");
	printf("- end -\n");
}

// End of file
