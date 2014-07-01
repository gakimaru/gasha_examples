//--------------------------------------------------------------------------------
// exmaple_rb_tree_sub.cpp
// 赤黒木テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_rb_tree.h"//赤黒木コンテナテスト

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/rb_tree.cpp.h>//赤黒木コンテナ【関数定義部】

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
INSTANCING_rbTree(ope);//template class rb_tree::stack_t<OPE_TYPE>; template class rb_tree::container<ope>; と同じ

//----------------------------------------
//シンプル双方向連結リストコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//※シンプルコンテナも明示的なインスタンス化は可能。

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <algorithm> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::find(), std::binary_search(), std::lower_bound()

//シンプル赤黒木コンテナテスト
void example_simple_rb_tree()
{
	printf("\n");
	printf("--- example_simple_linked_list ---\n");

	{
		simpleRBTree<short>::con con;//シンプル赤黒木コンテナ
		simpleRBTree<short>::node data[3];
		data[0].emplace("KEY5", 15);
		data[1].emplace("KEY1", 11);
		data[2].emplace("KEY3", 13);
		con.insert(data[0]);
		con.insert(data[1]);
		con.insert(data[2]);
		auto print = [&con]()
		{
			printf("data =");
			for (auto& data : con)
			{
				printf(" {0x%08x,%d}", data.key(), data.value());
			}
			printf("\n");
		};
		print();
		auto ite = con.find("KEY5");
		printf(".findValue(\"KEY5\"): ite={0x%08x, %d}\n", ite->m_key, ite->m_value);
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
		typedef simpleRBTree<data_t> con_t;
		con_t::con con;//シンプル赤黒木コンテナ
		con_t::node data[3];
		data[0].emplace("KEY5", 56);
		data[1].emplace("KEY1", 1, 2);
		data[2].emplace("KEY3", 34);
		con.insert(data[0]);
		con.insert(data[1]);
		con.insert(data[2]);
		auto print = [&con]()
		{
			printf("data =");
			for (auto& data : con)
			{
				printf(" {0x%08x,{%d,%d}}", data.m_key, data->m_val1, data->m_val2);
			}
			printf("\n");
		};
		print();
		auto ite = con.find("KEY5");
		printf(".findValue(\"KEY5\"): ite={0x%08x,{%d:%d}}\n", ite->m_key, ite->m_value.m_val1, ite->m_value.m_val2);
	}
}
//明示的インスタンス化する場合
//※専用マクロ使用
INSTANCING_simpleRBTree(short);
//INSTANCING_simpleRBTree(data_t);//ローカルクラス（関数内クラス）を使ったものは、明示的なインスタンス化ができない

// End of file
