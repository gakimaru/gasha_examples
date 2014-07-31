//--------------------------------------------------------------------------------
// exmaple_named_ref.cpp
// 名前付きデータ参照テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_named_ref.h"//名前付きデータ参照テスト

#include <gasha/named_ref.h>//名前付きデータ参照
#include <gasha/shared_spin_lock.h>//共有スピンロック

#include <cstdio>//std::printf()
#include <utility>//std::declval()

GASHA_USING_NAMESPACE;//ネームスペース使用

//名前付きデータ参照インスタンス識別用構造体
struct main_ref{};

//名前付きデータ参照の型宣言
static const std::size_t NAMED_REF_MAIN_TABLE_SIZE = 10;
using namedRef_main = namedRef<main_ref, NAMED_REF_MAIN_TABLE_SIZE, sharedSpinLock>;

//名前付きデータ参照用データ
static int s_valA = 0;//読み書き可能データ
static const int s_valB = 123;//読み取り専用データ
struct data_t
{
	int m_memberA;
	char m_memberB[2];
	float m_memberC;
};
static data_t s_dataC = { 0, { 0, 0 }, 0.f };//構造体のデータ

//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref()
{
	std::printf("----- Test for namedRef -----\n");
	
	//名前付きデータ参照
	namedRef_main ref;
	ref.regist("valA", s_valA);
	ref.regist("valB", s_valB);
	ref.regist("dataC", s_dataC);

	std::printf("s_valA=%d\n", s_valA);
	if (ref.isReadable<int>("valA"))
	{
		auto val_a = ref.load<int>("valA");
		std::printf("load<int>(): val_a=%d, s_valA=%d\n", val_a, s_valA);
	}
	if (ref.isWritable<int>("valA"))
	{
		auto val_a = ref.ref<int>("valA");
		*val_a = 1;
		std::printf("ref<int>(): val_a=%d, s_valA=%d\n", *val_a, s_valA);
	}
	if (ref.isReadable<int>("valA"))
	{
		const auto val_a = ref.cref<int>("valA");
		//*val_a = 2;//const 型に付き不可
		std::printf("cref<int>(): val_a=%d, s_valA=%d\n", *val_a, s_valA);
	}
	if (ref.isWritable<int>("valA"))
	{
		ref.store<int>("valA", 3);
		std::printf("store<int>(): s_valA=%d\n", s_valA);
	}
	if (ref.isReadOnly<int>("valB"))
	{
		auto val_b = ref.load<int>("valB");
		std::printf("load<int>(): val_b=%d, s_valB=%d\n", val_b, s_valB);
	}
	if (ref.isWritable<data_t>("dataC"))
	{
		auto data_c = ref.ref<data_t>("dataC");
		data_c->m_memberA = 1;
		data_c->m_memberB[0] = 2;
		data_c->m_memberB[1] = 3;
		(*data_c).m_memberC = 4.f;
		std::printf("ref<data_t>(): data_c={ %d, {%d, %d}, %.1f}, s_dataC={ %d, {%d, %d}, %.1f}\n", data_c->m_memberA, data_c->m_memberB[0], data_c->m_memberB[1], data_c->m_memberC, s_dataC.m_memberA, s_dataC.m_memberB[0], s_dataC.m_memberB[1], s_dataC.m_memberC);
	}
	if (ref.isReadable<data_t>("dataC"))
	{
		const auto data_c = ref.cref<data_t>("dataC");
		std::printf("ref<data_t>(): data_c={ %d, {%d, %d}, %.1f}, s_dataC={ %d, {%d, %d}, %.1f}\n", data_c->m_memberA, data_c->m_memberB[0], data_c->m_memberB[1], data_c->m_memberC, s_dataC.m_memberA, s_dataC.m_memberB[0], s_dataC.m_memberB[1], s_dataC.m_memberC);
	}
	if (ref.isReadable<char>("valA"))
	{
		auto val_a = ref.load<char>("valA");//型違いエラー
		std::printf("load<char>(): val_a=%d, s_valA=%d\n", val_a, s_valA);
	}
	if (ref.isWritable<char>("valA"))
	{
		auto val_a = ref.ref<char>("valA");//型違いエラー
		*val_a = 1;
		std::printf("ref<int>(): val_a=%d, s_valA=%d\n", *val_a, s_valA);
	}
	if (ref.isReadable<char>("valA"))
	{
		const auto val_a = ref.cref<char>("valA");//型違いエラー
		//*val_a = 2;//const 型に付き不可
		std::printf("cref<int>(): val_a=%d, s_valA=%d\n", *val_a, s_valA);
	}
	if (ref.isWritable<char>("valA"))
	{
		ref.store<char>("valA", 3);//型違いエラー
		std::printf("store<int>(): s_valA=%d\n", s_valA);
	}

	std::printf("- end -\n");
}

//明示的なインスタンス化
#include <gasha/named_ref.cpp.h>
GASHA_INSTANCING_namedRef_withLock(main_ref, NAMED_REF_MAIN_TABLE_SIZE, sharedSpinLock);

// End of file
