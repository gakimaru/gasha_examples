#pragma once
#ifndef GASHA_INCLUDED_TEST_DATA_H
#define GASHA_INCLUDED_TEST_DATA_H

//--------------------------------------------------------------------------------
// test_data.h
// シリアライズテスト：機能テスト：テスト用データ定義
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/serialization/friends.h>//シリライズ/フレンド宣言
                                        //※シリアライズ対象クラスの宣言時に必要なインクルードファイルはこれのみ

//【VC++】ワーニング設定を退避
#pragma warning(push)

//【VC++】例外を無効化した状態で <bitset> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <bitset>//std::bitset

static const std::size_t BITSET_DATA1_SIZE = 256;
static const std::size_t BITSET_DATA2_SIZE = 16;

//--------------------
//テスト用データクラス
class testData
{
	//クラスをシリアライズ可能にするためのフレンド宣言
	//※シリアライズ対象クラスの宣言時に必要な設定はこれのみ
	GASHA_SERIALIZATION_FRIENDS();
public:
	//ネストした構造体
	struct subData
	{
		int m_a;
		char m_b;
		float m_c;
		//コンストラクタ
		inline subData();
	};
public:
	//アクセッサ
	inline int data1() const;//データ1取得
	inline void setData1(const int value);//データ1更新
	inline float data2() const;//データ2取得
	inline void setData2(const float value);//データ2更新
	inline const char* data3() const;//データ3取得
	inline char data3(const int index) const;//データ3取得
	inline void setData3(const int index, const int value);//データ3更新
	inline std::size_t data3_num() const;//データ3配列要素数取得
	inline const subData& data4a() const;//データ4a取得
	inline subData& data4a();//データ4a取得
	inline const subData& data4b() const;//データ4b取得
	inline subData& data4b();//データ4b取得
	inline const std::bitset<BITSET_DATA1_SIZE>& data5a() const;//データ5a取得
	inline bool data5a(const int index) const;//データ5a取得
	inline void setData5a(const int index, const bool value);//データ5a更新
	inline const std::bitset<BITSET_DATA2_SIZE>& data5b() const;//データ5a取得
	inline bool data5b(const int index) const;//データ5b取得
	inline void setData5b(const int index, const bool value);//データ5b更新
	inline const subData* data6();//データ6取得
	inline const subData& data6(const int index) const;//データ6取得
	inline subData& data6(const int index);//データ6取得
	inline std::size_t data6_num();//データ6配列要素数取得
	inline const subData* data7a() const;//データ7a取得
	inline subData* data7a();//データ7a取得
	inline const subData* data7b() const;//データ7b取得
	inline subData* data7b();//データ7b取得
	inline subData*& data7c();//データ7c取得
	inline subData*& data7d();//データ7d取得
	inline subData*& data7e();//データ7e取得
	inline const int* data8a() const;//データ8a取得
	inline int* data8a();//データ8a取得
	inline const int* data8b() const;//データ8b取得
	inline int* data8b();//データ8b取得
	inline int data9() const;//データ9取得
	inline void setData9(const int value);//データ9更新
	inline int data10() const;//データ10取得
	inline void setData10(const int value);//データ10更新
	inline int data11() const;//データ11取得
	inline void setData11(const int value);//データ11更新
	inline int data11_deleted() const;//データ11(deleted)取得
	inline void setData11_deleted(const int value);//データ11(deleted)更新
	inline int data12() const;//データ12取得
	inline void setData12(const int value);//データ12更新
	inline const subData& data13() const;//データ13取得
	inline subData& data13();//データ13取得
	inline const subData& data13_deleted() const;//データ13(deleted)取得
	inline subData& data13_deleted();//データ13(deleted)取得
	inline const subData& data14() const;//データ14取得
	inline subData& data14();//データ14取得
	inline int data15a() const;//データ15a取得
	inline void setData15a(const int value);//データ15a更新
	inline char data15b() const;//データ15b取得
	inline void setData15b(const char value);//データ15b更新
	inline long long data15c() const;//データ15c取得
	inline void setData15c(const long long value);//データ15c更新
	inline double data15d() const;//データ15d取得
	inline void setData15d(const double value);//データ15d更新
	inline const int* data16a() const;//データ16a取得
	inline int data16a(const int index) const;//データ16a取得
	inline void setData16a(const int index, const int value);//データ16a更新
	inline std::size_t data16a_num() const;//データ16a配列要素数取得
	inline const int* data16b() const;//データ16b取得
	inline int data16b(const int index) const;//データ16b取得
	inline void setData16b(const int index, const int value);//データ16b更新
	inline std::size_t data16b_num() const;//データ16b配列要素数取得
	inline const int* data16c() const;//データ16c取得
	inline int data16c(const int index) const;//データ16c取得
	inline void setData16c(const int index, const int value);//データ16c更新
	inline std::size_t data16c_num() const;//データ16c配列要素数取得
	inline int data16d() const;//データ16d取得
	inline void setData16d(const int value);//データ16d更新
	inline const int* data16e() const;//データ16e取得
	inline int data16e(const int index) const;//データ16e取得
	inline void setData16e(const int index, const int value);//データ16e更新
	inline std::size_t data16e_num() const;//データ16e配列要素数取得
public:
	//コンストラクタ
	inline testData();
	//デストラクタ
	inline ~testData();
private:
	//フィールド
	int m_data1;//データ1
	float m_data2;//データ2
	char m_data3[3];//データ3
	subData m_data4a;//データ4a
	subData m_data4b;//データ4b
	std::bitset<BITSET_DATA1_SIZE> m_data5a;//データ5a
	std::bitset<BITSET_DATA2_SIZE> m_data5b;//データ5b
	subData m_data6[2];//データ6
	subData* m_data7a;//データ7a ※構造体のポインタテスト用
	subData* m_data7b;//データ7b ※構造体の nullptr テスト用（nullptr に nullptrをロードするテスト）
	subData* m_data7c;//データ7c ※構造体の nullptr テスト用（実体 に nullptrをロードするテスト）
	subData* m_data7d;//データ7d ※構造体の nullptr テスト用（nullptr に 実体をロードするテスト）
	subData* m_data7e;//データ7e ※ロード前処理でメモリ確保テスト用
	int* m_data8a;//データ8a ※プリミティブ型のポインタテスト用
	int* m_data8b;//データ8b ※プリミティブ型の nullptr テスト用
	int m_data9;//データ9 ※シリアライズ対象外データ項目（ロード後に m_data1 + m_data2 を格納）
	int m_data10;//データ10 ※暗号化シリアライズデータ（シリアライズとでシリアライズで処理を共通化できないデータ項目のテスト用）
	int m_data11;//データ11 ※削除されたデータ項目...という扱い（セーブデータにだけ存在し、実際のデータ構造からは削除されたデータ項目のテスト用）
	int m_data11_deleted;//データ11(deleted) ※削除されたデータ項目をロード時に移し替える確認用
	int m_data12;//データ12 ※追加されたデータ項目...という扱い（セーブデータに存在せず、実際のデータ構造に追加されたデータ項目のテスト用）
	subData m_data13;//データ13 ※削除されたデータ項目（オブジェクト）...という扱い
	subData m_data13_deleted;//データ13(deleted) ※削除されたデータ項目をロード時に移し替える確認用
	subData m_data14;//データ14 ※追加されたデータ項目（オブジェクト）...という扱い
	int m_data15a;//データ15a
	char m_data15b;//データ15b ※データ型が縮小された...という扱い（15aを15bとしてシリアライズする）
	long long m_data15c;//データ15c ※データ型が拡大された...という扱い（15aを15cとしてシリアライズする）
	double m_data15d;//データ15d ※データ型が変更された...という扱い（15aを15dとしてシリアライズする）
	int m_data16a[3];//データ16a
	int m_data16b[2];//データ16b ※配列サイズが縮小された...という扱い（16aを16bとしてシリアライズする）
	int m_data16c[4];//データ16c ※配列サイズが拡大された...という扱い（16aを16cとしてシリアライズする）
	int m_data16d;//データ16d ※配列から非配列に変更された...という扱い（16aを16dとしてシリアライズする）
	int m_data16e[2];//データ16e ※非配列から配列に変更された...という扱い（16dを16eとしてシリアライズする）
};

//【VC++】ワーニング設定を復元
#pragma warning(pop)

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "feature_test/test_data.inl"

#endif//GASHA_INCLUDED_TEST_DATA_H

// End of file
