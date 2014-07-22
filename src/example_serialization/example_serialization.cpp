//--------------------------------------------------------------------------------
// exmaple_serialization.cpp
// シリアライズテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト

#include <gasha/serialization.h>//シリアライズ/シリアライズ

#include <gasha/archive/output_binary_archive.h>//アーカイブ/バイナリアーカイブ書き込み
#include <gasha/archive/input_binary_archive.h>//アーカイブ/バイナリアーカイブ読み込み
#include <gasha/archive/output_text_archive.h>//アーカイブ/テキストアーカイブ書き込み
//#include <gasha/archive/input_text_archive.h>//アーカイブ/テキストアーカイブ読み込み

#include <bitset>//std::bitset
#include <cstdio>//std::printf(), std::FILE, std::fopen(), std::fclose(), std::fseek(), std::fread(), std::fwrite()

//【VC++】fopen() を使用すると、error C4996 が発生する
//  error C4996: 'fopen': This function or variable may be unsafe. Consider using strncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable: 4996)//C4996を抑える

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//シリアライズテスト１

//--------------------
//テスト用クラス
class CTest1
{
	//シリアライズ用のフレンド設定
	FRIEND_SERIALIZE();
public:
	//型
	struct STRUCT
	{
		//FRIEND_SERIALIZE(STRUCT);
		int m_a;
		char m_b;
		float m_c;
		//コンストラクタ
		STRUCT() :
			m_a(0),
			m_b(0),
			m_c(0.f)
		{}
	};
public:
	//アクセッサ
	int getData1() const { return m_data1; }//データ1取得
	void setData1(const int value){ m_data1 = value; }//データ1更新
	float getData2() const { return m_data2; }//データ2取得
	void setData2(const float value){ m_data2 = value; }//データ2更新
	char getData3(const int index) const { return m_data3[index]; }//データ3取得
	void setData3(const int index, const int value){ m_data3[index] = value; }//データ3更新
	STRUCT& getData4a(){ return m_data4a; }//データ4a取得
	const STRUCT& getData4a() const { return m_data4a; }//データ4a取得
	STRUCT& getData4b(){ return m_data4b; }//データ4b取得
	const STRUCT& getData4b() const { return m_data4b; }//データ4b取得
	bool getData5a(const int index) const { return m_data5a[index]; }//データ5a取得
	void setData5a(const int index, const bool value){ m_data5a[index] = value; }//データ5a更新
	bool getData5b(const int index) const { return m_data5b[index]; }//データ5b取得
	void setData5b(const int index, const bool value){ m_data5b[index] = value; }//データ5b更新
	STRUCT& getData6(const int index){ return m_data6[index]; }//データ6取得
	const STRUCT& getData6(const int index) const { return m_data6[index]; }//データ6取得
	STRUCT* getData7a(){ return m_data7a; }//データ7a取得
	const STRUCT* getData7a() const { return m_data7a; }//データ7a取得
	STRUCT* getData7b(){ return m_data7b; }//データ7b取得
	const STRUCT* getData7b() const { return m_data7b; }//データ7b取得
	int* getData8a(){ return m_data8a; }//データ8a取得
	const int* getData8a() const { return m_data8a; }//データ8a取得
	int* getData8b(){ return m_data8b; }//データ8b取得
	const int* getData8b() const { return m_data8b; }//データ8b取得
public:
	//コンストラクタ
	CTest1() :
		m_data1(0),
		m_data2(0.f),
		m_data4a(),
		m_data4b(),
		m_data7a(nullptr),
		m_data7b(nullptr),
		m_data8a(nullptr),
		m_data8b(nullptr)
	{
		m_data3[0] = 0;
		m_data3[1] = 0;
		m_data3[2] = 0;
		m_data5a.reset();
		m_data5b.reset();
		m_data7a = new STRUCT();
		//m_data7b = new STRUCT();
		m_data8a = new int;
		//m_data8b = new int;
	}
	//デストラクタ
	~CTest1()
	{
		if (m_data7a)
			delete m_data7a;
		if (m_data7b)
			delete m_data7b;
		if (m_data8a)
			delete m_data8a;
		if (m_data8b)
			delete m_data8b;
	}
private:
	//フィールド
	//int m_data0;
	int m_data1;//データ1
	float m_data2;//データ2
	char m_data3[3];//データ3
	STRUCT m_data4a;//データ4a
	STRUCT m_data4b;//データ4b
	std::bitset<8192> m_data5a;//データ5a
	std::bitset<8192> m_data5b;//データ5b
	STRUCT m_data6[2];//データ6
	STRUCT* m_data7a;//データ7a
	STRUCT* m_data7b;//データ7b
	int* m_data8a;//データ8a
	int* m_data8b;//データ8b
};
//--------------------
//セーブデータ作成用クラス
//※空のクラス
//※ギャザーラとディストリビュータのみ定義する
class CSaveDataSerializer
{};

