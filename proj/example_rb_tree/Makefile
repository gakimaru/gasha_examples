#-------------------------------------------------------------------------------
# Makefile
# example_rb_tree 用メイクファイル
#
# Gakimaru's standard library for C++ - GASHA
#   Copyright (c) 2014 Itagaki Mamoru
#   Released under the MIT license.
#     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------

# 実行ファイル名
PROG_NAME := example_rb_tree

# プロジェクトの先頭ディレクトリ
PROJ_BASE_DIR := ..

# ソースファイルの先頭ディレクトリ
SRC_BASE_DIR := $(PROJ_BASE_DIR)/../src
PROJ_SRC_DIR := $(SRC_BASE_DIR)/$(PROG_NAME)

# ソースファイル
SRCS := \
	main.cpp \
	$(PROJ_SRC_DIR)/example_rb_tree.cpp \
	$(PROJ_SRC_DIR)/example_rb_tree_sub.cpp \

# インクルードディレクトリ
INCLIDE_DIRS := \
	-I$(PROJ_SRC_DIR)

#-------------------------------------------------------------------------------
# 共通部分をインクルード

include $(PROJ_BASE_DIR)/Makefile_example_common

#-------------------------------------------------------------------------------

# End of file
