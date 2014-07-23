#pragma once
#ifndef GASHA_INCLUDED_ASVE_LOAD_DEF_CPP_H
#define GASHA_INCLUDED_ASVE_LOAD_DEF_CPP_H

//--------------------------------------------------------------------------------
// save_load_def.cpp.h
// シリアライズテスト：機能テスト：セーブ／ロード用処理定義
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "feature_test/feature_test.h"//シリアライズテスト：機能テスト
#include "feature_test/test_data.h"//テストデータ定義

#include <gasha/serialization.h>//シリアライズ
                                //※シリアライズ処理を定義する際にインクルードする

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//セーブデータ作成用クラス
//※空のクラス
class saveData{};

//--------------------
//シリアライズ対象データ構造にバージョンを設定
//※必須ではない（指定しない場合はバージョン 0.0 として扱われる）
//※【注意】マクロ内でネームスペースを指定していることに注意。
//　　　　　他のネームスペースの範囲内では使用不可。
GASHA_SERIALIZATION_VERSION_DEF(testData, 1, 2);
GASHA_SERIALIZATION_VERSION_DEF(testData::subData, 3, 4);
GASHA_SERIALIZATION_VERSION_DEF(saveData, 5, 6);

//--------------------
//シリアライズ処理定義
//※【注意】gasha/serialization ネームスペースを指定しなければならない点に注意。
//　　　　　他のネームスペースの範囲内では定義不可。
GASHA_NAMESPACE_BEGIN;//ネームスペース：開始
namespace serialization
{
	//--------------------
	//シリアライズ／デシリアライズ共通処理：testData 用
	//※ gasha::serializatoin::serialize<ARCHIVE_TYPE, T> クラスの T を任意の型で特殊化したクラスをユーザー定義する。
	//※基本的には、この処理だけを定義すればシリアライズ／デシリアライズ可能。
	//※必要に応じて定義する。（別途シリアライズ専用処理とデシリアライズ専用処理を用いる方法もあるため、この定義が必須ということはない）
	//※【注意】シリアライズ時とデシリアライズ時では処理の呼び出され方が異なる。詳しは、後述の struct load の所に記述。
	template<class ARCHIVE_TYPE>
	struct serialize<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, const testData& obj, const version& ver, const version& now_ver, const itemInfoBase* target_item)
		{
			if (!target_item)
			{
				std::printf("----------\n");
				std::printf("serialize<testData>(ver=%d.%d,now_ver=%d.%d) sizeof(testData)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			
			//シリアライズ対象項目の指定
			//※ pair() 関数にデータ項目名とデータ項目を指定し、その結果を「&」演算子でアーカイブオブジェクトに受け渡す。
			//※細かくデータ項目を設定することで、セーブデータと実際のデータ構造の不整合があっても、ロード時に自動的に解消する。
			//　（データ項目の追加／削除／順序変更に対応可能。）
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

	//--------------------
	//シリアライズ／デシリアライズ共通処理：testData::subData 用
	//※構造体（クラス）ごとにシリライズ／デシリアライズ処理を定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE>
	struct serialize<ARCHIVE_TYPE, testData::subData> {
		void operator()(ARCHIVE_TYPE& arc, const testData::subData& obj, const version& ver, const version& now_ver, const itemInfoBase* target_item)
		{
			if (!target_item)
			{
				std::printf("----------\n");
				std::printf("serialize<testData::subData>(ver=%d.%d,now_ver=%d.%d) sizeof(testData::subData)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData::subData));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			
			//シリアライズ対象項目の指定
			arc & pair("data1", obj.m_a);
			arc & pair("data2", obj.m_b);
			arc & pair("data3", obj.m_c);
		}
	};

#ifdef ENABLE_TO_SERIALIZE_BITSET_ON_FEATURE_TEST
	//--------------------
	//シリアライズ／デシリアライズ共通処理：std::bitset 用
	//※標準ライブラリのクラスなど、ユーザー定義クラス（構造体）以外であっても、シリアライズ／デシリアライズ処理を定義可能。
	//※シリアライズ／デシリアライズが未定義のクラス（構造体）に対しては、バイナリデータとして処理される。
	//※このサンプルは、std::bitsetを1ビットずつ別項目で書き出す。膨大なワークバッファを要するので注意。また、処理効率も非常に悪い。あくまでもサンプル。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE, std::size_t N>
	struct serialize<ARCHIVE_TYPE, std::bitset<N>> {
		void operator()(ARCHIVE_TYPE& arc, const std::bitset<N>& obj, const version& ver, const version& now_ver, const itemInfoBase* target_item)
		{
			if (!target_item)
			{
				std::printf("----------\n");
				std::printf("serialize<std::bitset<N>>(ver=%d.%d,now_ver=%d.%d) sizeof(std::bitset<N>)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(std::bitset<N>));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			
			//シリアライズ対象項目の指定
			for (int i = 0; i < N; ++i)
			{
				char name[16];
				GASHA_ spprintf(name, "no%d", i);
				bool bit = obj[i];
				arc & pair(name, bit);
				if (target_item && *target_item == name)
				{
					//※テストのための強引な処理
					std::bitset<N>* nonconst_obj = const_cast<std::bitset<N>*>(&obj);
					(*nonconst_obj)[i] = bit;
				}
			}
		}
	};
#endif//ENABLE_TO_SERIALIZE_BITSET_ON_FEATURE_TEST

	//--------------------
	//シリアライズ専用処理：testData 用
	//※シリアライズ（セーブ）専用の処理。必要に応じて定義。
	//※例えば、シリアライズとデシリアライズの処理を共通化できない項目に対して、専用のシリアライズ処理とデシリアライズ処理をそれぞれ定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE>
	struct save<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, const testData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("save<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

			//シリアライズ対象項目の指定
			int eccrypted_data10 = obj.m_data10 ^ 0xa5a5a5a5;//暗号化データをこの場で作成
			arc & pair("data10", eccrypted_data10);//※暗号化データ
			arc & pair("data11", obj.m_data11);//※削除されたデータ項目...という扱い（セーブデータにだけ存在し、実際のデータ構造からは削除されたデータ項目のテスト用）
			arc & pair("data13", obj.m_data13);//※削除されたデータ項目（オブジェクト）...という扱い
			arc & pair("data15a", obj.m_data15a);
			arc & pair("data15b", obj.m_data15a);//※データ型が縮小された...という扱い（15aを15bとしてシリアライズする）
			arc & pair("data15c", obj.m_data15a);//※データ型が拡大された...という扱い（15aを15cとしてシリアライズする）
			arc & pair("data15d", obj.m_data15a);//※データ型が変更された...という扱い（15aを15dとしてシリアライズする）
			arc & pair("data16a", obj.m_data16a);
			arc & pair("data16b", obj.m_data16a);//※配列サイズが縮小された...という扱い（16aを16bとしてシリアライズする）
			arc & pair("data16c", obj.m_data16a);//※配列サイズが拡大された...という扱い（16aを16bとしてシリアライズする）
			arc & pair("data16d", obj.m_data16a);//※配列から非配列に変更された...という扱い（16aを16dとしてシリアライズする）
			arc & pair("data16e", obj.m_data16d);//※非配列から配列に変更された...という扱い（16dを16eとしてシリアライズする）
		}
	};

	//--------------------
	//デシリアライズ専用処理：testData 用
	//※デシリアライズ（ロード）専用の処理。必要に応じて定義。
	//※例えば、シリアライズとデシリアライズの処理を共通化できない項目に対して、専用のシリアライズ処理とデシリアライズ処理をそれぞれ定義する。
	//※必要に応じて定義する。
	//※【注意】シリアライズ時はデータ項目一つ一つを順番に処理していくが、デシリアライズ時は関数を抜けた後に処理するので注意。
	//　　　　　これは、セーブデータ上のデータ項目が、プログラムの定義通りであるとは限らないため。
	//　　　　　一旦関数を実行してデータ項目の情報を一通り収集した後、セーブデータ上のデータ項目順に処理する。
	//　　　　　なお、データ項目を読み込む際に、再度関数を実行する点に注意。つまり、セーブデータ上のデータ項目の数だけ繰り返し関数が呼び出される。
	//　　　　　以上の内容は、struct load と strcut serialize の両方に当てはまる。
	template<class ARCHIVE_TYPE>
	struct load<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, const testData& obj, const version& ver, const version& now_ver, const itemInfoBase* target_item)
		{
			if (!target_item)
			{
				std::printf("----------\n");
				std::printf("load<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}

			//デシリアライズ対象項目の指定
			arc & pair("data10", obj.m_data10);//※暗号化データ
			if (target_item && *target_item == "data10")//暗号化データを複合化
			{
				const_cast<testData*>(&obj)->m_data10 ^= 0xa5a5a5a5;
				//※本来このような処理は「デシリアライズ後処理」（struct afterLoad）で行う方が良いが、テストのために強引に処理する
				//　（通常、この処理内でのデータの直接変更は禁止しており、const 扱いにしている。理由は、上記注意書きの特殊な動作を踏まえずに、誤った操作を行うことを防ぐため）
			}
			arc & pair("data12", obj.m_data12);//※追加されたデータ項目...という扱い（セーブデータに存在せず、実際のデータ構造に追加されたデータ項目のテスト用）
			arc & pair("data14", obj.m_data14);//※追加されたデータ項目（オブジェクト）...という扱い
			arc & pair("data15a", obj.m_data15a);
			arc & pair("data15b", obj.m_data15b);//※データ型が縮小された...という扱い（15aを15bとしてシリアライズしている）
			arc & pair("data15c", obj.m_data15c);//※データ型が拡大された...という扱い（15aを15cとしてシリアライズしている）
			arc & pair("data15d", obj.m_data15d);//※データ型が変更された...という扱い（15aを15dとしてシリアライズしている）
			arc & pair("data16a", obj.m_data16a);
			arc & pair("data16b", obj.m_data16b);//※配列サイズが縮小された...という扱い（16aを16bとしてシリアライズしている）
			arc & pair("data16c", obj.m_data16c);//※配列サイズが拡大された...という扱い（16aを16bとしてシリアライズしている）
			arc & pair("data16d", obj.m_data16d);//※配列から非配列に変更された...という扱い（16aを16dとしてシリアライズする）
			arc & pair("data16e", obj.m_data16e);//※非配列から配列に変更された...という扱い（16dを16eとしてシリアライズする）
		}
	};

	//--------------------
	//デシリアライズ前処理：testData 用
	//※デシリアライズ（ロード）の前に一度実行される処理。
	//※例えば、デシリアライズの前にメモリ確保や初期化が必要な場合に利用する。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE>
	struct beforeLoad<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, testData& obj, const version& ver, const version& now_ver)
		{
			std::printf("----------\n");
			std::printf("beforeLoad<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());

			//メモリ確保
			if (!obj.m_data7c)//※構造体の nullptr テスト用（実体 に nullptrをロードするテスト）
				obj.m_data7c = new testData::subData();
			if (!obj.m_data7e)//※ロード前処理でメモリ確保テスト用
				obj.m_data7e = new testData::subData();
		}
	};

	//--------------------
	//デシリアライズ後処理：testData 用
	//※デシリアライズ（ロード）の後に一度実行される処理。
	//※例えば、一通りのデータ項目のデシリアライズ後にデータの調整を行う場合などに利用する。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE>
	struct afterLoad<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, testData& obj, const version& ver, const version& now_ver)
		{
			std::printf("----------\n");
			std::printf("afterLoad<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());

			//ロード結果に基づいてデータを調整
			obj.m_data9 = 0;
			for(auto data : obj.m_data3)
				obj.m_data9 = data;
		}
	};

	//--------------------
	//セーブデータにだけ存在するデータ項目の通知：testData 用
	//※必要に応じて定義する。
	//※定義がなければ該する当データがあっても無視される。
	template<class ARCHIVE_TYPE>
	struct noticeUnrecognizedItem<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, testData& obj, version& ver, const version& now_ver, const itemInfoBase& unrecognized_item)
		{
			std::printf("----------\n");
			std::printf("noticeUnrecognizedItem<testData>(ver=%d.%d,now_ver=%d.%d), item=\"(no-name)\"(0x%08x, type=%s)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), unrecognized_item.nameCrc(), unrecognized_item.typeInfo().name());
			
			if (unrecognized_item == "data11")
			{
				arc & pair("data11", obj.m_data11_deleted);//セーブデータにだけ存在するデータを別のデータ項目に移し替え
			}
			else if (unrecognized_item == "data13")
			{
				arc & pair("data13", obj.m_data13_deleted);//セーブデータにだけ存在するデータを別のデータ項目に移し替え
			}
		}
	};
	
	//--------------------
	//セーブデータに存在しないデータ項目の通知：testData 用
	//※必要に応じて定義する。
	//※定義がなければ該する当データがあっても無視される。
	template<class ARCHIVE_TYPE>
	struct noticeUnloadedItem<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, testData& obj, version& ver, const version& now_ver, const itemInfoBase& unloaded_item)
		{
			std::printf("----------\n");
			std::printf("noticeUnloadedItem<testData>(ver=%d.%d,now_ver=%d.%d), item=\"%s\"(0x%08x,type=%s)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), unloaded_item.name(), unloaded_item.nameCrc(), unloaded_item.typeInfo().name());

			if (unloaded_item == "data12")
			{
				obj.m_data12 = 999;//初期値を設定
			}
			else if (unloaded_item == "data14")
			{
				obj.m_data14.m_a = 1;//初期値を設定
				obj.m_data14.m_b = 2;
				obj.m_data14.m_c = 3;
			}
		}
	};
	
	//--------------------
	//データ収集処理：saveData用
	//※シリアライズ専用処理
	//※（本来は）複数のデータをひとまとめにシリアライズしたい時に使用する。
	//※必要に応じて定義する。ただし、定義時は対となるデータ分配処理の定義も必須。
	template<class ARCHIVE_TYPE>
	struct collector<ARCHIVE_TYPE, saveData> {
		void operator()(ARCHIVE_TYPE& arc, const saveData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("collector<saveData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
			
			//シリアライズ対象データを取得
			testData& data = *getTestData();
			
			//シリアライズ
			arc << pair("testData", data);
			
			//※他に必要なデータのシリアライズがあれば列挙する...
		}
	};
	
	//--------------------
	//データ分配処理：saveData用
	//※デシリアライズ専用処理
	//※（本来は）複数のデータがひとまとめにシリアライズされたデータをデシリアライズする時に使用する。
	//※必要に応じて定義する。ただし、定義時は対となるデータ収集処理の定義も必須。
	template<class ARCHIVE_TYPE>
	struct distributor<ARCHIVE_TYPE, saveData> {
		void operator()(ARCHIVE_TYPE& arc, saveData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			std::printf("----------\n");
			std::printf("distributor<saveData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
			
			//デシリアライズのためにテストデータを初期化して取得
			destroyTestData();//インスタンス破棄
			createTestData();//インスタンス生成
			testData& data = *getTestData();//インスタンス取得
			
			//デシリアライズ
			arc >> pair("testData", data);
			
			//※他に必要なデータのデシリアライズがあれば列挙する...
		}
	};

	//--------------------
	//データ収集処理：testData用
	//※データ収集処理は、クラス（構造体）ごとに呼び出される。
	template<class ARCHIVE_TYPE>
	struct collector<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, const testData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("collector<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
		}
	};
	
	//--------------------
	//データ分配処理：testData用
	//※データ分配処理は、クラス（構造体）ごとに呼び出される。
	template<class ARCHIVE_TYPE>
	struct distributor<ARCHIVE_TYPE, testData> {
		void operator()(ARCHIVE_TYPE& arc, testData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			std::printf("----------\n");
			std::printf("distributor<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
		}
	};

	//--------------------
	//シリアライズ時の致命的なエラー発生時処理処理：saveData
	//※メモリ不足などの理由により、シリアライズ（セーブ）に失敗した時に初期状態にする場合などに使用する。
	//※シリアライズに致命的なエラーが検出された際、この処理が一度だけ呼び出される。その後、シリアライズ処理は続行せず終了する。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE>
	struct fatalSerializeErrorOccurred<ARCHIVE_TYPE, saveData> {
		void operator()(ARCHIVE_TYPE& arc, const saveData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("fatalSerializeErrorOccurred<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
		}
	};

	//--------------------
	//デシリアライズ時の致命的なエラー発生時処理処理：saveData
	//※メモリ不足やデータ破損などの理由により、デシリアライズ（ロード）に失敗した時に初期状態にする場合などに使用する。
	//※デシリアライズに致命的なエラーが検出された際、この処理が一度だけ呼び出される。その後、デシリアライズ処理は続行せず終了する。
	//※必要に応じて定義する。
	template<class ARCHIVE_TYPE>
	struct fatalDeserializeErrorOccurred<ARCHIVE_TYPE, saveData> {
		void operator()(ARCHIVE_TYPE& arc, saveData& obj, const version& ver, const version& now_ver)
		{
			std::printf("----------\n");
			std::printf("fatalDeserializeErrorOccurred<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
		}
	};
}//namespace serialization
GASHA_NAMESPACE_END;//ネームスペース：終了

#endif//GASHA_INCLUDED_ASVE_LOAD_DEF_CPP_H

// End of file
