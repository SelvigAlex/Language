# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Имя исполняемого файла
TARGET = sela

# Список исходных файлов
SRCS = main.cpp \
       ast/statement.cpp \
       ast/expression.cpp \
       lib/value.cpp \
       lib/variables.cpp \
       parser/parser.cpp \
	   parser/lexer.cpp \
	   parser/token.cpp \
	   lib/function.cpp


# Список объектных файлов
OBJS = $(SRCS:.cpp=.o)

# Правило по умолчанию для компиляции
all: $(TARGET)

# Правило для компиляции исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) -g $(OBJS) -o $(TARGET)

# Правило для компиляции исходных файлов в объектные
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка объектных файлов и исполняемого файла
clean:
	rm -f $(OBJS) $(TARGET)

# Правило для компиляции main.cpp
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -g -c main.cpp -o main.o

# Правило для удаления всех сгенерированных файлов
distclean: clean
	rm -f main.o

# Файл по умолчанию при вызове make
.PHONY: all clean distclean
