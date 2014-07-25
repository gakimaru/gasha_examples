#pragma once
#ifndef GASHA_INCLUDED_WORK_BUFF_H
#define GASHA_INCLUDED_WORK_BUFF_H

//--------------------------------------------------------------------------------
// work_buff.h
// シリアライズテスト：ワークバッファ
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <cstddef>//std::size_t

//--------------------
//ワークバッファ取得
//※固定サイズのワークバッファを返す。
void* getWrokBuff(std::size_t& buff_size);

//--------------------
//ワークバッファ解放
bool releaseWorkBuff(void* work_buff);

#endif//GASHA_INCLUDED_WORK_BUFF_H

// End of work_buff
