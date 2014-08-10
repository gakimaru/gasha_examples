#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_NAMED_REF_H
#define GASHA_INCLUDED_EXAMPLE_NAMED_REF_H

//--------------------------------------------------------------------------------
// exmaple_named_ref.h
// 名前付きデータ参照テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/named_ref.h>//名前付きデータ参照
#include <gasha/named_func.h>//名前付き関数
#include <gasha/named_value.h>//名前付き定数
#include <gasha/str_pool.h>//文字列プール
#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/limits.h>//限界値

#include <cstddef>//std::size_t
#include <cstdint>//C++11 std::int64_t, std::uint64_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付きデータ参照定義

//名前付きデータ参照テーブル用定数
static const std::size_t REF_TABLE_SIZE = 32;
static const std::size_t REF_TABLE_EX_SIZE = 10;

//名前付きデータ参照操作用構造体
//※インスタンスの区別を用途として兼ねる
struct refTable_type : public named_ref::baseOpe<refTable_type, REF_TABLE_SIZE>
//struct refTable_type : public namedRef_baseOpe<refTable_type, REF_TABLE_SIZE>//※別名バージョン
{
	typedef sharedSpinLock lock_type;//ロックポリシー
};

//名前付きデータ参照の型宣言
using refTable = named_ref::table<refTable_type>;
//using refTable = namedRef<refTable_type>;//※別名バージョン

//※複数インスタンスも生成可能
struct refTableEx_type : public named_ref::baseOpe<refTableEx_type, REF_TABLE_EX_SIZE>{};
using refTableEx = named_ref::table<refTableEx_type>;

//名前付き関数テーブル用定数
static const std::size_t FUNC_GROUP_TABLE_SIZE = 8;
static const std::size_t FUNC_TABLE_SIZE = 32;

//名前付き関数操作用構造体
struct funcTable_type : public named_func::baseOpe<funcTable_type, FUNC_GROUP_TABLE_SIZE, FUNC_TABLE_SIZE>
//struct funcTable_type : public namedFunc_baseOpe<funcTable_type, FUNC_GROUP_TABLE_SIZE, FUNC_TABLE_SIZE>//※別名バージョン
{
	typedef sharedSpinLock lock_type;//ロックポリシー
};

//名前付き関数の型宣言
using funcTable = named_func::table<funcTable_type>;
//using funcTable = namedFunc<funcTable_type>;//※別名バージョン

//名前付き定数テーブル用定数
static const std::size_t VALUE_GROUP_TABLE_SIZE = 8;
static const std::size_t VALUE_TABLE_SIZE = 32;
static const std::size_t VALUE_TABLE_STR_BUFF = 8192;

//名前付き定数操作用構造体
struct valueTable_type : public named_value::baseOpe<valueTable_type, VALUE_GROUP_TABLE_SIZE, VALUE_TABLE_SIZE>
//struct valueTable_type : public namedValue_baseOpe<valueTable_type, VALUE_GROUP_TABLE_SIZE, VALUE_TABLE_SIZE>//※別名バージョン
{
	typedef sharedSpinLock lock_type;//ロックポリシー
};

//名前付き定数の型宣言
using valueTable = named_value::table<valueTable_type>;
//using valueTable = namedValue<valueTable_type>;//※別名バージョン

//名前付き定数用の文字列プール
typedef strPool<VALUE_TABLE_STR_BUFF, VALUE_TABLE_SIZE, sharedSpinLock> valueStrPool;

//----------------------------------------
//名前付きデータ参照用構造体
struct data_t
{
	int m_memberA;
	char m_memberB[2];
	float m_memberC;
	int methodA(const int val);
};

//----------------------------------------
//簡易符号無し128ビット型
//※加減算のみ対応
struct uint128_t
{
	std::uint64_t m_hi;
	std::uint64_t m_lo;

	inline bool operator==(const uint128_t rhs) const { return m_hi == rhs.m_hi && m_lo == rhs.m_lo; }
	inline bool operator!=(const uint128_t rhs) const { return m_hi != rhs.m_hi || m_lo != rhs.m_lo; }
	inline bool operator<(const uint128_t rhs) const { return m_hi < rhs.m_hi || (m_hi == rhs.m_hi && m_lo < rhs.m_lo); }
	inline bool operator<=(const uint128_t rhs) const { return m_hi <= rhs.m_hi || (m_hi == rhs.m_hi && m_lo <= rhs.m_lo); }
	inline bool operator>(const uint128_t rhs) const { return m_hi > rhs.m_hi || (m_hi == rhs.m_hi && m_lo > rhs.m_lo); }
	inline bool operator>=(const uint128_t rhs) const { return m_hi >= rhs.m_hi || (m_hi == rhs.m_hi && m_lo >= rhs.m_lo); }

