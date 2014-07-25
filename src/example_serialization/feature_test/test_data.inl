#pragma once
#ifndef GASHA_INCLUDED_TEST_DATA_INL
#define GASHA_INCLUDED_TEST_DATA_INL

//--------------------------------------------------------------------------------
// test_data.inl
// シリアライズテスト：機能テスト：テスト用データ定義【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's standard library for C++ - GASHA
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
inline const short* testData::data3() const
{
	return m_data3;
}
inline short testData::data3(const int index) const
{
	return m_data3[index];
}
inline void testData::setData3(const int index, const short value)
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
inline testData::subData*& testData::data7b()
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
//データ7f
inline const testData::subData& testData::data7f() const
{
	return m_data7f;
}
inline testData::subData& testData::data7f()
{
	return m_data7f;
}
//データ7g
inline testData::subData*& testData::data7g()
{
	return m_data7g;
}
//データ7h
inline testData::subData*& testData::data7h()
{
	return m_data7h;
}
//データ7i
inline const testData::subData* testData::data7i()
{
	return m_data7i;
}
inline testData::subData& testData::data7i(const int index)
{
	return m_data7i[index];
}
inline const testData::subData& testData::data7i(const int index) const
{
	return m_data7i[index];
}
inline std::size_t testData::data7i_num()
{
	return extentof(m_data7i);
}
//データ7j
inline testData::subData*& testData::data7j()
{
	return m_data7j;
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
inline int*& testData::data8b()
{
	return m_data8b;
}
//データ8c
inline int*& testData::data8c()
{
	return m_data8c;
}
//データ8d
inline int*& testData::data8d()
{
	return m_data8d;
}
//データ8e
inline int*& testData::data8e()
{
	return m_data8e;
}
//データ8f
inline int testData::data8f() const
{
	return m_data8f;
}
inline void testData::setData8f(const int value)
{
	m_data8f = value;
}
//データ8g
inline int*& testData::data8g()
{
	return m_data8g;
}
//データ8h
inline int*& testData::data8h()
{
	return m_data8h;
}
//データ8i
inline const int* testData::data8i() const
{
	return m_data8i;
}
inline int testData::data8i(const int index) const
{
	return m_data8i[index];
}
inline void testData::setData8i(const int index, const int value)
{
	m_data8i[index] = value;
}
inline std::size_t testData::data8i_num() const
{
	return extentof(m_data8i);
}
//データ8j
inline int*& testData::data8j()
{
	return m_data8j;
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
//データ15b
inline short testData::data15b() const
{
	return m_data15b;
}
inline void testData::setData15b(const short value)
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
//データ17a
inline const testData::subData* testData::data17a()
{
	return m_data17a;
}
inline testData::subData& testData::data17a(const int index)
{
	return m_data17a[index];
}
inline const testData::subData& testData::data17a(const int index) const
{
	return m_data17a[index];
}
inline std::size_t testData::data17a_num()
{
	return extentof(m_data17a);
}
//データ17b
inline const testData::subData* testData::data17b()
{
	return m_data17b;
}
inline testData::subData& testData::data17b(const int index)
{
	return m_data17b[index];
}
inline const testData::subData& testData::data17b(const int index) const
{
	return m_data17b[index];
}
inline std::size_t testData::data17b_num()
{
	return extentof(m_data17b);
}
//データ17c
inline const testData::subData* testData::data17c()
{
	return m_data17c;
}
inline testData::subData& testData::data17c(const int index)
{
	return m_data17c[index];
}
inline const testData::subData& testData::data17c(const int index) const
{
	return m_data17c[index];
}
inline std::size_t testData::data17c_num()
{
	return extentof(m_data17c);
}
//データ17d
inline const testData::subData& testData::data17d() const
{
	return m_data17d;
}
inline testData::subData& testData::data17d()
{
	return m_data17d;
}
//データ17e
inline const testData::subData* testData::data17e()
{
	return m_data17e;
}
inline testData::subData& testData::data17e(const int index)
{
	return m_data17e[index];
}
inline const testData::subData& testData::data17e(const int index) const
{
	return m_data17e[index];
}
inline std::size_t testData::data17e_num()
{
	return extentof(m_data17e);
}
//データ18a
inline int testData::data18a() const
{
	return m_data18a;
}
inline void testData::setData18a(const int value)
{
	m_data18a = value;
}
//データ18b
inline const testData::subData& testData::data18b() const
{
	return m_data18b;
}
inline testData::subData& testData::data18b()
{
	return m_data18b;
}
//データ19a
inline int testData::data19a() const
{
	return m_data19a;
}
inline void testData::setData19a(const int value)
{
	m_data19a = value;
}
//データ19b
inline const testData::subData3a& testData::data19b() const
{
	return m_data19b;
}
inline testData::subData3a& testData::data19b()
{
	return m_data19b;
}
//データ20a
inline const testData::subData& testData::data20a() const
{
	return m_data20a;
}
inline testData::subData& testData::data20a()
{
	return m_data20a;
}
//データ20b
inline const testData::subData3a& testData::data20b() const
{
	return m_data20b;
}
inline testData::subData3a& testData::data20b()
{
	return m_data20b;
}
//データ21a
inline const testData::subData3a& testData::data21a() const
{
	return m_data21a;
}
inline testData::subData3a& testData::data21a()
{
	return m_data21a;
}
//データ21b
inline const testData::subData3b& testData::data21b() const
{
	return m_data21b;
}
inline testData::subData3b& testData::data21b()
{
	return m_data21b;
}
//データ21c
inline const testData::subData3c& testData::data21c() const
{
	return m_data21c;
}
inline testData::subData3c& testData::data21c()
{
	return m_data21c;
}
//データ22a
inline const testData::subData& testData::data22a() const
{
	return m_data22a;
}
inline testData::subData& testData::data22a()
{
	return m_data22a;
}
//データ22b
inline const testData::subData2& testData::data22b() const
{
	return m_data22b;
}
inline testData::subData2& testData::data22b()
{
	return m_data22b;
}
//データ23
inline const testData::subDataExt& testData::data23() const
{
	return m_data23;
}
inline testData::subDataExt& testData::data23()
{
	return m_data23;
}
//データ24
inline const testData::subDataExt2& testData::data24() const
{
	return m_data24;
}
inline testData::subDataExt2& testData::data24()
{
	return m_data24;
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
	m_data7f(),
	m_data7g(nullptr),
	m_data7h(nullptr),
	m_data7j(nullptr),
	m_data8a(nullptr),
	m_data8b(nullptr),
	m_data8c(nullptr),
	m_data8d(nullptr),
	m_data8e(nullptr),
	m_data8f(),
	m_data8g(nullptr),
	m_data8h(nullptr),
	m_data8j(nullptr),
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
	m_data16d(0),
	m_data17d(),
	m_data18a(0),
	m_data18b(),
	m_data19a(0),
	m_data19b(),
	m_data20a(),
	m_data20b(),
	m_data21a(),
	m_data21b(),
	m_data21c(),
	m_data22a(),
	m_data22b(),
	m_data23(),
	m_data24()
{
	for (auto& data : m_data3)
	 data = 0;
	for (auto& data : m_data6)
		data = subData();
	m_data5a.reset();
	m_data5b.reset();
	m_data7a = new subData();
	//m_data7b = new subData();//nullptrテストのため、メモリ確保しない
	//m_data7c = new subData();//nullptrテストのため、メモリ確保しない
	//m_data7d = new subData();//nullptrテストのため、メモリ確保しない
	//m_data7e = new subData();//nullptrテストのため、メモリ確保しない
	m_data7g = new subData();
	m_data7h = new subData[2];
	for (int i = 0; i < 2; ++i)
		m_data7h[i] = subData();
	for (auto& data : m_data7i)
		data = subData();
	m_data7j = new subData[2];
	for (int i = 0; i < 2; ++i)
		m_data7j[i] = subData();
	m_data8a = new int(0);
	//m_data8b = new int;//nullptrテストのため、メモリ確保しない
	//m_data8c = new int;//nullptrテストのため、メモリ確保しない
	//m_data8d = new int;//nullptrテストのため、メモリ確保しない
	//m_data8e = new int;//nullptrテストのため、メモリ確保しない
	m_data8g = new int(0);
	m_data8h = new int[2];
	for (int i = 0; i < 2; ++i)
		m_data8h[i] = 0;
	for (auto& data : m_data8i)
		data = 0;
	m_data8j = new int[2];
	for (int i = 0; i < 2; ++i)
		m_data8j[i] = 0;
	for (auto& data : m_data16a)
		data = 0;
	for (auto& data : m_data16b)
		data = 0;
	for (auto& data : m_data16c)
		data = 0;
	for (auto& data : m_data16e)
		data = 0;
	for (auto& data : m_data17a)
		data = subData();
	for (auto& data : m_data17b)
		data = subData();
	for (auto& data : m_data17c)
		data = subData();
	for (auto& data : m_data17e)
		data = subData();
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
	if (m_data7g)
		delete m_data7g;
	if (m_data7h)
		delete[] m_data7h;
	if (m_data7j)
		delete[] m_data7j;
	if (m_data8a)
		delete m_data8a;
	if (m_data8b)
		delete m_data8b;
	if (m_data8c)
		delete m_data8c;
	if (m_data8d)
		delete m_data8d;
	if (m_data8e)
		delete m_data8e;
	if (m_data8g)
		delete m_data8g;
	if (m_data8h)
		delete[] m_data8h;
	if (m_data8j)
		delete[] m_data8j;
}

//ネストした構造体：コンストラクタ
inline testData::subData::subData() :
	m_a(0),
	m_b(0),
	m_c(0.f)
{}

//ネストした構造体：コンストラクタ
inline testData::subData2::subData2() :
	m_d(0),
	m_c(0.),
	m_b(0),
	m_a(0)
{}

//ネストした構造体：コンストラクタ
inline testData::subData3a::subData3a() :
	m_a(0),
	m_b(0),
	m_c(0.f)
{}

//ネストした構造体：コンストラクタ
inline testData::subData3b::subData3b() :
	m_a(0),
	m_b(0)
{}

//ネストした構造体：コンストラクタ
inline testData::subData3c::subData3c() :
	m_a(0),
	m_b(0),
	m_c(0.f),
	m_d(0)
{}

//継承した構造体：コンストラクタ
inline testData::subDataExt::subDataExt() :
	subData(),
	m_z(0)
{}

//多重継承した構造体：コンストラクタ
inline testData::subDataExt2::subDataExt2() :
	subData(),
	subData2(),
	m_z(0)
{}

#endif//GASHA_INCLUDED_TEST_DATA_INL

// End of file