//--------------------
//テスト用クラスのバージョンを設定
SERIALIZATION_VERSION_DEF(CTest1, 1, 2);
SERIALIZATION_VERSION_DEF(CTest1::STRUCT, 3, 4);
SERIALIZATION_VERSION_DEF(CSaveDataSerializer, 5, 6);

//セーブデータインスタンス
static CTest1* s_saveData = nullptr;

//セーブデータ初期化
void initSaveData()
{
	//セーブデータ取得
	CTest1& data = *s_saveData;
	//データを初期化
	data.setData1(1);
	data.setData2(2.34f);
	data.setData3(0, 5);
	data.setData3(1, 6);
	data.setData3(2, 7);
	data.getData4a().m_a = 8;
	data.getData4a().m_b = 9;
	data.getData4a().m_c = 10.f;
	data.getData4b().m_a = 11;
	data.getData4b().m_b = 12;
	data.getData4b().m_c = 13.f;
	data.setData5a(0, true);
	data.setData5a(1, true);
	data.setData5a(8191, true);
	data.setData5b(2, true);
	data.setData5b(8190, true);
	data.getData6(0).m_a = 14;
	data.getData6(0).m_b = 15;
	data.getData6(0).m_c = 16.f;
	data.getData6(1).m_a = 17;
	data.getData6(1).m_b = 18;
	data.getData6(1).m_c = 19.f;
	if (data.getData7a())
	{
		data.getData7a()->m_a = 20;
		data.getData7a()->m_b = 21;
		data.getData7a()->m_c = 22.f;
	}
	if (data.getData7b())
	{
		data.getData7b()->m_a = 23;
		data.getData7b()->m_b = 24;
		data.getData7b()->m_c = 25.f;
	}
	if (data.getData8a())
		*data.getData8a() = 26;
	if (data.getData8b())
		*data.getData8b() = 27;
}

//セーブデータ内容表示
void printSaveData()
{
	//セーブデータ取得
	CTest1& data = *s_saveData;
	//データ表示
	std::printf("data1=%d\n", data.getData1());
	std::printf("data2=%.2f\n", data.getData2());
	std::printf("data3={%d, %d, %d}\n", data.getData3(0), data.getData3(1), data.getData3(2));
	std::printf("data4a:a=%d,b=%d,c=%.1f\n", data.getData4a().m_a, data.getData4a().m_b, data.getData4a().m_c);
	std::printf("data4b:a=%d,b=%d,c=%.1f\n", data.getData4b().m_a, data.getData4b().m_b, data.getData4b().m_c);
	std::printf("data5a:[0]=%d,[1]=%d,[2]=%d,[8190]=%d,[8191]=%d\n", data.getData5a(0), data.getData5a(1), data.getData5a(2), data.getData5a(8190), data.getData5a(8191));
	std::printf("data5b:[0]=%d,[1]=%d,[2]=%d,[8190]=%d,[8191]=%d\n", data.getData5b(0), data.getData5b(1), data.getData5b(2), data.getData5b(8190), data.getData5b(8191));
	std::printf("data6:[0]={a=%d,b=%d,c=%.1f},[1]={a=%d,b=%d,c=%.1f}\n", data.getData6(0).m_a, data.getData6(0).m_b, data.getData6(0).m_c, data.getData6(1).m_a, data.getData6(1).m_b, data.getData6(1).m_c);
	if (data.getData7a())
		std::printf("data7a(0x%p):a=%d,b=%d,c=%.1f\n", data.getData7a(), data.getData7a()->m_a, data.getData7a()->m_b, data.getData7a()->m_c);
	else
		std::printf("data7a(0x%p)\n", data.getData7a());
	if (data.getData7b())
		std::printf("data7b(0x%p):a=%d,b=%d,c=%.1f\n", data.getData7b(), data.getData7b()->m_a, data.getData7b()->m_b, data.getData7b()->m_c);
	else
		std::printf("data7b(0x%p)\n", data.getData7b());

	if (data.getData8a())
		std::printf("data8a(0x%p)=%d\n", data.getData8a(), *data.getData8a());
	else
		std::printf("data8a(0x%p)\n", data.getData8a());
	if (data.getData8b())
		std::printf("data8b(0x%p)=%d\n", data.getData8b(), *data.getData8b());
	else
		std::printf("data8b(0x%p)\n", data.getData8b());
}


