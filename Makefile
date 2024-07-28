CC = gcc
EXE = renderer

SRC_DIR = src
INC_DIR = include


CFLAGS = -I$(INC_DIR) -Wall -Wextra -Werror -Wshadow -Wdouble-promotion \
		 -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common \
		 -Wstack-usage=255 -Wconversion -Os -ffunction-sections \
		 -fdata-sections -Wpadded -fno-short-enums

DEPS = $(wildcard $(INC_DIR)/*.h)

SRC = $(wildcard $(SRC_DIR)/*.c) main.c
OBJ = $(SRC:%.c=%.o)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	-rm -r $(OBJ)
	-rm -r $(EXE)