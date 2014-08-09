//--------------------------------------------------------------------------------
// exmaple_named_ref_sub.cpp
// 名前付きデータ参照テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_named_ref.h"//名前付きデータ参照テスト

#include <gasha/singleton.h>//シングルトン

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付きデータ参照用構造体
int data_t::methodA(const int val)
{
	return m_memberA * val;
}

//----------------------------------------
//名前付き関数テスト
//※メンバー関数
bool objType::memberA(const int a, const int b)
{
	std::printf("objType::memberA() const: a=%d, b=%d, m_mem=%d\n", a, b, m_mem);
	return true;
}
void objType::memberB() const
{
	std::printf("objType::memberB() const: m_mem=%d\n", m_mem);
}
void objType::memberB_noconst()
{
	std::printf("objType::memberB_noconst() const: m_mem=%d\n", m_mem);
}

//----------------------------------------
//名前付きデータ参照の列挙
void enumNamedRef()
{
	refTable ref_tbl;
	std::printf("----------------------------------------\n");
	std::printf("Registered named-ref items:(num=%d)\n", static_cast<int>(ref_tbl.size()));
	for (const auto& info : ref_tbl)
	{
		std::printf("  ref: name(crc)=0x%08x, type=%s, access_type=%s\n", info.m_nameCrc, info.m_typeInfo ? info.m_typeInfo->name() : "(unknown)", info.m_accessType == refTable::READ_ONLY ? "ReadOnly" : info.m_accessType == refTable::WRITABLE ? "Writable" : info.m_accessType == refTable::WRITABLE_WRAPAROUND ? "Writable(Wrap-around)" : info.m_accessType == refTable::WRITABLE_SATURATION ? "Writable(Saturation)" : "(Unknown)");
	}
};

//----------------------------------------
//名前付き関数の列挙
void enumNamedFunc()
{
	funcTable func_tbl;
	std::printf("----------------------------------------\n");
	std::printf("Registered named-func items:(num=%d)\n", static_cast<int>(func_tbl.size()));
	for (const auto& group_info : func_tbl)
	{
		std::printf("  group: group-name(crc)=0x%08x\n", group_info.m_groupNameCrc);
		for (const auto& func_info : group_info.m_funcList)
		{
			std::printf("    func: name(crc)=0x%08x, funcType=%s, retType=%s, objType=%s\n", func_info.m_key.m_nameCrc, func_info.m_funcType == funcTable::CONST_FUNCTION ? "Const" : "Writable", func_info.m_retTypeInfo ? func_info.m_retTypeInfo->name() : "(null)", func_info.m_objTypeInfo ? func_info.m_objTypeInfo->name() : "(none)");
		}
	}
};

//----------------------------------------
//名前付き定数の列挙
void enumNamedValue()
{
	valueTable value_tbl;
	simpleSingleton<valueStrPool> value_str("enum");
	std::printf("----------------------------------------\n");
	std::printf("Registered named-value items:(num=%d)\n", static_cast<int>(value_tbl.size()));
	for (const auto& group_info : value_tbl)
	{
		std::printf("  group: group-name(crc)=0x%08x\n", group_info.m_groupNameCrc);
		for (const auto& value_info : group_info.m_valueList)
		{
			const char* name = (*value_str)(value_info.m_nameCrc);
			std::printf("    func: name=\"%s\"(0x%08x), type=%s\n", name, value_info.m_nameCrc, value_info.m_typeInfo ? value_info.m_typeInfo->name() : "(null)");
		}
	}
};

// End of file