//--------------------
//テスト用クラスのシリアライズ処理定義
GASHA_NAMESPACE_BEGIN;//ネームスペース：開始
namespace serialization
{
#if 1
	//--------------------
	//ロード前処理用関数オブジェクト
	//※デシリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct beforeLoad<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, CTest1& obj, const version& ver, const version& now_ver)
		{
			std::printf("beforeLoad<CTest1>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
			if (!obj.m_data7a)
				obj.m_data7a = new CTest1::STRUCT();
		}
	};
#endif
#if 1
	//--------------------
	//シリアライズ処理用関数オブジェクト
	//※シリアライズとデシリアライズ兼用共通処理
	template<class ARCHIVE_TYPE>
	struct serialize<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, const CTest1& obj, const version& ver, const version& now_ver)
		{
			std::printf("arc.is_read=%d, arc.is_write=%d\n", arc.isInputClass, arc.isOutputClass);
			std::printf("serialize<CTest1>(ver=%d,%d) sizeof(CTest1)=%d\n", ver.majorVer(), ver.minorVer(), sizeof(CTest1));
			//arc & pair("data0", obj.m_data0);
			arc & pair("data1", obj.m_data1)
				& pair("data2", obj.m_data2)
				& pair("data3", obj.m_data3);
			arc & pairBin("data4a", obj.m_data4a);
			arc & pair("data4b", obj.m_data4b);
			arc & pairBin("data5a", obj.m_data5a);
			arc & pair("data5b", obj.m_data5b);
			arc & pair("data6", obj.m_data6);
			arc & pair("data7a", obj.m_data7a);
			arc & pair("data7b", obj.m_data7b);
			arc & pair("data8a", obj.m_data8a);
			arc & pair("data8b", obj.m_data8b);
		}
	};
#endif
#if 1
	template<class ARCHIVE_TYPE>
	struct serialize<ARCHIVE_TYPE, CTest1::STRUCT> {
		void operator()(ARCHIVE_TYPE& arc, const CTest1::STRUCT& obj, const version& ver, const version& now_ver)
		{
			std::printf("serialize<CTest1::STRUCT>(ver=%d,%d) sizeof(CTest1::STRUCT)=%d\n", ver.majorVer(), ver.minorVer(), sizeof(CTest1::STRUCT));
			arc & pair("data1", obj.m_a);
			arc & pair("data2", obj.m_b);
			arc & pair("data3", obj.m_c);
		}
	};
#endif
#if 0
	//標準クラスライブラリに対応する関数も定義できる
	//std::bitsetを1ビットずつ別項目で書き出すサンプル
	//※膨大なワークバッファを要するので注意
	template<class ARCHIVE_TYPE, std::size_t N>
	struct serialize<ARCHIVE_TYPE, std::bitset<N> > {
		void operator()(ARCHIVE_TYPE& arc, const std::bitset<N>& obj, const version& ver, const version& now_ver)
		{
			std::printf("serialize<std::bitset<N>>(ver=%d,%d) sizeof(std::bitset<N>)=%d\n", ver.majorVer(), ver.minorVer(), sizeof(std::bitset<N>));
			for (int i = 0; i < N; ++i)
			{
				char name[16];
				GASHA_ spprintf(name, "no%d", i);
				bool bit = obj[i];
				arc & pair(name, bit);
			}
		}
	};
#endif
#if 1
	//--------------------
	//セーブ処理用関数オブジェクトクラス
	//※シリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct save<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, const CTest1& obj, const version& ver)
		{
			std::printf("save<CTest1>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
		}
	};
#endif
#if 1
	//--------------------
	//ロード処理用関数オブジェクトクラス
	//※デシリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct load<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, const CTest1& obj, const version& ver, const version& now_ver)
		{
			std::printf("load<CTest1>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
		}
	};
