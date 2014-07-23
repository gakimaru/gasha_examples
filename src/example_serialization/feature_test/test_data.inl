#pragma once
#ifndef GASHA_INCLUDED_TEST_DATA_INL
#define GASHA_INCLUDED_TEST_DATA_INL

//--------------------------------------------------------------------------------
// test_data.inl
// シリアライズテスト：機能テスト：テスト用データ定義【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト
#include "feature_test/test_data.h"//機能テスト：テスト用データ定義

#include <gasha/type_traits.h>//型特性ユーティリティ

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//テスト用データクラス

//データ1
inline int testData::data1() const
{
	return m_data1;
}
inline void testData::setData1(const int value)
{
	m_data1 = value;
}
//データ2
inline float testData::data2() const
{
	return m_data2;
}
inline void testData::setData2(const float value)
{
	m_data2 = value;
}
//データ3
inline const char* testData::data3() const
{
	return m_data3;
}
inline char testData::data3(const int index) const
{
	return m_data3[index];
}
inline void testData::setData3(const int index, const int value)
{
	m_data3[index] = value;
}
inline std::size_t testData::data3_num() const
{
	return extentof(m_data3);
}
//データ4a
inline const testData::subData& testData::data4a() const
{
	return m_data4a;
}
inline testData::subData& testData::data4a()
{
	return m_data4a;
}
//データ4b
inline const testData::subData& testData::data4b() const
{
	return m_data4b;
}
inline testData::subData& testData::data4b()
{
	return m_data4b;
}
//データ5a
inline const std::bitset<BITSET_DATA1_SIZE>& testData::data5a() const
{
	return m_data5a;
}
inline bool testData::data5a(const int index) const
{
	return m_data5a[index];
}
inline void testData::setData5a(const int index, const bool value)
{
	m_data5a[index] = value;
}
//データ5b
inline const std::bitset<BITSET_DATA2_SIZE>& testData::data5b() const
{
	return m_data5b;
}
inline bool testData::data5b(const int index) const
{
	return m_data5b[index];
}
inline void testData::setData5b(const int index, const bool value)
{
	m_data5b[index] = value;
}
//データ6
inline const testData::subData* testData::data6()
{
	return m_data6;
}
inline testData::subData& testData::data6(const int index)
{
	return m_data6[index];
}
inline const testData::subData& testData::data6(const int index) const
{
	return m_data6[index];
}
inline std::size_t testData::data6_num()
{
	return extentof(m_data6);
}
//データ7a
inline const testData::subData* testData::data7a() const
{
	return m_data7a;
}
inline testData::subData* testData::data7a()
{
	return m_data7a;
}
//データ7b
inline const testData::subData* testData::data7b() const
{
	return m_data7b;
}
inline testData::subData* testData::data7b()
{
	return m_data7b;
}
//データ7c
inline testData::subData*& testData::data7c()
{
	return m_data7c;
}
//データ7d
inline testData::subData*& testData::data7d()
{
	return m_data7d;
}
//データ7e
inline testData::subData*& testData::data7e()
{
	return m_data7e;
}
//データ8a
inline const int* testData::data8a() const
{
	return m_data8a;
}
inline int* testData::data8a()
{
	return m_data8a;
}
//データ8b
inline const int* testData::data8b() const
{
	return m_data8b;
}
inline int* testData::data8b()
{
	return m_data8b;
}
//データ9
inline int testData::data9() const
{
	return m_data9;
}
inline void testData::setData9(const int value)
{
	m_data9 = value;
}
//データ10
inline int testData::data10() const
{
	return m_data10;
}
inline void testData::setData10(const int value)
{
	m_data10 = value;
}
//データ11
inline int testData::data11() const
{
	return m_data11;
}
inline void testData::setData11(const int value)
{
	m_data11 = value;
}
//データ11(deleted)
inline int testData::data11_deleted() const
{
	return m_data11_deleted;
}
inline void testData::setData11_deleted(const int value)
{
	m_data11_deleted = value;
}
//データ12
inline int testData::data12() const
{
	return m_data12;
}
inline void testData::setData12(const int value)
{
	m_data12 = value;
}
//データ13
inline const testData::subData& testData::data13() const
{
	return m_data13;
}
inline testData::subData& testData::data13()
{
	return m_data13;
}
//データ13(deleted)
inline const testData::subData& testData::data13_deleted() const
{
	return m_data13_deleted;
}
inline testData::subData& testData::data13_deleted()
{
	return m_data13_deleted;
}
//データ14
inline const testData::subData& testData::data14() const
{
	return m_data14;
}
inline testData::subData& testData::data14()
{
	return m_data14;
}
//データ15a
inline int testData::data15a() const
{
	return m_data15a;
}
inline void testData::setData15a(const int value)
{
	m_data15a = value;
}
//データ15a
inline char testData::data15b() const
{
	return m_data15b;
}
inline void testData::setData15b(const char value)
{
	m_data15b = value;
}
//データ15c
inline long long testData::data15c() const
{
	return m_data15c;
}
inline void testData::setData15c(const long long value)
{
	m_data15c = value;
}
//データ15d
inline double testData::data15d() const
{
	return m_data15d;
}
inline void testData::setData15d(const double value)
{
	m_data15d = value;
}
//データ16a
inline const int* testData::data16a() const
{
	return m_data16a;
}
inline int testData::data16a(const int index) const
{
	return m_data16a[index];
}
inline void testData::setData16a(const int index, const int value)
{
	m_data16a[index] = value;
}
inline std::size_t testData::data16a_num() const
{
	return extentof(m_data16a);
}
//データ16b
inline const int* testData::data16b() const
{
	return m_data16b;
}
inline int testData::data16b(const int index) const
{
	return m_data16b[index];
}
inline void testData::setData16b(const int index, const int value)
{
	m_data16b[index] = value;
}
inline std::size_t testData::data16b_num() const
{
	return extentof(m_data16b);
}
//データ16c
inline const int* testData::data16c() const
{
	return m_data16c;
}
inline int testData::data16c(const int index) const
{
	return m_data16c[index];
}
inline void testData::setData16c(const int index, const int value)
{
	m_data16c[index] = value;
}
inline std::size_t testData::data16c_num() const
{
	return extentof(m_data16c);
}
//データ16d
inline int testData::data16d() const
{
	return m_data16d;
}
inline void testData::setData16d(const int value)
{
	m_data16d = value;
}
//データ16e
inline const int* testData::data16e() const
{
	return m_data16e;
}
inline int testData::data16e(const int index) const
{
	return m_data16e[index];
}
inline void testData::setData16e(const int index, const int value)
{
	m_data16e[index] = value;
}
inline std::size_t testData::data16e_num() const
{
	return extentof(m_data16e);
}

