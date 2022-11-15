# Root directory
ROOT = src

# Compiler and default flags
CXX = g++
CXX_FLAGS = -std=c++2a -lpheval -O3 -Ilib/PokerHandEvaluator/cpp/include -Ilib/PokerHandEvaluator/cpp/src -Llib/PokerHandEvaluator/cpp

# Object directory
OBJ_DIR = target

# Object files
OBJ =         		\
	main.o       	\
	table.o			\
	player.o   		\
	bot.o		  	\
	dealer.o

LIB = $(patsubst %,$(OBJ_DIR)/%,$(OBJ))

main: $(OBJ_DIR)/main.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)

# Intermediate targets
$(OBJ_DIR)/%.o: $(ROOT)/%.cpp $(OBJ_DIR)
	@echo Compiling $<
	@$(CXX) -o $@ -c $< $(CXX_FLAGS)

%.o: %.cpp
	@echo Compiling $<
	@$(CXX) -o $@ -c $< $(CXX_FLAGS)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	-rm -rf main $(OBJ_DIR)