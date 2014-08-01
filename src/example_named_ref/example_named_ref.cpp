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

#include "gasha/type_traits.h"//型特性ユーティリティ：toStr()

#include <cstdio>//std::printf()
#include <utility>//std::declval()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref()
{
	std::printf("----- Test for namedRef -----\n");
	
	//名前付きデータ参照
	refTableWithLock refTbl;

	//名前をCRC値取得
	constexpr crc32_t name_valA = calcStaticCRC32("valA");
	constexpr crc32_t name_valB = calcStaticCRC32("valB");
	constexpr crc32_t name_dataC = calcStaticCRC32("dataC");

	//読み書き可能データとして登録されたデータにアクセス

	//値の取得①
	if (refTbl.isReadable<int>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		auto val = refTbl.load<int>(name_valA);//値の取得
		std::printf("load<int>(): valA=%d\n", val);
	}
	//値の取得②：名前で直接指定
	if (refTbl.isReadable<int>("valA"))//※名前を直接指定してもよい（事前計算している方が当然速い）
	{
		auto val = refTbl.load<int>("valA");//※名前を直接指定してもよい
		std::printf("load<int>(): valA=%d\n", val);
	}
	//値の変更
	if (refTbl.isWritable<int>(name_valA))//書き込み可能判定（必要に応じて使用）
	{
		refTbl.store<int>(name_valA, 1);//値の変更 ※何も返さない
		auto val = refTbl.load<int>(name_valA);//値の取得
		std::printf("store<int>(): valA=%d\n", val);
	}
	//値の変更①：読み書き可能参照
	if (refTbl.isWritable<int>(name_valA))//書き込み可能判定（必要に応じて使用）
	{
		auto val = refTbl.ref<int>(name_valA);//参照を取得 ※戻り値が有効なスコープの間、排他ロックを取得することに注意
		*val = 2;//値を変更
		std::printf("ref<int>(): valA=%d\n", *val);
	}
	//値の変更②：読み取り専用参照
	if (refTbl.isReadable<int>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		auto val = refTbl.cref<int>(name_valA);
		//*val = 3;//変更不可
		std::printf("cref<int>(): valA=%d\n", *val);
	}
	//誤ったデータ型でアクセス①：読み込み
	if (refTbl.isReadable<short>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		auto val = refTbl.load<short>(name_valA);//値の取得
		std::printf("load<int>(): valA=%d\n", val);
	}
	//誤ったデータ型でアクセス②：書き込み
	if (refTbl.isWritable<short>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		refTbl.store<int>(name_valA, 4);//値の変更 ※何も返さない
		auto val = refTbl.load<int>(name_valA);//値の取得
		std::printf("load<int>(): valA=%d\n", val);
	}

	//読み取り専用データとして登録されたデータにアクセス

	//値の取得①
	if (refTbl.isReadable<int>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		auto val = refTbl.load<int>(name_valB);//値の取得
		std::printf("load<int>(): valB=%d\n", val);
	}
	//値の取得②：名前で直接指定
	if (refTbl.isReadable<int>("valB"))//※名前を直接指定してもよい（事前計算している方が当然速い）
	{
		auto val = refTbl.load<int>("valB");//※名前を直接指定してもよい
		std::printf("load<int>(): valB=%d\n", val);
	}
	//値の変更
	if (refTbl.isWritable<int>(name_valB))//書き込み可能判定（必要に応じて使用）
	{
		refTbl.store<int>(name_valB, 1);//値の変更 ※何も返さない
		auto val = refTbl.load<int>(name_valB);//値の取得
		std::printf("store<int>(): valB=%d\n", val);
	}
	//値の変更①：読み書き可能参照
	if (refTbl.isWritable<int>(name_valB))//書き込み可能判定（必要に応じて使用）
	{
		auto val = refTbl.ref<int>(name_valB);//参照を取得 ※戻り値 val が有効なスコープの間、排他ロックすることに注意
		*val = 2;//値を変更
		std::printf("ref<int>(): valB=%d\n", *val);
	}
	//値の変更②：読み取り専用参照
	if (refTbl.isReadable<int>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		auto val = refTbl.cref<int>(name_valB);//参照を取得 ※戻り値 val が有効なスコープの間、共有ロックすることに注意
		//*val = 3;//変更不可
		std::printf("cref<int>(): valB=%d\n", *val);
	}
	//誤ったデータ型でアクセス①：読み込み
	if (refTbl.isReadable<short>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		auto val = refTbl.load<short>(name_valB);//値の取得
		std::printf("load<int>(): valB=%d\n", val);
	}
	//誤ったデータ型でアクセス②：書き込み
	if (refTbl.isWritable<short>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		refTbl.store<int>(name_valB, 4);//値の変更 ※何も返さない
		auto val = refTbl.load<int>(name_valB);//値の取得
		std::printf("load<int>(): valB=%d\n", val);
	}

	//構造体にアクセス

	//値の変更①：読み書き可能参照
	if (refTbl.isWritable<data_t>(name_dataC))//書き込み可能判定（必要に応じて使用）
	{
		auto data_c = refTbl.ref<data_t>(name_dataC);//参照を取得 ※戻り値 val が有効なスコープの間、排他ロックすることに注意
		data_c->m_memberA = 1;
		data_c->m_memberB[0] = 2;
		data_c->m_memberB[1] = 3;
		(*data_c).m_memberC = 4.f;
		std::printf("ref<data_t>(): dataC={ %d, {%d, %d}, %.1f}\n", data_c->m_memberA, data_c->m_memberB[0], data_c->m_memberB[1], data_c->m_memberC);
	}
	//値の変更②：読み取り専用参照
	if (refTbl.isReadable<data_t>(name_dataC))//読み取り可能判定（必要に応じて使用）
	{
		auto data_c = refTbl.cref<data_t>(name_dataC);//参照を取得 ※戻り値 val が有効なスコープの間、共有ロックすることに注意
		std::printf("ref<data_t>(): dataC={ %d, {%d, %d}, %.1f}\n", data_c->m_memberA, data_c->m_memberB[0], data_c->m_memberB[1], data_c->m_memberC);
	}
	
	//演算
	refTbl.inc<int>("valA"); std::printf("inc<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.incWA<int>("valA", 10, 0); std::printf("incWA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.incSA<int>("valA", 10, 0); std::printf("incSA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.dec<int>("valA"); std::printf("dec<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.decWA<int>("valA", 10, 0); std::printf("decWA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.decSA<int>("valA", 10, 0); std::printf("decSA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.add<int>("valA", 1); std::printf("add<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.addWA<int>("valA", 1, 10, 0); std::printf("addWA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.addSA<int>("valA", 1, 10, 0); std::printf("addSA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.sub<int>("valA", 1); std::printf("sub<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.subWA<int>("valA", 1, 10, 0); std::printf("subWA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.subSA<int>("valA", 1, 10, 0); std::printf("subSA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.mul<int>("valA", 2); std::printf("mul<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.mulWA<int>("valA", 2, 10, 0); std::printf("mulWA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.mulSA<int>("valA", 2, 10, 0); std::printf("mulSA<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.div<int>("valA", 2); std::printf("div<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.mod<int>("valA", 2); std::printf("mod<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.bitAnd<int>("valA", 15); std::printf("bitAnd<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.bitOr<int>("valA", 15); std::printf("bitOr<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.bitXor<int>("valA", 15); std::printf("bitXor<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.bitNot<int>("valA"); std::printf("bitNot<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.lShift<int>("valA", 2); std::printf("lShift<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.rShift<int>("valA", 2); std::printf("rShift<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.bitOn<int>("valA", 15); std::printf("bitOn<int>: valA=%d\n", refTbl.load<int>("valA"));
	refTbl.bitOff<int>("valA", 255); std::printf("bitOff<int>: valA=%d\n", refTbl.load<int>("valA"));
	std::printf("eq<int>: valA=%s\n", toStr(refTbl.eq<int>("valA", 15)));
	std::printf("ne<int>: valA=%s\n", toStr(refTbl.ne<int>("valA", 15)));
	std::printf("gt<int>: valA=%s\n", toStr(refTbl.gt<int>("valA", 15)));
	std::printf("ge<int>: valA=%s\n", toStr(refTbl.ge<int>("valA", 15)));
	std::printf("lt<int>: valA=%s\n", toStr(refTbl.lt<int>("valA", 15)));
	std::printf("le<int>: valA=%s\n", toStr(refTbl.le<int>("valA", 15)));
	std::printf("inOn<int>: valA=%s\n", toStr(refTbl.isOn<int>("valA", 15)));
	std::printf("inOff<int>: valA=%s\n", toStr(refTbl.isOff<int>("valA", 15)));
	std::printf("logicalAnd<int>: valA=%s\n", toStr(refTbl.logicalAnd<int>("valA", 15)));
	std::printf("logicalOr<int>: valA=%s\n", toStr(refTbl.logicalOr<int>("valA", 15)));
	std::printf("isTrue<int>: valA=%s\n", toStr(refTbl.isTrue<int>("valA")));
	std::printf("isFalse<int>: valA=%s\n", toStr(refTbl.isFalse<int>("valA")));

	//列挙
	std::printf("----------------------------------------\n");
	std::printf("Registered data items:(num=%d)\n", refTbl.size());
	for (const auto& info : refTbl)
	{
		std::printf("data: name(crc)=0x%08x, type=%s, readble=%s, writable=%s\n", info.m_nameCrc, info.m_typeInfo ? info.m_typeInfo->name() : "(unknown)", info.m_loadRef ? "yes" : "no", info.m_storeRef ? "yes" : "no");
	}

	std::printf("- end -\n");
}

// End of file