#endif
#if 1
	//--------------------
	//ロード後処理用関数オブジェクトクラス
	//※デシリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct afterLoad<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, CTest1& obj, const version& ver, const version& now_ver)
		{
			std::printf("afterLoad<CTest1>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
		}
	};
#endif
#if 1
	//--------------------
	//データ収集処理用関数オブジェクトクラス
	//※シリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct collector<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, const CTest1& obj, const version& ver)
		{
			std::printf("collector<CTest1>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
		}
	};
#endif
#if 1
	//--------------------
	//データ分配処理用関数オブジェクトクラス
	//※デシリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct distributor<ARCHIVE_TYPE, CTest1> {
		void operator()(ARCHIVE_TYPE& arc, CTest1& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			std::printf("distributor<CTest1>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
		}
	};
#endif
#if 1
	//--------------------
	//データ収集処理用関数オブジェクトクラス
	//※シリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct collector<ARCHIVE_TYPE, CSaveDataSerializer> {
		void operator()(ARCHIVE_TYPE& arc, const CSaveDataSerializer& obj, const version& ver)
		{
			std::printf("collector<CSaveDataSerializer>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
			//対象データ取得
			CTest1& data = *s_saveData;
			//シリアライズ
			arc << pair("CTest1", data);
		}
	};
#endif
#if 1
	//--------------------
	//データ分配処理用関数オブジェクトクラス
	//※デシリアライズ専用処理
	template<class ARCHIVE_TYPE>
	struct distributor<ARCHIVE_TYPE, CSaveDataSerializer> {
		void operator()(ARCHIVE_TYPE& arc, CSaveDataSerializer& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			std::printf("distributor<CSaveDataSerializer>(ver=%d,%d)\n", ver.majorVer(), ver.minorVer());
			//対象データ取得
			CTest1& data = *s_saveData;
			//デシリアライズ
			arc >> pair("CTest1", data);
		}
	};
#endif
}//namespace serialization
GASHA_NAMESPACE_END;//ネームスペース：終了

