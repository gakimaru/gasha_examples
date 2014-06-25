//--------------------------------------------------------------------------------
// exmaple_ring_buffer_sub.cpp
// リングバッファテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_ring_buffer.h"//リングバッファコンテナテスト

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/ring_buffer.cpp.h>//リングバッファコンテナ【関数実装部】

//※コンパイル速度の劣化や、ソース改変時の広範囲な再コンパイルの影響を抑えるために、
//　コンテナのヘッダーファイルを三つに分割。
//　　・*.h     ... 宣言部                                  （クラスの宣言が必要な場所でインクルード）
//　　・*.inl   ... インライン関数／テンプレート関数の実装部（クラスの操作が必要な場所でインクルード）
//　　・*.cpp.h ... 関数実装部                              （クラスの実体化が必要な場所でインクルード）
//※わざわざ分けて書くのが面倒なら、クラスの操作が必要な場所で
//　このヘッダーファイルをインクルードすれば、明示的なインスタンス化は
//　必要ない。
//　（コンパイルへの影響を気にしないなら、ヘッダーファイルに
//　インクルードしてしまっても良い）

#if 0

template class ring_buffer::container<int_ope_t>;
template class ring_buffer::container<ope_t>;
template class ring_buffer::container<another_ope_t>;
template class ring_buffer::container<mt_ope_t>;

#endif

// End of file
