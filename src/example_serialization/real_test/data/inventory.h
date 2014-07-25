#pragma once
#ifndef GASHA_INCLUDED_INVENTORY_H
#define GASHA_INCLUDED_INVENTORY_H

//--------------------------------------------------------------------------------
// inventory.h
// 所持アイテム【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/item_data.h"//アイテムデータ
#include "real_test/data/data_collection.h"//データコレクション

#include <cstddef>//std::size_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//インベントリ（アイテムコレクション）
namespace _private
{
	static const std::size_t ITEM_DATA_COLLECTION_POOL = 100;
	using itemCollection = dataCollection<itemData, ITEM_DATA_COLLECTION_POOL>;
};
class inventory : public ::_private::itemCollection
{
public:
	//コンストラクタ
	inline inventory();
	//デストラクタ
	inline ~inventory();
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/inventory.inl"

#endif//GASHA_INCLUDED_INVENTORY_H

// End of file
