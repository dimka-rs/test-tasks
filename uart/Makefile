BIN_NAME = ./uart
DOCS_DIR = docs
SRC = src/*

all: $(SRC)
	$(CC) -o $(BIN_NAME) $(SRC) -Iinclude

test: all
	python3 test.py $(BIN_NAME)

docs:
	doxygen

clean:
	rm -rf $(DOCS_DIR)
	rm $(BIN_NAME)
