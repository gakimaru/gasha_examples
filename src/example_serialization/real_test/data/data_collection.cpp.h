#pragma once
#ifndef GASHA_INCLUDED_DATA_COLLECTION_CPP_H
#define GASHA_INCLUDED_DATA_COLLECTION_CPP_H

//--------------------------------------------------------------------------------
// data_collection.cpp.h
// データコレクション【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/data_collection.inl"//データコレクション【インライン関数／テンプレート関数定義部】

#include<gasha/pool_allocator.cpp.h>//プールアロケータ【関数／実体定義部】
#include<gasha/hash_table.cpp.h>//ハッシュテーブル【関数／実体定義部】
#include<gasha/dynamic_array.cpp.h>//動的配列【関数／実体定義部】
#include<gasha/iterator.h>//イテレータ操作
#include<gasha/fast_string.h>//高速文字列操作

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <functional>//C++11 std::function
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//データコレクションクラス

//登録
template<class T, std::size_t _POOL_SIZE>
typename dataCollection<T, _POOL_SIZE>::value_type* dataCollection<T, _POOL_SIZE>::regist(const typename dataCollection<T, _POOL_SIZE>::value_type& data)
{
	//プールアロケータから割り当て
	value_type* value = m_pool.newDefault(data);
	if (!value)
		return nullptr;//規定の個数が割り当て済みなら失敗

	//検索テーブルに登録
	m_searchTable.emplace(data.key(), value);
	
	//整列テーブルに登録
	//※登録順に登録されることに注意
	m_orderTable.push_back(value);

	return value;
}

//破棄
template<class T, std::size_t _POOL_SIZE>
void dataCollection<T, _POOL_SIZE>::unregist(const crc32_t key)
{
	value_type* value = find(key);
	if (!value)
		return;

	//検索テーブルから破棄
	m_searchTable.erase(key);

	//プールアロケータから解放
	m_pool.deleteDefault(value);
}

//整列
template<class T, std::size_t _POOL_SIZE>
void dataCollection<T, _POOL_SIZE>::sort(const bool is_descendant)
{
	//整列テーブルに全データを再登録
	m_orderTable.clear();//一旦クリア
	orderTable& order_table = m_orderTable;
	forEach(m_searchTable, [&order_table](reference value)
		{
			order_table.push_back(value);//テーブルに登録
		}
	);

	//整列
	std::function<bool(const reference lhs, const reference rhs)> pred = nullptr;
	auto compare_name = [](const char* lhs, const char* rhs) -> int { return strcmp_fast(lhs, rhs); };
	if (!is_descendant)
		pred = [&compare_name](const reference lhs, const reference rhs) -> bool { return compare_name(lhs->name(), rhs->name()) <= 0; };
	else
		pred = [&compare_name](const reference lhs, const reference rhs) -> bool { return compare_name(lhs->name(), rhs->name()) >= 0; };
	m_orderTable.sort(pred);
}

//インスタンス化用マクロ
#define INSTANCING_dataCollection(T, _POOL_SIZE) \
	template class dataCollection<T, _POOL_SIZE>; \
	template class GASHA_ poolAllocator<dataCollection<T, _POOL_SIZE>::POOL_SIZE>; \
	template class GASHA_ hash_table::container<typename dataCollection<T, _POOL_SIZE>::searchTableOpe, dataCollection<T, _POOL_SIZE>::SEARCH_TABLE_SIZE>; \
	template class GASHA_ dynamic_array::container<typename dataCollection<T, _POOL_SIZE>::orderTableOpe>;

#endif//GASHA_INCLUDED_DATA_COLLECTION_CPP_H

// End of file
