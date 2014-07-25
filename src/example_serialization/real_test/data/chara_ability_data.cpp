//--------------------------------------------------------------------------------
// chara_ability_data.cpp
// キャラアビリティデータ【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/chara_ability_data.inl"//キャラアビリティデータ【インライン関数／テンプレート関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//キャラ習得アビリティプールクラスの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(charaAbilityPool);//シングルトンのインスタンス化

// End of file
