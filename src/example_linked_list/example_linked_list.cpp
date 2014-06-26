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

#include <algorithm>//std::for_each用
#include <chrono>//C++11 時間計測用
#include <vector>//std::vector用（比較用）
#include <assert.h>//assert用

#include <stdio.h>//printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//双方向連結リストテスト
//--------------------------------------------------------------------------------

#include <algorithm>//std::for_each用
#include <chrono>//C++11 時間計測用
#include <list>//std::list用（比較用）
#include <assert.h>//assert用

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
#ifdef PRINT_TEST_DATA_SEARCH
template<typename... Tx>
inline int printf_dbg_search(const char* fmt, Tx... args)
{
	return printf(fmt, args...);
}
#else//PRINT_TEST_DATA_SEARCH
inline int printf_dbg_search(const char* fmt, ...){ return 0; }
#endif//PRINT_TEST_DATA_SEARCH

//----------------------------------------
//テストデータ
struct data_t
{
	mutable const data_t* m_prev;//前ノード
	mutable const data_t* m_next;//次ノード
	
	int m_key;//キー
	int m_val;//値
	
	//コンストラクタ
	data_t(const int key, const int val) :
		m_next(nullptr),
		m_prev(nullptr),
		m_key(key),
		m_val(val)
	{}
	data_t() :
		m_next(nullptr),
		m_prev(nullptr),
		m_key(0),
		m_val(0)
	{}

	//デフォルトのソート用の比較演算子（必須ではない）
	inline bool operator<(const data_t& rhs) const
	{
		return m_key < rhs.m_key;
	}
	//デフォルトの線形／二分探索用の比較演算子（必須ではない）
	inline bool operator==(const int key) const
	{
		return m_key == key;
	}
#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
	inline bool operator<(const int key) const
	{
		return m_key < key;
	}
#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
};
#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
//※std::binary_searchを使用する場合は、このオペレータも必要
static bool operator<(const int key, const data_t& rhs)
{
	return key < rhs.m_key;
}
#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH

//----------------------------------------
//テストデータ向けノード操作用クラス（CRTP）
struct ope_t : public linked_list::baseOpe_t<ope_t, data_t>
{
	//前ノードを取得
	inline static const node_type* getPrev(const node_type& node){ return node.m_prev; }
	//前ノードを変更
	inline static void setPrev(node_type& node, const node_type* prev){ node.m_prev = prev; }
	
	//次ノードを取得
	inline static const node_type* getNext(const node_type& node){ return node.m_next; }
	//次ノードを変更
	inline static void setNext(node_type& node, const node_type* next){ node.m_next = next; }

	//ロック型
	//※デフォルト（dummy_shared_lock）のままとする
	//typedef shared_spin_lock lock_type;//ロックオブジェクト型
};

//----------------------------------------
//テストデータ操作クラス②：ソート／探索方法をデフォルトから変える
struct another_ope_t : public ope_t
{
	//ソート用プレディケート関数オブジェクト
	//※m_valメンバーを基準にソート
	struct predicateForSort{
		inline bool operator()(const node_type& lhs, const node_type& rhs) const
		{
			return lhs.m_val < rhs.m_val;
		}
	};

	//線形探索用プレディケート関数オブジェクト
	//※m_valメンバーを探索
	struct predicateForFind{
		inline bool operator()(const node_type& lhs, const int rhs) const
		{
			return lhs.m_val == rhs;
		}
	};

#ifdef GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
	//二分探索用比較関数オブジェクト
	//※m_valメンバーを比較
	struct comparisonForSearch{
		inline int operator()(const node_type& lhs, const int rhs) const
		{
			return rhs - lhs.m_val;
		}
	};
#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH
};

