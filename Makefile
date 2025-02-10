CC = gcc
EXE = renderer

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
LIB_DIR = /usr/lib

LIBS = -lX11 -lGL -lm

CFLAGS = -I$(INC_DIR) -g -Wall -Wextra -Wshadow -Wdouble-promotion \
		 -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common \
		 -Wstack-usage=1000 -Wconversion -ffunction-sections \
		 -fdata-sections -Wpadded -fno-short-enums -fPIC

DEPS = $(wildcard $(INC_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEPS)
	mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	-rm -r $(OBJ)
	-rm -r $(EXE)
