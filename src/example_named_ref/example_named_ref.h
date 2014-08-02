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
#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/limits.h>//限界値

#include <cstddef>//std::size_t
#include <cstdint>//C++11 std::int64_t, std::uint64_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付きデータ参照定義

//名前付きデータ参照テーブル用定数
static const std::size_t REF_TABLE_WITH_LOCK_SIZE = 32;
static const std::size_t REF_TABLE_SIZE = 10;

//名前付きデータ参照インスタンス識別用構造体
struct refTableWithLock_type : public named_ref::baseOpe<refTableWithLock_type, REF_TABLE_WITH_LOCK_SIZE>//共有ロック付き参照テーブル
{};
struct refTable_type : public named_ref::baseOpe<refTable_type, REF_TABLE_SIZE>//ロックなし参照テーブル
{};

//名前付きデータ参照の型宣言
using refTableWithLock = named_ref::table<refTableWithLock_type>;
using refTable = named_ref::table<refTable_type>;

//----------------------------------------
//名前付きデータ参照用構造体
struct data_t
{
	int m_memberA;
	char m_memberB[2];
	float m_memberC;
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
	static inline constexpr uint128_t zero(){ return uint128_t(); }//ゼロ
	static inline constexpr uint128_t min(){ return uint128_t(0x00000000, 0x00000000); }//最小値
	static inline constexpr uint128_t max(){ return uint128_t(0xffffffff, 0xffffffff); }//最大値
};
GASHA_NAMESPACE_END;//ネームスペース：終了

//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref();

#endif//GASHA_INCLUDED_EXAMPLE_NAMED_REF_H

// End of file
