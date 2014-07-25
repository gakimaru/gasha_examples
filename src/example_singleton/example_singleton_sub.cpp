//--------------------------------------------------------------------------------
// exmaple_singleton_sub.cpp
// シングルトンテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_singleton.h"//シングルトンテスト

//----------------------------------------
//テンプレートの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数定義部】
#include <gasha/singleton_debug.cpp.h>//シングルトンデバッグ用処理【関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//明示的インスタンス化
//※専用マクロを使用
GASHA_INSTANCING_singleton(common_data1_t);//template class _private::singleton<test_data1_t>; と同じ
GASHA_INSTANCING_simpleSingleton(common_data2_t);//template class _private::singleton<test_data1_t>; と同じ
GASHA_INSTANCING_singletonDebug_withLock(50, sharedSpinLock);//template class singletonDebug<5, sharedSpinLock>; と同じ

// End of file
