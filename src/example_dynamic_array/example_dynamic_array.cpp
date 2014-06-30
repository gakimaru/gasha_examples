//--------------------------------------------------------------------------------
// exmaple_dynamic_array.cpp
// 動的配列コンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_dynamic_array.h"//動的配列コンテナテスト

#include <gasha/dynamic_array.inl>//動的配列コンテナ【インライン関数／テンプレート関数定義部】

#include <gasha/iterator.h>//イテレータ操作

#include <utility>//C++11 std::move, std::forward
#include <chrono>//C++11 std::chrono
#include <stdio.h>//printf()

#include <assert.h>//assert()

//【VC++】例外を無効化した状態で <algorithm> <vector> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::for_each()
#include <vector>//std::vector（比較用）

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テストデータ

//コンストラクタ
data_t::data_t(const int key, const int val) :
	m_key(key),
	m_val(val)
{
#ifdef TEST_DATA_WATCH_CONSTRUCTOR
	printf("data_t::constructor(%d, %d)\n", key, val);
#endif//TEST_DATA_WATCH_CONSTRUCTOR
}
//デフォルトコンストラクタ
data_t::data_t() :
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

//----------------------------------------
//スレッドテスト用関数の参照
template <class C>
extern void testThread(const char* container_type);

//----------------------------------------
//動的配列コンテナテスト
void example_dynamic_array()
{
	//--------------------
	//テスト①：基本ロジックテスト：プリミティブ型の配列を扱う場合
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for dynamic_array::container(Primitive type)]\n");

		int arr[20];//配列

		//コンテナ生成
		//※既存の配列を渡して動的配列コンテナとして扱う
		dynamic_array::container<int_ope_t> con(arr);//配列要素数を自動取得

		//データを表示
		auto printAll = [&con]()
		{
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			printf("array=");
			if (con.empty())
				printf("(empty)");
			for (auto val : con)
				printf(" %d", val);
			printf("\n");
		};

		//データを逆順に表示
		auto printReverse = [&con]()
		{
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			printf("array(reverse)=");
			if (con.empty())
				printf("(empty)");
			std::for_each(con.rbegin(), con.rend(),
				[](int val)
				{
					printf(" %d", val);
				}
			);
			printf("\n");
		};

		//値を追加
		printf("\n");
		printf("[push_back]\n");
		con.push_back(5);
		con.push_back(8);
		con.push_back(3);
		con.push_back(1);
		con.push_back(7);
		con.push_back(4);
		con.push_back(13);
		con.push_back(10);
		con.push_back(5);

		//データを表示
		printAll();

		//データを逆順に表示
		printReverse();

		//リサイズ（拡大）
		printf("\n");
		printf("[resize(expand)]\n");
		con.resize(12, 1000);//12件に拡張
		printAll();//全件表示

		//ソ―ト
		printf("\n");
		printf("[sort]\n");
	#ifdef USE_STL_ALGORITM
		std::sort(con.begin(), con.end());//高速ソート(STL版)
	#else//USE_STL_ALGORITM
		con.sort();//高速ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//逆順にソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom sort]\n");
		auto reverse_pred = [](const int lhs, const int rhs) -> bool {return lhs > rhs; };
	#ifdef USE_STL_ALGORITM
		std::sort(con.begin(), con.end(), reverse_pred);//高速ソート(STL版)
	#else//USE_STL_ALGORITM
		con.sort(reverse_pred);//高速ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//安定ソ―ト
		printf("\n");
		printf("[stable_sort]\n");
	#ifdef USE_STL_ALGORITM
		std::stable_sort(con.begin(), con.end());//安定ソート(STL版)
	#else//USE_STL_ALGORITM
		con.stableSort();//安定ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//逆順に安定ソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom stable_sort]\n");
		//auto reverse_pred = [](const int lhs, const int rhs) -> bool {return lhs > rhs; };
	#ifdef USE_STL_ALGORITM
		std::stable_sort(con.begin(), con.end(), reverse_pred);//安定ソート(STL版)
	#else//USE_STL_ALGORITM
		con.stableSort(reverse_pred);//安定ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//ポップ(1)
		printf("\n");
		printf("[pop_back(1)]\n");
		{
			const int* val = con.back();//末尾の値を取得
			printf("back=%d\n", *val);
			con.pop_back();//末尾を削除
			printAll();//全件表示
		}

		//ポップ(2)
		printf("\n");
		printf("[pop_back(2)]\n");
		{
			int pop_val = 0;
			con.pop_back(pop_val);//値のコピーを受け取って末尾を削除
			printf("pop_back=[%d]\n", pop_val);
			printAll();//全件表示
		}

		//線形探索
		printf("\n");
		printf("[find]\n");
		printAll();//全件表示
		auto find = [&con](const int val)
		{
			printf("findValue(%d)=", val);
		#ifdef USE_STL_ALGORITM
			auto ite = std::find(con.begin(), con.end(), val);//線形探索(STL版)
		#else//USE_STL_ALGORITM
			auto ite = con.findValue(val);//線形探索
		#endif//USE_STL_ALGORITM
			if (ite.isExist())
			{
				printf("%d", *ite);
				++ite;
				if (ite.isExist())
					printf(" next=%d", *ite);
			}
			else
				printf("(not found)");
			printf("\n");
		};
		find(5);
		find(6);
		find(7);

		//二分探索（ソート前）
		printf("\n");
		printf("[binary search(before sort)]\n");
		printAll();//全件表示
		auto binary_search = [&con](const int val)
		{
			printf("binarySearchValue(%d)=", val);
		#ifdef USE_STL_ALGORITM
			if (std::binary_search(con.begin(), con.end(), val))//二分探索(STL版)
			{
				auto ite = std::lower_bound(con.begin(), con.end(), val);
		#else//USE_STL_ALGORITM
			auto ite = con.binarySearchValue(val);//二分探索
			if (ite.isExist())
			{
		#endif//USE_STL_ALGORITM
				printf("%d", *ite);
				--ite;
				if (ite.isExist())
					printf(" prev=%d", *ite);
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

		//削除１：イテレータと数で指定
		printf("\n");
		printf("[erase(1)]\n");
		{
			auto ite = con.end();
			ite -= 4;
			con.erase(ite, 2);//後ろから4つ目の位置から2件削除
			printAll();//全件表示
		}

		//削除２：イテレータの範囲で指定
		printf("\n");
		printf("[erase(2)]\n");
		{
			auto start = con.end();
			start -= 4;
			auto end = start;
			end += 2;
			con.erase(start, end);//後ろから4つ目の位置から2件削除
			printAll();//全件表示
		}

		//挿入
		printf("\n");
		printf("[insert]\n");
		{
			auto ite = con.begin();
			++ite;
			con.insert(ite, 5, 999);//先頭から2つ目の位置に5件挿入
			printAll();//全件表示
		}

		//リサイズ（縮小）
		printf("\n");
		printf("[resize(shrink)]\n");
		con.resize(5);//要素数=5に縮小
		printAll();//全件表示

		//最大サイズを縮小
		printf("\n");
		printf("[shrink_to_fit]\n");
		con.shrink_to_fit();
		printAll();//全件表示

		//データ割り当て
		printf("\n");
		printf("[assign]\n");
		con.assign(-1, 0);//※-1で最大要素数全件に割り当て
		printAll();//全件表示

		//クリア
		printf("\n");
		printf("[clear]\n");
		con.clear();
		printAll();//全件表示
		//printReverse();//全件逆順表示
	}

	//--------------------
	//テスト②：基本ロジックテスト：ユーザー定義型を扱う場合
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for dynamic_array::container(User defined type)]\n");

		//配列データ
		data_t array[20];
		//※この宣言だと、デフォルトコンストラクタとデストラクタが呼び出される点に注意

		//動的配列コンテナ生成
		typedef dynamic_array::container<ope> container_t;
		container_t con(array);//※配列要素数を自動取得
		//container_t con(&array[0], 10);//※要素数を明示的に受け渡す方法
		//char buff[1024];
		//container_t con(buff, sizeof(buff), dynamic_array::AUTO_CLEAR);//バッファとバッファサイズを受け渡す方法＋コンテナ破棄時に自動クリア属性追加
		//                                                               //（コンテナのデストラクタで、残っている要素のデストラクタを呼び出す。デフォルトは自動クリアなし）
		//container_t con;//初期状態で配列の割り当てをせずにコンテナを生成する場合

		//後から配列を割り当てる場合は assignArray() を使用する
		//container_t con;//デフォルトコンストラクタ ※コンテナ生成時に配列を割り当てない
		//con.assignArray(array);//※配列要素数を自動取得
		//con.assignArray(&array[0], 10);//※要素数を明示的に受け渡す方法
		//char buff[1024];
		//con.assignArray(buff, sizeof(buff), dynamic_array::AUTO_CLEAR);//バッファとバッファサイズを受け渡す方法＋コンテナ破棄時に自動クリア属性追加
		//                                                                //（コンテナのデストラクタで、残っている要素のデストラクタを呼び出す。デフォルトは自動クリアなし）

		//データを表示
		auto printAll = [&con]()
		{
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			printf("array=");
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
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			printf("array(reverse)=");
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
#if 0
		{
			con.push_back();
			con.clear();
			for (int i = 0; i < 100; ++i)
			{
				for (int j = 0; j <= i; ++j)
				{
					con.push_back(i, i * 100 + j);
				}
			}
			printAll();
			for (int i = 0; i < 100; ++i)
			{
				auto& ite = con.binarySearchValue(i);
				printf("binary_search(%d)=", i);
				if (ite.isExist())
				{
					printf("[%d:%d]", ite->m_key, ite->m_val);
					if (ite->m_val % 100 != 0)
						printf(" !!!\n");
				}
				printf("\n");
			}
			con.clear();
		}
#endif

		//データ登録１：push_back()メソッド＋コンストラクタパラメータ（コンストラクタ呼び出しを行う）
		printf("\n");
		printf("[push_back(1)]\n");
		con.push_back(5, 101);
		con.push_back(8, 102);
		con.push_back(3, 103);

		//データを表示
		printAll();

		//データを逆順に表示
		printReverse();

		//データ登録２：push_back()メソッド＋オブジェクト（オブジェクトのコピーを行う）
		printf("\n");
		printf("[push_back(2)]\n");
		{ data_t obj(1, 104); con.push_back(obj); }//コピーで追加
		{ data_t obj(7, 105); con.push_back(obj); }//コピーで追加
		{ data_t obj(4, 106); con.push_back(obj); }//コピーで追加
		con.push_back(data_t(13, 107));//ムーブ（右辺値）で追加
		con.push_back(data_t(10, 108));//ムーブ（右辺値）で追加
		con.push_back(data_t(5, 109));//ムーブ（右辺値）で追加
		printAll();//全件表示

		//リサイズ１：resize()メソッド＋コンストラクタパラメータ（コンストラクタ呼び出しを行う）
		printf("\n");
		printf("[resize(1)]\n");
		con.resize(12, 1000, 1000);//12件に拡張
		printAll();

		//リサイズ２：resize()メソッド＋オブジェクト（オブジェクトのコピーを行う）
		printf("\n");
		printf("[resize(2)]\n");
		{
			data_t prototype(1001, 1001);//コピー用のオブジェクト
			con.resize(15, prototype);//15件に拡張
		}
		printAll();

		//ソ―ト
		printf("\n");
		printf("[sort]\n");
	#ifdef USE_STL_ALGORITM
		std::sort(con.begin(), con.end());//高速ソート(STL版)
	#else//USE_STL_ALGORITM
		con.sort();//高速ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//逆順にソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom sort]\n");
		auto reverse_pred = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key > rhs.m_key; };
	#ifdef USE_STL_ALGORITM
		std::sort(con.begin(), con.end(), reverse_pred);//高速ソート(STL版)
	#else//USE_STL_ALGORITM
		con.sort(reverse_pred);//高速ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//安定ソ―ト
		printf("\n");
		printf("[stable_sort]\n");
	#ifdef USE_STL_ALGORITM
		std::stable_sort(con.begin(), con.end());//安定ソート(STL版)
	#else//USE_STL_ALGORITM
		con.stableSort();//安定ソート
	#endif//USE_STL_ALGORITM
		printAll();//全件表示

		//逆順に安定ソート ※カスタムプレディケート関数を使用
		printf("\n");
		printf("[custom stable_sort]\n");
		//auto reverse_pred = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key > rhs.m_key; };
	#ifdef USE_STL_ALGORITM
		std::stable_sort(con.begin(), con.end(), reverse_pred);//安定ソート(STL版)
	#else//USE_STL_ALGORITM
		con.stableSort(reverse_pred);//安定ソート
	#endif//USE_STL_ALGORITM
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
			if (ite.isExist()) printf("ite:[%d] key=%d, value=%d\n", ite.getIndex(), ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] key=%d, value=%d\n", rite.getIndex(), rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end:[%d] key=%d, value=%d\n", ite_end.getIndex(), ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end:[%d] key=%d, value=%d\n", rite_end.getIndex(), rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2:[%d] key=%d, value=%d\n", ite2.getIndex(), ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2:[%d] key=%d, value=%d\n", rite2.getIndex(), rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end:[%d] key=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end:[%d] key=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_key, rite2_end->m_val);
			printf("copy operator\n");
			ite = con.begin();
			rite = con.rbegin();
			ite_end = con.end();
			rite_end = con.rend();
			ite2 = con.rbegin();
			rite2 = con.begin();
			ite2_end = con.rend();
			rite2_end = con.end();
			if (ite.isExist()) printf("ite:[%d] key=%d, value=%d\n", ite.getIndex(), ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] key=%d, value=%d\n", rite.getIndex(), rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end:[%d] key=%d, value=%d\n", ite_end.getIndex(), ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end:[%d] key=%d, value=%d\n", rite_end.getIndex(), rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2:[%d] key=%d, value=%d\n", ite2.getIndex(), ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2:[%d] key=%d, value=%d\n", rite2.getIndex(), rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end:[%d] key=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end:[%d] key=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_key, rite2_end->m_val);
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
				if (ite.isExist()) printf("ite:[%d] key=%d, value=%d\n", ite.getIndex(), ite->m_key, ite->m_val);
				if (rite.isExist()) printf("rite:[%d] key=%d, value=%d\n", rite.getIndex(), rite->m_key, rite->m_val);
				if (ite_end.isExist()) printf("ite_end:[%d] key=%d, value=%d\n", ite_end.getIndex(), ite_end->m_key, ite_end->m_val);
				if (rite_end.isExist()) printf("rite_end:[%d] key=%d, value=%d\n", rite_end.getIndex(), rite_end->m_key, rite_end->m_val);
				if (ite2.isExist()) printf("ite2:[%d] key=%d, value=%d\n", ite2.getIndex(), ite2->m_key, ite2->m_val);
				if (rite2.isExist()) printf("rite2:[%d] key=%d, value=%d\n", rite2.getIndex(), rite2->m_key, rite2->m_val);
				if (ite2_end.isExist()) printf("ite2_end:[%d] key=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_key, ite2_end->m_val);
				if (rite2_end.isExist()) printf("rite2_end:[%d] key=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_key, rite2_end->m_val);
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
			if (ite.isExist()) printf("ite:[%d] key=%d, value=%d\n", ite.getIndex(), ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] key=%d, value=%d\n", rite.getIndex(), rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end:[%d] key=%d, value=%d\n", ite_end.getIndex(), ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end:[%d] key=%d, value=%d\n", rite_end.getIndex(), rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2:[%d] key=%d, value=%d\n", ite2.getIndex(), ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2:[%d] key=%d, value=%d\n", rite2.getIndex(), rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end:[%d] key=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end:[%d] key=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_key, rite2_end->m_val);
			printf("-= 3\n");
			ite -= 3;
			rite -= 3;
			ite_end -= 3;
			rite_end -= 3;
			ite2 -= 3;
			rite2 -= 3;
			ite2_end -= 3;
			rite2_end -= 3;
			if (ite.isExist()) printf("ite:[%d] key=%d, value=%d\n", ite.getIndex(), ite->m_key, ite->m_val);
			if (rite.isExist()) printf("rite:[%d] key=%d, value=%d\n", rite.getIndex(), rite->m_key, rite->m_val);
			if (ite_end.isExist()) printf("ite_end:[%d] key=%d, value=%d\n", ite_end.getIndex(), ite_end->m_key, ite_end->m_val);
			if (rite_end.isExist()) printf("rite_end:[%d] key=%d, value=%d\n", rite_end.getIndex(), rite_end->m_key, rite_end->m_val);
			if (ite2.isExist()) printf("ite2:[%d] key=%d, value=%d\n", ite2.getIndex(), ite2->m_key, ite2->m_val);
			if (rite2.isExist()) printf("rite2:[%d] key=%d, value=%d\n", rite2.getIndex(), rite2->m_key, rite2->m_val);
			if (ite2_end.isExist()) printf("ite2_end:[%d] key=%d, value=%d\n", ite2_end.getIndex(), ite2_end->m_key, ite2_end->m_val);
			if (rite2_end.isExist()) printf("rite2_end:[%d] key=%d, value=%d\n", rite2_end.getIndex(), rite2_end->m_key, rite2_end->m_val);
		}
#endif

		//末尾をポップ(1)
		printf("\n");
		printf("[pop_back(1)]\n");
		{
			const data_t* val = con.back();//末尾の値を取得
			printf("back=[%d:%d]\n", val->m_key, val->m_val);
			con.pop_back();//末尾を削除
			printAll();//全件表示
		}

		//末尾をポップ(2)
		printf("\n");
		printf("[pop_back(2)]\n");
		{
			data_t pop_val;
			con.pop_back(pop_val);//値のコピーを受け取って末尾を削除
			printf("pop_back=[%d:%d]\n", pop_val.m_key, pop_val.m_val);
			printAll();//全件表示
		}

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
		#else//USE_STL_ALGORITM
			auto ite = con.binarySearchValue(key);//二分探索
			if (ite.isExist())
			{
		#endif//USE_STL_ALGORITM
				auto ite = std::lower_bound(con.begin(), con.end(), key);
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

		//削除１：イテレータと削除数で指定
		printf("\n");
		printf("[erase(1)]\n");
		{
			auto ite = con.end();
			ite -= 4;
			con.erase(ite, 2);//後ろから4つ目の位置から2件削除
			printAll();//全件表示
		}

		//削除２：イテレータの範囲で指定
		printf("\n");
		printf("[erase(2)]\n");
		{
			auto start = con.end();
			start -= 4;
			auto end = start;
			end += 2;
			con.erase(start, end);//後ろから4つ目の位置から2件削除
			printAll();//全件表示
		}

		//挿入１：insert()メソッド＋コンストラクタパラメータ（コンストラクタ呼び出しを行う）
		printf("\n");
		printf("[insert(1)]\n");
		{
			auto ite = con.begin();
			++ite;
			con.insert(ite, 5, 99, 999);//先頭から2つ目の位置に5件挿入
			printAll();//全件表示
		}

		//挿入２：insert()メソッド＋オブジェクト（オブジェクトのコピーを行う）
		printf("\n");
		printf("[insert(2)]\n");
		{
			auto ite = con.begin();
			ite += 10;
			data_t prototype(88, 888);//コピー用のオブジェクト
			con.insert(ite, 2, prototype);//先頭から11個目の位置に2件挿入
			printAll();//全件表示
		}

		//リサイズ（縮小）
		printf("\n");
		printf("[resize(shrink)]\n");
		con.resize(5);//要素数=5に縮小
		printAll();//全件表示

		//最大サイズを縮小
		printf("\n");
		printf("[shrink_to_fit]\n");
		con.shrink_to_fit();
		printAll();//全件表示

		//データ割り当て１：assign()メソッド＋コンストラクタパラメータ（コンストラクタ呼び出しを行う）
		printf("\n");
		printf("[assign(1)]\n");
		{
			con.assign(2, 0, 0);//先頭から2件に割り当て
			printAll();//全件表示
		}

		//データ割り当て２：assign()メソッド＋オブジェクト（オブジェクトのコピーを行う）
		printf("\n");
		printf("[assign(2)]\n");
		{
			data_t prototype(-1, 0);//コピー用のオブジェクト
			con.assign(-1, prototype);//※-1で最大要素数全件に割り当て
			printAll();//全件表示
		}

		//クリア
		printf("\n");
		printf("[clear]\n");
		con.clear();
		printAll();//全件表示
		//printReverse();//全件逆順表示
	}

	//--------------------
	//テスト③：ソート、探索の設定を変える
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for dynamic_array::container(User defined type with custom operator type)]\n");

		//配列データ
		data_t array[20];

		//動的配列コンテナ生成
		typedef dynamic_array::container<another_ope_t> container_t;
		container_t con(array);

		//コンテナのインスタンス生成時に配列を渡せない場合は、sertArray() を使用する
		//container_t con;
		//con.assignArray(array);

		//データ登録１：push_back()メソッド＋コンストラクタパラメータ（コンストラクタ呼び出しを行う）
		printf("\n");
		printf("[push_back]\n");
		con.push_back(1, 105);
		con.push_back(1, 108);
		con.push_back(2, 103);
		con.push_back(2, 101);
		con.push_back(3, 107);
		con.push_back(3, 104);
		con.push_back(4, 113);
		con.push_back(4, 111);
		con.push_back(5, 105);
		con.push_back(5, 106);

		//データを表示
		auto printAll = [&con]()
		{
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			printf("array=");
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
		con.sort();//高速ソート
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

		//カスタムソート
		{
			printf("\n");
			printf("[sort with custom predicate]\n");
			auto predicate = [](const data_t& lhs, const data_t& rhs) -> bool {return lhs.m_key < rhs.m_key ? true : lhs.m_key == rhs.m_key ? lhs.m_val > rhs.m_val : false; };
			con.sort(predicate);//高速ソート
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
	}

	//--------------------
	//テスト④：データ設定済みの既存の配列を扱う
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for dynamic_array::container(User defined type for existing data)]\n");

		//データ設定済みの既存の配列
		data_t array[] =
		{
			{ 6, 111 },
			{ 2, 103 },
			{ 2, 104 },
			{ 4, 107 },
			{ 3, 105 },
			{ 3, 106 },
			{ 1, 102 },
			{ 5, 109 },
			{ 1, 101 },
			{ 4, 108 },
			{ 5, 110 },
			{ 6, 112 },
		};

		//設定済みのデータを残したまま、動的配列コンテナのデータとして活用
		dynamic_array::container<ope> con(array, -1);//第二引数で使用中のデータサイズを指定（-1で全域）

		//コンテナのインスタンス生成時に配列を渡せない場合は、sertArray() を使用する
		//dynamic_array::container<ope> con;
		//con.assignArray(array, -1);//第二引数で使用中のデータサイズを指定（-1で全域）

		//データを表示
		auto printAll = [&con]()
		{
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			printf("array=");
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
		printf("\n");
		printf("[sort(before)]\n");
		printAll();

		//ソ―ト
		printf("\n");
		printf("[sort(after)]\n");
		//con.sort();//高速ソート
		con.stableSort();//安定ソート
		printAll();//全件表示
	}

	//--------------------
	//テスト⑤：ロック制御を行う
	{
		printf("--------------------------------------------------------------------------------\n");
		printf("[Test for dynamic_array::container(User defined type for multi-thread)]\n");

		testThread<dynamic_array::container<ope> >("normal container");//ロックなし版のスレッド
		testThread<dynamic_array::container<mt_ope_t> >("multi-thread container");//ロックあり版のスレッド
	}

	//--------------------
	//テスト⑥：大量登録テスト
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
			printf("[Test for performance dynamic_array]\n");

			const std::chrono::system_clock::time_point begin_time = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point prev_time = begin_time;

			//データを初期化
			printf("\n");
			printf("[create container & assign() * %d]\n", TEST_DATA_NUM);
			const std::size_t buff_size = sizeof(data_t)* TEST_DATA_NUM;
			char* buff = new char[buff_size];
			typedef dynamic_array::container<ope> container_t;
			container_t* con = new container_t(buff, buff_size, 0, dynamic_array::AUTO_CLEAR);//コンテナ生成（バッファを割り当て）
			con->assign(-1, 0, 0);//全件初期化
			prev_time = printElapsedTime(prev_time, true);

			//データをクリア
			printf("\n");
			printf("[clear() * %d]\n", TEST_DATA_NUM);
			con->clear();
			prev_time = printElapsedTime(prev_time, true);

			//データを登録
			printf("\n");
			printf("[push_back() * %d]\n", TEST_DATA_NUM);
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; ++i)
				{
					con->push_back(i, 10000000 + i);
					++num;
				}
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

			//イテレータ(1)
			printf("\n");
			printf("[iterator(1)]\n");
			{
				printf_detail("size=%d, max_size=%d(%d)\n", con->size(), con->max_size(), con->max_sizeReal());
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

			//逆順ソート
			printf("\n");
			printf("[reverse sort]\n");
			auto reverse_sort = [](const data_t& lhs, const data_t& rhs){return lhs.m_key > rhs.m_key; };
			con->sort(reverse_sort);
			assert(con->isOrdered(reverse_sort));
			prev_time = printElapsedTime(prev_time, true);

			//イテレータ(2)
			printf("\n");
			printf("[iterator(2)]\n");
			{
				printf_detail("size=%d, max_size=%d(%d)\n", con->size(), con->max_size(), con->max_sizeReal());
				printf_detail("array=");
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

			//正順ソート
			printf("\n");
			printf("[sort]\n");
			con->sort();
			assert(con->isOrdered());
			prev_time = printElapsedTime(prev_time, true);

			//リバースイテレータ
			printf("\n");
			printf("[reverse_iterator]\n");
			{
				printf_detail("size=%d, max_size=%d(%d)\n", con->size(), con->max_size(), con->max_sizeReal());
				printf_detail("array=");
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

		#if 0
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
		#endif

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

			//二分探索
			printf("\n");
			printf("[binarySearchValue]\n");
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; ++i)
				{
					container_t::iterator ite = std::move(con->binarySearchValue(i));
					printf_detail(" [%d:%d]", ite->m_key, ite->m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

			//データを破棄
			printf("\n");
			printf("[delete container]\n");
			delete con;//コンテナを破棄
			con = nullptr;
			delete buff;
			buff = nullptr;
			prev_time = printElapsedTime(prev_time, true);

			//総時間
			printf("\n");
			printf("[finish]\n");
			printElapsedTime(begin_time, true);
		}

		{
			printf("--------------------------------------------------------------------------------\n");
			printf("[Test for performance std::vector]\n");

			const std::chrono::system_clock::time_point begin_time = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point prev_time = begin_time;

			//データを初期化
			printf("\n");
			printf("[create container & assign() * %d]\n", TEST_DATA_NUM);
			typedef std::vector<data_t> container_t;
			container_t* con = new container_t();//std::vectorコンテナを生成
			con->reserve(TEST_DATA_NUM);
			{
				data_t prototype(0, 0);
				con->assign(TEST_DATA_NUM, prototype);
			}
			prev_time = printElapsedTime(prev_time, true);

			//データをクリア
			printf("\n");
			printf("[clear() * %d]\n", TEST_DATA_NUM);
			con->clear();
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

			//逆順ソート
			printf("\n");
			printf("[reverse sort]\n");
			auto reverse_sort = [](const data_t& lhs, const data_t& rhs){return lhs.m_key > rhs.m_key; };
			std::sort(con->begin(), con->end(), reverse_sort);
			prev_time = printElapsedTime(prev_time, true);

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

			//正順ソート
			printf("\n");
			printf("[sort]\n");
			std::sort(con->begin(), con->end());
			prev_time = printElapsedTime(prev_time, true);

			//リバースイテレータ
			printf("\n");
			printf("[reverse_iterator]\n");
			{
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

		#if 0
			//逆順安定ソート
			printf("\n");
			printf("[reverse stable sort]\n");
			std::stable_sort(con->begin(), con->end(), reverse_sort);
			prev_time = printElapsedTime(prev_time, true);

			//正順安定ソート
			printf("\n");
			printf("[stable sort]\n");
			std::stable_sort(con->begin(), con->end());
			prev_time = printElapsedTime(prev_time, true);
		#endif

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

			//二分探索
			printf("\n");
			printf("[binarySearchValue]\n");
			{
				int num = 0;
				for (int i = 0; i < TEST_DATA_NUM; ++i)
				{
					container_t::iterator ite = std::lower_bound(con->begin(), con->end(), i);
					printf_detail(" [%d:%d]", ite->m_key, ite->m_val);
					++num;
				}
				printf_detail("\n");
				printf("num=%d\n", num);
			}
			prev_time = printElapsedTime(prev_time, true);

			//データを破棄
			printf("\n");
			printf("[delete container]\n");
			delete con;//std::vectorコンテナを破棄
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

//--------------------
//テスト⑤：ロック制御を行う

#include <thread>//C++11 std::thread用
#include <mutex>//C++11 std::mutex用
#include <condition_variable>//C++11 std::condition_variable用
#include <atomic>//C++11 std::atomic用
#include <chrono>//C++11 std::chrono用

template <class C>
void testThread(const char* container_type)
{
	typedef C container_t;//テンプレートパラメータでコンテナの型を受け取る

	printf("\n");
	printf("[%s]\n", container_type);

	//配列データ
	data_t array[100];

	//動的配列コンテナ生成
	container_t con(array);

	//条件変数
	std::condition_variable cond;
	std::mutex mutex;
	std::atomic<bool> is_ready;
	std::atomic<int> start_count;
	is_ready.store(false);
	auto isReady = [&is_ready]() -> bool
	{
		return is_ready.load();
	};
	start_count.store(0);

	//スレッド
	auto test_thread = [&con, &mutex, &cond, &start_count, &isReady](const int key, const int value)
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			cond.wait(lock, isReady);
		}
		start_count.fetch_add(1);
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		//スタート
		{
			//GASHA_ lock_guard<typename container_t::lock_type> lock(con);//ライト・ロック取得 ※スコープロック
			auto lock = con.lockScoped();//（上の処理と同じ意味）
			con.push_back(key, value);
		}
		{
			//GASHA_ lock_guard<typename container_t::lock_type> lock(con);//ライト・ロック取得 ※スコープロック
			auto lock = con.lockScoped();//（上の処理と同じ意味）
			auto ite = con.findValue(key);
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			con.erase(ite, 1);
		}
		{
			//GASHA_ lock_guard<typename container_t::lock_type> lock(con);//ライト・ロック取得 ※スコープロック
			auto lock = con.lockScoped();//（上の処理と同じ意味）
			con.push_back(key, value + 10);
		}
	};

	static const int THREAD_NUM = 10;//同時実行スレッド数
	{
		std::thread* th[THREAD_NUM];

		//スレッド生成
		is_ready.store(false);
		start_count.store(0);
		{
			std::lock_guard<std::mutex> lock(mutex);
			for (int i = 0; i < THREAD_NUM; ++i)
				th[i] = new std::thread(test_thread, i, i);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		//スレッド開始
		//※条件変数を使用して、全スレッドのタイミングを揃えて一斉に処理開始
		is_ready.store(true);
		while (true)
		{
			cond.notify_all();
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (start_count.load() == THREAD_NUM)
				break;
		}

		//配列を表示
		auto print_data = [&con](const char* caption)
		{
			//GASHA_ shared_lock_guard<typename container_t::lock_type> lock(con);//リード・ロック取得 ※スコープロック
			auto lock = con.lockSharedScoped();//（上の処理と同じ意味）
			printf("(%s)\n", caption);
			printf("size=%d, max_size=%d(%d)\n", con.size(), con.max_size(), con.max_sizeReal());
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			printf("array=");
			if (con.empty())
			{
				printf("(empty)");
			}
			else
			{
				for (auto& data : con)
				{
					printf(" [%d:%d]", data.m_key, data.m_val);
				}
			}
			printf("\n");
		};
		print_data("before");

		//スレッド終了待ち
		for (int i = 0; i < THREAD_NUM; ++i)
		{
			th[i]->join();
			delete th[i];
			th[i] = nullptr;
		}

		//配列を表示
		print_data("after");
	}
}

// End of file
