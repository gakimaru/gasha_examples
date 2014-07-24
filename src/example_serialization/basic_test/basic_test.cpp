//--------------------------------------------------------------------------------
// basic_test.cpp
// シリアライズテスト：基本テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "basic_test/basic_test.h"//シリアライズテスト：基本テスト

#include <cstring>//std::memset()
#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//シリアライズ

//--------------------
//シリアライズ対象構造体
struct basicTestData
{
	int m_memberA;
	float m_memberB;
	int m_memberC[2];
	struct subData
	{
		int m_item1;
		int m_item2;
	} m_memberD;
};

//--------------------
//シリアライズ
//※シリアライズ（セーブ）とデシリアライズ（ロード）兼用の処理
//※【注意】gasha/serialization ネームスペースを指定しなければならない点に注意。

#include <gasha/serialization.h>//シリアライズ

GASHA_NAMESPACE_BEGIN;//ネームスペース：開始
namespace serialization
{
	//--------------------
	//シリアライズ／デシリアライズ共通処理：basicTestData 用
	//※ gasha::serializatoin::serialize<ARCHIVE, T> の特殊化クラスをユーザー定義する。
	//※基本的には、この処理だけを定義すればシリアライズ／デシリアライズ可能。
	template<class ARCHIVE>
	struct serialize<ARCHIVE, basicTestData> {
		void operator()(ARCHIVE& arc, const basicTestData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			//シリアライズ対象項目の指定
			//※ pair() 関数にデータ項目名とデータ項目を指定し、その結果を「&」演算子でアーカイブオブジェクトに受け渡す。
			//※細かくデータ項目を設定することで、セーブデータと実際のデータ構造の不整合があっても、ロード時に自動的に解消する。
			arc & pair("A", obj.m_memberA)
				& pair("B", obj.m_memberB)
				& pair("C", obj.m_memberC);
			arc & pairBin("D", obj.m_memberD);//「&」演算子をつなげて書いても分けて書いてもOK
			//※pairBin() を使用すれば、構造体をひとまとめのバイナリデータとして扱える
			//　（構造体ごとのシリアライズを定義することも可能）
		}
	};
}//namespace serialization
GASHA_NAMESPACE_END;//ネームスペース：終了

//--------------------------------------------------------------------------------
//セーブ／ロード

static const char* BINARY_SAVE_DATA_FILE_PATH = "data/example_serialization/basic_test_data.bin";//バイナリ形式セーブデータファイルパス
static const char* TEXT_SAVE_DATA_FILE_PATH = "data/example_serialization/basic_test_data.json";//テキスト（json）形式セーブデータファイルパス

#include "file.h"//ファイル操作

//--------------------
//バイナリ形式シリアライズとセーブ
//※バイナリ形式セーブデータイメージを作成してファイルに保存
#include <gasha/archive/output_binary_archive.h>//バイナリ形式アーカイブ（書き込み用）
static void saveBinary(const basicTestData& data)
{
	//準備
	char save_data_image[8 * 1024];//セーブデータイメージ用のバッファ
	char work_buff[8 * 1024];//ワークバッファ（シリアライズ処理に必要）

	//アーカイブオブジェクト生成
	archive::outputBinaryArchive arc(save_data_image, work_buff);

	//シリアライズ（セーブデータイメージ作成）
	arc << serialization::pair("Test", data);//アーカイブオブジェクトにセーブ対象データを受け渡す

	//ファイルに保存
	writeFile(BINARY_SAVE_DATA_FILE_PATH, arc.saveData(), arc.saveDataSize());

	//終了
	std::printf("Save file = \"%s\"\n", BINARY_SAVE_DATA_FILE_PATH);
	std::printf("Binary save data image size = %d\n", arc.saveDataSize());
}

//--------------------
//テキスト形式シリアライズとセーブ
//※テキスト形式セーブデータイメージを作成してファイルに保存
#include <gasha/archive/output_text_archive.h>//テキスト形式アーカイブ（書き込み用）
static void saveText(const basicTestData& data)
{
	//準備
	char save_data_image[8 * 1024];//セーブデータイメージ用のバッファ
	char work_buff[8 * 1024];//ワークバッファ（シリアライズ処理に必要）

	//アーカイブオブジェクト生成
	archive::outputTextArchive arc(save_data_image, work_buff);

	//シリアライズ（セーブデータイメージ作成）
	arc << serialization::pair("Test", data);//アーカイブオブジェクトにセーブ対象データを受け渡す

	//ファイルに保存
	writeFile(TEXT_SAVE_DATA_FILE_PATH, arc.saveData(), arc.saveDataSize());

	//終了
	std::printf("Save file = \"%s\"\n", TEXT_SAVE_DATA_FILE_PATH);
	std::printf("Text save data image size = %d\n", arc.saveDataSize());
}

