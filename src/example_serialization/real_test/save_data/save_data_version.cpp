//--------------------------------------------------------------------------------
// save_data_version.cpp
// セーブデータバージョン【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/save_data/save_data_version.inl"//セーブデータバージョン【インライン関数／テンプレート関数定義部】

#include <ctime>//std::localtime

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//セーブデータバージョン管理クラス

//セーブ日時の文字列取得
void saveDataVersion::saveTimeStr(char* buff, std::size_t buff_size) const
{
	const time_t save_time = std::chrono::system_clock::to_time_t(m_saveTime);

	#pragma warning(push)
	#pragma warning(disable : 4996)
	const struct tm* save_time_tm_p = std::localtime(&save_time);
	#pragma warning(pop)

	strftime(buff, buff_size, "%Y/%m/%d(%a) %H:%M:%S", save_time_tm_p);
}

//--------------------
//セーブデータバージョンシングルトンの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(saveDataVersion);//シングルトンのインスタンス化

// End of file
