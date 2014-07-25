#pragma once
#ifndef GASHA_INCLUDED_FIXED_LEN_STR_H
#define GASHA_INCLUDED_FIXED_LEN_STR_H

//--------------------------------------------------------------------------------
// fixed_len_str.h
// 固定長文字列【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/serialization/friends.h>//シリアライズ/フレンド宣言

#include <cstddef>//std::size_t
#include <cstdint>//C++11 std::uint16_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//固定長文字列クラス
template<std::size_t _MAX_SIZE>
class fixedLenStr
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();
public:
	//定数
	static const std::size_t MAX_SIZE = _MAX_SIZE;//バッファサイズ
	static const std::size_t MAX_LEN = _MAX_SIZE - 1;//最大文字列長
public:
	//アクセッサ
	inline const char* c_str() const { return m_str; }//文字列を取得　※std::string互換
	inline std::size_t length() const { return m_len; }//文字列長を取得　※std::string互換
	inline std::size_t size() const { return m_len; }//文字列長を取得　※std::string互換
	inline std::size_t max_size() const { return MAX_LEN; }//文字列の最大長を取得　※std::string互換
public:
	//キャストオペレータ
	inline operator const char*() const { return m_str; }
public:
	//コピーオペレータ
	fixedLenStr& operator=(const char* str);
	template<std::size_t _SRC_MAX_SIZE>
	fixedLenStr& operator=(const fixedLenStr<_SRC_MAX_SIZE>& src);
public:
	//コピーコンストラクタ
	template<std::size_t _SRC_MAX_SIZE>
	fixedLenStr(const fixedLenStr<_SRC_MAX_SIZE>& src);
	//コンストラクタ
	inline fixedLenStr(const char* str);
	//デフォルトコンストラクタ
	inline fixedLenStr();
	//デストラクタ
	inline ~fixedLenStr();
protected:
	//フィールド
	char m_str[MAX_SIZE];//文字列
	std::uint16_t m_len;//文字列長
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/fixed_len_str.inl"

#endif//GASHA_INCLUDED_FIXED_LEN_STR_H

// End of file