//----------------------------------------
//テストメイン
int main(const int argc, const char* argv[])
{
	//--------------------
	//テスト①：基本ロジックテスト
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for linked_list::container(User defined type)]\n");

		//双方向連結リストコンテナ生成
		typedef linked_list::container<ope_t> container_t;
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
		//con.stableSort();//安定ソート
		printAll();//全件表示

		//逆順にソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom sort]\n");
		auto reverse_pred = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key > rhs.m_key; };
		con.sort(reverse_pred);//通常ソート
		//con.stableSort(reverse_pred);//安定ソート
		printAll();//全件表示

	#if 0
		{
			printf("\n");
			printf("constructor\n");
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
			printf("copy operator\n");
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
				if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
				if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
				if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
				if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
				if (ite2.isExist()) printf("ite2: key=%d, value=%d\n", ite2->m_key, ite2->m_val);
				if (rite2.isExist()) printf("rite2: key=%d, value=%d\n", rite2->m_key, rite2->m_val);
				if (ite2_end.isExist()) printf("ite2_end: key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
				if (rite2_end.isExist()) printf("rite2_end: key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
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
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2: key=%d, value=%d\n", ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2: key=%d, value=%d\n", rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end: key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end: key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
			printf("-= 3\n");
			ite -= 3;
			rite -= 3;
			ite_end -= 3;
			rite_end -= 3;
			ite2 -= 3;
			rite2 -= 3;
			ite2_end -= 3;
			rite2_end -= 3;
			if (ite.isExist()) printf("ite: key=%d, value=%d\n", ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite: key=%d, value=%d\n", rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end: key=%d, value=%d\n", ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end: key=%d, value=%d\n", rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2: key=%d, value=%d\n", ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2: key=%d, value=%d\n", rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end: key=%d, value=%d\n", ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end: key=%d, value=%d\n", rite2_end->m_key, rite2_end->m_val);
		}
	#endif

		//線形探索
		printf("\n");
		printf("[find]\n");
		printAll();//全件表示
		auto find = [&con](const int key)
		{
			printf("findValue(key=%d)=", key);
			auto ite = con.findValue(key);//線形探索
			//auto ite = std::find(con.begin(), con.end(), key);//線形探索(STL版)
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
			auto ite = con.binarySearchValue(key);//二分探索
			if (ite.isExist())
			{
			//if (std::binary_search(con.begin(), con.end(), key))//二分探索(STL版)
			//{
			//	auto ite = std::lower_bound(con.begin(), con.end(), key);
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
	#endif//GASHA_LINKED_LIST_ENABLE_BINARY_SEARCH

	#if 0
		con.sort(reverse_pred);//通常ソート
		printAll();//全件表示
		con.sort();//通常ソート
		printAll();//全件表示
		con.stableSort(reverse_pred);//安定ソート
		printAll();//全件表示
		con.stableSort();//安定ソート
		printAll();//全件表示
		find(1);
		find(2);
		find(3);
		binary_search(1);
		binary_search(2);
		binary_search(3);
	#endif

		//削除１：削除ノードで指定
		printf("\n");
		printf("[remove]\n");
		{
			auto ite = con.begin();
			ite += 2;
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
			ite -= 4;
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
			start -= 4;
			auto end = start;
			end += 2;
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
			start -= 2;
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
			ite += 3;
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
		typedef linked_list::container<another_ope_t> container_t;
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
			auto ite = con.binary_search(comparison);//二分探索
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
				printf("*elapsed_time=%.9llf sec\n", elapsed_time);
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
			typedef linked_list::container<ope_t> container_t;
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

		#ifdef ENABLE_SORT_TEST
		#ifdef ENABLE_STABLE_SORT
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
		#endif//ENABLE_STABLE_SORT
		#endif//ENABLE_SORT_TEST

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
				for (int i = 0; i < TEST_DATA_NUM; i += TEST_DATA_FIND_STEP)
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

		#ifdef ENABLE_SORT_TEST
		#ifdef ENABLE_STABLE_SORT
			//逆順ソート　※安定ソートの代わり
			printf("\n");
			printf("[reverse (stable) sort]\n");
			con->sort(reverse_sort);
			prev_time = printElapsedTime(prev_time, true);

			//正順ソート　※安定ソートの代わり
			printf("\n");
			printf("[(stable) sort]\n");
			con->sort();
			prev_time = printElapsedTime(prev_time, true);
		#endif//ENABLE_STABLE_SORT
		#endif//ENABLE_SORT_TEST

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
				for (int i = 0; i < TEST_DATA_NUM; i += TEST_DATA_FIND_STEP)
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

	return EXIT_SUCCESS;
}

#endif

//----------------------------------------
//双方向連結リストテスト
void example_linked_list()
{
}

// End of file
