#pragma once
#ifndef GASHA_INCLUDED_FIXED_LEN_STR_EXT_CPP_H
#define GASHA_INCLUDED_FIXED_LEN_STR_EXT_CPP_H

//--------------------------------------------------------------------------------
// short_string_ext.cpp.h
// CRC付き固定長文字列【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/fixed_len_str_ext.inl"//CRC付き固定長文字列【インライン関数／テンプレート関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//CRC付き固定長文字列型

//コピーオペレータ
template<std::size_t _MAX_SIZE>
fixedLenStrExt<_MAX_SIZE>& fixedLenStrExt<_MAX_SIZE>::operator=(const char* str)
{
	*static_cast<parent_type*>(this) = str;
	m_crc = calcCRC32(parent_type::m_str);
	return *this;
}

//コピーオペレータ
template<std::size_t _MAX_SIZE>
template<std::size_t _SRC_MAX_SIZE>
fixedLenStrExt<_MAX_SIZE>& fixedLenStrExt<_MAX_SIZE>::operator=(const fixedLenStrExt<_SRC_MAX_SIZE>& src)
{
	*static_cast<parent_type*>(this) = src;
	m_crc = src.m_crc;
}

//コピーオペレータ
template <std::size_t _MAX_SIZE>
template <std::size_t _SRC_MAX_SIZE>
fixedLenStrExt<_MAX_SIZE>& fixedLenStrExt<_MAX_SIZE>::operator=(const fixedLenStr<_SRC_MAX_SIZE>& src)
{
	*static_cast<parent_type*>(this) = src;
	m_crc = calcCRC32(parent_type::m_str);
}

//コピーコンストラクタ
template <std::size_t _MAX_SIZE>
template <std::size_t _SRC_MAX_SIZE>
fixedLenStrExt<_MAX_SIZE>::fixedLenStrExt(const fixedLenStrExt<_SRC_MAX_SIZE>& src) :
	parent_type(src),
	m_crc(src.m_crc)
{}

//コピーコンストラクタ
template <std::size_t _MAX_SIZE>
template <std::size_t _SRC_MAX_SIZE>
fixedLenStrExt<_MAX_SIZE>::fixedLenStrExt(const fixedLenStr<_SRC_MAX_SIZE>& src) :
	parent_type(src),
	m_crc(GASHA_ calcCRC32(src.m_crc))
{}

#endif//GASHA_INCLUDED_FIXED_LEN_STR_EXT_CPP_H

// End of file
