//--------------------------------------------------------------------------------
// work_buff.cpp
// シリアライズテスト：ワークバッファ
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "work_buff.h"//シリアライズテスト：ワークバッファ

static const std::size_t WORK_BUFF_SIZE = 256 * 1024;

//--------------------
//ワークバッファ取得
void* getWrokBuff(std::size_t& buff_size)
{
	buff_size = WORK_BUFF_SIZE;
	char* work_buff = new char[buff_size];
	return reinterpret_cast<void*>(work_buff);
}

//--------------------
//ワークバッファ解放
bool releaseWorkBuff(void* work_buff)
{
	delete[] reinterpret_cast<char*>(work_buff);
	return true;
}

// End of work_buff
