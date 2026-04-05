CXX  = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude
BUILD = build

.PHONY: all test check clean

all:
	@mkdir $(BUILD)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(BUILD)/Cript_Estg_Cartas
	@echo "Compilado: $(BUILD)/Cript_Estg_Cartas"

test:
	@mkdir $(BUILD)
	$(CXX) $(CXXFLAGS) tests/test.cpp -o $(BUILD)/test
	@echo "Compilado: $(BUILD)/test_all"
	@echo ""
	@./$(BUILD)/test

check: all test

clean:
	rmdir /s /q $(BUILD)