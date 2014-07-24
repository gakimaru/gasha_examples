//--------------------------------------------------------------------------------
// feature_test.cpp
// シリアライズテスト：機能テスト（メイン）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "feature_test/feature_test.h"//シリアライズテスト：機能テスト

#include "feature_test/save_load.h"//セーブ／ロード処理

#include "work_buff.h"//ワークバッファ
#include "file.h"//ファイル操作

//--------------------
//ファイルパス
static const char* BINARY_SAVE_DATA_FILE_PATH = "data/example_serialization/feature_test_data.bin";//バイナリ形式セーブデータファイルパス
static const char* TEXT_SAVE_DATA_FILE_PATH = "data/example_serialization/feature_test_data.json";//テキスト（json）形式セーブデータファイルパス

//--------------------
//グローバルテストデータ
static testData* s_testData = nullptr;

//--------------------
//テストデータ取得
testData* getTestData()
{
	return s_testData;
}

//--------------------
//テストデータインスタンス生成
testData* createTestData()
{
	if (s_testData)
		return s_testData;
	s_testData = new testData();
	return s_testData;
}

//--------------------
//テストデータインスタンス破棄
void destroyTestData()
{
	if (!s_testData)
		return;
	delete s_testData;
	s_testData = nullptr;
}

//--------------------
//テストデータ作成
static void makeTestData()
{
	//テストデータ取得
	testData& data = *getTestData();
	
	//データを作成
	data.setData1(1);
	data.setData2(2.34f);
	data.setData3(0, 5);
	data.setData3(1, 6);
	data.setData3(2, 7);
	data.data4a().m_a = 8;
	data.data4a().m_b = 9;
	data.data4a().m_c = 10.f;
	data.data4b().m_a = 11;
	data.data4b().m_b = 12;
	data.data4b().m_c = 13.f;
	data.setData5a(0, true);
	data.setData5a(1, true);
	data.setData5a(BITSET_DATA1_SIZE - 2, true);
	data.setData5a(BITSET_DATA1_SIZE - 1, true);
	data.setData5b(2, true);
	data.setData5b(3, true);
	data.setData5b(BITSET_DATA2_SIZE - 3, true);
	data.setData5b(BITSET_DATA2_SIZE - 1, true);
	data.data6(0).m_a = 14;
	data.data6(0).m_b = 15;
	data.data6(0).m_c = 16.f;
	data.data6(1).m_a = 17;
	data.data6(1).m_b = 18;
	data.data6(1).m_c = 19.f;
	if (data.data7a())//構造体のポインタテスト（実体に実体をロードするテスト）
	{
		data.data7a()->m_a = 20;
		data.data7a()->m_b = 21;
		data.data7a()->m_c = 22.f;
	}
	if (!data.data7b())//構造体のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
	{
		data.data7b() = new testData::subData();
	}
	if (data.data7b())
	{
		data.data7b()->m_a = 23;
		data.data7b()->m_b = 24;
		data.data7b()->m_c = 25.f;
	}
	if (data.data7c())//構造体のポインタテスト（nullptr に nullptrをロードするテスト）
	{
		data.data7c()->m_a = 26;
		data.data7c()->m_b = 27;
		data.data7c()->m_c = 28.f;
	}
	if (data.data7d())//構造体のポインタテスト（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
	{
		data.data7d()->m_a = 29;
		data.data7d()->m_b = 30;
		data.data7d()->m_c = 31.f;
	}
	if (!data.data7e())//構造体のポインタテスト（nullptr に 実体をロードするテスト）
	{
		data.data7e() = new testData::subData();
	}
	if (data.data7e())
	{
		data.data7e()->m_a = 32;
		data.data7e()->m_b = 33;
		data.data7e()->m_c = 34.f;
	}
	data.data7f().m_a = 35;
	data.data7f().m_b = 36;
	data.data7f().m_c = 37.f;
	if (data.data7g())
	{
		data.data7g()->m_a = 38;
		data.data7g()->m_b = 39;
		data.data7g()->m_c = 40.f;
	}
	if (data.data7h())
	{
		data.data7h()[0].m_a = 41;
		data.data7h()[0].m_b = 42;
		data.data7h()[0].m_c = 43.f;
		data.data7h()[1].m_a = 44;
		data.data7h()[1].m_b = 45;
		data.data7h()[1].m_c = 46.f;
	}
	data.data7i(0).m_a = 47;
	data.data7i(0).m_b = 48;
	data.data7i(0).m_c = 49.f;
	data.data7i(1).m_a = 50;
	data.data7i(1).m_b = 51;
	data.data7i(1).m_c = 52.f;
	if (data.data7j())
	{
		data.data7j()[0].m_a = 53;
		data.data7j()[0].m_b = 54;
		data.data7j()[0].m_c = 55.f;
		data.data7j()[1].m_a = 56;
		data.data7j()[1].m_b = 57;
		data.data7j()[1].m_c = 58.f;
	}
	if (data.data8a())
		*data.data8a() = 59;
	if (!data.data8b())//構造体のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
	{
		data.data8b() = new int(0);
	}
	if (data.data8b())
		*data.data8b() = 60;
	if (data.data8c())//構造体のポインタテスト（nullptr に nullptrをロードするテスト）
		*data.data8c() = 61;
	if (data.data8d())//構造体のポインタテスト（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
		*data.data8d() = 62;
	if (!data.data8e())//構造体のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
	{
		data.data8e() = new int(0);
	}
	if (data.data8e())
		*data.data8e() = 63;
	data.setData8f(64);
	if (data.data8g())
		*data.data8g() = 65;
	if (data.data8h())
	{
		data.data8h()[0] = 66;
		data.data8h()[1] = 67;
	}
	data.setData8i(0, 68);
	data.setData8i(1, 69);
	if (data.data8j())
	{
		data.data8j()[0] = 70;
		data.data8j()[1] = 71;
	}
	data.setData9(72);
	data.setData10(73);
	data.setData11(74);
	data.setData11_deleted(75);
	data.setData12(76);
	data.data13().m_a = 77;
	data.data13().m_b = 78;
	data.data13().m_c = 79.f;
	data.data13_deleted().m_a = 80;
	data.data13_deleted().m_b = 81;
	data.data13_deleted().m_c = 82.f;
	data.data14().m_a = 83;
	data.data14().m_b = 84;
	data.data14().m_c = 85.f;
	data.setData15a(86);
	data.setData15b(87);
	data.setData15c(88);
	data.setData15d(89.);
	data.setData16a(0, 90);
	data.setData16a(1, 91);
	data.setData16a(2, 92);
	data.setData16b(0, 93);
	data.setData16b(1, 94);
	data.setData16c(0, 95);
	data.setData16c(1, 96);
	data.setData16c(2, 97);
	data.setData16c(3, 98);
	data.setData16d(99);
	data.setData16e(0, 100);
	data.setData16e(1, 101);
	data.data17a(0).m_a = 102;
	data.data17a(0).m_b = 103;
	data.data17a(0).m_c = 104.f;
	data.data17a(1).m_a = 105;
	data.data17a(1).m_b = 106;
	data.data17a(1).m_c = 107.f;
	data.data17a(2).m_a = 108;
	data.data17a(2).m_b = 109;
	data.data17a(2).m_c = 110.f;
	data.data17b(0).m_a = 111;
	data.data17b(0).m_b = 112;
	data.data17b(0).m_c = 113.f;
	data.data17b(1).m_a = 114;
	data.data17b(1).m_b = 115;
	data.data17b(1).m_c = 116.f;
	data.data17c(0).m_a = 117;
	data.data17c(0).m_b = 118;
	data.data17c(0).m_c = 119.f;
	data.data17c(1).m_a = 120;
	data.data17c(1).m_b = 121;
	data.data17c(1).m_c = 122.f;
	data.data17c(2).m_a = 123;
	data.data17c(2).m_b = 124;
	data.data17c(2).m_c = 125.f;
	data.data17c(3).m_a = 126;
	data.data17c(3).m_b = 127;
	data.data17c(3).m_c = 128.f;
	data.data17d().m_a = 129;
	data.data17d().m_b = 130;
	data.data17d().m_c = 131.f;
	data.data17e(0).m_a = 132;
	data.data17e(0).m_b = 133;
	data.data17e(0).m_c = 134.f;
	data.data17e(1).m_a = 135;
	data.data17e(1).m_b = 136;
	data.data17e(1).m_c = 137.f;
	data.setData18a(138);
	data.data18b().m_a = 139;
	data.data18b().m_b = 140;
	data.data18b().m_c = 141.f;
	data.setData19a(142);
	data.data19b().m_a = 143;
	data.data19b().m_b = 144;
	data.data19b().m_c = 145.f;
	data.data20a().m_a = 146;
	data.data20a().m_b = 147;
	data.data20a().m_c = 148.f;
	data.data20b().m_a = 149;
	data.data20b().m_b = 150;
	data.data20b().m_c = 151.f;
	data.data21a().m_a = 152;
	data.data21a().m_b = 153;
	data.data21a().m_c = 154.f;
	data.data21b().m_a = 155;
	data.data21b().m_b = 156;
	data.data21c().m_a = 157;
	data.data21c().m_b = 158;
	data.data21c().m_c = 159.f;
	data.data21c().m_d = 160;
	data.data22a().m_a = 161;
	data.data22a().m_b = 162;
	data.data22a().m_c = 163.f;
	data.data22b().m_a = 164;
	data.data22b().m_b = 165;
	data.data22b().m_c = 166.f;
	data.data22b().m_d = 167;
	data.data23().m_a = 168;
	data.data23().m_b = 169;
	data.data23().m_c = 170.f;
	data.data23().m_z = 171;
	static_cast<testData::subData&>(data.data24()).m_a = 172;
	static_cast<testData::subData&>(data.data24()).m_b = 173;
	static_cast<testData::subData&>(data.data24()).m_c = 174.f;
	static_cast<testData::subData2&>(data.data24()).m_a = 175;
	static_cast<testData::subData2&>(data.data24()).m_b = 176;
	static_cast<testData::subData2&>(data.data24()).m_c = 177.;
	data.data24().m_d = 178;
	data.data24().m_z = 179;
}

