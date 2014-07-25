#pragma once
#ifndef GASHA_INCLUDED_FILE_H
#define GASHA_INCLUDED_FILE_H

//--------------------------------------------------------------------------------
// file.h
// シリアライズテスト：ファイル操作
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <cstddef>//std::size_t

//--------------------
//ファイル読み込み
//※読み込んだファイルのサイズを返す
//※バッファサイズが足りなければ 0 を返し、読み込みを行わない
std::size_t readFile(const char* file_path, void* buff, const std::size_t buff_size_max);

//--------------------
//ファイル書き込み
bool writeFile(const char* file_path, const void* buff, const std::size_t data_size);

#endif//GASHA_INCLUDED_FILE_H

// End of file
