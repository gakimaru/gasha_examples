#-------------------------------------------------------------------------------
# Makefile
# example_unit_test 用メイクファイル
#
# Gakimaru's standard library for C++ - GASHA
#   Copyright (c) 2014 Itagaki Mamoru
#   Released under the MIT license.
#     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------

# 実行ファイル名
PROG_NAME := example_unit_test

# プロジェクトの先頭ディレクトリ
PROJ_BASE_DIR := ..

# ソースファイルの先頭ディレクトリ
SRC_BASE_DIR := $(PROJ_BASE_DIR)/../src
PROJ_SRC_DIR := $(SRC_BASE_DIR)/$(PROG_NAME)

# ソースファイル
SRCS := \
	main.cpp \
	$(PROJ_SRC_DIR)/example_unit_test.cpp \
	$(PROJ_SRC_DIR)/real_test/game_loop/game_loop.cpp \
	$(PROJ_SRC_DIR)/real_test/modules/module_a.cpp \
	$(PROJ_SRC_DIR)/real_test/modules/module_b.cpp \

# インクルードディレクトリ
INCLIDE_DIRS := \
	-I$(PROJ_SRC_DIR)

#-------------------------------------------------------------------------------
# 共通部分をインクルード

include $(PROJ_BASE_DIR)/Makefile_example_common

#-------------------------------------------------------------------------------

# End of file
