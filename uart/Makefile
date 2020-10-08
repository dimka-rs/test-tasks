BIN_NAME = ./uart
DOCS_DIR = docs
SRC = src/*

all: $(SRC)
	$(CC) -o $(BIN_NAME) $(SRC) -Iinclude

test: all
	./uart &
	python3 test.py /tmp/uart.in /tmp/uart.out
	killall uart

docs:
	doxygen

clean:
	rm -rf $(DOCS_DIR)
	rm $(BIN_NAME)
