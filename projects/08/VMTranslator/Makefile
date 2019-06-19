CC        = g++
CFLAGS    = -Wall -pedantic -Wno-sequence-point -lstdc++fs
PROG	  = VMTranslator

SRC_DIR   = src
BUILD_DIR = build
SRCS	  = $(wildcard $(SRC_DIR)/*.cpp)
DEPS	  = $(wildcard $(SRC_DIR)/*.h)
OBJS	  = $(subst $(SRC_DIR),$(BUILD_DIR),$(SRCS:.cpp=.o))

.PHONY: test

all: $(PROG)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

test:
	@cd test; ./test.sh

clean:
	rm $(PROG) $(BUILD_DIR)/*.o
