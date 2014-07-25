#pragma once
#ifndef GASHA_INCLUDED_LOAD_TARGET_H
#define GASHA_INCLUDED_LOAD_TARGET_H

//--------------------------------------------------------------------------------
// load_target.h
// ロード対象（部分ロード用）【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/crc32.h>//CRC32計算

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//ロード対象（部分ロード用）管理クラス
class loadTarget
{
public:
	//メソッド
	inline void setLoadTarget(const crc32_t target_name_crc);//ロード対象をセット
	inline void setLoadTarget(const char* target_name);//ロード対象をセット
	inline void resetLoadTarget();//ロード対象をリセット
	inline bool isLoadTarget(const crc32_t name_crc);//ロード対象か？
	inline bool isLoadTarget(const char* target_name);//ロード対象か？
	inline bool isPartLoad();//部分ロードか？
	inline bool isPartLoad(const crc32_t name_crc);//部分ロードか？かつ、その対象項目か？
	inline bool isPartLoad(const char* target_name);//部分ロードか？かつ、その対象項目か？
private:
	//フィールド
	crc32_t m_targetData;//ロード対象
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/save_data/load_target.inl"

#endif//GASHA_INCLUDED_LOAD_TARGET_H

// End of file
