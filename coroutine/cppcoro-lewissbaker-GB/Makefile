src = $(wildcard *.cpp)
pgm = $(src:.cpp=)

all : $(pgm)

############################################################
# NOTE -fcoroutines-ts for clang++ but -fcoroutine for g++ #
############################################################

# CLANG
GNU = true

ifdef CLANG
  CXX=/usr/bin/clang++-10 
  CXX=/usr/bin/clang++-11 
  CXXFLAGS1 = -fcoroutines    -fno-exceptions -std=c++2a -Wall -ggdb -Ofast
  CXXFLAGS2 = -fcoroutines    -fexceptions    -std=c++2a -Wall -ggdb -Ofast
  CXXFLAGS1 = -fcoroutines-ts -fno-exceptions -std=c++2a -Wall -ggdb -Ofast
  CXXFLAGS2 = -fcoroutines-ts -fexceptions    -std=c++2a -Wall -ggdb -Ofast
endif

ifdef GNU
  # g++ flags-fmax-errors=1 -Wfatal-errors
  CXX=/usr/bin/g++-10
  CXXFLAGS1 = -fcoroutines -fno-exceptions -std=c++20 -fmax-errors=1 -Wfatal-errors -Wall -ggdb -Ofast
  CXXFLAGS2 = -fcoroutines -fexceptions    -std=c++20 -fmax-errors=1 -Wfatal-errors -Wall -ggdb -Ofast
  # g++ If all the content is marked "noexcept" you can omit the "-fno-exceptions".
endif

LDFLAGS = -lcppcoro -pthread

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -o $@ -c $<

%: %.cpp
	@echo +++++++++++++++++++++++++++++
	@echo "  " $@
	@echo +++++++++++++++++++++++++++++
	-rm $@
	# @echo ++++++++++++++++ compile -fno-exceptions 
	# -$(CXX) $(CXXFLAGS1) $< -o $@ $(LDFLAGS)
	# @size $@
	# @echo 
	@echo ++++++++++++++++ compile -fexceptions 
	-$(CXX) $(CXXFLAGS2) $< -o $@ $(LDFLAGS) 
	@size $@


all : $(pgm)

.PHONY: clean
clean:
	rm -f $(pgm)
