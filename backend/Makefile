CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/opt/openssl@3/include
LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

SRC = main.cpp portfolio.cpp auth.cpp sp500_data.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = sim

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
