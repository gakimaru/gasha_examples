#pragma once
#ifndef GASHA_INCLUDED_FIXED_LEN_STR_INL
#define GASHA_INCLUDED_FIXED_LEN_STR_INL

//--------------------------------------------------------------------------------
// fixed_len_str.inl
// 固定長文字列【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/fixed_len_str.h"//固定長文字列【宣言部】

#include <gasha/utility.h>//汎用ユーティリティ

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//固定長文字列クラス

//コンストラクタ
template <std::size_t _MAX_SIZE>
inline fixedLenStr<_MAX_SIZE>::fixedLenStr(const char* str)
{
	*this = str;
}

//デフォルトコンストラクタ
template <std::size_t _MAX_SIZE>
inline fixedLenStr<_MAX_SIZE>::fixedLenStr() :
	m_len(0)
{
	m_str[0] = '\0';
}

//デストラクタ
template <std::size_t _MAX_SIZE>
inline fixedLenStr<_MAX_SIZE>::~fixedLenStr()
{}

#endif//GASHA_INCLUDED_FIXED_LEN_STR_INL

// End of file
