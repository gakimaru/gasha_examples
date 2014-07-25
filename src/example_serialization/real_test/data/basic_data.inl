#pragma once
#ifndef GASHA_INCLUDED_BASIC_DATA_INL
#define GASHA_INCLUDED_BASIC_DATA_INL

//--------------------------------------------------------------------------------
// basic_data.inl
// 基本データ型【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/basic_data.h"//基本データ【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//基本データ構造体

//コピーコンストラクタ
inline basicData::basicData(const basicData& src) :
	m_atk(src.m_atk),
	m_def(src.m_def)
{}

//コンストラクタ
inline basicData::basicData(const int atk, const int def) :
	m_atk(static_cast<basic_param_type>(atk)),
	m_def(static_cast<basic_param_type>(def))
{}

//デフォルトコンストラクタ
inline basicData::basicData()
{}

//デストラクタ
inline basicData::~basicData()
{}

#endif//GASHA_INCLUDED_BASIC_DATA_INL

// End of file
