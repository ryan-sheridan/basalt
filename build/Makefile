ifeq ($(shell uname -s),Darwin)
    CC = /opt/homebrew/bin/arm-none-eabi-gcc
else
    CC = arm-none-eabi-gcc
endif

# Set any constants based on the raspberry pi model.  Version 1 has some differences to 2 and 3
ifeq ($(RASPI_MODEL),1)
	CPU = arm1176jzf-s
	DIRECTIVES = -D MODEL_1
else
	CPU = cortex-a7
endif

CFLAGS= -mcpu=$(CPU) -fpic -ffreestanding $(DIRECTIVES)

ifeq ($(DEBUG),1)
	CFLAGS += -DOS_TARGET_QEMU -g
	CSRCFLAGS= -O0 -g -Wall -Wextra
else
	CSRCFLAGS= -O2 -Wall -Wextra
endif
LFLAGS= -ffreestanding -O2 -nostdlib

# Location of the files
KER_SRC = ../src/kernel
KER_HEAD = ../include
COMMON_SRC = ../src/common
OBJ_DIR = objects
KERSOURCES = $(wildcard $(KER_SRC)/*.c)
COMMONSOURCES = $(wildcard $(COMMON_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(KERSOURCES))
OBJECTS += $(patsubst $(COMMON_SRC)/%.c, $(OBJ_DIR)/%.o, $(COMMONSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))
HEADERS = $(wildcard $(KER_HEAD)/*.h) $(wildcard $(KER_HEAD)/*/*.h)

IMG_NAME=kernel.img


build: $(OBJECTS) $(HEADERS)
	echo $(OBJECTS)
	$(CC) -T ../build/linker.ld -o $(IMG_NAME) $(LFLAGS) $(OBJECTS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -I$(KER_HEAD)/kernel -I$(KER_HEAD)/common -I$(COMMON_SRC) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -I$(KER_HEAD)/kernel -I$(KER_HEAD)/common -I$(COMMON_SRC) -c $< -o $@ $(CSRCFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(IMG_NAME)

run: build
	qemu-system-arm -m 1024 -M raspi2b -serial stdio -kernel $(IMG_NAME)
