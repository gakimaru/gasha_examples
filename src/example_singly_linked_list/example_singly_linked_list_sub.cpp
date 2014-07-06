//--------------------------------------------------------------------------------
// exmaple_singly_linked_list_sub.cpp
// 片方向連結リストテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_singly_linked_list.h"//片方向連結リストコンテナテスト

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/singly_linked_list.cpp.h>//片方向連結リストコンテナ【関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//※コンパイル速度の劣化や、ソース改変時の広範囲な再コンパイルの影響を抑えるために、
//　コンテナのヘッダーファイルを三つに分割。
//　　・*.h     ... 宣言部                                （クラスの宣言が必要な場所でインクルード）
//　　・*.inl   ... インライン関数／テンプレート関数定義部（クラスの操作が必要な場所でインクルード）
//　　・*.cpp.h ... 関数定義部                            （クラスの実体化が必要な場所でインクルード）
//※わざわざ分けて書くのが面倒なら、クラスの操作が必要な場所で
//　このヘッダーファイルをインクルードすれば、明示的なインスタンス化は
//　必要ない。
//　（コンパイルへの影響を気にしないなら、ヘッダーファイルに
//　インクルードしてしまっても良い）

//明示的インスタンス化
//※専用マクロを使用
GASHA_INSTANCING_slList(ope);//template class singly_linked_list::container<ope>; と同じ
GASHA_INSTANCING_slList(another_ope);//template class singly_linked_list::container<another_ope>; と同じ

//----------------------------------------
//シンプル片方向連結リストコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//※シンプルコンテナも明示的なインスタンス化は可能。

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <algorithm> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::find(), std::binary_search(), std::lower_bound()

//シンプル片方向連結リストコンテナテスト
void example_simple_singly_linked_list()
{
	printf("\n");
	printf("--- example_simple_singly_linked_list ---\n");

	{
		simpleSLList<short>::con con;//シンプル片方向連結リストコンテナ
		simpleSLList<short>::node data[3];
		data[0] = 5;
		data[1] = 1;
		data[2] = 3;
		con.push_back(data[0]);
		con.push_back(data[1]);
		con.push_back(data[2]);
		auto print = [&con]()
		{
			printf("data =");
			for (auto data : con)
			{
				printf(" %d", data.value());
			}
			printf("\n");
		};
		print();
		con.sort();
		print();
		auto ite = con.findValue(5);
		printf(".findValue(5): ite=%d\n", ite->m_value);
	#ifdef GASHA_SINGLY_LINKED_LIST_ENABLE_BINARY_SEARCH
		ite = con.binarySearchValue(3);
		printf(".binarySearchValue(3): ite=%d\n", ite->m_value);
	#endif//GASHA_SINGLY_LINKED_LIST_ENABLE_BINARY_SEARCH
		ite = std::find(con.begin(), con.end(), 5);
		printf("std::find(.begin(), .end(), 5): ite=%d\n", ite->m_value);
		if (std::binary_search(con.begin(), con.end(), 3))
			ite = std::lower_bound(con.begin(), con.end(), 3);
		printf("std::lower_bound(.begin(), .end(), 3): ite=%d\n", ite->m_value);
	}
	{
		//ローカルクラス（関数内クラス）を使うと、明示的なインスタンス化ができない点に注意
		struct data_t
		{
			int m_val1;
			int m_val2;
			bool operator==(const int rhs) const { return m_val1 == rhs; }//find(), std::find()用
			bool operator<(const data_t& rhs) const { return m_val1 < rhs.m_val1; }//sort(), std::sort()用
			bool operator<(const int rhs) const { return m_val1 < rhs; }//binarySearch(), std::binary_search()用
			//friend bool operator<(const int lhs, const data_t& rhs)//std::binary_search()用
			//{                                                      //※ローカルクラス（関数内クラス）ではfriend関数を定義できないため、
			//	return rhs < rhs.m_val1;                             //　この演算子を定義できない
			//}                                                      //　（つまり、std::binary_searchは使えない）
			data_t(const int val) :
				m_val1(val / 10),
				m_val2(val % 10)
			{}
			data_t(const int val1, const int val2) :
				m_val1(val1),
				m_val2(val2)
			{}
			data_t() :
				m_val1(0),
				m_val2(0){}
		};
		typedef simpleSLList<data_t> con_t;
		con_t::con con;//シンプル片方向連結リストコンテナ
		con_t::node data[3];
		data[0] = 56;
		data[1].constructor(1, 2);
		data[2] = 34;
		con.push_back(data[0]);
		con.push_back(data[1]);
		con.push_back(data[2]);
		auto print = [&con]()
		{
			printf("data =");
			for (auto& data : con)
			{
				printf(" {%d,%d}", data->m_val1, data->m_val2);
			}
			printf("\n");
		};
		print();
		con.sort();
		print();
		auto ite = con.findValue(5);
		printf(".findValue(5): ite={%d,%d}\n", ite->m_value.m_val1, ite->m_value.m_val2);
	#ifdef GASHA_SINGLY_LINKED_LIST_ENABLE_BINARY_SEARCH
		ite = con.binarySearchValue(3);
		printf(".binarySearchValue(3): ite={%d,%d}\n", ite->m_value.m_val1, ite->m_value.m_val2);
	#endif//GASHA_SINGLY_LINKED_LIST_ENABLE_BINARY_SEARCH
		ite = std::find(con.begin(), con.end(), 5);
		printf("std::find(.begin(), .end(), 5): ite={%d,%d}\n", ite->m_value.m_val1, ite->m_value.m_val2);
		//if (std::binary_search(con.begin(), con.end(), 3))//operator<(const int, const data_t&) が定義できないのでNG
		//	ite = std::lower_bound(con.begin(), con.end(), 3);
		//printf("std::lower_bound(.begin(), .end(), 3): ite={%d,%d}\n", ite->m_value.m_val1, ite->m_value.m_val2);
		data[1].destructor();
	}
}
//明示的インスタンス化する場合
//※専用マクロ使用
GASHA_INSTANCING_simpleSLList(short);
//GASHA_INSTANCING_simpleSLList(data_t);//ローカルクラス（関数内クラス）を使ったものは、明示的なインスタンス化ができない

#if 1
//明示的インスタンス化のテスト
//※operatorCRTP を使って基本比較オペレータを実装
#include <gasha/type_traits.h>
struct derived : public operatorCRTP<derived, int>
{
	operator int() const { return m_primaryData; }//int型にキャスト ※operatorCRTP への第二テンプレート引数の型に対するキャストオペレータを実装しておく必要がある
	int m_primaryData;//（比較に用いる）主要データ
	int m_otherData;//他のデータ
};
bool funct()
{
	derived a;
	derived b;
	a.m_primaryData = 1;
	b.m_primaryData = 1;
	return a == b;
}
GASHA_INSTANCING_simpleSLList(derived);
#endif

// End of file
