//--------------------------------------------------------------------------------
// exmaple_allocaotr_sub.cpp
// アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

//明示的なインスタンス化

#include <gasha/stack_allocator.cpp.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.cpp.h>//ロックフリースタックアロケータ
#include <gasha/dual_stack_allocator.cpp.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.cpp.h>//ロックフリー双方向スタックアロケータ
#include <gasha/mono_allocator.cpp.h>//単一アロケータ
#include <gasha/pool_allocator.cpp.h>//プールアロケータ
#include <gasha/lf_pool_allocator.cpp.h>//ロックフリープールアロケータ
#include <gasha/std_allocator.cpp.h>//標準アロケータ

#include <gasha/spin_lock.h>//スピンロック

GASHA_USING_NAMESPACE;//ネームスペース使用

GASHA_INSTANCING_stackAllocator();
GASHA_INSTANCING_smartStackAllocator();
GASHA_INSTANCING_stackAllocator_withLock(spinLock);
GASHA_INSTANCING_smartStackAllocator_withLock(spinLock);
//GASHA_INSTANCING_stackAllocator_withBuff(80);
//GASHA_INSTANCING_smartStackAllocator_withBuff(80);
//GASHA_INSTANCING_stackAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_smartStackAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_stackAllocator_withType(int, 20);
//GASHA_INSTANCING_smartStackAllocator_withType(int, 20);
//GASHA_INSTANCING_stackAllocator_withType_withLock(int, 20, spinLock);
//GASHA_INSTANCING_smartStackAllocator_withType_withLock(int, 20, spinLock);

GASHA_INSTANCING_lfStackAllocator();
GASHA_INSTANCING_lfSmartStackAllocator();
//GASHA_INSTANCING_lfStackAllocator_withBuff(80);
//GASHA_INSTANCING_lfSmartStackAllocator_withBuff(80);
//GASHA_INSTANCING_lfStackAllocator_withType(int, 20);
//GASHA_INSTANCING_lfSmartStackAllocator_withType(int, 20);

GASHA_INSTANCING_dualStackAllocator();
GASHA_INSTANCING_smartDualStackAllocator();
GASHA_INSTANCING_dualStackAllocator_withLock(spinLock);
GASHA_INSTANCING_smartDualStackAllocator_withLock(spinLock);
//GASHA_INSTANCING_dualStackAllocator_withBuff(80);
//GASHA_INSTANCING_smartDualStackAllocator_withBuff(80);
//GASHA_INSTANCING_dualStackAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_smartDualStackAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_dualStackAllocator_withType(int, 20);
//GASHA_INSTANCING_smartDualStackAllocator_withType(int, 20);
//GASHA_INSTANCING_dualStackAllocator_withType_withLock(int, 20, spinLock);
//GASHA_INSTANCING_smartDualStackAllocator_withType_withLock(int, 20, spinLock);

GASHA_INSTANCING_lfDualStackAllocator();
GASHA_INSTANCING_lfSmartDualStackAllocator();
//GASHA_INSTANCING_lfDualStackAllocator_withBuff(80);
//GASHA_INSTANCING_lfSmartDualStackAllocator_withBuff(80);
//GASHA_INSTANCING_lfDualStackAllocator_withType(int, 20);
//GASHA_INSTANCING_lfSmartDualStackAllocator_withType(int, 20);

GASHA_INSTANCING_monoAllocator();
GASHA_INSTANCING_monoAllocator_withLock(spinLock);
//GASHA_INSTANCING_monoAllocator_withBuff(80);
//GASHA_INSTANCING_monoAllocator_withBuff_withLock(80, spinLock);
//GASHA_INSTANCING_monoAllocator_withType(int, 20);
//GASHA_INSTANCING_monoAllocator_withType_withLock(int, 20, spinLock);

GASHA_INSTANCING_stdAllocator();
GASHA_INSTANCING_stdAllocator_withLock(spinLock);
GASHA_INSTANCING_stdAlignAllocator();
GASHA_INSTANCING_stdAlignAllocator_withLock(spinLock);

// End of file
