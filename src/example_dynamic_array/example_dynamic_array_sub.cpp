//--------------------------------------------------------------------------------
// exmaple_dynamic_array_sub.cpp
// 動的配列コンテナテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_dynamic_array.h"//動的配列コンテナテスト

#include <stdio.h>//printf()

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/dynamic_array.cpp.h>//動的配列コンテナ【関数定義部】

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

template class dynamic_array::container<int_ope_t>;
template class dynamic_array::container<ope>;
template class dynamic_array::container<another_ope_t>;
template class dynamic_array::container<mt_ope_t>;

//----------------------------------------
//シンプル動的配列コンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//　シンプルコンテナも明示的なインスタンス化は可能。
void example_simple_dynamic_array()
{
	printf("\n");
	printf("--- example_simple_dynamic_array ---\n");

	short arr[10];
	simpleDArray<short>::con con(arr);//シンプル動的配列コンテナ
	con.push_back(5);
	con.push_back(1);
	con.push_back(3);
	con.sort();
	printf("data =");
	for(auto data : con)
	{
		printf(" %d", data);
	}
	printf("\n");
}

// End of file
