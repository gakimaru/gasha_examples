#pragma once
#ifndef GASHA_INCLUDED_DATA_COLLECTION_INL
#define GASHA_INCLUDED_DATA_COLLECTION_INL

//--------------------------------------------------------------------------------
// data_collection.inl
// データコレクション【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/data_collection.h"//データコレクション【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//データコレクションクラス

//開始イテレータ
template<class T, std::size_t _POOL_SIZE>
inline const typename dataCollection<T, _POOL_SIZE>::iterator dataCollection<T, _POOL_SIZE>::begin() const
{
	return m_orderTable.begin();
}

//終了イテレータ
template<class T, std::size_t _POOL_SIZE>
inline const typename dataCollection<T, _POOL_SIZE>::iterator dataCollection<T, _POOL_SIZE>::end() const
{
	return m_orderTable.end();
}

//開始イテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::iterator dataCollection<T, _POOL_SIZE>::begin()
{
	return m_orderTable.begin();
}

//終了イテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::iterator dataCollection<T, _POOL_SIZE>::end()
{
	return m_orderTable.end();
}

//開始イテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::const_iterator dataCollection<T, _POOL_SIZE>::cbegin() const
{
	return m_orderTable.cbegin();
}

//終了イテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::const_iterator dataCollection<T, _POOL_SIZE>::cend() const
{
	return m_orderTable.cend();
}

//開始リバースイテレータ
template<class T, std::size_t _POOL_SIZE>
inline const typename dataCollection<T, _POOL_SIZE>::reverse_iterator dataCollection<T, _POOL_SIZE>::rbegin() const
{
	return m_orderTable.rbegin();
}

//終了リバースイテレータ
template<class T, std::size_t _POOL_SIZE>
inline const typename dataCollection<T, _POOL_SIZE>::reverse_iterator dataCollection<T, _POOL_SIZE>::rend() const
{
	return m_orderTable.rend();
}

//開始リバースイテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::reverse_iterator dataCollection<T, _POOL_SIZE>::rbegin()
{
	return m_orderTable.rbegin();
}

//終了リバースイテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::reverse_iterator dataCollection<T, _POOL_SIZE>::rend()
{
	return m_orderTable.rend();
}

//開始リバースイテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::const_reverse_iterator dataCollection<T, _POOL_SIZE>::crbegin() const
{
	return m_orderTable.crbegin();
}

//終了リバースイテレータ
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::const_reverse_iterator dataCollection<T, _POOL_SIZE>::crend() const
{
	return m_orderTable.crend();
}

//検索
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::value_type* dataCollection<T, _POOL_SIZE>::find(const crc32_t key)
{
	reference* ref = m_searchTable.at(key);
	if (!ref)
		return nullptr;
	return *ref;
}
template<class T, std::size_t _POOL_SIZE>
inline typename dataCollection<T, _POOL_SIZE>::value_type* dataCollection<T, _POOL_SIZE>::find(const char* key)
{
	return find(calcCRC32(key));
}

//デフォルトコンストラクタ
template<class T, std::size_t _POOL_SIZE>
inline dataCollection<T, _POOL_SIZE>::dataCollection() :
	m_pool(),
	m_searchTable(),
	m_orderTable(m_orderTableBuff)
{}

//デストラクタ
template<class T, std::size_t _POOL_SIZE>
inline dataCollection<T, _POOL_SIZE>::~dataCollection()
{}

#endif//GASHA_INCLUDED_DATA_COLLECTION_INL

// End of file