//--------------------
//シリアライズテスト１：バイナリアーカイブ
void serializeTest1bin(const char* file_path)
{
	std::printf("--------------------\n");
	std::printf("シリアライズ：バイナリアーカイブ\n");
	//バッファの用意
	//char* save_buff = new char[1 * 1024 * 1024];//セーブデータバッファ
	//char* work_buff = new char[8 * 1024 * 1024];//ワークバッファ
	char save_buff[16 * 1024];//セーブデータバッファ
	char work_buff[16 * 1024];//ワークバッファ
	//セーブデータ初期化
	initSaveData();
	//セーブデータ表示
	printSaveData();
	//シリアライズ
	archive::outputBinaryArchive arc(save_buff, work_buff);
	arc << serialization::pair<CSaveDataSerializer>("SaveData");//シリアライズ
	std::printf("処理結果：%s\n", arc.hasFatalError() ? "致命的なエラーあり" : "エラーなし");
	//ファイルに書き出し
	std::FILE* fp = std::fopen(file_path, "wb");
	std::fwrite(save_buff, 1, arc.saveDataSize(), fp);
	std::fclose(fp);
	fp = NULL;
}
//--------------------
//シリアライズテスト１：テキストアーカイブ
void serializeTest1txt(const char* file_path)
{
	std::printf("--------------------\n");
	std::printf("シリアライズ：テキストアーカイブ\n");
	//バッファの用意
	//char* save_buff = new char[1 * 1024 * 1024];//セーブデータバッファ
	//char* work_buff = new char[8 * 1024 * 1024];//ワークバッファ
	char save_buff[32 * 1024];//セーブデータバッファ
	char work_buff[16 * 1024];//ワークバッファ
	//セーブデータ初期化
	initSaveData();
	//セーブデータ表示
	printSaveData();
	//シリアライズ
	archive::outputTextArchive arc(save_buff, work_buff);
	arc << serialization::pair<CSaveDataSerializer>("SaveData");//シリアライズ
	std::printf("処理結果：%s\n", arc.hasFatalError() ? "致命的なエラーあり" : "エラーなし");
	//ファイルに書き出し
	std::FILE* fp = std::fopen(file_path, "wb");
	std::fwrite(save_buff, 1, arc.saveDataSize(), fp);
	std::fclose(fp);
	fp = NULL;
}
//--------------------
//デシリアライズテスト１：バイナリアーカイブ
void deserializeTest1bin(const char* file_path)
{
	std::printf("--------------------\n");
	std::printf("デシリアライズ：バイナリアーカイブ\n");
	//バッファの用意
	//char* save_buff = new char[1 * 1024 * 1024];//セーブデータバッファ
	//char* work_buff = new char[8 * 1024 * 1024];//ワークバッファ
	char save_buff[16 * 1024];//セーブデータバッファ
	char work_buff[16 * 1024];//ワークバッファ
	//ファイルから読み込み
	std::FILE* fp = fopen(file_path, "rb");
	//ファイルサイズを求める
	std::fseek(fp, 0, SEEK_END);
	const std::size_t file_size = ftell(fp);
	//ファイルを読み込み
	std::fseek(fp, 0, SEEK_SET);
	std::fread(save_buff, 1, file_size, fp);
	std::fclose(fp);
	fp = NULL;
	//デシリアライズ
	archive::inputBinaryArchive arc(save_buff, file_size, work_buff);
	arc >> serialization::pair<CSaveDataSerializer>("SaveData", file_size);
	std::printf("処理結果：%s\n", arc.hasFatalError() ? "致命的なエラーあり" : "エラーなし");
	archive::results& result = arc.result();
	std::printf("  getNumSmallerSizeItem()=%d\n", result.getNumSmallerSizeItem());//サイズが縮小されたデータ項目の数を取得
	std::printf("  getNumLargerSizeItem()=%d\n", result.getNumLargerSizeItem());//サイズが拡大されたデータ項目の数を取得
	std::printf("  getNumSmallerArrItem()=%d\n", result.getNumSmallerArrItem());//配列要素数が縮小されたデータ項目の数を取得
	std::printf("  getNumLargerArrItem()=%d\n", result.getNumLargerArrItem());//配列要素数が拡大されたデータ項目の数を取得
	std::printf("  getNumIsOnlyOnSaveData()=%d\n", result.getNumIsOnlyOnSaveData());//セーブデータ上にのみ存在するデータ項目の数を取得
	std::printf("  getNumIsOnlyOnMem()=%d\n", result.getNumIsOnlyOnMem());//セーブデータ上にないデータ項目の数を取得
	std::printf("  getNumIsObjOnSaveDataOnly()=%d\n", result.getNumIsObjOnSaveDataOnly());//現在オブジェクト型ではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  getNumIsObjOnMemOnly()=%d\n", result.getNumIsObjOnMemOnly());//現在オブジェクト型だが、セーブデータ上ではそうではなかったデータ項目の数を取得
	std::printf("  getNumIsArrOnSaveDataOnly()=%d\n", result.getNumIsArrOnSaveDataOnly());//現在配列型ではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  getNumIsArrOnMemOnly()=%d\n", result.getNumIsArrOnMemOnly());//現在配列型だが、セーブデータ上ではそうではなかったデータ項目の数を取得
	std::printf("  getNumIsPtrOnSaveDataOnly()=%d\n", result.getNumIsPtrOnSaveDataOnly());//現在ポインタ型ではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  getNumIsPtrOnMemOnly()=%d\n", result.getNumIsPtrOnMemOnly());//現在ポインタ型だが、セーブデータ上ではそうではなかったデータ項目の数を取得
	std::printf("  getNumIsNulOnSaveDataOnly()=%d\n", result.getNumIsNulOnSaveDataOnly());//現在ヌルではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  getNumIsNulOnMemOnly()=%d\n", result.getNumIsNulOnMemOnly());//現在ヌルだが、セーブデータ上ではそうではなかったデータ項目の数を取得
	//セーブデータ表示
	printSaveData();
}
//--------------------
//シリアライズ＆デシリアライズテスト１：バイナリアーカイブ
void test1()
{
	static const char* file_path_bin = "test1.bin";
	static const char* file_path_txt = "test1.txt";
	//シリアライズ
	s_saveData = new CTest1();
	serializeTest1bin(file_path_bin);
	serializeTest1txt(file_path_txt);
	delete s_saveData;
	s_saveData = nullptr;

	char* dummy = new char[10 * 1024];

	//デシリアライズ
	s_saveData = new CTest1();
	deserializeTest1bin(file_path_bin);
	delete s_saveData;
	s_saveData = nullptr;

	delete dummy;
}

//----------------------------------------
//シリアライズテスト
void example_serialization()
{
	test1();
}

// End of file
