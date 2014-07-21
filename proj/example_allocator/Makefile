#-------------------------------------------------------------------------------
# Makefile
# example_allocator 用メイクファイル
#
# Gakimaru's researched and standard library for C++ - GASHA
#   Copyright (c) 2014 Itagaki Mamoru
#   Released under the MIT license.
#     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------

# 実行ファイル名
PROG_NAME := example_allocator

# プロジェクトの先頭ディレクトリ
PROJ_BASE_DIR := ..

# ソースファイルの先頭ディレクトリ
SRC_BASE_DIR := $(PROJ_BASE_DIR)/../src
PROJ_SRC_DIR := $(SRC_BASE_DIR)/$(PROG_NAME)

# ソースファイル
SRCS := \
	main.cpp \
	$(PROJ_SRC_DIR)/example_allocator.cpp \
	$(PROJ_SRC_DIR)/example_allocator_sub.cpp \
	$(PROJ_SRC_DIR)/basic_test/example_allocator_basic_test.cpp \
	$(PROJ_SRC_DIR)/feature_test/example_stack_allocator.cpp \
	$(PROJ_SRC_DIR)/feature_test/example_dual_stack_allocator.cpp \
	$(PROJ_SRC_DIR)/feature_test/example_mono_allocator.cpp \
	$(PROJ_SRC_DIR)/feature_test/example_pool_allocator.cpp \
	$(PROJ_SRC_DIR)/feature_test/example_std_allocator.cpp \
	$(PROJ_SRC_DIR)/feature_test/example_poly_allocator.cpp \
	$(PROJ_SRC_DIR)/performance_test/example_allocator_performance.cpp \
	$(PROJ_SRC_DIR)/examination/example_new_delete_test.cpp \

# インクルードディレクトリ
INCLIDE_DIRS := \
	-I$(PROJ_SRC_DIR)

#-------------------------------------------------------------------------------
# 共通部分をインクルード

include $(PROJ_BASE_DIR)/Makefile_example_common

#-------------------------------------------------------------------------------

# End of file
