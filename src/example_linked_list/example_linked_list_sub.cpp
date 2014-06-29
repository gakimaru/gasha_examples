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

template class linked_list::container<ope>;
template class linked_list::container<another_ope_t>;

//----------------------------------------
//シンプル双方向連結リストコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//　シンプルコンテナも明示的なインスタンス化は可能。
void example_simple_linked_list()
{
	printf("\n");
	printf("--- example_simple_dynamic_array ---\n");

	simpleLList<short>::con con;//シンプル双方向連結リストコンテナ
	simpleLList<short>::node data[3];
	data[0] = 5;
	data[1] = 1;
	data[2] = 3;
	con.push_back(data[0]);
	con.push_back(data[1]);
	con.push_back(data[2]);
	con.sort();
	printf("data =");
	for (auto data : con)
	{
		printf(" %d", data.value());
	}
	printf("\n");
}

// End of file
