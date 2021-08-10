src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
pgm = $(src:.cpp=)

all : $(pgm)

############################################################
# NOTE -fcoroutines-ts for clang++ but -fcoroutine for g++ #
############################################################

CXX=/usr/bin/clang++-10 
CXX=/usr/bin/clang++-11 
# clang++ flags
CXXFLAGS1 = -fcoroutines    -fno-exceptions -std=c++2a -Wall -ggdb
CXXFLAGS2 = -fcoroutines    -fexceptions    -std=c++2a -Wall -ggdb
CXXFLAGS1 = -fcoroutines-ts -fno-exceptions -std=c++2a -Wall -ggdb
CXXFLAGS2 = -fcoroutines-ts -fexceptions    -std=c++2a -Wall -ggdb


# g++ flags-fmax-errors=1 -Wfatal-errors

CXX=/usr/bin/g++-10
CXXFLAGS1 = -fcoroutines -fexceptions    -std=c++20 -fmax-errors=1 -Wfatal-errors -Wall -ggdb
CXXFLAGS2 = -fcoroutines -fno-exceptions -std=c++20 -fmax-errors=1 -Wfatal-errors -Wall -ggdb
 
# g++ If all the content is marked "noexcept" you can omit the "-fno-exceptions".

LDFLAGS = -lcppcoro -pthread

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -o $@ -c $<

%: %.cpp
	@echo +++++++++++++++++++++++++++++
	@echo "  " $@
	@echo +++++++++++++++++++++++++++++
	-rm $@
	@echo ++++++++++++++++ compile -fexceptions 
	-$(CXX) $(CXXFLAGS1) $< -o $@ $(LDFLAGS) 
	@echo ++++++++++++++++ compile -fno-exceptions 
	-$(CXX) $(CXXFLAGS2) $< -o $@ $(LDFLAGS)
	@size $@
	@echo 
	-./$@

PlayWithCoroutines : PlayWithCoroutines.cpp funs.cpp funs.h
	@echo +++++++++++++++++++++++++++++
	@echo "  " $@
	@echo +++++++++++++++++++++++++++++
	-rm $@
	@echo ++++++++++++++++ compile -fexceptions 
	-$(CXX) $(CXXFLAGS1) PlayWithCoroutines.cpp funs.cpp -o $@ $(LDFLAGS) 
	@size $@
	@echo 
	-./$@


all : $(pgm)

.PHONY: clean
clean:
	rm -f $(obj) $(pgm)
