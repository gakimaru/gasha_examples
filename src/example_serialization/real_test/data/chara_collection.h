#pragma once
#ifndef GASHA_INCLUDED_CHARA_COLLECTION_H
#define GASHA_INCLUDED_CHARA_COLLECTION_H

//--------------------------------------------------------------------------------
// chara_collection.h
// キャラコレクション【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/chara_data.h"//キャラデータ
#include "real_test/data/data_collection.h"//データコレクション

#include <cstddef>//std::size_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//キャラコレクション
static const std::size_t CHARA_DATA_COLLECTION_POOL = 10;
using charaCollection = dataCollection<charaData, CHARA_DATA_COLLECTION_POOL>;

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/chara_collection.inl"

#endif//GASHA_INCLUDED_CHARA_COLLECTION_H

// End of file
