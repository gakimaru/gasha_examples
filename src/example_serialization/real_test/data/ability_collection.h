#pragma once
#ifndef GASHA_INCLUDED_ABILITY_COLLECTION_H
#define GASHA_INCLUDED_ABILITY_COLLECTION_H

//--------------------------------------------------------------------------------
// ability_collection.h
// アビリティコレクション【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/ability_data.h"//アビリティデータ
#include "real_test/data/data_collection.h"//データコレクション

#include <cstddef>//std::size_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//アビリティコレクション
static const std::size_t ABILITY_DATA_COLLECTION_POOL = 40;
using abilityCollection = dataCollection<abilityData, ABILITY_DATA_COLLECTION_POOL>;

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/ability_collection.inl"

#endif//GASHA_INCLUDED_ABILITY_COLLECTION_H

// End of file
