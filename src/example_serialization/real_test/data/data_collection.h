#pragma once
#ifndef GASHA_INCLUDED_DATA_COLLECTION_H
#define GASHA_INCLUDED_DATA_COLLECTION_H

//--------------------------------------------------------------------------------
// data_collection.h
// データコレクション【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include<gasha/pool_allocator.h>//プールアロケータ
#include<gasha/hash_table.h>//ハッシュテーブル
#include<gasha/dynamic_array.h>//動的配列
#include<gasha/crc32.h>//CRC32計算

#include <cstddef>//std::szie_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//データコレクションクラス
//※マルチスレッド対応なし
template<class T, std::size_t _POOL_SIZE>
class dataCollection
{
public:
	//定数
	static const std::size_t POOL_SIZE = _POOL_SIZE;//最大データ数
	static const std::size_t SEARCH_TABLE_SIZE = POOL_SIZE * 15 / 10;//検索テーブルサイズ
	static const std::size_t ORDER_TABLE_SIZE = POOL_SIZE;//整列テーブルサイズ
public:
	//型
	typedef T value_type;//値型
	typedef T* reference;//参照型
	typedef crc32_t key_type;//キー型
public:
	//コンテナ操作型
	struct searchTableOpe : public hash_table::baseOpe<searchTableOpe, reference, key_type>//検索テーブル操作型
	{
		inline static key_type getKey(const reference& value){ return value->key(); }
	};
	struct orderTableOpe : public dynamic_array::baseOpe<orderTableOpe, reference>{};//整列テーブル操作型
public:
	//コンテナ型
	typedef hash_table::container<searchTableOpe, SEARCH_TABLE_SIZE> searchTable;//検索テーブル型
	typedef dynamic_array::container<orderTableOpe> orderTable;//整列テーブル型
	typedef typename orderTable::iterator iterator;//イテレータ型
	typedef typename orderTable::const_iterator const_iterator;//イテレータ型
	typedef typename orderTable::reverse_iterator reverse_iterator;//リバースイテレータ型
	typedef typename orderTable::const_reverse_iterator const_reverse_iterator;//リバースイテレータ型
public:
	//イテレータ取得
	inline const iterator begin() const;//開始イテレータ
	inline const iterator end() const;//終了イテレータ
	inline iterator begin();//開始イテレータ
	inline iterator end();//終了イテレータ
	inline const_iterator cbegin() const;//開始イテレータ
	inline const_iterator cend() const;//終了イテレータ
	inline const reverse_iterator rbegin() const;//開始リバースイテレータ
	inline const reverse_iterator rend() const;//終了リバースイテレータ
	inline reverse_iterator rbegin();//開始リバースイテレータ
	inline reverse_iterator rend();//終了リバースイテレータ
	inline const_reverse_iterator crbegin() const;//開始リバースイテレータ
	inline const_reverse_iterator crend() const;//終了リバースイテレータ
public:
	//検索
	inline value_type* find(const crc32_t key);
	inline value_type* find(const char* key);
	//登録
	value_type* regist(const value_type& data);
	//破棄
	//※全データ破棄後に「整列」を行わないと、正しくデータにアクセスできないので注意
	void unregist(const crc32_t key);
	//整列
	void sort(const bool is_descendant = false);
public:
	//デフォルトコンストラクタ
	inline dataCollection();
	//デストラクタ
	inline ~dataCollection();
private:
	//フィールド
	poolAllocator_withType<value_type, POOL_SIZE> m_pool;//データ用のプール
	searchTable m_searchTable;//検索テーブル
	reference m_orderTableBuff[ORDER_TABLE_SIZE];//整列テーブル用バッファ
	orderTable m_orderTable;//整列テーブル
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/data_collection.inl"

#endif//GASHA_INCLUDED_DATA_COLLECTION_H

// End of file