//--------------------
//テストデータの内容表示
static void printTestData()
{
	//テストデータ取得
	testData& data = *getTestData();

	//データ表示
	auto print_int = [](const char* name, const int var)
	{
		std::printf("%s=%d\n", name, var);
	};
	auto print_int_p = [](const char* name, const int* var)
	{
		if (var)
			std::printf("%s(%p)=%d\n", name, var, *var);
		else
			std::printf("%s(%p)\n", name, var);
	};
	auto print_int_array = [](const char* name, const int* var, const std::size_t num)
	{
		std::printf("%s={", name);
		for (std::size_t i = 0; i < num; ++i)
			std::printf("[%d]=%d,", i, var[i]);
		std::printf("}\n");
	};
	auto print_short = [](const char* name, short var)
	{
		std::printf("%s=%d\n", name, var);
	};
	auto print_short_array = [](const char* name, const short* var, const std::size_t num)
	{
		std::printf("%s={", name);
		for (std::size_t i = 0; i < num; ++i)
			std::printf("[%d]=%d,", i, var[i]);
		std::printf("}\n");
	};
	auto print_longlong = [](const char* name, const long long var)
	{
		std::printf("%s=%lld\n", name, var);
	};
	auto print_float = [](const char* name, const float var)
	{
		std::printf("%s=%.2f\n", name, var);
	};
	auto print_double = [](const char* name, const double var)
	{
		std::printf("%s=%.2lf\n", name, var);
	};
	auto print_subdata = [](const char* name, const testData::subData& var)
	{
		std::printf("%s:{a=%d,b=%d,c=%.1f}\n", name, var.m_a, var.m_b, var.m_c);
	};
	auto print_subdata2 = [](const char* name, const testData::subData2& var)
	{
		std::printf("%s:{a=%d,b=%d,c=%.1lf,d=%d}\n", name, var.m_a, var.m_b, var.m_c, var.m_d);
	};
	auto print_subdata3a = [](const char* name, const testData::subData3a& var)
	{
		std::printf("%s:{a=%d,b=%d,c=%.1f}\n", name, var.m_a, var.m_b, var.m_c);
	};
	auto print_subdata3b = [](const char* name, const testData::subData3b& var)
	{
		std::printf("%s:{a=%d,b=%d}\n", name, var.m_a, var.m_b);
	};
	auto print_subdata3c = [](const char* name, const testData::subData3c& var)
	{
		std::printf("%s:{a=%d,b=%d,c=%.1f,d=%lld}\n", name, var.m_a, var.m_b, var.m_c, var.m_d);
	};
	auto print_subdata_ext = [](const char* name, const testData::subDataExt& var)
	{
		std::printf("%s:{{a=%d,b=%d,c=%.1f},z=%lld}\n", name, var.m_a, var.m_b, var.m_c, var.m_z);
	};
	auto print_subdata_ext2 = [](const char* name, const testData::subDataExt2& var)
	{
		std::printf("%s:{{a=%d,b=%d,c=%.1f},{a=%d,b=%d,c=%.1lf,d=%d},z=%lld}\n", name, static_cast<const testData::subData&>(var).m_a, static_cast<const testData::subData&>(var).m_b, static_cast<const testData::subData&>(var).m_c, static_cast<const testData::subData2&>(var).m_a, static_cast<const testData::subData2&>(var).m_b, static_cast<const testData::subData2&>(var).m_c, static_cast<const testData::subData2&>(var).m_d, var.m_z);
	};
	auto print_subdata_p = [](const char* name, const testData::subData* var)
	{
		if (var)
			std::printf("%s(%p):{a=%d,b=%d,c=%.1f}\n", name, var, var->m_a, var->m_b, var->m_c);
		else
			std::printf("%s(%p)\n", name, var);
	};
	auto print_subdata_array = [](const char* name, const testData::subData* var, const std::size_t num)
	{
		std::printf("%s={", name);
		for (std::size_t i = 0; i < num; ++i)
			std::printf("[%d]={a=%d,b=%d,c=%.1f},", i, var[i].m_a, var[i].m_b, var[i].m_c);
		std::printf("}\n");
	};
	auto print_bitset1 = [](const char* name, const std::bitset<BITSET_DATA1_SIZE>& var)
	{
		std::printf("%s:{[0]=%d,[1]=%d,[2]=%d,[3]=%d,...,[%d]=%d,[%d]=%d,[%d]=%d,}\n", name, var[0], var[1], var[2], var[3], BITSET_DATA1_SIZE - 3, var[BITSET_DATA1_SIZE - 3], BITSET_DATA1_SIZE - 2, var[BITSET_DATA1_SIZE - 2], BITSET_DATA1_SIZE - 1, var[BITSET_DATA1_SIZE - 1]);
	};
	auto print_bitset2 = [](const char* name, const std::bitset<BITSET_DATA2_SIZE>& var)
	{
		std::printf("%s:{[0]=%d,[1]=%d,[2]=%d,[3]=%d,...,[%d]=%d,[%d]=%d,[%d]=%d,}\n", name, var[0], var[1], var[2], var[3], BITSET_DATA2_SIZE - 3, var[BITSET_DATA2_SIZE - 3], BITSET_DATA2_SIZE - 2, var[BITSET_DATA2_SIZE - 2], BITSET_DATA2_SIZE - 1, var[BITSET_DATA2_SIZE - 1]);
	};

	print_int("data1", data.data1());
	print_float("data2", data.data2());
	print_short_array("data3", data.data3(), data.data3_num());
	print_subdata("data4a", data.data4a());
	print_subdata("data4b", data.data4b());
	print_bitset1("data5a", data.data5a());
	print_bitset2("data5b", data.data5b());
	print_subdata_array("data6", data.data6(), data.data6_num());
	print_subdata_p("data7a", data.data7a());
	print_subdata_p("data7b", data.data7b());
	print_subdata_p("data7c", data.data7c());
	print_subdata_p("data7d", data.data7d());
	print_subdata_p("data7e", data.data7e());
	print_subdata("data7f", data.data7f());
	print_subdata_p("data7g", data.data7g());
	print_subdata_array("data7h", data.data7h(), 2);
	print_subdata_array("data7i", data.data7i(), data.data7i_num());
	print_subdata_array("data7j", data.data7j(), 2);
	print_int_p("data8a", data.data8a());
	print_int_p("data8b", data.data8b());
	print_int_p("data8c", data.data8c());
	print_int_p("data8d", data.data8d());
	print_int_p("data8e", data.data8e());
	print_int("data8f", data.data8f());
	print_int_p("data8g", data.data8g());
	print_int_array("data8h", data.data8h(), 2);
	print_int_array("data8i", data.data8i(), data.data8i_num());
	print_int_array("data8j", data.data8j(), 2);
	print_int("data9", data.data9());
	print_int("data10", data.data10());
	print_int("data11", data.data11());
	print_int("data11_deleted", data.data11_deleted());
	print_int("data12", data.data12());
	print_subdata("data13", data.data13());
	print_subdata("data13_deleted", data.data13_deleted());
	print_subdata("data14", data.data14());
	print_int("data15a", data.data15a());
	print_short("data15b", data.data15b());
	print_longlong("data15c", data.data15c());
	print_double("data15d", data.data15d());
	print_int_array("data16a", data.data16a(), data.data16a_num());
	print_int_array("data16b", data.data16b(), data.data16b_num());
	print_int_array("data16c", data.data16c(), data.data16c_num());
	print_int("data16d", data.data16d());
	print_int_array("data16e", data.data16e(), data.data16e_num());
	print_subdata_array("data17a", data.data17a(), data.data17a_num());
	print_subdata_array("data17b", data.data17b(), data.data17b_num());
	print_subdata_array("data17c", data.data17c(), data.data17c_num());
	print_subdata("data17d", data.data17d());
	print_subdata_array("data17e", data.data17e(), data.data17e_num());
	print_int("data18a", data.data18a());
	print_subdata("data18b", data.data18b());
	print_int("data19a", data.data19a());
	print_subdata3a("data19b", data.data19b());
	print_subdata("data20a", data.data20a());
	print_subdata3a("data20b", data.data20b());
	print_subdata3a("data21a", data.data21a());
	print_subdata3b("data21b", data.data21b());
	print_subdata3c("data21c", data.data21c());
	print_subdata("data22a", data.data22a());
	print_subdata2("data22b", data.data22b());
	print_subdata_ext("data23", data.data23());
	print_subdata_ext2("data24", data.data24());
}

