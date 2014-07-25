#pragma once
#ifndef GASHA_INCLUDED_SERIALIZATION_CPP_H
#define GASHA_INCLUDED_SERIALIZATION_CPP_H

//--------------------------------------------------------------------------------
// serialization.cpp.h
// シリアライズテスト：機能テスト：シリアライズ処理定義
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "feature_test/feature_test.h"//シリアライズテスト：機能テスト
#include "feature_test/test_data.h"//テストデータ定義

#include <gasha/serialization.h>//シリアライズ
                                //※シリアライズ処理を定義する際にインクルードする

#include <cstdio>//std::printf()

namespace for_feature_test
{
	//--------------------
	//セーブデータ作成用クラス
	//※空のクラス
	class saveData{};
}//namespace for_feature_test

GASHA_USING_NAMESPACE;//ネームスペース使用
using namespace for_feature_test;//ネームスペース使用

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
	//※ gasha::serializatoin::serialize<ARCHIVE, T> クラスの T を任意の型で特殊化したクラスをユーザー定義する。
	//※基本的には、この処理だけを定義すればシリアライズ／デシリアライズ可能。
	//※必要に応じて定義する。（別途シリアライズ専用処理とデシリアライズ専用処理を用いる方法もあるため、この定義が必須ということはない）
	//※【注意】シリアライズ時とデシリアライズ時では処理の呼び出され方が異なる。詳しは、後述の struct load の所に記述。
	template<class ARCHIVE>
	struct serialize<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, const testData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("serialize<testData>(ver=%d.%d,now_ver=%d.%d) sizeof(testData)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** serialize<testData>:Deserialize object=\"%s\"\n", deserialize_obj->name());
			}

			//シリアライズ対象項目の指定
			//※ pair() 関数にデータ項目名とデータ項目を指定し、その結果を「&」演算子でアーカイブオブジェクトに受け渡す。
			//※細かくデータ項目を設定することで、セーブデータと実際のデータ構造の不整合があっても、ロード時に自動的に解消する。
			//　（データ項目の追加／削除／順序変更に対応可能。）
			arc & pair("data1", obj.m_data1) //単純なデータ項目のテスト
				& pair("data2", obj.m_data2) //単純なデータ項目のテスト
				& pair("data3", obj.m_data3); //配列ののテスト
		#ifdef FEATURE_TEST_HAS_INVALID_ITEM
			arc & pair("data1", obj.m_data1);//多重定義のテスト（無効なデータ項目としてエラーとなり、無視されるが、処理は続行する）
		#endif//FEATURE_TEST_HAS_INVALID_ITEM
			arc & pairBin("data4a", obj.m_data4a);//構造体をバイナリデータとしてシリアライズするテスト
			arc & pair("data4b", obj.m_data4b);//構造体を独自の形式でシリアライズするテスト
			arc & pairBin("data5a", obj.m_data5a);//標準ライブラリのデータをバイナリデータとしてシリアライズするテスト
			arc & pair("data5b", obj.m_data5b);//標準ライブラリのデータを独自の形式でシリアライズするテスト
			arc & pair("data6", obj.m_data6);//構造体の配列テスト
			arc & pair("data7a", obj.m_data7a);//構造体のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はインスタンス生成時にメモリ確保
			arc & pair("data7b", obj.m_data7b);//構造体のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
			arc & pair("data7c", obj.m_data7c);//構造体のポインタテスト（nullptr に nullptrをロードするテスト）
			arc & pair("data7d", obj.m_data7d);//構造体のポインタテスト（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
			arc & pair("data7e", obj.m_data7e);//構造体のポインタテスト（nullptr に 実体をロードするテスト）
			arc & pairArray("data7h", obj.m_data7h, 2);//構造体の配列のポインタテスト用（実体に実体をロードするテスト）
			arc & pair("data8a", obj.m_data8a);//プリミティブ型のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はインスタンス生成時にメモリ確保
			arc & pair("data8b", obj.m_data8b);//プリミティブ型のポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
			arc & pair("data8c", obj.m_data8c);//プリミティブ型のポインタテスト（nullptr に nullptrをロードするテスト）
			arc & pair("data8d", obj.m_data8d);//プリミティブ型のポインタテスト（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
			arc & pair("data8e", obj.m_data8e);//プリミティブ型のポインタテスト（nullptr に 実体をロードするテスト）
			arc & pairArray("data8h", obj.m_data8h, 2);//プリミティブ型の配列のポインタテスト用（実体に実体をロードするテスト）
			arc & pair("data23", obj.m_data23);//継承した構造体のテスト
			arc & pair("data24", obj.m_data24);//多重継承した構造体のテスト
		}
	};

	//--------------------
	//シリアライズ／デシリアライズ共通処理：testData::subData 用
	//※構造体（クラス）ごとにシリライズ／デシリアライズ処理を定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct serialize<ARCHIVE, testData::subData> {
		void operator()(ARCHIVE& arc, const testData::subData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("serialize<testData::subData>(ver=%d.%d,now_ver=%d.%d) sizeof(testData::subData)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData::subData));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** serialize<testData::subData>:Deserialize object=\"%s\"\n", deserialize_obj->name());
			}

			//シリアライズ対象項目の指定
			arc & pair("dataA", obj.m_a);
			arc & pair("dataB", obj.m_b);
			arc & pair("dataC", obj.m_c);
		}
	};

	//--------------------
	//シリアライズ／デシリアライズ共通処理：testData::subData2 用
	//※構造体（クラス）ごとにシリライズ／デシリアライズ処理を定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct serialize<ARCHIVE, testData::subData2> {
		void operator()(ARCHIVE& arc, const testData::subData2& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("serialize<testData::subData2>(ver=%d.%d,now_ver=%d.%d) sizeof(testData::subData2)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData::subData2));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** serialize<testData::subData2>:Deserialize object=\"%s\"\n", deserialize_obj->name());
			}

			//シリアライズ対象項目の指定
			arc & pair("dataA", obj.m_a);
			arc & pair("dataB", obj.m_b);
			arc & pair("dataC", obj.m_c);
			arc & pair("dataD", obj.m_d);
		}
	};

	//--------------------
	//シリアライズ／デシリアライズ共通処理：testData::subDataExt 用
	//※構造体（クラス）ごとにシリライズ／デシリアライズ処理を定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct serialize<ARCHIVE, testData::subDataExt> {
		void operator()(ARCHIVE& arc, const testData::subDataExt& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("serialize<testData::subDataExt>(ver=%d.%d,now_ver=%d.%d) sizeof(testData::subDataExt)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData::subDataExt));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** serialize<testData::subDataExt>:Deserialize object=\"%s\"\n", deserialize_obj->name());
			}

			//シリアライズ対象項目の指定
			arc & pair<testData::subData>("subData", obj);
			arc & pair("dataZ", obj.m_z);
		}
	};

	//--------------------
	//シリアライズ／デシリアライズ共通処理：testData::subDataExt2 用
	//※構造体（クラス）ごとにシリライズ／デシリアライズ処理を定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct serialize<ARCHIVE, testData::subDataExt2> {
		void operator()(ARCHIVE& arc, const testData::subDataExt2& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("serialize<testData::subDataExt2>(ver=%d.%d,now_ver=%d.%d) sizeof(testData::subDataExt2)=%d\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), sizeof(testData::subDataExt2));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** serialize<testData::subDataExt2>:Deserialize object=\"%s\"\n", deserialize_obj->name());
			}

			//シリアライズ対象項目の指定
			arc & pair<testData::subData>("subData", obj);
			arc & pair<testData::subData2>("subData2", obj);
			arc & pair("dataZ", obj.m_z);
		}
	};

