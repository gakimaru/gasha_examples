#pragma once
#ifndef GASHA_INCLUDED_SAVE_DTA_VERSION_INL
#define GASHA_INCLUDED_SAVE_DTA_VERSION_INL

//--------------------------------------------------------------------------------
// save_data_version.inl
// セーブデータバージョン【インライン関数／テンプレート関数定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/save_data/save_data_version.h"//セーブデータバージョン【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//セーブデータバージョン管理クラス

//メジャーバージョン取得
inline int saveDataVersion::majorVer() const
{
	return MAJOR;
}

//マイナーバージョン取得
inline int saveDataVersion::minorVer() const
{
	return MINOR;
}

//セーブデータバージョンからロードしたメジャーバージョン取得
inline int saveDataVersion::loadedMajorVer() const
{
	return m_loadedMajorVer;
}

//セーブデータバージョンからロードしたマイナーバージョン取得
inline int saveDataVersion::loadedMinorVer() const
{
	return m_loadedMinorVer;
}

//セーブ日時取得
inline std::chrono::system_clock::time_point saveDataVersion::saveTime() const
{
	return m_saveTime;
};

//セーブしたか？
inline bool saveDataVersion::hasBeenSaved() const
{
	return m_hasBeenSaved;
}

//ロードしたか？
inline bool saveDataVersion::hasBeenLoaded() const
{
	return m_hasBeenLoaded;
}

//セーブしたことにする
inline void saveDataVersion::finishToSave()
{
	m_hasBeenSaved = true;
}

//ロードしたことにする
inline void saveDataVersion::finishToLoad()
{
	m_hasBeenLoaded = true;
}

//セーブ日時を更新する
inline void saveDataVersion::updateSaveTime()
{
	m_saveTime = std::chrono::system_clock::now();
}

//セーブしたことがある状態を解除
inline void saveDataVersion::refinishToSave()
{
	m_hasBeenSaved = false;
}

//ロードしたことがある状態を解除
inline void saveDataVersion::refinishToLoad()
{
	m_hasBeenLoaded = false;
}

//ロードしたバージョンを現在のバージョンに更新
inline void saveDataVersion::updateLoadedVer()
{
	m_loadedMajorVer = MAJOR;//セーブデータバージョンからロードしたメジャーバージョン取得
	m_loadedMinorVer = MINOR;//セーブデータバージョンからロードしたマイナーバージョン取得
}

//セーブ日時の文字列取得
template<std::size_t N>
inline void saveDataVersion::saveTimeStr(char(&buff)[N])
{
	saveTimeStr(buff, N);
}

//コンストラクタ
inline saveDataVersion::saveDataVersion() :
	m_loadedMajorVer(0),
	m_loadedMinorVer(0),
	m_saveTime(std::chrono::system_clock::now()),
	m_hasBeenSaved(false),
	m_hasBeenLoaded(false)
{}

//デストラクタ
inline saveDataVersion::~saveDataVersion()
{}

#endif//GASHA_INCLUDED_SAVE_DTA_VERSION_INL

// End of file
