//--------------------------------------------------------------------------------
// item_data.cpp
// アイテムデータ【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/item_data.inl"//アイテムデータ【インライン関数／テンプレート関数定義部】

#include <gasha/fast_string.h>//高速文字列処理

GASHA_USING_NAMESPACE;//ネームスペース使用

//メソッド
void itemData::setTitle(const char* title)//称号をセット
{
	if (!title)
		m_title = nullptr;
	else
	{
		m_title = m_titleBuff;
		strncpy_fast(m_title, title, sizeof(m_titleBuff));
		m_title[sizeof(m_titleBuff) - 1] = '\0';
	}
}

// End of file
