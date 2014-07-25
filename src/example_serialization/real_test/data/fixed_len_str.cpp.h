#pragma once
#ifndef GASHA_INCLUDED_FIXED_LEN_STR_CPP_H
#define GASHA_INCLUDED_FIXED_LEN_STR_CPP_H

//--------------------------------------------------------------------------------
// fixed_len_str.cpp.h
// 固定長文字列【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/fixed_len_str.inl"//固定長文字列【インライン関数／テンプレート関数定義部】

#include <gasha/fast_string.h>//高速文字列処理：strncpy_fast(), strnlen_fast()

#include <cstring>//std::memcpy()

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//固定長文字列クラス

//コピーオペレータ
template<std::size_t _MAX_SIZE>
fixedLenStr<_MAX_SIZE>& fixedLenStr<_MAX_SIZE>::operator=(const char* str)
{
	if (!str)
	{
		m_str[0] = '\0';
		m_len = 0;
		return *this;
	}
	strncpy_fast(m_str, str, MAX_LEN);
	m_str[MAX_LEN] = '\0';
	m_len = static_cast<std::uint16_t>(strnlen_fast(m_str, MAX_LEN));
	return *this;
}

//コピーオペレータ
template<std::size_t _MAX_SIZE>
template<std::size_t _SRC_MAX_SIZE>
fixedLenStr<_MAX_SIZE>& fixedLenStr<_MAX_SIZE>::operator=(const fixedLenStr<_SRC_MAX_SIZE>& src)
{
	m_len = GASHA_ min(src.m_len, MAX_LEN);
	std::memcpy(m_str, src.m_str, m_len);
	m_str[m_len] = '\0';
	return *this;
}

//コピーコンストラクタ
template <std::size_t _MAX_SIZE>
template <std::size_t _SRC_MAX_SIZE>
fixedLenStr<_MAX_SIZE>::fixedLenStr(const fixedLenStr<_SRC_MAX_SIZE>& src) :
m_len(GASHA_ min(src.m_len, MAX_LEN))
{
	std::memcpy(m_str, src.m_str, m_len);
	m_str[m_len] = '\0';
}

#endif//GASHA_INCLUDED_FIXED_LEN_STR_CPP_H

// End of file
