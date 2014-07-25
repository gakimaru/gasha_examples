#pragma once
#ifndef GASHA_INCLUDED_EXAMPLE_SINGLETON_H
#define GASHA_INCLUDED_EXAMPLE_SINGLETON_H

//--------------------------------------------------------------------------------
// exmaple_singleton.h
// シングルトンテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/singleton.h>//シングルトン
#include <gasha/singleton_debug.h>//シングルトンデバッグ用処理
#include <gasha/shared_spin_lock.h>//共有スピンロック

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//シングルトンテスト用設定とコンパイラスイッチ

//#define TEST_DATA_WATCH_CONSTRUCTOR//コンストラクタ／デストラクタの動作を確認する場合、このマクロを有効にする

//--------------------------------------------------------------------------------
//テストデータ

//----------------------------------------
//シングルトン対象クラス①
//※シングルトンに特化したクラス
//　- シングルトン用のロック型 lock_type とデバッグ型 debug_type を定義
//　- コンストラクタを private化してシングルトンを friend 化（シングルトンとしてしかクラスをインスタンス化できない）
//　- デフォルトコンストラクタを delete して、明示的なインスタンス化を必須に
class common_data1_t
{
public:
	//型
	typedef sharedSpinLock lock_type;//ロック型
	typedef singletonDebug<50, sharedSpinLock> debug_type;//シングルトンデバッグ型
	//フレンド宣言
	GASHA_SINGLETON_FRIEND_CLASS(common_data1_t);
public:
	//アクセッサ
	int val1() const { return m_val1; }
	int& val1(){ return m_val1; }
	int val2() const { return m_val2; }
	int& val2(){ return m_val2; }
public:
	//メソッド
	void printInfo();
private:
	//コンストラクタ
	common_data1_t(const int val1, const int val2);
	common_data1_t() = delete;
	//デストラクタ
	~common_data1_t();
private:
	int m_val1;//値1
	int m_val2;//値2
};

//----------------------------------------
//シングルトン対象クラス②
//※シングルトンに特化しないクラス
//　- シングルトン用のロック型 lock_type とデバッグ型 debug_type を定義しない
//　- コンストラクタは public
//　- デフォルトコンストラクタでシングルトンへの最初のアクセス時に自動インスタンス化
class common_data2_t
{
public:
	//アクセッサ
	int val1() const { return m_val1; }
	int& val1(){ return m_val1; }
	int val2() const { return m_val2; }
	int& val2(){ return m_val2; }
public:
	//メソッド
	void printInfo();
public:
	//コンストラクタ
	common_data2_t();
	//デストラクタ
	~common_data2_t();
private:
	int m_val1;//値1
	int m_val2;//値2
};

//----------------------------------------
//シングルトンテスト
void example_singleton();

#endif//GASHA_INCLUDED_EXAMPLE_SINGLETON_H

// End of file
