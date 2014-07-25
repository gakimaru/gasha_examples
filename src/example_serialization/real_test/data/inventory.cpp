//--------------------------------------------------------------------------------
// inventory.cpp
// 所持アイテム【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/inventory.inl"//所持アイテム【インライン関数／テンプレート関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//アイテムコレクションの明示的なインスタンス化

#include "real_test/data/data_collection.cpp.h"//データコレクション【関数／実体定義部】

INSTANCING_dataCollection(itemData, ::_private::ITEM_DATA_COLLECTION_POOL);

//--------------------
//インベントリ（アイテムコレクション）シングルトンの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(inventory);//シングルトンのインスタンス化

// End of file