#ifdef FEATURE_TEST_ENABLE_TO_SERIALIZE_BITSET
	//--------------------
	//シリアライズ／デシリアライズ共通処理：std::bitset 用
	//※標準ライブラリのクラスなど、ユーザー定義クラス（構造体）以外であっても、シリアライズ／デシリアライズ処理を定義可能。
	//※シリアライズ／デシリアライズが未定義のクラス（構造体）に対しては、バイナリデータとして処理される。
	//※このサンプルは、std::bitsetを1ビットずつ別項目で書き出す。膨大なワークバッファを要するので注意。また、処理効率も非常に悪い。あくまでもサンプル。
	//※必要に応じて定義する。
	template<class ARCHIVE, std::size_t N>
	struct serialize<ARCHIVE, std::bitset<N>> {
		void operator()(ARCHIVE& arc, const std::bitset<N>& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("serialize<std::bitset<%d>>(ver=%d.%d,now_ver=%d.%d) sizeof(std::bitset<%d>)=%d\n", N, ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), N, sizeof(std::bitset<N>));
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** serialize<std::bitset<%d>>:Deserialize object=\"%s\"\n", N, deserialize_obj->name());
			}

			//シリアライズ対象項目の指定
			for (int i = 0; i < N; ++i)
			{
				char name[16];
				GASHA_ spprintf(name, "no%d", i);
				static bool bit = false;//※デシリアライズはこの関数を抜けた後で処理するため、ワークバッファは静的な領域である必要がある
				if (arc.isOutputClass)
					bit = obj[i];
				std::function<void(std::bitset<N>&, const itemInfoBase&)> load_bit = [](std::bitset<N>& obj, const itemInfoBase& item)//※個別デシリアライズ処理
				{
					printf("***** serialize<std::bitset<N>>:Special deserialize item=\"%s\"\n", item.name());
					for (int i = 0; i < N; ++i)
					{
						char name[16];
						GASHA_ spprintf(name, "no%d", i);
						if (item == name)
						{
							obj[i] = bit;
							break;
						}
					}
				};
				arc & pair(name, bit, load_bit);
			}
		}
	};
