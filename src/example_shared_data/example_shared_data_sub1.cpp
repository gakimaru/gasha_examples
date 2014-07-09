//--------------------------------------------------------------------------------
// exmaple_shared_data1.cpp
// マルチスレッド共有データテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_shared_data.h"//マルチスレッド共有データテスト

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/pool_allocator.cpp.h>//プールアロケータ【関数定義部】

#include <gasha/lf_pool_allocator.cpp.h>//ロックフリープールアロケータ【関数定義部】

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

//GASHA_INSTANCING_poolAllocator_withType_withLock(data_t, TEST_POOL_SIZE, lock_type);//プールアロケータ ※template class poolAllocator_withType<data_t, TEST_POOL_SIZE, lock_type>; と同じ
GASHA_INSTANCING_poolAllocator_withLock(TEST_POOL_SIZE, lock_type);//プールアロケータ ※template class poolAllocator<TEST_POOL_SIZE, lock_type>; と同じ

//GASHA_INSTANCING_lfPoolAllocator_withType(data_t, TEST_POOL_SIZE);//ロックフリープールアロケータ ※template class lfPoolAllocator_withType<data_t, TEST_POOL_SIZE>; と同じ
GASHA_INSTANCING_lfPoolAllocator(TEST_POOL_SIZE);//ロックフリープールアロケータ ※template class lfPoolAllocator<TEST_POOL_SIZE>; と同じ

// End of file
