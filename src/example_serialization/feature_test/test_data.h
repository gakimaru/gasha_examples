#pragma once
#ifndef GASHA_INCLUDED_TEST_DATA_H
#define GASHA_INCLUDED_TEST_DATA_H

//--------------------------------------------------------------------------------
// test_data.h
// シリアライズテスト：機能テスト：テスト用データ定義【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/serialization/friends.h>//シリライズ/フレンド宣言
                                        //※シリアライズ対象クラスの宣言時に必要なインクルードファイル
                                        //※シリアライズの際に、クラス／構造体のプライベートメンバーにアクセスする場合に必要（必須ではない）

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <bitset>//std::bitset
#pragma warning(pop)//【VC++】ワーニング設定を復元

static const std::size_t BITSET_DATA1_SIZE = 256;
static const std::size_t BITSET_DATA2_SIZE = 16;

//--------------------
//テスト用データクラス
class testData
{
	//シリアライズでプライベートメンバーにアクセスするためのフレンド宣言
	//※シリアライズ対象クラスの宣言時に必要な設定はこれのみ
	//※プライベートメンバーにアクセスする必要がなければ不要
	GASHA_SERIALIZATION_FRIENDS();
public:
	//ネストした構造体
	//※シリアライズが設定された構造体
	struct subData
	{
		int m_a;
		short m_b;
		float m_c;
		//コンストラクタ
		inline subData();
	};
	//ネストした構造体2
	//※シリアライズが設定された構造体
	struct subData2
	{
		int m_d;
		double m_c;
		short m_b;
		short m_a;
		//コンストラクタ
		inline subData2();
	};
	//ネストした構造体3a
	//※シリアライズが設定されていない構造体
	struct subData3a
	{
		int m_a;
		short m_b;
		float m_c;
		//コンストラクタ
		inline subData3a();
	};
	//ネストした構造体3b
	//※シリアライズが設定されていない構造体
	struct subData3b
	{
		int m_a;
		short m_b;
		//コンストラクタ
		inline subData3b();
	};
	//ネストした構造体3c
	//※シリアライズが設定されていない構造体
	struct subData3c
	{
		int m_a;
		short m_b;
		float m_c;
		long long m_d;
		//コンストラクタ
		inline subData3c();
	};
	//継承した構造体
	//※シリアライズが設定された構造体
	struct subDataExt : public subData
	{
		long long m_z;
		//コンストラクタ
		inline subDataExt();
	};
	//多重継承した構造体
	//※シリアライズが設定された構造体
	struct subDataExt2 : public subData, public subData2
	{
		long long m_z;
		//コンストラクタ
		inline subDataExt2();
	};
public:
	//アクセッサ
	inline int data1() const;//データ1取得
	inline void setData1(const int value);//データ1更新
	inline float data2() const;//データ2取得
	inline void setData2(const float value);//データ2更新
	inline const short* data3() const;//データ3取得
	inline short data3(const int index) const;//データ3取得
	inline void setData3(const int index, const short value);//データ3更新
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
	inline subData*& data7b();//データ7b取得
	inline subData*& data7c();//データ7c取得
	inline subData*& data7d();//データ7d取得
	inline subData*& data7e();//データ7e取得
	inline subData& data7f();//データ7f取得
	inline const subData& data7f() const;//データ7f取得
	inline subData*& data7g();//データ7g取得
	inline subData*& data7h();//データ7h取得
	inline const subData* data7i();//データ7i取得
	inline const subData& data7i(const int index) const;//データ7i取得
	inline subData& data7i(const int index);//データ7i取得
	inline std::size_t data7i_num();//データ7i配列要素数取得
	inline subData*& data7j();//データ7j取得
	inline const int* data8a() const;//データ8a取得
	inline int* data8a();//データ8a取得
	inline int*& data8b();//データ8b取得
	inline int*& data8c();//データ8c取得
	inline int*& data8d();//データ8d取得
	inline int*& data8e();//データ8e取得
	inline int data8f() const;//データ8f取得
	inline void setData8f(const int value);//データ8f更新
	inline int*& data8g();//データ8g取得
	inline int*& data8h();//データ8h取得
	inline const int* data8i() const;//データ8i取得
	inline int data8i(const int index) const;//データ8i取得
	inline void setData8i(const int index, const int value);//データ8i更新
	inline std::size_t data8i_num() const;//データ8i配列要素数取得
	inline int*& data8j();//データ8j取得
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
	inline short data15b() const;//データ15b取得
	inline void setData15b(const short value);//データ15b更新
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
	inline const subData* data17a();//データ17a取得
	inline const subData& data17a(const int index) const;//データ17a取得
	inline subData& data17a(const int index);//データ17a取得
	inline std::size_t data17a_num();//データ17a配列要素数取得
	inline const subData* data17b();//データ17b取得
	inline const subData& data17b(const int index) const;//データ17b取得
	inline subData& data17b(const int index);//データ17b取得
	inline std::size_t data17b_num();//データ17b配列要素数取得
	inline const subData* data17c();//データ17c取得
	inline const subData& data17c(const int index) const;//データ17c取得
	inline subData& data17c(const int index);//データ17c取得
	inline std::size_t data17c_num();//データ17c配列要素数取得
	inline const subData& data17d() const;//データ17d取得
	inline subData& data17d();//データ17d取得
	inline const subData* data17e();//データ17e取得
	inline const subData& data17e(const int index) const;//データ17e取得
	inline subData& data17e(const int index);//データ17e取得
	inline std::size_t data17e_num();//データ17e配列要素数取得
	inline int data18a() const;//データ18a取得
	inline void setData18a(const int value);//データ18a更新
	inline const subData& data18b() const;//データ18b取得
	inline subData& data18b();//データ18b取得
	inline int data19a() const;//データ19a取得
	inline void setData19a(const int value);//データ19a更新
	inline const subData3a& data19b() const;//データ19b取得
	inline subData3a& data19b();//データ19b取得
	inline const subData& data20a() const;//データ20a取得
	inline subData& data20a();//データ20a取得
	inline const subData3a& data20b() const;//データ20b取得
	inline subData3a& data20b();//データ20b取得
	inline const subData3a& data21a() const;//データ21a取得
	inline subData3a& data21a();//データ21a取得
	inline const subData3b& data21b() const;//データ21b取得
	inline subData3b& data21b();//データ21b取得
	inline const subData3c& data21c() const;//データ21c取得
	inline subData3c& data21c();//データ21c取得
	inline const subData& data22a() const;//データ22a取得
	inline subData& data22a();//データ22a取得
	inline const subData2& data22b() const;//データ22b取得
	inline subData2& data22b();//データ22b取得
	inline const subDataExt& data23() const;//データ23取得
	inline subDataExt& data23();//データ23取得
	inline const subDataExt2& data24() const;//データ24取得
	inline subDataExt2& data24();//データ24取得
public:
	//コンストラクタ
	inline testData();
	//デストラクタ
	inline ~testData();
private:
	//フィールド
	int m_data1;//データ1 ※単純なデータ項目のテスト
	float m_data2;//データ2 ※単純なデータ項目のテスト
	short m_data3[3];//データ3 ※配列のテスト
	subData m_data4a;//データ4a ※構造体をバイナリデータとしてシリアライズするテスト
	subData m_data4b;//データ4b ※構造体を独自の形式でシリアライズするテスト
	std::bitset<BITSET_DATA1_SIZE> m_data5a;//データ5a ※標準ライブラリのデータ型をバイナリデータとしてシリアライズするテスト
	std::bitset<BITSET_DATA2_SIZE> m_data5b;//データ5b ※標準ライブラリのデータ型を独自の形式でシリアライズするテスト
	subData m_data6[2];//データ6 ※構造体の配列テスト
	subData* m_data7a;//データ7a ※構造体のポインタテスト用（実体に実体をロードするテスト） ※ロード先の実体はインスタンス生成時にメモリ確保
	subData* m_data7b;//データ7b ※構造体のポインタテスト用（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
	subData* m_data7c;//データ7c ※構造体の nullptr テスト用（nullptr に nullptrをロードするテスト）
	subData* m_data7d;//データ7d ※構造体の nullptr テスト用（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
	subData* m_data7e;//データ7e ※構造体の nullptr テスト用（nullptr に 実体をロードするテスト）
	subData m_data7f;//データ7f ※構造体のポインタを実体に変更...という扱い（7gを7fとしてシリアライズする）
	subData* m_data7g;//データ7g ※構造体の実体をポインタに変更...という扱い（7fを7gとしてシリアライズする）
	subData* m_data7h;//データ7h ※構造体の配列のポインタテスト用（実体に実体をロードするテスト）
	subData m_data7i[2];//データ7i ※構造体のポインタを実体の配列に変更...という扱い（7jを7iとしてシリアライズする）
	subData* m_data7j;//データ7j ※構造体の実体の配列をポインタに変更...という扱い（7iを7jとしてシリアライズする）
	int* m_data8a;//データ8a ※プリミティブ型のポインタテスト用（実体に実体をロードするテスト） ※ロード先の実体はインスタンス生成時にメモリ確保
	int* m_data8b;//データ8b ※プリミティブ型のポインタテスト用（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
	int* m_data8c;//データ8c ※プリミティブ型の nullptr テスト用（nullptr に nullptrをロードするテスト）
	int* m_data8d;//データ8d ※プリミティブ型の nullptr テスト用（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
	int* m_data8e;//データ8e ※プリミティブ型の nullptr テスト用（nullptr に 実体をロードするテスト）
	int m_data8f;//データ8f ※プリミティブ型のポインタを実体に変更...という扱い（8gを8fとしてシリアライズする）
	int* m_data8g;//データ8g ※プリミティブ型の実体をポインタに変更...という扱い（8fを8gとしてシリアライズする）
	int* m_data8h;//データ8h ※プリミティブ型の配列のポインタテスト用（実体に実体をロードするテスト）
	int m_data8i[2];//データ8i ※プリミティブ型のポインタに実体の配列に変更...という扱い（8jを8iとしてシリアライズする）
	int* m_data8j;//データ8j ※プリミティブ型の実体の配列をポインタに変更...という扱い（8iを8jとしてシリアライズする）
	int m_data9;//データ9 ※シリアライズ対象外データ項目（ロード後に m_data3[0]～[2]の合計を格納）
	int m_data10;//データ10 ※暗号化シリアライズデータ（シリアライズとでシリアライズで処理を共通化できないデータ項目のテスト用）
	int m_data11;//データ11 ※削除されたデータ項目...という扱い（セーブデータにだけ存在し、実際のデータ構造からは削除されたデータ項目のテスト用）
	int m_data11_deleted;//データ11(deleted) ※削除されたデータ項目をロード時に移し替える確認用
	int m_data12;//データ12 ※追加されたデータ項目...という扱い（セーブデータに存在せず、実際のデータ構造に追加されたデータ項目のテスト用）
	subData m_data13;//データ13 ※削除されたデータ項目（オブジェクト）...という扱い
	subData m_data13_deleted;//データ13(deleted) ※削除されたデータ項目をロード時に移し替える確認用
	subData m_data14;//データ14 ※追加されたデータ項目（オブジェクト）...という扱い
	int m_data15a;//データ15a
	short m_data15b;//データ15b ※データ型が縮小された...という扱い（15aを15bとしてシリアライズする）
	long long m_data15c;//データ15c ※データ型が拡大された...という扱い（15aを15cとしてシリアライズする）
	double m_data15d;//データ15d ※データ型が変更された...という扱い（15aを15dとしてシリアライズする） ※浮動小数点型の変換は不可（float←→doubleも不可）
	int m_data16a[3];//データ16a
	int m_data16b[2];//データ16b ※配列サイズが縮小された...という扱い（16aを16bとしてシリアライズする）
	int m_data16c[4];//データ16c ※配列サイズが拡大された...という扱い（16aを16cとしてシリアライズする）
	int m_data16d;//データ16d ※配列から非配列に変更された...という扱い（16aを16dとしてシリアライズする）
	int m_data16e[2];//データ16e ※非配列から配列に変更された...という扱い（16dを16eとしてシリアライズする）
	subData m_data17a[3];//データ17a
	subData m_data17b[2];//データ17b ※構造体の配列サイズが縮小された...という扱い（17aを17bとしてシリアライズする）
	subData m_data17c[4];//データ17c ※構造体の配列サイズが拡大された...という扱い（17aを17cとしてシリアライズする）
	subData m_data17d;//データ17d ※構造体の配列から非配列に変更された...という扱い（17aを17dとしてシリアライズする）
	subData m_data17e[2];//データ17e ※構造体の非配列から配列に変更された...という扱い（17dを17eとしてシリアライズする）
	int m_data18a;//データ18a ※構造体（専用シリアライズあり）からプリミティブ型に変更された...という扱い（18bを18aとしてシリアライズする）※デシリアライズできないので注意
	subData m_data18b;//データ18b ※プリミティブ型から構造体（専用シリアライズあり）に変更された...という扱い（18aを18bとしてシリアライズする）
	int m_data19a;//データ19a ※構造体（専用シリアライズなし）からプリミティブ型に変更された...という扱い（19bを19aとしてシリアライズする）
	subData3a m_data19b;//データ19b ※プリミティブ型から構造体（専用シリアライズなし）に変更された...という扱い（19aを19bとしてシリアライズする）
	subData m_data20a;//データ20a ※構造体（専用シリアライズなし）から構造体（専用シリアライズあり）に変更された...という扱い（20bを20aとしてシリアライズする）
	subData3a m_data20b;//データ20b ※構造体（専用シリアライズあり）から構造体（専用シリアライズなし）に変更された...という扱い（20aを20bとしてシリアライズする）※デシリアライズできないので注意
	subData3a m_data21a;//データ21a
	subData3b m_data21b;//データ21b ※構造体（専用シリアライズなし）のメンバー数が縮小された...という扱い（21aを21bとしてシリアライズする）
	subData3c m_data21c;//データ21c ※構造体（専用シリアライズなし）のメンバー数が拡大された...という扱い（21aを21cとしてシリアライズする）
	subData m_data22a;//データ22a ※構造体（専用シリアライズあり）のメンバーと順序が変更された...という扱い（22bを22aとしてシリアライズする）
	subData2 m_data22b;//データ22b ※構造体（専用シリアライズあり）のメンバーと順序が変更された...という扱い（22aを22bとしてシリアライズする）
	subDataExt m_data23;//データ23 ※継承構造体（専用シリアライズあり）
	subDataExt2 m_data24;//データ24 ※多重継承構造体（専用シリアライズあり）
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "feature_test/test_data.inl"

#endif//GASHA_INCLUDED_TEST_DATA_H

// End of file
