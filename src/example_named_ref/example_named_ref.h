#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_NAMED_REF_H
#define GASHA_INCLUDED_EXAMPLE_NAMED_REF_H

//--------------------------------------------------------------------------------
// exmaple_named_ref.h
// 名前付きデータ参照テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/named_ref.h>//名前付きデータ参照
#include <gasha/shared_spin_lock.h>//共有スピンロック

#include <cstddef>//std::size_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付きデータ参照定義

//名前付きデータ参照インスタンス識別用構造体
struct refTableWithLock_type{};//共有ロック付き参照テーブル
struct refTable_type{};//ロックなし参照テーブル

//名前付きデータ参照の型宣言
static const std::size_t REF_TABLE_WITH_LOCK_SIZE = 10;
static const std::size_t REF_TABLE_SIZE = 10;
using refTableWithLock = namedRef<refTableWithLock_type, REF_TABLE_WITH_LOCK_SIZE, sharedSpinLock>;
using refTable = namedRef<refTable_type, REF_TABLE_SIZE>;

//----------------------------------------
//名前付きデータ参照用構造体
struct data_t
{
	int m_memberA;
	char m_memberB[2];
	float m_memberC;
};

//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref();

#endif//GASHA_INCLUDED_EXAMPLE_NAMED_REF_H

// End of file