//--------------------
//バイナリ形式セーブデータをセーブ
static void saveBinary()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);
#ifdef ENABLE_TO_OCCUR_FATAL_ERROR
	work_buff_size = 256;//ワークバッファ不足による致命的エラー発生
#endif//ENABLE_TO_OCCUR_FATAL_ERROR

	//セーブデータ作成（シリアライズ）
	const std::size_t save_data_size = makeBinarySaveDataImage(save_data_buff, save_data_buff_size, work_buff, work_buff_size);
	if (save_data_size == 0)//失敗判定
		return;

	//ファイル保存
	writeFile(BINARY_SAVE_DATA_FILE_PATH, save_data_buff, save_data_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}

//--------------------
//テキスト形式セーブデータをセーブ
static void saveText()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);
#ifdef ENABLE_TO_OCCUR_FATAL_ERROR
	work_buff_size = 256;//ワークバッファ不足による致命的エラー発生
#endif//ENABLE_TO_OCCUR_FATAL_ERROR

	//セーブデータ作成（シリアライズ）
	const std::size_t save_data_size = makeTextSaveDataImage(save_data_buff, save_data_buff_size, work_buff, work_buff_size);
	if (save_data_size == 0)//失敗判定
		return;

	//ファイル保存
	writeFile(TEXT_SAVE_DATA_FILE_PATH, save_data_buff, save_data_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}

