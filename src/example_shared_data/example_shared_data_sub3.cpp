//--------------------------------------------------------------------------------
// exmaple_shared_data3.cpp
// マルチスレッド共有データテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_shared_data.h"//マルチスレッド共有データテスト

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/shared_queue.cpp.h>//マルチスレッド共有キュー【関数定義部】

#include <gasha/lf_queue.cpp.h>//ロックフリーキュー【関数定義部】

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

GASHA_INSTANCING_sharedQueue_withLock(data_t, TEST_POOL_SIZE, lock_type);//マルチスレッド共有キュー ※template class sharedQueue<data_t, TEST_POOL_SIZE, lock_type>; と同じ

GASHA_INSTANCING_lfQueue_withTag(data_t, TEST_POOL_SIZE, TEST_TAGGED_PTR_TAG_SIZE, TEST_TAGGED_PTR_TAG_SHIFT);//ロックフリーキュー ※ template class lfQueue<data_t, TEST_POOL_SIZE, TEST_TAGGED_PTR_TAG_SIZE, TEST_TAGGED_PTR_TAG_SHIFT>;と同じ

// End of file
