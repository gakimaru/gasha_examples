//--------------------------------------------------------------------------------
// exmaple_hash_table_sub.cpp
// 開番地法ハッシュテーブルテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_hash_table.h"//開番地法ハッシュテーブルコンテナテスト

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/hash_table.cpp.h>//開番地法ハッシュテーブルコンテナ【関数定義部】

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
INSTANCING_hTable(ope, TEST_DATA_TABLE_SIZE);//template class hash_table::container<ope, TEST_DATA_TABLE_SIZE>; と同じ
INSTANCING_hTable(ptr_ope, TEST_DATA_TABLE_SIZE_FOR_POINTER);//template class hash_table::container<ptr_ope, TEST_DATA_TABLE_SIZE_FOR_POINTER>; と同じ
INSTANCING_hTable(func_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC);//template class hash_table::container<func_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC>; と同じ
INSTANCING_hTable(obj_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC);//template class hash_table::container<obj_ope, TEST_DATA_TABLE_SIZE_FOR_FUNC>; と同じ

//----------------------------------------
//シンプル開番地法ハッシュテーブルコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//　シンプルコンテナも明示的なインスタンス化は可能。

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <algorithm> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <algorithm>//std::find(), std::binary_search(), std::lower_bound()

//シンプル開番地法ハッシュテーブルコンテナテスト
void example_simple_hash_table()
{
	printf("\n");
	printf("--- example_simple_hash_table ---\n");

	{
		simpleHTable<short, 100>::con con;//シンプル開番地法ハッシュテーブルコンテナ
		con.insert("KEY1,", 1);
		con.insert("KEY3", 3);
		con.insert("KEY5", 5);
		auto print = [&con]()
		{
			printf("data =");
			for (auto data : con)
			{
				printf(" %d", data);
			}
			printf("\n");
		};
		print();
		auto ite = con.find("KEY5");
		printf(".find(\"KEY5\"): ite=%d\n",  *ite);
	}
	{
		//ローカルクラス（関数内クラス）を使うと、明示的なインスタンス化ができない点に注意
		struct data_t
		{
			int m_val1;
			int m_val2;
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
		typedef simpleHTable<data_t, 100> con_t;
		con_t::con con;//シンプル動的配列コンテナ
		con.emplace("KEY1", 1, 2);
		con.insert("KEY3", 34);
		con.insert("KEY5", 56);
		auto print = [&con]()
		{
			printf("data =");
			for (auto& data : con)
			{
				printf(" {%d,%d}", data.m_val1, data.m_val2);
			}
			printf("\n");
		};
		print();
		auto ite = con.find("KEY5");
		printf(".find(\"KEY5\"): ite={%d,%d}\n", ite->m_val1, ite->m_val2);
	}
}
//明示的インスタンス化する場合
//※専用マクロ使用
INSTANCING_simpleHTable(short, 100);
//INSTANCING_simpleDArray(data_t, 100);//ローカルクラス（関数内クラス）を使ったものは、明示的なインスタンス化ができない

// End of file
