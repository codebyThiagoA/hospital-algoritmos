# Makefile simples para Windows/Linux

CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -I include
SRC_DIR := src
BUILD_DIR := build
BIN := $(BUILD_DIR)/sgh

ifeq ($(OS),Windows_NT)
  EXE := .exe
  RM := rmdir /S /Q
  MK := if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
  EXE :=
  RM := rm -rf
  MK := mkdir -p $(BUILD_DIR)
endif

APP_SRCS := \
  $(SRC_DIR)/main.c \
  $(SRC_DIR)/menu.c \
  $(SRC_DIR)/paciente.c \
  $(SRC_DIR)/fila.c \
  $(SRC_DIR)/historico.c \
  $(SRC_DIR)/arquivos.c

APP_OBJS := $(APP_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TEST_HIST_SRC := tests/teste_historico.c
TEST_HIST_BIN := $(BUILD_DIR)/teste_historico$(EXE)

.PHONY: all 1 2 3 4 build run clean tests

all: 1

# 1) Build do app
1: build

build: $(BUILD_DIR) $(BIN)$(EXE)
	@echo Build concluido: $(BIN)$(EXE)

$(BUILD_DIR):
	@$(MK)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)$(EXE): $(APP_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# 2) Run do app
2: run

run: 1
	$(BIN)$(EXE)

# 3) Clean
3: clean

clean:
	-$(RM) $(BUILD_DIR)

# 4) Tests
4: tests

tests: $(BUILD_DIR) $(TEST_HIST_BIN)
	@echo Executando teste: teste_historico
	$(TEST_HIST_BIN)

$(TEST_HIST_BIN): $(TEST_HIST_SRC) $(SRC_DIR)/paciente.c $(SRC_DIR)/historico.c
	$(CC) $(CFLAGS) $^ -o $@