//--------------------
//バイナリ形式データのロードとデシリアライズ
//※ファイルからバイナリ形式セーブデータイメージを読み込んでデシリアライズ
#include <gasha/archive/input_binary_archive.h>//バイナリ形式アーカイブ（読み込み用）
static void loadBinary(basicTestData& data)
{
	//準備
	char save_data_image[8 * 1024];//セーブデータイメージ用のバッファ
	char work_buff[8 * 1024];//ワークバッファ（デシリアライズ処理に必要）

	//ファイル読み込み
	const std::size_t save_data_image_size = readFile(BINARY_SAVE_DATA_FILE_PATH, save_data_image, sizeof(save_data_image));

	//アーカイブオブジェクト生成
	archive::inputBinaryArchive arc(save_data_image, save_data_image_size, work_buff);

	//デシリアライズ（セーブデータイメージから復元）
	arc >> serialization::pair("Test", data);//アーカイブオブジェクトからセーブ対象データに受け渡す

	//終了
	std::printf("Save file = \"%s\"\n", BINARY_SAVE_DATA_FILE_PATH);
	std::printf("Binary save data image size = %d\n", arc.saveDataSize());
}

#if 0//未実装
//--------------------
//テキスト形式データのロードとデシリアライズ
//※ファイルからテキスト形式セーブデータイメージを読み込んでデシリアライズ
#include <gasha/archive/input_text_archive.h>//テキスト形式アーカイブ（読み込み用）
static void loadText(basicTestData& data)
{
	//準備
	char save_data_image[8 * 1024];//セーブデータイメージ用のバッファ
	char work_buff[8 * 1024];//ワークバッファ（デシリアライズ処理に必要）

	//ファイル読み込み
	const std::size_t save_data_image_size = readFile(TEXT_SAVE_DATA_FILE_PATH, save_data_image, sizeof(save_data_image));

	//アーカイブオブジェクト生成
	archive::inputTextArchive arc(save_data_image, save_data_image_size, work_buff);

	//デシリアライズ（セーブデータイメージから復元）
	arc >> serialization::pair("Test", data);//アーカイブオブジェクトからセーブ対象データに受け渡す

	//終了
	std::printf("Save file = \"%s\"\n", TEXT_SAVE_DATA_FILE_PATH);
	std::printf("Text save data image size = %d\n", arc.saveDataSize());
}
#endif

//--------------------
//基本テスト
void basic_test()
{
	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("[ Test for serialization basic ]\n");
	std::printf("\n");

	//テストデータ作成
	std::printf("----------------------------------------\n");
	std::printf("[ Make data ]\n");
	basicTestData data;
	data.m_memberA = 123;
	data.m_memberB = 4.56f;
	data.m_memberC[0] = 78;
	data.m_memberC[1] = 90;
	data.m_memberD.m_item1 = 987;
	data.m_memberD.m_item2 = 654;

	//テストデータの内容表示
	std::printf("----------------------------------------\n");
	std::printf("[ Print data(1) : before save ]\n");
	auto print_data = [](basicTestData& data)
	{
		std::printf("basicTestData\n");
		std::printf("  .m_memberA = %d\n", data.m_memberA);
		std::printf("  .m_memberB = %.2f\n", data.m_memberB);
		std::printf("  .m_memberC = { %d, %d }\n", data.m_memberC[0], data.m_memberC[1]);
		std::printf("  .m_memberD\n");
		std::printf("      .m_item1 = %d\n", data.m_memberD.m_item1);
		std::printf("      .m_item2 = %d\n", data.m_memberD.m_item2);
	};
	print_data(data);
	
	//バイナリ形式セーブデータをセーブ
	std::printf("----------------------------------------\n");
	std::printf("[ Save binary ]\n");
	saveBinary(data);

	//テキスト形式セーブデータをセーブ
	std::printf("----------------------------------------\n");
	std::printf("[ Save text ]\n");
	saveText(data);

	//一旦データをクリア
	std::memset(&data, 0, sizeof(data));

	//テストデータの内容表示
	std::printf("----------------------------------------\n");
	std::printf("[ Print data(2) : before load ]\n");
	print_data(data);

	//バイナリ形式セーブデータをロード
	std::printf("----------------------------------------\n");
	std::printf("[ Load binary ]\n");
	loadBinary(data);

#if 0//未実装
	//テキスト形式セーブデータをロード
	std::printf("----------------------------------------\n");
	std::printf("[ Load text ]\n");
	loadText(data);
#endif
	
	//テストデータの内容表示
	std::printf("----------------------------------------\n");
	std::printf("[ Print data(3) : after load ]\n");
	print_data(data);

	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("finish.\n");
}

// End of file
