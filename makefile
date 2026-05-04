CC:=gcc
CCFLAGS:=-Wall -Wextra -g 
#folders
SRC_DIR:=src
OBJ_DIR:=build/obj
BIN_DIR:=build/bin
#get every .c file in src
SRCS:=$(wildcard $(SRC_DIR)/*.c)
#for every %.c file in src have an o file in obj
OBJS:=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET:=$(BIN_DIR)/basic_calc

all : $(TARGET)

#link all .o files from the build/obj/ directory into
#a single target executable and store it in build/bin/
$(TARGET) : $(OBJS) | $(BIN_DIR)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS)

#compile each src/.c file into a corresponding build/obj/.o file
#ensure that build/obj exists before it
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

#create directories if bin and obj directories don't exist
$(OBJ_DIR) $(BIN_DIR) :
	mkdir -p $@

clean :
	rm -rf ./build