//コンストラクタ
inline testData::testData() :
	m_data1(0),
	m_data2(0.f),
	m_data4a(),
	m_data4b(),
	m_data7a(nullptr),
	m_data7b(nullptr),
	m_data7c(nullptr),
	m_data7d(nullptr),
	m_data7e(nullptr),
	m_data8a(nullptr),
	m_data8b(nullptr),
	m_data9(0),
	m_data10(0),
	m_data11(0),
	m_data11_deleted(0),
	m_data12(0),
	m_data13(),
	m_data13_deleted(),
	m_data14(),
	m_data15a(0),
	m_data15b(0),
	m_data15c(0),
	m_data15d(0),
	m_data16d(0)
{
	for (auto& data : m_data3)
	 data = 0;
	m_data5a.reset();
	m_data5b.reset();
	m_data7a = new subData();
	//m_data7b = new subData();//nullptrテストのため、メモリ確保しない
	//m_data7c = new subData();//nullptrテストのため、メモリ確保しない
	//m_data7d = new subData();//nullptrテストのため、メモリ確保しない
	//m_data7e = new subData();//nullptrテストのため、メモリ確保しない
	m_data8a = new int;
	//m_data8b = new int;//nullptrテストのため、メモリ確保しない
	for (auto& data : m_data16a)
		data = 0;
	for (auto& data : m_data16b)
		data = 0;
	for (auto& data : m_data16c)
		data = 0;
	for (auto& data : m_data16e)
		data = 0;
}

//デストラクタ
inline testData::~testData()
{
	if (m_data7a)
		delete m_data7a;
	if (m_data7b)
		delete m_data7b;
	if (m_data7c)
		delete m_data7c;
	if (m_data7d)
		delete m_data7d;
	if (m_data7e)
		delete m_data7e;
	if (m_data8a)
		delete m_data8a;
	if (m_data8b)
		delete m_data8b;
}

//ネストした構造体：コンストラクタ
inline testData::subData::subData() :
	m_a(0),
	m_b(0),
	m_c(0.f)
{}

#endif//GASHA_INCLUDED_TEST_DATA_INL

// End of file
