CC        = g++
CFLAGS    = -Wall -pedantic
PROG	  = HackAssembler

SRC_DIR   = src
BUILD_DIR = build
SRCS	  = $(wildcard $(SRC_DIR)/*.cpp)
DEPS	  = $(wildcard $(SRC_DIR)/*.h)
OBJS	  = $(subst $(SRC_DIR),$(BUILD_DIR),$(SRCS:.cpp=.o))

all: $(PROG)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(PROG) $(BUILD_DIR)/*.o