#endif//FEATURE_TEST_ENABLE_TO_SERIALIZE_BITSET

	//--------------------
	//シリアライズ専用処理：testData 用
	//※シリアライズ（セーブ）専用の処理。必要に応じて定義。
	//※例えば、シリアライズとデシリアライズの処理を共通化できない項目に対して、専用のシリアライズ処理とデシリアライズ処理をそれぞれ定義する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct save<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, const testData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("save<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

			//シリアライズ対象項目の指定
			arc & pair("data7f", obj.m_data7g);//構造体の構造体のポインタを実体に変更...という扱い（7gを7fとしてシリアライズする）
			arc & pair("data7g", obj.m_data7f);//構造体の実体をポインタに変更...という扱い（7fを7gとしてシリアライズする）
			arc & pairArray("data7i", obj.m_data7j, 2);//構造体のポインタを実体の配列に変更...という扱い（7jを7iとしてシリアライズする）
			arc & pair("data7j", obj.m_data7i);//構造体の実体の配列をポインタに変更...という扱い（7iを7jとしてシリアライズする）
			arc & pair("data8f", obj.m_data8f);//プリミティブ型のポインタを実体に変更...という扱い（8gを8fとしてシリアライズする）
			arc & pair("data8g", obj.m_data8g);//プリミティブ型の実体をポインタに変更...という扱い（8fを8gとしてシリアライズする）
			arc & pairArray("data8i", obj.m_data8j, 2);//プリミティブ型のポインタに実体の配列に変更...という扱い（8jを8iとしてシリアライズする）
			arc & pair("data8j", obj.m_data8i);//プリミティブ型の実体の配列をポインタに変更...という扱い（8iを8jとしてシリアライズする）
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
			arc & pair("data17a", obj.m_data17a);
			arc & pair("data17b", obj.m_data17a);//※構造体の配列サイズが縮小された...という扱い（17aを17bとしてシリアライズする）
			arc & pair("data17c", obj.m_data17a);//※構造体の配列サイズが拡大された...という扱い（17aを17cとしてシリアライズする）
			arc & pair("data17d", obj.m_data17a);//※構造体の配列から非配列に変更された...という扱い（17aを17dとしてシリアライズする）
			arc & pair("data17e", obj.m_data17d);//※構造体の非配列から配列に変更された...という扱い（17dを17eとしてシリアライズする）
			arc & pair("data18a", obj.m_data18b);//※構造体（専用シリアライズあり）からプリミティブ型に変更された...という扱い（18bを18aとしてシリアライズする）
			arc & pair("data18b", obj.m_data18a);//※プリミティブ型から構造体（専用シリアライズあり）に変更された...という扱い（18aを18bとしてシリアライズする）
			arc & pair("data19a", obj.m_data19b);//※構造体（専用シリアライズなし）からプリミティブ型に変更された...という扱い（19bを19aとしてシリアライズする）
			arc & pair("data19b", obj.m_data19a);//※プリミティブ型から構造体（専用シリアライズなし）に変更された...という扱い（19aを19bとしてシリアライズする）
			arc & pair("data20a", obj.m_data20b);//※構造体（専用シリアライズなし）から構造体（専用シリアライズあり）に変更された...という扱い（20bを20aとしてシリアライズする）
			arc & pair("data20b", obj.m_data20a);//※構造体（専用シリアライズあり）から構造体（専用シリアライズなし）に変更された...という扱い（20aを20bとしてシリアライズする）
			arc & pair("data21a", obj.m_data21a);
			arc & pair("data21b", obj.m_data21a);//※構造体（専用シリアライズなし）のメンバー数が縮小された...という扱い（21aを21bとしてシリアライズする）
			arc & pair("data21c", obj.m_data21a);//※構造体（専用シリアライズなし）のメンバー数が拡大された...という扱い（21aを21cとしてシリアライズする）
			arc & pair("data22a", obj.m_data22b);//※構造体（専用シリアライズあり）のメンバーと順序が変更された...という扱い（22bを22aとしてシリアライズする）
			arc & pair("data22b", obj.m_data22a);//※構造体（専用シリアライズあり）のメンバーと順序が変更された...という扱い（22aを22bとしてシリアライズする）
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
	//　　　　　データ項目を読み込む際に、それがオブジェクト型（任意のデシリアライズ処理が設定されたクラス／構造体）である場合、
	//          再度関数を実行する点に注意。
	//　　　　　以上の内容は、struct load と strcut serialize の両方に当てはまる。
	template<class ARCHIVE>
	struct load<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, const testData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			if (!deserialize_obj)
			{
				std::printf("----------\n");
				std::printf("load<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
				std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
			}
			else
			{
				printf("***** load<testData>:Deserialize object=\"%s\"\n", deserialize_obj->name());
			}

			//デシリアライズ対象項目の指定
			arc & pair("data7f", obj.m_data7f);//構造体の構造体のポインタを実体に変更...という扱い（7gを7fとしてシリアライズする）
			arc & pair("data7g", obj.m_data7g);//構造体の実体をポインタに変更...という扱い（7fを7gとしてシリアライズする）
			arc & pair("data7i", obj.m_data7i);//構造体のポインタを実体の配列に変更...という扱い（7jを7iとしてシリアライズする）
			arc & pairArray("data7j", obj.m_data7j, 2);//構造体の実体の配列をポインタに変更...という扱い（7iを7jとしてシリアライズする）
			arc & pair("data8f", obj.m_data8f);//プリミティブ型のポインタを実体に変更...という扱い（8gを8fとしてシリアライズする）
			arc & pair("data8g", obj.m_data8g);//プリミティブ型の実体をポインタに変更...という扱い（8fを8gとしてシリアライズする）
			arc & pair("data8i", obj.m_data8i);//プリミティブ型のポインタに実体の配列に変更...という扱い（8jを8iとしてシリアライズする）
			arc & pairArray("data8j", obj.m_data8j, 2);//プリミティブ型の実体の配列をポインタに変更...という扱い（8iを8jとしてシリアライズする）
			static int eccrypted_data10 = 0;//※デシリアライズはこの関数を抜けた後で処理するため、ワークバッファは静的な領域である必要がある
			std::function<void(testData&, const itemInfoBase&)> decrypt_data10 = [](testData& obj, const itemInfoBase& item)//※個別デシリアライズ処理（暗号化データの複合化）
			{
				printf("***** load<testData>:Special deserialize item=\"%s\"\n", item.name());
				obj.m_data10 = eccrypted_data10 ^ 0xa5a5a5a5;
			};
			arc & pair("data10", eccrypted_data10, decrypt_data10);//※暗号化データ
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
			arc & pair("data17a", obj.m_data17a);
			arc & pair("data17b", obj.m_data17b);//※構造体の配列サイズが縮小された...という扱い（17aを17bとしてシリアライズする）
			arc & pair("data17c", obj.m_data17c);//※構造体の配列サイズが拡大された...という扱い（17aを17cとしてシリアライズする）
			arc & pair("data17d", obj.m_data17d);//※構造体の配列から非配列に変更された...という扱い（17aを17dとしてシリアライズする）
			arc & pair("data17e", obj.m_data17e);//※構造体の非配列から配列に変更された...という扱い（17dを17eとしてシリアライズする）
			arc & pair("data18a", obj.m_data18a);//※構造体（専用シリアライズあり）からプリミティブ型に変更された...という扱い（18bを18aとしてシリアライズする）
			arc & pair("data18b", obj.m_data18b);//※プリミティブ型から構造体（専用シリアライズあり）に変更された...という扱い（18aを18bとしてシリアライズする）
			arc & pair("data19a", obj.m_data19a);//※構造体（専用シリアライズなし）からプリミティブ型に変更された...という扱い（19bを19aとしてシリアライズする）
			arc & pair("data19b", obj.m_data19b);//※プリミティブ型から構造体（専用シリアライズなし）に変更された...という扱い（19aを19bとしてシリアライズする）
			arc & pair("data20a", obj.m_data20a);//※構造体（専用シリアライズなし）から構造体（専用シリアライズあり）に変更された...という扱い（20bを20aとしてシリアライズする）
			arc & pair("data20b", obj.m_data20b);//※構造体（専用シリアライズあり）から構造体（専用シリアライズなし）に変更された...という扱い（20aを20bとしてシリアライズする）
			arc & pair("data21a", obj.m_data21a);
			arc & pair("data21b", obj.m_data21b);//※構造体（専用シリアライズなし）のメンバー数が縮小された...という扱い（21aを21bとしてシリアライズする）
			arc & pair("data21c", obj.m_data21c);//※構造体（専用シリアライズなし）のメンバー数が拡大された...という扱い（21aを21cとしてシリアライズする）
			arc & pair("data22a", obj.m_data22a);//※構造体（専用シリアライズあり）のメンバーと順序が変更された...という扱い（22bを22aとしてシリアライズする）
			arc & pair("data22b", obj.m_data22b);//※構造体（専用シリアライズあり）のメンバーと順序が変更された...という扱い（22aを22bとしてシリアライズする）
		}
	};

	//--------------------
	//デシリアライズ前処理：testData 用
	//※デシリアライズ（ロード）の前に一度実行される処理。
	//※例えば、デシリアライズの前にメモリ確保や初期化が必要な場合に利用する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct beforeLoad<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, testData& obj, const version& ver, const version& now_ver)
		{
			std::printf("----------\n");
			std::printf("beforeLoad<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

			//メモリ確保
			//if (!obj.m_data7a)//ポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はインスタンス生成時にメモリ確保
			//	obj.m_data7a = new testData::subData();
			if (!obj.m_data7b)//ポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
				obj.m_data7b = new testData::subData();
			if (!obj.m_data7d)//ポインタテスト（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
				obj.m_data7d = new testData::subData();
			//if (!obj.m_data8a)//ポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はインスタンス生成時にメモリ確保
			//	obj.m_data8a = new int(0);
			if (!obj.m_data8b)//ポインタテスト（実体に実体をロードするテスト） ※ロード先の実体はロード前処理でメモリ確保
				obj.m_data8b = new int(0);
			if (!obj.m_data8d)//ポインタテスト（実体に nullptrをロードするテスト）※ロード先の実体はロード前処理でメモリ確保
				obj.m_data8d = new int(0);
		}
	};

	//--------------------
	//デシリアライズ後処理：testData 用
	//※デシリアライズ（ロード）の後に一度実行される処理。
	//※例えば、一通りのデータ項目のデシリアライズ後にデータの調整を行う場合などに利用する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct afterLoad<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, testData& obj, const version& ver, const version& now_ver)
		{
			std::printf("----------\n");
			std::printf("afterLoad<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

			//ロード結果に基づいてデータを調整
			obj.m_data9 = 0;//m_data3の合計値を格納する
			for(auto data : obj.m_data3)
				obj.m_data9 += data;
		}
	};

	//--------------------
	//セーブデータにだけ存在するデータ項目の通知：testData 用
	//※必要に応じて定義する。
	//※定義がなければ該する当データがあっても無視される。
	template<class ARCHIVE>
	struct noticeUnrecognizedItem<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, testData& obj, version& ver, const version& now_ver, const itemInfoBase& unrecognized_item)
		{
			std::printf("----------\n");
			std::printf("noticeUnrecognizedItem<testData>(ver=%d.%d,now_ver=%d.%d), unrecognized_item=\"(no-name)\"(0x%08x, type=%s)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), unrecognized_item.nameCrc(), unrecognized_item.typeInfo().name());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

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
	template<class ARCHIVE>
	struct noticeUnloadedItem<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, testData& obj, version& ver, const version& now_ver, const itemInfoBase& unloaded_item)
		{
			std::printf("----------\n");
			std::printf("noticeUnloadedItem<testData>(ver=%d.%d,now_ver=%d.%d), unloaded_item=\"%s\"(0x%08x,type=%s)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), unloaded_item.name(), unloaded_item.nameCrc(), unloaded_item.typeInfo().name());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

			if (unloaded_item == "data12")
			{
				obj.m_data12 = 999;//初期値を設定
			}
			else if (unloaded_item == "data14")
			{
				obj.m_data14.m_a = 998;//初期値を設定
				obj.m_data14.m_b = 997;
				obj.m_data14.m_c = 996.f;
			}
		}
	};
	
	//--------------------
	//データ収集処理：saveData用
	//※シリアライズ専用処理
	//※（本来は）複数のデータをひとまとめにシリアライズしたい時に使用する。
	//※必要に応じて定義する。ただし、定義時は対となるデータ分配処理の定義も必須。
	template<class ARCHIVE>
	struct collector<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, const saveData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("collector<saveData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

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
	template<class ARCHIVE>
	struct distributor<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, saveData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			std::printf("----------\n");
			std::printf("distributor<saveData>(ver=%d.%d,now_ver=%d.%d), target_item=\"%s\"\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer(), target_item.name());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);

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
	//※シリアライズの後、他のデータを連結してまとめてシリアライズする時に使用する。
	//※必要に応じて定義する。ただし、定義時は対となるデータ分配処理の定義も必須。
	template<class ARCHIVE>
	struct collector<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, const testData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("collector<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
		}
	};
	
	//--------------------
	//データ分配処理：testData用
	//※データ分配処理は、クラス（構造体）ごとに呼び出される。
	//※デシリアライズの後、連結してシリアライズされている他のデータをデシリアライズする際に使用する。
	//※必要に応じて定義する。ただし、定義時は対となるデータ収集処理の定義も必須。
	template<class ARCHIVE>
	struct distributor<ARCHIVE, testData> {
		void operator()(ARCHIVE& arc, testData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			std::printf("----------\n");
			std::printf("distributor<testData>(ver=%d.%d), target_item=\"%s\"\n", ver.majorVer(), ver.minorVer(), target_item.name());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
		}
	};

	//--------------------
	//シリアライズ時の致命的なエラー発生時処理処理：saveData
	//※メモリ不足などの理由により、シリアライズ（セーブ）に失敗した時に初期状態にする場合などに使用する。
	//※シリアライズに致命的なエラーが検出された際、この処理が一度だけ呼び出される。その後、シリアライズ処理は続行せず終了する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct fatalSerializeErrorOccurred<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, const saveData& obj, const version& ver)
		{
			std::printf("----------\n");
			std::printf("fatalSerializeErrorOccurred<testData>(ver=%d.%d)\n", ver.majorVer(), ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
		}
	};

	//--------------------
	//デシリアライズ時の致命的なエラー発生時処理処理：saveData
	//※メモリ不足やデータ破損などの理由により、デシリアライズ（ロード）に失敗した時に初期状態にする場合などに使用する。
	//※デシリアライズに致命的なエラーが検出された際、この処理が一度だけ呼び出される。その後、デシリアライズ処理は続行せず終了する。
	//※必要に応じて定義する。
	template<class ARCHIVE>
	struct fatalDeserializeErrorOccurred<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, saveData& obj, const version& ver, const version& now_ver)
		{
			std::printf("----------\n");
			std::printf("fatalDeserializeErrorOccurred<testData>(ver=%d.%d,now_ver=%d.%d)\n", ver.majorVer(), ver.minorVer(), now_ver.majorVer(), now_ver.minorVer());
			std::printf("arc.isInputClass=%d, arc.isOutputClass=%d\n", arc.isInputClass, arc.isOutputClass);
		}
	};
}//namespace serialization
GASHA_NAMESPACE_END;//ネームスペース：終了

#endif//GASHA_INCLUDED_SERIALIZATION_CPP_H

// End of file
