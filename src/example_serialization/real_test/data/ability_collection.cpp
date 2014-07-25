//--------------------------------------------------------------------------------
// ability_collection.cpp
// アビリティコレクション【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/ability_collection.inl"//アビリティコレクション【インライン関数／テンプレート関数定義部】

//--------------------
//アビリティコレクションの明示的なインスタンス化

#include "real_test/data/data_collection.cpp.h"//データコレクション【関数／実体定義部】

INSTANCING_dataCollection(abilityData, ABILITY_DATA_COLLECTION_POOL);

//--------------------
//アビリティコレクションシングルトンの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(abilityCollection);//シングルトンのインスタンス化

// End of file
