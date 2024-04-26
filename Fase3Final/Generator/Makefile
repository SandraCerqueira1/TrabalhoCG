CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCDIR = .
OBJDIR = obj
BINDIR = .

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(wildcard $(SRCDIR)/*.hpp)

TARGET = generator

$(BINDIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(BINDIR)/$(TARGET)
