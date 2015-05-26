//
//  TFLog.h
//  CeloxicaTest
//
//  Created by Tom Fewster on 25/04/2013.
//

#ifndef CeloxicaTest_TFLog_h
#define CeloxicaTest_TFLog_h

#include <sstream>
#include <iostream>

#define cout_sync TFLog()

class TFLog {
public:
    explicit TFLog() : os(std::cout) {}
	
    ~TFLog() {
		// write out out string on destruction
        os << ss.rdbuf() << std::endl;
    }

    template <typename T>
    TFLog& operator<<(T const& t) {
        ss << t;
        return *this;
	}

private:
	std::ostream& os;
	std::stringstream ss; // stringstream is not copyable, so copies are already forbidden
};

#endif
