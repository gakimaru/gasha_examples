﻿//--------------------------------------------------------------------------------
// exmaple_ring_buffer_sub.cpp
// リングバッファテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_ring_buffer.h"//リングバッファコンテナテスト

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/ring_buffer.cpp.h>//リングバッファコンテナ【関数定義部】

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

template class ring_buffer::container<int_ope_t>;
template class ring_buffer::container<ope>;
template class ring_buffer::container<another_ope_t>;
template class ring_buffer::container<mt_ope_t>;

//----------------------------------------
//シンプルリングバッファコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
void example_simple_ring_buffer()
{
	printf("\n");
	printf("--- example_simple_dynamic_array ---\n");

	short arr[10];
	simpleRBuff<short>::con con(arr);//シンプルリングバッファコンテナ
	con.push_back(5);
	con.push_back(1);
	con.push_back(3);
	con.sort();
	printf("data =");
	for (auto data : con)
	{
		printf(" %d", data);
	}
	printf("\n");
}
// End of file
