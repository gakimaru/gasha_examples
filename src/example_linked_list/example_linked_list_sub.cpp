//--------------------------------------------------------------------------------
// exmaple_linked_list_sub.cpp
// 双方向連結リストテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_linked_list.h"//双方向連結リストコンテナテスト

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/linked_list.cpp.h>//双方向連結リストコンテナ【関数定義部】

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
//※専用マクロ使用
INSTANCING_lList(ope);
INSTANCING_lList(another_ope_t);

//----------------------------------------
//シンプル双方向連結リストコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//※シンプルコンテナも明示的なインスタンス化は可能。
void example_simple_linked_list()
{
	printf("\n");
	printf("--- example_simple_dynamic_array ---\n");

	{
		simpleLList<short>::con con;//シンプル双方向連結リストコンテナ
		simpleLList<short>::node data[3];//そのノード
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
		auto ite1 = con.findValue(5);
		auto ite2 = con.binarySearchValue(3);
		printf("ite1=%d\n", ite1->m_value);
		printf("ite2=%d\n", ite2->m_value);
	}
	{
		struct data_t
		{
			int m_val1;
			int m_val2;
			bool operator==(const int rhs) const { return m_val1 == rhs; }//find(), std::find()用
			bool operator<(const data_t& rhs) const { return m_val1 < rhs.m_val1; }//sort(), std::sort()用
			bool operator<(const int rhs) const { return m_val1 < rhs; }//binarySearch(), std::binary_search()用
			//friend bool operator<(const int lhs, const data_t& rhs)//std::binary_search()用
			//{                                                             //※ローカルクラスではfriend関数を定義できないため、
			//	return rhs < rhs.m_val1;                                    //　この演算子を定義できない
			//}                                                             //　（つまり、std::binary_searchは使えない）
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
		typedef simpleLList<data_t> con_t;
		con_t::con con;//シンプル双方向連結リストコンテナ
		con_t::node data[3];//そのノード
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
		auto ite1 = con.findValue(5);
		auto ite2 = con.binarySearchValue(3);
		printf("ite1={%d,%d}\n", ite1->m_value.m_val1, ite1->m_value.m_val2);
		printf("ite2={%d,%d}\n", ite2->m_value.m_val1, ite2->m_value.m_val2);
		ite1 = std::find(con.begin(), con.end(), 5);
		//if (std::binary_search(con.begin(), con.end(), 3))//operator<(const int, const data_t&) が定義できないのでNG
		//	ite2 = std::lower_bound(con.begin(), con.end(), 3);
		printf("ite1={%d,%d}\n", ite1->m_value.m_val1, ite1->m_value.m_val2);
		//printf("ite2={%d,%d}\n", ite2->m_value.m_val1, ite2->m_value.m_val2);
		data[1].destructor();
	}
}
//明示的インスタンス化する場合
//※専用マクロ使用
//INSTANCING_simpleLList(short);

// End of file
