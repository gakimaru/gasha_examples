#pragma once
#ifndef GASHA_INCLUDED_SAVE_DTA_VERSION_H
#define GASHA_INCLUDED_SAVE_DTA_VERSION_H

//--------------------------------------------------------------------------------
// save_data_version.h
// セーブデータバージョン【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <chrono>//C++11 std::chrono

#include <gasha/serialization/friends.h>//シリアライズ/フレンド宣言

//--------------------
//セーブデータバージョン管理クラス
class saveDataVersion
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();
public:
	//定数
	static const int MAJOR = 1;//メジャーバージョン
	static const int MINOR = 0;//マイナーバージョン
public:
	//アクセッサ
	inline int majorVer() const;//メジャーバージョン取得
	inline int minorVer() const;//マイナーバージョン取得
	inline int loadedMajorVer() const;//セーブデータバージョンからロードしたメジャーバージョン取得
	inline int loadedMinorVer() const;//セーブデータバージョンからロードしたマイナーバージョン取得
	inline std::chrono::system_clock::time_point saveTime() const;//セーブ日時取得
	inline bool hasBeenSaved() const;//セーブしたか？
	inline bool hasBeenLoaded() const;//ロードしたか？
private:
	inline void finishToSave();//セーブしたことにする
	inline void finishToLoad();//ロードしたことにする
	inline void updateSaveTime();//セーブ日時を更新する
	inline void refinishToSave();//セーブしたことがある状態を解除
	inline void refinishToLoad();//ロードしたことがある状態を解除
public:
	//メソッド
	//セーブ日時の文字列取得
	void saveTimeStr(char* buff, std::size_t buff_size) const;
	template<std::size_t N>
	inline void saveTimeStr(char(&buff)[N]);
	//ロードしたバージョンを現在のバージョンに更新
	inline void updateLoadedVer();
public:
	//コンストラクタ
	inline saveDataVersion();
	//デストラクタ
	inline ~saveDataVersion();
private:
	//フィールド
	int m_loadedMajorVer;//セーブデータバージョンからロードしたメジャーバージョン取得
	int m_loadedMinorVer;//セーブデータバージョンからロードしたマイナーバージョン取得
	std::chrono::system_clock::time_point m_saveTime;//セーブ日時
	bool m_hasBeenSaved;//セーブしたか？
	bool m_hasBeenLoaded;//ロードしたか？
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/save_data/save_data_version.inl"

#endif//GASHA_INCLUDED_SAVE_DTA_VERSION_H

// End of file
