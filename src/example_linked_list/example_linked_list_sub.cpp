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

#if 0

template class linked_list::container<int_ope_t>;
template class linked_list::container<ope_t>;
template class linked_list::container<another_ope_t>;
template class linked_list::container<mt_ope_t>;

#endif

// End of file
