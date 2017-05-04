TEST_SRCS = $(wildcard test/*.cpp)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_DEPS = $(TEST_OBJS:.o=.d)

GTEST_ROOT = thirdparty/googletest/googletest
GTEST_LIB = $(GTEST_ROOT)/libgtest_main.a
INCLUDES := -I$(GTEST_ROOT)/include
LDFLAGS = -L$(GTEST_ROOT) -lgtest_main -lgtest
CFLAGS = -std=c++1y -g -Wall $(INCLUDES) -MT $@ -MMD -MF $*.d 

%.o: %.cpp
%.o: %.cpp %.d
	g++ $(CFLAGS) -c $< -o $@

%.d: ;
.PRECIOUS: %.d

.PHONY: all test run_tests clean

all: test

test: run_tests

run_tests: unit_tests
	./unit_tests

clean:
	rm -f unit_tests $(BINARIES) $(TEST_OBJS)

$(GTEST_LIB): $(wildcard $(GTEST_ROOT)/src/*.cc)
	cd $(GTEST_ROOT) && cmake . && make

unit_tests: $(TEST_OBJS) $(GTEST_LIB)
	g++ -pthread $^ -o $@ $(LDFLAGS) 

$(foreach dep, $(TEST_DEPS), $(eval include $(dep)))
