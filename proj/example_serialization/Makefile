#-------------------------------------------------------------------------------
# Makefile
# example_serialization 用メイクファイル
#
# Gakimaru's standard library for C++ - GASHA
#   Copyright (c) 2014 Itagaki Mamoru
#   Released under the MIT license.
#     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------

# 実行ファイル名
PROG_NAME := example_serialization

# プロジェクトの先頭ディレクトリ
PROJ_BASE_DIR := ..

# ソースファイルの先頭ディレクトリ
SRC_BASE_DIR := $(PROJ_BASE_DIR)/../src
PROJ_SRC_DIR := $(SRC_BASE_DIR)/$(PROG_NAME)

# ソースファイル
SRCS := \
	main.cpp \
	$(PROJ_SRC_DIR)/example_serialization.cpp \
	$(PROJ_SRC_DIR)/work_buff.cpp \
	$(PROJ_SRC_DIR)/file.cpp \
	$(PROJ_SRC_DIR)/basic_test/basic_test.cpp \
	$(PROJ_SRC_DIR)/feature_test/feature_test.cpp \
	$(PROJ_SRC_DIR)/feature_test/save_load.cpp \
	$(PROJ_SRC_DIR)/real_test/real_test.cpp \
	$(PROJ_SRC_DIR)/real_test/data/basic_data.cpp \
	$(PROJ_SRC_DIR)/real_test/data/item_data.cpp \
	$(PROJ_SRC_DIR)/real_test/data/inventory.cpp \
	$(PROJ_SRC_DIR)/real_test/data/ability_data.cpp \
	$(PROJ_SRC_DIR)/real_test/data/ability_collection.cpp \
	$(PROJ_SRC_DIR)/real_test/data/chara_data.cpp \
	$(PROJ_SRC_DIR)/real_test/data/chara_ability_data.cpp \
	$(PROJ_SRC_DIR)/real_test/data/chara_collection.cpp \
	$(PROJ_SRC_DIR)/real_test/data/game_status.cpp \
	$(PROJ_SRC_DIR)/real_test/save_data/save_data.cpp \
	$(PROJ_SRC_DIR)/real_test/save_data/save_data_version.cpp \
	$(PROJ_SRC_DIR)/real_test/save_data/load_target.cpp \
	$(PROJ_SRC_DIR)/real_test/test_data/test_data.cpp \

# インクルードディレクトリ
INCLIDE_DIRS := \
	-I$(PROJ_SRC_DIR)

#-------------------------------------------------------------------------------
# 共通部分をインクルード

include $(PROJ_BASE_DIR)/Makefile_example_common

#-------------------------------------------------------------------------------

# End of file
