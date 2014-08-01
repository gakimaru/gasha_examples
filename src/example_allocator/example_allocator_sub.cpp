//--------------------------------------------------------------------------------
// exmaple_allocaotr_sub.cpp
// アロケータテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

GASHA_USING_NAMESPACE;//ネームスペース使用

//明示的なインスタンス化

#include <gasha/stack_allocator.cpp.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.cpp.h>//ロックフリースタックアロケータ
#include <gasha/dual_stack_allocator.cpp.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.cpp.h>//ロックフリー双方向スタックアロケータ
#include <gasha/mono_allocator.cpp.h>//単一アロケータ
#include <gasha/pool_allocator.cpp.h>//プールアロケータ
#include <gasha/lf_pool_allocator.cpp.h>//ロックフリープールアロケータ
#include <gasha/std_allocator.cpp.h>//標準アロケータ

GASHA_INSTANCING_stackAllocator_withLock(lock_type);
GASHA_INSTANCING_smartStackAllocator_withLock(lock_type);
GASHA_INSTANCING_lfStackAllocator();
GASHA_INSTANCING_lfSmartStackAllocator();

GASHA_INSTANCING_dualStackAllocator_withLock(lock_type);
GASHA_INSTANCING_smartDualStackAllocator_withLock(lock_type);
GASHA_INSTANCING_lfDualStackAllocator();
GASHA_INSTANCING_lfSmartDualStackAllocator();

GASHA_INSTANCING_poolAllocator_withLock(16, lock_type);
GASHA_INSTANCING_lfPoolAllocator(16);
GASHA_INSTANCING_poolAllocator(8);

GASHA_INSTANCING_monoAllocator_withLock(lock_type);

GASHA_INSTANCING_stdAllocator_withLock(lock_type);
GASHA_INSTANCING_stdAlignAllocator_withLock(lock_type);

// End of file
