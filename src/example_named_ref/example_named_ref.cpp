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
	testCommon1<refTableWithLock_type, int>(refTbl, "valA");
	testCommon1<refTableWithLock_type, int>(refTbl, "valA'");
	testCommon1<refTableWithLock_type, int>(refTbl, "valB");
	testCommon2<refTableWithLock_type, data_t>(refTbl, "dataC");
	testCommon3<refTableWithLock_type, float>(refTbl, "valD");
	testCommon4<refTableWithLock_type, double>(refTbl, "valE");
	testCommon1<refTableWithLock_type, int>(refTbl, "valF");
	testCommon1<refTableWithLock_type, unsigned short>(refTbl, "valG");
	testCommon3<refTableWithLock_type, float>(refTbl, "valH");
	testCommon3<refTableWithLock_type, float>(refTbl, "valI");
	testCommon4<refTableWithLock_type, double>(refTbl, "valJ");
	testCommon4<refTableWithLock_type, double>(refTbl, "valK");
	testCommon5<refTableWithLock_type, bool>(refTbl, "valL");
	testCommon6<refTableWithLock_type, uint128_t>(refTbl, "valM");
	testCommon2<refTableWithLock_type, int>(refTbl, "unregistered data");
	testCommon2<refTableWithLock_type, char>(refTbl, "valA");

	//列挙
	std::printf("----------------------------------------\n");
	std::printf("Registered data items:(num=%d)\n", refTbl.size());
	for (const auto& info : refTbl)
	{
		std::printf("data: name(crc)=0x%08x, type=%s, access_type=%s\n", info.m_nameCrc, info.m_typeInfo ? info.m_typeInfo->name() : "(unknown)", info.m_accessType == refTableWithLock::READ_ONLY ? "ReadOnly" : info.m_accessType == refTableWithLock::WRITABLE ? "Writable" : info.m_accessType == refTableWithLock::WRITABLE_WRAPAROUND ? "Writable(Wrap-around)" : info.m_accessType == refTableWithLock::WRITABLE_SATURATION ? "Writable(Saturation)" : "(Unknown)");
	}

	std::printf("- end -\n");
}

// End of file
