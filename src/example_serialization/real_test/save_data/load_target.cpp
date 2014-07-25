//--------------------------------------------------------------------------------
// load_target.cpp
// ロード対象（部分ロード用）【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/save_data/load_target.inl"//ロード対象（部分ロード用）【インライン関数／テンプレート関数定義部】

#include <ctime>//std::localtime

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//ロード対象（部分ロード用）シングルトンの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(loadTarget);//シングルトンのインスタンス化

// End of file
