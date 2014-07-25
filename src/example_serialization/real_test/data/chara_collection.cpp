//--------------------------------------------------------------------------------
// chara_collection.cpp
// キャラコレクション【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/chara_collection.inl"//キャラコレクション【インライン関数／テンプレート関数定義部】

//--------------------
//キャラコレクションの明示的なインスタンス化

#include "real_test/data/data_collection.cpp.h"//データコレクション【関数／実体定義部】

INSTANCING_dataCollection(charaData, CHARA_DATA_COLLECTION_POOL);

//--------------------
//キャラコレクションシングルトンの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(charaCollection);//シングルトンのインスタンス化

// End of file
