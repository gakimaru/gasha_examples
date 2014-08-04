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

GASHA_USING_NAMESPACE;//ネームスペース使用

//共通処理
template<class TABLE_TYPE, typename T>
void testCommon1(named_ref::table<TABLE_TYPE>& table, const char* name)
{
	std::printf("\n");
	std::printf("--- testCommon1<%s>(\"%s\") ---\n", typeid(T).name(), name);
	std::printf("isRegistered<T>(\"%s\"): %s\n", name, toStr(table.isRegistered(name)));
	std::printf("isReadOnly<T>(\"%s\"): %s\n", name, toStr(table.template isReadOnly<T>(name)));
	std::printf("isReadable<T>(\"%s\"): %s\n", name, toStr(table.template isReadable<T>(name)));
	std::printf("isWritable<T>(\"%s\"): %s\n", name, toStr(table.template isWritable<T>(name)));
	std::printf("isWritableRanged<T>(\"%s\"): %s\n", name, toStr(table.template isWritableRanged<T>(name)));
	std::printf("isWritableWraparound<T>(\"%s\"): %s\n", name, toStr(table.template isWritableWraparound<T>(name)));
	std::printf("isWritableSaturation<T>(\"%s\"): %s\n", name, toStr(table.template isWritableSaturation<T>(name)));
	if (table.template isWritableRanged<T>(name))
	{
		std::printf("max<T>(\"%s\"): %d\n", name, table.template max<T>(name));
		std::printf("min<T>(\"%s\"): %d\n", name, table.template min<T>(name));
	}
	std::printf("load<T>(\"%s\"): %d\n", name, table.template load<T>(name));
	std::printf("cref<T>(\"%s\"): %d\n", name, *table.template cref<T>(name));
	if (table.template isWritable<T>(name))
	{
		std::printf("ref<T>(\"%s\"): %d\n", name, *table.template ref<T>(name));
		*table.template ref<T>(name) = 1; std::printf("ref<T>(\"%s\")=1: %d\n", name, table.template load<T>(name));
		table.template store<T>(name, 2); std::printf("store<T>(\"%s\", 1): %d\n", name, table.template load<T>(name));
		for (int i = 0; i < 10; ++i)
		{
			table.template inc<T>(name);
			std::printf("inc<T>(\"%s\"): %d\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template dec<T>(name);
			std::printf("dec<T>(\"%s\"): %d\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template add<T>(name, 5);
			std::printf("add<T>(\"%s\", 5): %d\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template sub<T>(name, 5);
			std::printf("sub<T>(\"%s\", 5): %d\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template mul<T>(name, 2);
			std::printf("mul<T>(\"%s\", 2): %d\n", name, table.template load<T>(name));
		}
		table.template div<T>(name, 3); std::printf("div<T>(\"%s\", 3): %d\n", name, table.template load<T>(name));
		table.template mod<T>(name, 4); std::printf("mod<T>(\"%s\", 5): %d\n", name, table.template load<T>(name));
		table.template bitAnd<T>(name, 15); std::printf("bitAnd<T>(\"%s\", 15): %d\n", name, table.template load<T>(name));
		table.template bitOr<T>(name, 15); std::printf("bitOr<T>(\"%s\", 15): %d\n", name, table.template load<T>(name));
		table.template bitXor<T>(name, 15); std::printf("bitXor<T>(\"%s\", 15): %d\n", name, table.template load<T>(name));
		table.template bitNot<T>(name); std::printf("bitNot<T>(\"%s\"): %d\n", name, table.template load<T>(name));
		table.template lShift<T>(name, 2); std::printf("lShift<T>(\"%s\", 2): %d\n", name, table.template load<T>(name));
		table.template rShift<T>(name, 2); std::printf("rShift<T>(\"%s\", 2): %d\n", name, table.template load<T>(name));
		table.template bitOn<T>(name, 15); std::printf("bitOn<T>(\"%s\", 15): %d\n", name, table.template load<T>(name));
		table.template bitOff<T>(name, 255); std::printf("bitOff<T>(\"%s\", 255): %d\n", name, table.template load<T>(name));
	}
	std::printf("eq<T>(\"%s\", 15): %s\n", name, toStr(table.template eq<T>(name, 15)));
	std::printf("ne<T>(\"%s\", 15): %s\n", name, toStr(table.template ne<T>(name, 15)));
	std::printf("gt<T>(\"%s\", 15): %s\n", name, toStr(table.template gt<T>(name, 15)));
	std::printf("ge<T>(\"%s\", 15): %s\n", name, toStr(table.template ge<T>(name, 15)));
	std::printf("lt<T>(\"%s\", 15): %s\n", name, toStr(table.template lt<T>(name, 15)));
	std::printf("le<T>(\"%s\", 15): %s\n", name, toStr(table.template le<T>(name, 15)));
	std::printf("inOn<T>(\"%s\", 15): %s\n", name, toStr(table.template isOn<T>(name, 15)));
	std::printf("inOff<T>(\"%s\", 15): %s\n", name, toStr(table.template isOff<T>(name, 15)));
	std::printf("logicalAnd<T>(\"%s\", 15): %s\n", name, toStr(table.template logicalAnd<T>(name, 15)));
	std::printf("logicalOr<T>(\"%s\", 15): %s\n", name, toStr(table.template logicalOr<T>(name, 15)));
	std::printf("isTrue<T>(\"%s\"): %s\n", name, toStr(table.template isTrue<T>(name)));
	std::printf("isFalse<T>(\"%s\"): %s\n", name, toStr(table.template isFalse<T>(name)));
}
template<class TABLE_TYPE, typename T>
void testCommon2(named_ref::table<TABLE_TYPE>& table, const char* name)
{
	std::printf("\n");
	std::printf("--- testCommon2<%s>(\"%s\") ---\n", typeid(T).name(), name);
	std::printf("isRegistered<T>(\"%s\"): %s\n", name, toStr(table.isRegistered(name)));
	std::printf("isReadOnly<T>(\"%s\"): %s\n", name, toStr(table.template isReadOnly<T>(name)));
	std::printf("isReadable<T>(\"%s\"): %s\n", name, toStr(table.template isReadable<T>(name)));
	std::printf("isWritable<T>(\"%s\"): %s\n", name, toStr(table.template isWritable<T>(name)));
	std::printf("isWritableRanged<T>(\"%s\"): %s\n", name, toStr(table.template isWritableRanged<T>(name)));
	std::printf("isWritableWraparound<T>(\"%s\"): %s\n", name, toStr(table.template isWritableWraparound<T>(name)));
	std::printf("isWritableSaturation<T>(\"%s\"): %s\n", name, toStr(table.template isWritableSaturation<T>(name)));
}
template<class TABLE_TYPE, typename T>
void testCommon3(named_ref::table<TABLE_TYPE>& table, const char* name)
{
	std::printf("\n");
	std::printf("--- testCommon3<%s>(\"%s\") ---\n", typeid(T).name(), name);
	std::printf("isRegistered<T>(\"%s\"): %s\n", name, toStr(table.isRegistered(name)));
	std::printf("isReadOnly<T>(\"%s\"): %s\n", name, toStr(table.template isReadOnly<T>(name)));
	std::printf("isReadable<T>(\"%s\"): %s\n", name, toStr(table.template isReadable<T>(name)));
	std::printf("isWritable<T>(\"%s\"): %s\n", name, toStr(table.template isWritable<T>(name)));
	std::printf("isWritableRanged<T>(\"%s\"): %s\n", name, toStr(table.template isWritableRanged<T>(name)));
	std::printf("isWritableWraparound<T>(\"%s\"): %s\n", name, toStr(table.template isWritableWraparound<T>(name)));
	std::printf("isWritableSaturation<T>(\"%s\"): %s\n", name, toStr(table.template isWritableSaturation<T>(name)));
	if (table.template isWritableRanged<T>(name))
	{
		std::printf("max<T>(\"%s\"): %f\n", name, table.template max<T>(name));
		std::printf("min<T>(\"%s\"): %f\n", name, table.template min<T>(name));
	}
	std::printf("load<T>(\"%s\"): %f\n", name, table.template load<T>(name));
	std::printf("cref<T>(\"%s\"): %f\n", name, *table.template cref<T>(name));
	if (table.template isWritable<T>(name))
	{
		std::printf("ref<T>(\"%s\"): %f\n", name, *table.template ref<T>(name));
		*table.template ref<T>(name) = 1.f; std::printf("ref<T>(\"%s\")=1.f: %f\n", name, table.template load<T>(name));
		table.template store<T>(name, 2.f); std::printf("store<T>(\"%s\", 1.f): %f\n", name, table.template load<T>(name));
		for (int i = 0; i < 10; ++i)
		{
			table.template inc<T>(name);
			std::printf("inc<T>(\"%s\"): %f\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template dec<T>(name);
			std::printf("dec<T>(\"%s\"): %f\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template add<T>(name, 5.f);
			std::printf("add<T>(\"%s\", 5.f): %f\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template sub<T>(name, 5.f);
			std::printf("sub<T>(\"%s\", 5.f): %f\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template mul<T>(name, 2.f);
			std::printf("mul<T>(\"%s\", 2.f): %f\n", name, table.template load<T>(name));
		}
		table.template div<T>(name, 3.f); std::printf("div<T>(\"%s\", 3.f): %f\n", name, table.template load<T>(name));
		//table.template mod<T>(name, 5.f); std::printf("mod<T>(\"%s\", 5.f): %f\n", name, table.template load<T>(name));
		//table.template bitAnd<T>(name, 15.f); std::printf("bitAnd<T>(\"%s\", 15.f): %f\n", name, table.template load<T>(name));
		//table.template bitOr<T>(name, 15.f); std::printf("bitOr<T>(\"%s\", 15.f): %f\n", name, table.template load<T>(name));
		//table.template bitXor<T>(name, 15.f); std::printf("bitXor<T>(\"%s\", 15.f): %f\n", name, table.template load<T>(name));
		//table.template bitNot<T>(name); std::printf("bitNot<T>(\"%s\"): %f\n", name, table.template load<T>(name));
		//table.template lShift<T>(name, 2.f); std::printf("lShift<T>(\"%s\", 2.f): %f\n", name, table.template load<T>(name));
		//table.template rShift<T>(name, 2.f); std::printf("rShift<T>(\"%s\", 2.f): %f\n", name, table.template load<T>(name));
		//table.template bitOn<T>(name, 15.f); std::printf("bitOn<T>(\"%s\", 15.f): %f\n", name, table.template load<T>(name));
		//table.template bitOff<T>(name, 255.f); std::printf("bitOff<T>(\"%s\", 255.f): %f\n", name, table.template load<T>(name));
	}
	std::printf("eq<T>(\"%s\", 15.f): %s\n", name, toStr(table.template eq<T>(name, 15.f)));
	std::printf("ne<T>(\"%s\", 15.f): %s\n", name, toStr(table.template ne<T>(name, 15.f)));
	std::printf("gt<T>(\"%s\", 15.f): %s\n", name, toStr(table.template gt<T>(name, 15.f)));
	std::printf("ge<T>(\"%s\", 15.f): %s\n", name, toStr(table.template ge<T>(name, 15.f)));
	std::printf("lt<T>(\"%s\", 15.f): %s\n", name, toStr(table.template lt<T>(name, 15.f)));
	std::printf("le<T>(\"%s\", 15.f): %s\n", name, toStr(table.template le<T>(name, 15.f)));
	//std::printf("inOn<T>(\"%s\", 15.f): %s\n", name, toStr(table.template isOn<T>(name, 15.f)));
	//std::printf("inOff<T>(\"%s\", 15.f): %s\n", name, toStr(table.template isOff<T>(name, 15.f)));
	std::printf("logicalAnd<T>(\"%s\", 15.f): %s\n", name, toStr(table.template logicalAnd<T>(name, 15.f)));
	std::printf("logicalOr<T>(\"%s\", 15.f): %s\n", name, toStr(table.template logicalOr<T>(name, 15.f)));
	std::printf("isTrue<T>(\"%s\"): %s\n", name, toStr(table.template isTrue<T>(name)));
	std::printf("isFalse<T>(\"%s\"): %s\n", name, toStr(table.template isFalse<T>(name)));
}
template<class TABLE_TYPE, typename T>
void testCommon4(named_ref::table<TABLE_TYPE>& table, const char* name)
{
	std::printf("\n");
	std::printf("--- testCommon4<%s>(\"%s\") ---\n", typeid(T).name(), name);
	std::printf("isRegistered<T>(\"%s\"): %s\n", name, toStr(table.isRegistered(name)));
	std::printf("isReadOnly<T>(\"%s\"): %s\n", name, toStr(table.template isReadOnly<T>(name)));
	std::printf("isReadable<T>(\"%s\"): %s\n", name, toStr(table.template isReadable<T>(name)));
	std::printf("isWritable<T>(\"%s\"): %s\n", name, toStr(table.template isWritable<T>(name)));
	std::printf("isWritableRanged<T>(\"%s\"): %s\n", name, toStr(table.template isWritableRanged<T>(name)));
	std::printf("isWritableWraparound<T>(\"%s\"): %s\n", name, toStr(table.template isWritableWraparound<T>(name)));
	std::printf("isWritableSaturation<T>(\"%s\"): %s\n", name, toStr(table.template isWritableSaturation<T>(name)));
	if (table.template isWritableRanged<T>(name))
	{
		std::printf("max<T>(\"%s\"): %lf\n", name, table.template max<T>(name));
		std::printf("min<T>(\"%s\"): %lf\n", name, table.template min<T>(name));
	}
	std::printf("load<T>(\"%s\"): %lf\n", name, table.template load<T>(name));
	std::printf("cref<T>(\"%s\"): %lf\n", name, *table.template cref<T>(name));
	if (table.template isWritable<T>(name))
	{
		std::printf("ref<T>(\"%s\"): %lf\n", name, *table.template ref<T>(name));
		*table.template ref<T>(name) = 1.; std::printf("ref<T>(\"%s\")=1.: %lf\n", name, table.template load<T>(name));
		table.template store<T>(name, 2.); std::printf("store<T>(\"%s\", 1.): %lf\n", name, table.template load<T>(name));
		for (int i = 0; i < 10; ++i)
		{
			table.template inc<T>(name);
			std::printf("inc<T>(\"%s\"): %lf\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template dec<T>(name);
			std::printf("dec<T>(\"%s\"): %lf\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template add<T>(name, 5.);
			std::printf("add<T>(\"%s\", 5.): %lf\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template sub<T>(name, 5.);
			std::printf("sub<T>(\"%s\", 5.): %lf\n", name, table.template load<T>(name));
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template mul<T>(name, 2.);
			std::printf("mul<T>(\"%s\", 2.): %lf\n", name, table.template load<T>(name));
		}
		table.template div<T>(name, 3.); std::printf("div<T>(\"%s\", 3.): %lf\n", name, table.template load<T>(name));
		//table.template mod<T>(name, 5.); std::printf("mod<T>(\"%s\", 5.): %lf\n", name, table.template load<T>(name));
		//table.template bitAnd<T>(name, 15.); std::printf("bitAnd<T>(\"%s\", 15.): %lf\n", name, table.template load<T>(name));
		//table.template bitOr<T>(name, 15.); std::printf("bitOr<T>(\"%s\", 15.): %lf\n", name, table.template load<T>(name));
		//table.template bitXor<T>(name, 15.); std::printf("bitXor<T>(\"%s\", 15.): %lf\n", name, table.template load<T>(name));
		//table.template bitNot<T>(name); std::printf("bitNot<T>(\"%s\"): %lf\n", name, table.template load<T>(name));
		//table.template lShift<T>(name, 2.); std::printf("lShift<T>(\"%s\", 2.): %lf\n", name, table.template load<T>(name));
		//table.template rShift<T>(name, 2.); std::printf("rShift<T>(\"%s\", 2.): %lf\n", name, table.template load<T>(name));
		//table.template bitOn<T>(name, 15.); std::printf("bitOn<T>(\"%s\", 15.): %lf\n", name, table.template load<T>(name));
		//table.template bitOff<T>(name, 255.); std::printf("bitOff<T>(\"%s\", 255.): %lf\n", name, table.template load<T>(name));
	}
	std::printf("eq<T>(\"%s\", 15.): %s\n", name, toStr(table.template eq<T>(name, 15.)));
	std::printf("ne<T>(\"%s\", 15.): %s\n", name, toStr(table.template ne<T>(name, 15.)));
	std::printf("gt<T>(\"%s\", 15.): %s\n", name, toStr(table.template gt<T>(name, 15.)));
	std::printf("ge<T>(\"%s\", 15.): %s\n", name, toStr(table.template ge<T>(name, 15.)));
	std::printf("lt<T>(\"%s\", 15.): %s\n", name, toStr(table.template lt<T>(name, 15.)));
	std::printf("le<T>(\"%s\", 15.): %s\n", name, toStr(table.template le<T>(name, 15.)));
	//std::printf("inOn<T>(\"%s\", 15.): %s\n", name, toStr(table.template isOn<T>(name, 15.)));
	//std::printf("inOff<T>(\"%s\", 15.): %s\n", name, toStr(table.template isOff<T>(name, 15.)));
	std::printf("logicalAnd<T>(\"%s\", 15.): %s\n", name, toStr(table.template logicalAnd<T>(name, 15.)));
	std::printf("logicalOr<T>(\"%s\", 15.): %s\n", name, toStr(table.template logicalOr<T>(name, 15.)));
	std::printf("isTrue<T>(\"%s\"): %s\n", name, toStr(table.template isTrue<T>(name)));
	std::printf("isFalse<T>(\"%s\"): %s\n", name, toStr(table.template isFalse<T>(name)));
}
template<class TABLE_TYPE, typename T>
void testCommon5(named_ref::table<TABLE_TYPE>& table, const char* name)
{
	std::printf("\n");
	std::printf("--- testCommon5<%s>(\"%s\") ---\n", typeid(T).name(), name);
	std::printf("isRegistered<T>(\"%s\"): %s\n", name, toStr(table.isRegistered(name)));
	std::printf("isReadOnly<T>(\"%s\"): %s\n", name, toStr(table.template isReadOnly<T>(name)));
	std::printf("isReadable<T>(\"%s\"): %s\n", name, toStr(table.template isReadable<T>(name)));
	std::printf("isWritable<T>(\"%s\"): %s\n", name, toStr(table.template isWritable<T>(name)));
	std::printf("isWritableRanged<T>(\"%s\"): %s\n", name, toStr(table.template isWritableRanged<T>(name)));
	std::printf("isWritableWraparound<T>(\"%s\"): %s\n", name, toStr(table.template isWritableWraparound<T>(name)));
	std::printf("isWritableSaturation<T>(\"%s\"): %s\n", name, toStr(table.template isWritableSaturation<T>(name)));
	if (table.template isWritableRanged<T>(name))
	{
		std::printf("max<T>(\"%s\"): %d\n", name, table.template max<T>(name));
		std::printf("min<T>(\"%s\"): %d\n", name, table.template min<T>(name));
	}
	std::printf("load<T>(\"%s\"): %s\n", name, toStr(table.template load<T>(name)));
	std::printf("cref<T>(\"%s\"): %s\n", name, toStr(*table.template cref<T>(name)));
	if (table.template isWritable<T>(name))
	{
		std::printf("ref<T>(\"%s\"): %s\n", name, toStr(*table.template ref<T>(name)));
		*table.template ref<T>(name) = false; std::printf("ref<T>(\"%s\")=false: %s\n", name, toStr(table.template load<T>(name)));
		table.template store<T>(name, true); std::printf("store<T>(\"%s\", true): %s\n", name, toStr(table.template load<T>(name)));
	}
	std::printf("eq<T>(\"%s\", true): %s\n", name, toStr(table.template eq<T>(name, true)));
	std::printf("ne<T>(\"%s\", true): %s\n", name, toStr(table.template ne<T>(name, true)));
	std::printf("gt<T>(\"%s\", true): %s\n", name, toStr(table.template gt<T>(name, true)));
	std::printf("ge<T>(\"%s\", true): %s\n", name, toStr(table.template ge<T>(name, true)));
	std::printf("lt<T>(\"%s\", true): %s\n", name, toStr(table.template lt<T>(name, true)));
	std::printf("le<T>(\"%s\", true): %s\n", name, toStr(table.template le<T>(name, true)));
	std::printf("logicalAnd<T>(\"%s\", true): %s\n", name, toStr(table.template logicalAnd<T>(name, true)));
	std::printf("logicalOr<T>(\"%s\", true): %s\n", name, toStr(table.template logicalOr<T>(name, true)));
	std::printf("isTrue<T>(\"%s\"): %s\n", name, toStr(table.template isTrue<T>(name)));
	std::printf("isFalse<T>(\"%s\"): %s\n", name, toStr(table.template isFalse<T>(name)));
}
//共通処理
template<class TABLE_TYPE, typename T>
void testCommon6(named_ref::table<TABLE_TYPE>& table, const char* name)
{
	std::printf("\n");
	std::printf("--- testCommon1<%s>(\"%s\") ---\n", typeid(T).name(), name);
	std::printf("isRegistered<T>(\"%s\"): %s\n", name, toStr(table.isRegistered(name)));
	std::printf("isReadOnly<T>(\"%s\"): %s\n", name, toStr(table.template isReadOnly<T>(name)));
	std::printf("isReadable<T>(\"%s\"): %s\n", name, toStr(table.template isReadable<T>(name)));
	std::printf("isWritable<T>(\"%s\"): %s\n", name, toStr(table.template isWritable<T>(name)));
	std::printf("isWritableRanged<T>(\"%s\"): %s\n", name, toStr(table.template isWritableRanged<T>(name)));
	std::printf("isWritableWraparound<T>(\"%s\"): %s\n", name, toStr(table.template isWritableWraparound<T>(name)));
	std::printf("isWritableSaturation<T>(\"%s\"): %s\n", name, toStr(table.template isWritableSaturation<T>(name)));
	if (table.template isWritableRanged<T>(name))
	{
		std::printf("max<T>(\"%s\"): %llu\n", name, table.template max<T>(name).m_lo);
		std::printf("min<T>(\"%s\"): %llu\n", name, table.template min<T>(name).m_lo);
	}
	std::printf("load<T>(\"%s\"): %llu\n", name, table.template load<T>(name).m_lo);
	std::printf("cref<T>(\"%s\"): %llu\n", name, table.template cref<T>(name)->m_lo);
	if (table.template isWritable<T>(name))
	{
		std::printf("ref<T>(\"%s\"): %llu\n", name, table.template ref<T>(name)->m_lo);
		*table.template ref<T>(name) = 1; std::printf("ref<T>(\"%s\")=1: %llu\n", name, table.template load<T>(name).m_lo);
		table.template store<T>(name, 2); std::printf("store<T>(\"%s\", 1): %llu\n", name, table.template load<T>(name).m_lo);
		for (int i = 0; i < 10; ++i)
		{
			table.template inc<T>(name);
			std::printf("inc<T>(\"%s\"): %llu\n", name, table.template load<T>(name).m_lo);
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template dec<T>(name);
			std::printf("dec<T>(\"%s\"): %llu\n", name, table.template load<T>(name).m_lo);
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template add<T>(name, 5);
			std::printf("add<T>(\"%s\", 5): %llu\n", name, table.template load<T>(name).m_lo);
		}
		for (int i = 0; i < 10; ++i)
		{
			table.template sub<T>(name, 5);
			std::printf("sub<T>(\"%s\", 5): %llu\n", name, table.template load<T>(name).m_lo);
		}
	}
	std::printf("eq<T>(\"%s\", 15): %s\n", name, toStr(table.template eq<T>(name, 15)));
	std::printf("ne<T>(\"%s\", 15): %s\n", name, toStr(table.template ne<T>(name, 15)));
	std::printf("gt<T>(\"%s\", 15): %s\n", name, toStr(table.template gt<T>(name, 15)));
	std::printf("ge<T>(\"%s\", 15): %s\n", name, toStr(table.template ge<T>(name, 15)));
	std::printf("lt<T>(\"%s\", 15): %s\n", name, toStr(table.template lt<T>(name, 15)));
	std::printf("le<T>(\"%s\", 15): %s\n", name, toStr(table.template le<T>(name, 15)));
	std::printf("logicalAnd<T>(\"%s\", 15): %s\n", name, toStr(table.template logicalAnd<T>(name, 15)));
	std::printf("logicalOr<T>(\"%s\", 15): %s\n", name, toStr(table.template logicalOr<T>(name, 15)));
	std::printf("isTrue<T>(\"%s\"): %s\n", name, toStr(table.template isTrue<T>(name)));
	std::printf("isFalse<T>(\"%s\"): %s\n", name, toStr(table.template isFalse<T>(name)));
}

//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref()
{
	std::printf("----- Test for namedRef -----\n");
	
	//名前付きデータ参照
	refTable ref_tbl;

	//名前をCRC値取得
	constexpr crc32_t name_valA = calcStaticCRC32("valA");
	constexpr crc32_t name_valB = calcStaticCRC32("valB");
	constexpr crc32_t name_dataC = calcStaticCRC32("dataC");

	//読み書き可能データとして登録されたデータにアクセス

	//値の取得①
	if (ref_tbl.isReadable<int>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.load<int>(name_valA);//値の取得
		std::printf("load<int>(): valA=%d\n", val);
	}
	//値の取得②：名前で直接指定
	if (ref_tbl.isReadable<int>("valA"))//※名前を直接指定してもよい（事前計算している方が当然速い）
	{
		auto val = ref_tbl.load<int>("valA");//※名前を直接指定してもよい
		std::printf("load<int>(): valA=%d\n", val);
	}
	//値の変更
	if (ref_tbl.isWritable<int>(name_valA))//書き込み可能判定（必要に応じて使用）
	{
		ref_tbl.store<int>(name_valA, 1);//値の変更 ※何も返さない
		auto val = ref_tbl.load<int>(name_valA);//値の取得
		std::printf("store<int>(): valA=%d\n", val);
	}
	//値の変更①：読み書き可能参照
	if (ref_tbl.isWritable<int>(name_valA))//書き込み可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.ref<int>(name_valA);//参照を取得 ※戻り値が有効なスコープの間、排他ロックを取得することに注意
		*val = 2;//値を変更
		std::printf("ref<int>(): valA=%d\n", *val);
	}
	//値の変更②：読み取り専用参照
	if (ref_tbl.isReadable<int>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.cref<int>(name_valA);
		//*val = 3;//変更不可
		std::printf("cref<int>(): valA=%d\n", *val);
	}
	//誤ったデータ型でアクセス①：読み込み
	if (ref_tbl.isReadable<short>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.load<short>(name_valA);//値の取得
		std::printf("load<int>(): valA=%d\n", val);
	}
	//誤ったデータ型でアクセス②：書き込み
	if (ref_tbl.isWritable<short>(name_valA))//読み取り可能判定（必要に応じて使用）
	{
		ref_tbl.store<int>(name_valA, 4);//値の変更 ※何も返さない
		auto val = ref_tbl.load<int>(name_valA);//値の取得
		std::printf("load<int>(): valA=%d\n", val);
	}

	//読み取り専用データとして登録されたデータにアクセス

	//値の取得①
	if (ref_tbl.isReadable<int>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.load<int>(name_valB);//値の取得
		std::printf("load<int>(): valB=%d\n", val);
	}
	//値の取得②：名前で直接指定
	if (ref_tbl.isReadable<int>("valB"))//※名前を直接指定してもよい（事前計算している方が当然速い）
	{
		auto val = ref_tbl.load<int>("valB");//※名前を直接指定してもよい
		std::printf("load<int>(): valB=%d\n", val);
	}
	//値の変更
	if (ref_tbl.isWritable<int>(name_valB))//書き込み可能判定（必要に応じて使用）
	{
		ref_tbl.store<int>(name_valB, 1);//値の変更 ※何も返さない
		auto val = ref_tbl.load<int>(name_valB);//値の取得
		std::printf("store<int>(): valB=%d\n", val);
	}
	//値の変更①：読み書き可能参照
	if (ref_tbl.isWritable<int>(name_valB))//書き込み可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.ref<int>(name_valB);//参照を取得 ※戻り値 val が有効なスコープの間、排他ロックすることに注意
		*val = 2;//値を変更
		std::printf("ref<int>(): valB=%d\n", *val);
	}
	//値の変更②：読み取り専用参照
	if (ref_tbl.isReadable<int>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.cref<int>(name_valB);//参照を取得 ※戻り値 val が有効なスコープの間、共有ロックすることに注意
		//*val = 3;//変更不可
		std::printf("cref<int>(): valB=%d\n", *val);
	}
	//誤ったデータ型でアクセス①：読み込み
	if (ref_tbl.isReadable<short>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		auto val = ref_tbl.load<short>(name_valB);//値の取得
		std::printf("load<int>(): valB=%d\n", val);
	}
	//誤ったデータ型でアクセス②：書き込み
	if (ref_tbl.isWritable<short>(name_valB))//読み取り可能判定（必要に応じて使用）
	{
		ref_tbl.store<int>(name_valB, 4);//値の変更 ※何も返さない
		auto val = ref_tbl.load<int>(name_valB);//値の取得
		std::printf("load<int>(): valB=%d\n", val);
	}

	//構造体にアクセス

	//値の変更①：読み書き可能参照
	if (ref_tbl.isWritable<data_t>(name_dataC))//書き込み可能判定（必要に応じて使用）
	{
		auto data_c = ref_tbl.ref<data_t>(name_dataC);//参照を取得 ※戻り値 val が有効なスコープの間、排他ロックすることに注意
		data_c->m_memberA = 1;
		data_c->m_memberB[0] = 2;
		data_c->m_memberB[1] = 3;
		(*data_c).m_memberC = 4.f;
		std::printf("ref<data_t>(): dataC={ %d, {%d, %d}, %.1f}\n", data_c->m_memberA, data_c->m_memberB[0], data_c->m_memberB[1], data_c->m_memberC);
	}
	//値の変更②：読み取り専用参照
	if (ref_tbl.isReadable<data_t>(name_dataC))//読み取り可能判定（必要に応じて使用）
	{
		auto data_c = ref_tbl.cref<data_t>(name_dataC);//参照を取得 ※戻り値 val が有効なスコープの間、共有ロックすることに注意
		std::printf("ref<data_t>(): dataC={ %d, {%d, %d}, %.1f}\n", data_c->m_memberA, data_c->m_memberB[0], data_c->m_memberB[1], data_c->m_memberC);
	}
	
	//演算
	testCommon1<refTable_type, int>(ref_tbl, "valA");
	testCommon1<refTable_type, int>(ref_tbl, "valA'");
	testCommon1<refTable_type, int>(ref_tbl, "valB");
	testCommon2<refTable_type, data_t>(ref_tbl, "dataC");
	testCommon3<refTable_type, float>(ref_tbl, "valD");
	testCommon4<refTable_type, double>(ref_tbl, "valE");
	testCommon1<refTable_type, int>(ref_tbl, "valF");
	testCommon1<refTable_type, unsigned short>(ref_tbl, "valG");
	testCommon3<refTable_type, float>(ref_tbl, "valH");
	testCommon3<refTable_type, float>(ref_tbl, "valI");
	testCommon4<refTable_type, double>(ref_tbl, "valJ");
	testCommon4<refTable_type, double>(ref_tbl, "valK");
	testCommon5<refTable_type, bool>(ref_tbl, "valL");
	testCommon6<refTable_type, uint128_t>(ref_tbl, "valM");
	testCommon2<refTable_type, int>(ref_tbl, "unregistered data");
	testCommon2<refTable_type, char>(ref_tbl, "valA");

	//名前付き関数呼び出し
	funcTable func_tbl;
	objType obj;
	obj.m_mem = 123;
	const objType const_obj(obj);

	if (func_tbl.isRegistered("funcGroupA", "funcA") && func_tbl.isWritable<bool>("funcGroupA", "funcA"))
	{
		const bool ret = func_tbl.func<bool>("funcGroupA", "funcA", 1, 2); std::printf("funcA:ret=%d\n", ret);
	}
	if (func_tbl.isRegistered("funcGroupB", "lambdaA") && func_tbl.isWritable<bool>("funcGroupB", "lambdaA"))
	{
		const bool ret = func_tbl.func<bool>("funcGroupB", "lambdaA", 3, 4); std::printf("lambdaA:ret=%d\n", ret);
	}
	if (func_tbl.isRegistered("funcGroupC", "functorA") && func_tbl.isWritable<bool>("funcGroupC", "functorA"))
	{
		const bool ret = func_tbl.func<bool>("funcGroupC", "functorA", 5, 6); std::printf("functorA:ret=%d\n", ret);
	}
	if (func_tbl.isRegistered("funcGroupD", "memberA") && func_tbl.isWritableObj<bool, objType>("funcGroupD", "memberA"))
	{
		const bool ret = func_tbl.func<bool>(obj, "funcGroupD", "memberA", 7, 8); std::printf("memberA:ret=%d\n", ret);
	}

	if (func_tbl.isRegistered("funcGroupA", "funcA const") && func_tbl.isConstType<bool>("funcGroupA", "funcA const"))
	{
		const bool ret = func_tbl.func<bool>("funcGroupA", "funcA const", 1, 2); std::printf("funcA:ret=%d\n", ret);
	}
	if (func_tbl.isRegistered("funcGroupB", "lambdaA const") && func_tbl.isConstType<bool>("funcGroupB", "lambdaA const"))
	{
		const bool ret = func_tbl.func<bool>("funcGroupB", "lambdaA const", 3, 4); std::printf("lambdaA:ret=%d\n", ret);
	}
	if (func_tbl.isRegistered("funcGroupC", "functorA const") && func_tbl.isConstType<bool>("funcGroupC", "functorA const"))
	{
		const bool ret = func_tbl.func<bool>("funcGroupC", "functorA const", 5, 6); std::printf("functorA:ret=%d\n", ret);
	}
	if (func_tbl.isRegistered("funcGroupD", "memberA const") && func_tbl.isConstObj<bool, objType>("funcGroupD", "memberA const"))
	{
		const bool ret = func_tbl.func<bool>(const_obj, "funcGroupD", "memberA const", 7, 8); std::printf("memberA:ret=%d\n", ret);
	}

	if (func_tbl.isRegistered("funcGroupA", "funcB") && func_tbl.isWritable<void>("funcGroupA", "funcB"))
	{
		func_tbl.proc("funcGroupA", "funcB");
	}
	if (func_tbl.isRegistered("funcGroupB", "lambdaB") && func_tbl.isWritable<void>("funcGroupB", "lambdaB"))
	{
		func_tbl.proc("funcGroupB", "lambdaB");
	}
	if (func_tbl.isRegistered("funcGroupC", "functorB") && func_tbl.isWritable<void>("funcGroupC", "functorB"))
	{
		func_tbl.proc("funcGroupC", "functorB");
	}
	if (func_tbl.isRegistered("funcGroupD", "memberB") && func_tbl.isWritableObj<void, objType>("funcGroupD", "memberB"))
	{
		func_tbl.proc(obj, "funcGroupD", "memberB");
	}

	if (func_tbl.isRegistered("funcGroupA", "funcB const") && func_tbl.isConstType<void>("funcGroupA", "funcB const"))
	{
		func_tbl.proc("funcGroupA", "funcB const");
	}
	if (func_tbl.isRegistered("funcGroupB", "lambdaB const") && func_tbl.isConstType<void>("funcGroupB", "lambdaB const"))
	{
		func_tbl.proc("funcGroupB", "lambdaB const");
	}
	if (func_tbl.isRegistered("funcGroupC", "functorB const") && func_tbl.isConstType<void>("funcGroupC", "functorB const"))
	{
		func_tbl.proc("funcGroupC", "functorB const");
	}
	if (func_tbl.isRegistered("funcGroupD", "memberB const") && func_tbl.isConstObj<void, objType>("funcGroupD", "memberB const"))
	{
		func_tbl.proc(const_obj, "funcGroupD", "memberB const");
	}

	//列挙
	std::printf("----------------------------------------\n");
	std::printf("Registered named-ref items:(num=%d)\n", ref_tbl.size());
	for (const auto& info : ref_tbl)
	{
		std::printf("  ref: name(crc)=0x%08x, type=%s, access_type=%s\n", info.m_nameCrc, info.m_typeInfo ? info.m_typeInfo->name() : "(unknown)", info.m_accessType == refTable::READ_ONLY ? "ReadOnly" : info.m_accessType == refTable::WRITABLE ? "Writable" : info.m_accessType == refTable::WRITABLE_WRAPAROUND ? "Writable(Wrap-around)" : info.m_accessType == refTable::WRITABLE_SATURATION ? "Writable(Saturation)" : "(Unknown)");
	}
	std::printf("----------------------------------------\n");
	std::printf("Registered named-func items:(num=%d)\n", func_tbl.size());
	for (const auto& group_info : func_tbl)
	{
		std::printf("  group: group-name(crc)=0x%08x\n", group_info.m_groupNameCrc);
		for (const auto& func_info : group_info.m_funcList)
		{
			std::printf("    func: name(crc)=0x%08x, funcType=%s, ret=%s, obj=%s\n", func_info.m_key.m_nameCrc, func_info.m_funcType == funcTable::CONST_FUNCTION ? "Const" : "Writable", func_info.m_retTypeInfo ? func_info.m_retTypeInfo->name() : "(null)", func_info.m_objTypeInfo ? func_info.m_objTypeInfo->name() : "(none)");
		}
	}

	std::printf("- end -\n");
}

// End of file
