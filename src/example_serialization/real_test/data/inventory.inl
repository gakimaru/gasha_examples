#pragma once
#ifndef GASHA_INCLUDED_INVENTORY_INL
#define GASHA_INCLUDED_INVENTORY_INL

//--------------------------------------------------------------------------------
// inventory.inl
// 所持アイテム【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/inventory.h"//所持アイテム【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//インベントリ（アイテムコレクション）

//コンストラクタ
inline inventory::inventory() :
	::_private::itemCollection()
{}

//デストラクタ
inline inventory::~inventory()
{}

#endif//GASHA_INCLUDED_INVENTORY_INL

// End of file