//--------------------
//バイナリ形式セーブデータをロード
static void loadBinary()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);
#ifdef ENABLE_TO_OCCUR_FATAL_ERROR
	work_buff_size = 256;//ワークバッファ不足による致命的エラー発生
#endif//ENABLE_TO_OCCUR_FATAL_ERROR

	//ファイル読み込み
	const std::size_t save_data_size = readFile(BINARY_SAVE_DATA_FILE_PATH, save_data_buff, save_data_buff_size);

	//セーブデータ読み込み（デシリアライズ）
	loadBinarySaveDataImage(save_data_buff, save_data_size, work_buff, work_buff_size);
	
	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}

#if 0//未実装
//--------------------
//テキスト形式セーブデータをロード
static void loadText()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);
#ifdef ENABLE_TO_OCCUR_FATAL_ERROR
	work_buff_size = 256;//ワークバッファ不足による致命的エラー発生
#endif//ENABLE_TO_OCCUR_FATAL_ERROR

	//ファイル読み込み
	const std::size_t save_data_size = readFile(TEXT_SAVE_DATA_FILE_PATH, save_data_buff, save_data_buff_size);

	//セーブデータ読み込み（デシリアライズ）
	loadTextSaveDataImage(save_data_buff, save_data_size, work_buff, work_buff_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}