	inline bool operator==(const std::uint64_t rhs) const { return m_hi == 0 && m_lo == rhs; }
	inline bool operator!=(const std::uint64_t rhs) const { return m_hi != 0 || m_lo != rhs; }
	inline bool operator<(const std::uint64_t rhs) const { return m_hi < 0 || (m_hi == 0 && m_lo < rhs); }
	inline bool operator<=(const std::uint64_t rhs) const { return m_hi <= 0 || (m_hi == 0 && m_lo <= rhs); }
	inline bool operator>(const std::uint64_t rhs) const { return m_hi > 0 || (m_hi == 0 && m_lo > rhs); }
	inline bool operator>=(const std::uint64_t rhs) const { return m_hi >= 0 || (m_hi == 0 && m_lo >= rhs); }

	inline operator bool() const { return m_hi != 0 || m_lo != 0; }

	inline uint128_t& operator++(){ ++m_lo; if (m_lo == 0) ++m_hi; return *this; }
	inline uint128_t operator++(int){ const uint128_t prev = *this; ++(*this); return prev; }
	inline uint128_t& operator--(){ --m_lo; if (m_lo == 0xffffffff) --m_hi; return *this; }
	inline uint128_t operator--(int){ const uint128_t prev = *this; --(*this); return prev; }
	inline uint128_t& operator+=(const uint128_t rhs){ const std::uint64_t prev = m_lo; m_lo += rhs.m_lo; m_hi += rhs.m_hi; if (prev > m_lo) ++m_hi; return *this; }
	inline uint128_t operator+(const uint128_t rhs) const { uint128_t result = *this; result += rhs; return result; }
	inline uint128_t& operator-=(const uint128_t rhs){ const std::uint64_t prev = m_lo; m_lo -= rhs.m_lo; m_hi -= rhs.m_hi; if (prev < m_lo) --m_hi; return *this; }
	inline uint128_t operator-(const uint128_t rhs) const { uint128_t result = *this; result -= rhs; return result; }

	inline uint128_t& operator=(uint128_t&& rhs){ m_hi = rhs.m_hi; m_lo = rhs.m_lo; return *this; }
	inline uint128_t& operator=(const uint128_t& rhs){ m_hi = rhs.m_hi; m_lo = rhs.m_lo; return *this; }
	inline uint128_t& operator=(const std::uint64_t rhs){ m_hi = 0; m_lo = rhs; return *this; }
	
	inline constexpr uint128_t(uint128_t&& obj) : m_hi(obj.m_hi), m_lo(obj.m_lo) {}
	inline constexpr uint128_t(const uint128_t& obj) : m_hi(obj.m_hi), m_lo(obj.m_lo) {}
	inline constexpr uint128_t(const std::uint64_t hi, const std::uint64_t lo) : m_hi(hi), m_lo(lo) {}
	inline constexpr uint128_t(const std::uint64_t val) : m_hi(0), m_lo(val) {}
	inline constexpr uint128_t() : m_hi(0), m_lo(0) {}
};
//型の限界値情報：std::int8_t型
GASHA_NAMESPACE_BEGIN;//ネームスペース：開始
template<>
class numeric_limits<::uint128_t>
{
public:
	static const std::size_t SIZE = sizeof(uint128_t);//サイズ
	inline static constexpr uint128_t zero(){ return uint128_t(); }//ゼロ
	inline static constexpr uint128_t min(){ return uint128_t(0x00000000, 0x00000000); }//最小値
	inline static constexpr uint128_t max(){ return uint128_t(0xffffffff, 0xffffffff); }//最大値
};

//----------------------------------------
//名前付き関数テスト用
struct objType
{
	bool memberA(const int a, const int b);
	void memberB() const;
	void memberB_noconst();
	int m_mem;
};

//----------------------------------------
//名前付き定数テスト用
enum testValues
{
	testValueA = 0,
	testValueB,
	testValueC,
};

GASHA_NAMESPACE_END;//ネームスペース：終了

//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref();

//----------------------------------------
//名前付きデータ参照の列挙
void enumNamedRef();
//名前付き関数の列挙
void enumNamedFunc();
//名前付き定数の列挙
void enumNamedValue();

#endif//GASHA_INCLUDED_EXAMPLE_NAMED_REF_H

// End of file
