#pragma once
#ifndef GASHA_INCLUDED_FIXED_LEN_STR_EXT_H
#define GASHA_INCLUDED_FIXED_LEN_STR_EXT_H

//--------------------------------------------------------------------------------
// short_string_ext.h
// CRC付き固定長文字列【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/fixed_len_str.h"//固定長文字列

#include <gasha/crc32.h>//CRC32計算

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//CRC付き固定長文字列型
template<std::size_t _MAX_SIZE>
class fixedLenStrExt : public fixedLenStr<_MAX_SIZE>
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();
public:
	//型
	typedef fixedLenStr<_MAX_SIZE> parent_type;//親クラス型
public:
	//アクセッサ
	inline crc32_t crc() const { return m_crc; }//CRCを取得
public:
	//キャストオペレータ
	inline operator crc32_t() const { return m_crc; }
public:
	//コピーオペレータ
	fixedLenStrExt& operator=(const char* str);
	template <std::size_t _SRC_MAX_SIZE>
	fixedLenStrExt& operator=(const fixedLenStrExt<_SRC_MAX_SIZE>& src);
	template <std::size_t _SRC_MAX_SIZE>
	fixedLenStrExt& operator=(const fixedLenStr<_SRC_MAX_SIZE>& src);
public:
	//コピーコンストラクタ
	template <std::size_t _SRC_MAX_SIZE>
	fixedLenStrExt(const fixedLenStrExt<_SRC_MAX_SIZE>& src);
	template <std::size_t _SRC_MAX_SIZE>
	fixedLenStrExt(const fixedLenStr<_SRC_MAX_SIZE>& src);
	//コンストラクタ
	inline fixedLenStrExt(const char* str);
	//デフォルトコンストラクタ
	inline fixedLenStrExt();
	//デストラクタ
	inline ~fixedLenStrExt();
private:
	//フィールド
	GASHA_ crc32_t m_crc;//CRC
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/fixed_len_str_ext.inl"

#endif//GASHA_INCLUDED_FIXED_LEN_STR_EXT_H

// End of file