#endif

//--------------------
//機能テスト
void feature_test()
{
	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("[ Test for serialization features ]\n");
	std::printf("\n");

	//テストデータインスタンス生成
	std::printf("----------------------------------------\n");
	std::printf("[ Create instance ]\n");
	createTestData();
	
	//テストデータ作成
	std::printf("----------------------------------------\n");
	std::printf("[ Make data ]\n");
	makeTestData();
	
	//テストデータの内容表示
	std::printf("----------------------------------------\n");
	std::printf("[ Print data(1) : before save ]\n");
	printTestData();
	
	//バイナリ形式セーブデータをセーブ
	std::printf("----------------------------------------\n");
	std::printf("[ Save binary ]\n");
	saveBinary();

	//テキスト形式セーブデータをセーブ
	std::printf("----------------------------------------\n");
	std::printf("[ Save text ]\n");
	saveText();

	//バイナリ形式セーブデータをロード
	std::printf("----------------------------------------\n");
	std::printf("[ Load binary ]\n");
	loadBinary();

#if 0//未実装
	//テキスト形式セーブデータをロード
	std::printf("----------------------------------------\n");
	std::printf("[ Load text ]\n");
	loadText();
#endif
	
	//テストデータの内容表示
	std::printf("----------------------------------------\n");
	std::printf("[ Print data(2) : after load ]\n");
	printTestData();
	
	//テストデータインスタンス破棄
	std::printf("----------------------------------------\n");
	std::printf("[ Destroy instance ]\n");
	destroyTestData();

	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("finish.\n");
}

// End of file
