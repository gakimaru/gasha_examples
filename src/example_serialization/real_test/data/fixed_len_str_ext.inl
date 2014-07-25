#pragma once
#ifndef GASHA_INCLUDED_FIXED_LEN_STR_EXT_INL
#define GASHA_INCLUDED_FIXED_LEN_STR_EXT_INL

//--------------------------------------------------------------------------------
// short_string_ext.inl
// CRC付き固定長文字列【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/fixed_len_str_ext.h"//CRC付き固定長文字列【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//CRC付き固定長文字列型

//コンストラクタ
template<std::size_t _MAX_SIZE>
inline fixedLenStrExt<_MAX_SIZE>::fixedLenStrExt(const char* str) :
	parent_type(),
	m_crc(0)
{
	*this = str;
}

//デフォルトコンストラクタ
template<std::size_t _MAX_SIZE>
inline fixedLenStrExt<_MAX_SIZE>::fixedLenStrExt() :
	parent_type(nullptr),
	m_crc(0)
{}

//デストラクタ
template<std::size_t _MAX_SIZE>
inline fixedLenStrExt<_MAX_SIZE>::~fixedLenStrExt()
{}

#endif//GASHA_INCLUDED_FIXED_LEN_STR_EXT_INL

// End of file
