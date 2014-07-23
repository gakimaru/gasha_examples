//--------------------------------------------------------------------------------
// file.cpp
// シリアライズテスト：ファイル操作
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "file.h"//シリアライズテスト：ファイル操作

#include <cstdio>//std::printf(), std::FILE, std::fopen(), std::fclose(), std::fseek(), std::fread(), std::fwrite()

//【VC++】fopen() を使用すると、error C4996 が発生する
//  error C4996: 'fopen': This function or variable may be unsafe. Consider using strncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable: 4996)//C4996を抑える

//--------------------
//ファイル読み込み
std::size_t readFile(const char* file_path, void* buff, const std::size_t buff_size_max)
{
	//ファイルオープン
	std::FILE* fp = fopen(file_path, "rb");
	if (!fp)
		return 0;
	//ファイルサイズを求める
	std::fseek(fp, 0, SEEK_END);
	const std::size_t file_size = ftell(fp);
	if (file_size > buff_size_max)
	{
		std::fclose(fp);
		return 0;
	}
	//ファイルを読み込み
	std::fseek(fp, 0, SEEK_SET);
	const std::size_t read_size = std::fread(buff, 1, file_size, fp);
	std::fclose(fp);
	if (read_size < file_size)
		return 0;
	return read_size;
}

//--------------------
//ファイル書き込み
bool writeFile(const char* file_path, const void* buff, const std::size_t data_size)
{
	//ファイルオープン
	std::FILE* fp = std::fopen(file_path, "wb");
	if (!fp)
		return false;
	const std::size_t write_size = std::fwrite(buff, 1, data_size, fp);
	std::fclose(fp);
	if (write_size < data_size)
		return false;
	return true;
}

// End of file
