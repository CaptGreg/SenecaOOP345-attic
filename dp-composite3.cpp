// http://www.sourcetricks.com/2013/04/composite-pattern.html
// 
// Composite Pattern
// 
//     Composite pattern is useful when you want to treat a group of objects in the same 
//     manner as a single instance of the object. The objects grouped, themselves could 
//     be in composite manner in a tree fashion.
// 
//     The primitive and compound objects are treated uniformly.
// 
//     A component interface is defined and both the leaf as well as composite objects 
//     implement it in uniform fashion.
// 
//     Below example shows an implementation with a phone diagnostics composite pattern. 
//     A composite object is formed by combining all the phone related diagnostics objects 
//     in a tree manner. Then a diagnostic run on the composite object, which in turn 
//     reciprocates the request to all the diagnostics objects in a tree manner.
// 
// Sample implementation of composite pattern in C++

#include <iostream>
#include <string>
#include <list>

using namespace std;

class DiagnosticInterface {
public:
    virtual void runDiagnostics() = 0;
    virtual string getDiagnosticsResult() = 0;
};

class DiagnosticCompositeInterface : public DiagnosticInterface  {
public:
    void runDiagnostics() {
        for (list<DiagnosticInterface*>::iterator it = mCompositeObjs.begin(); it != mCompositeObjs.end(); ++it )
            (*it)->runDiagnostics();
    }
    string getDiagnosticsResult() {
        string ret = "";
        for (list<DiagnosticInterface*>::iterator it = mCompositeObjs.begin(); it != mCompositeObjs.end(); ++it )
            ret += (*it)->getDiagnosticsResult() + "\n";
        return ret;
    }
    void addChild(DiagnosticInterface *child) {
        mCompositeObjs.push_back(child);
    }
    void removeChild(DiagnosticInterface *child) {
        // find and remove the child. not implemented yet.
    }
    DiagnosticInterface* getChild(int i) {
        // get the child at i. Not implemented yet.
    }
protected:
    list<DiagnosticInterface*> mCompositeObjs;
};

class PhoneDiagnostics : public DiagnosticCompositeInterface {
public:
    void runDiagnostics() {
        cout << "running PhoneDiagnostics..." << "\n";
        DiagnosticCompositeInterface::runDiagnostics();
    }
    string getDiagnosticsResult() {
        string ret = "PhoneDiagnostics all tested Okay!\n";
        ret += DiagnosticCompositeInterface::getDiagnosticsResult();
        return ret;
    }
};

class ScreenSaverDiagnostics : public DiagnosticInterface {
public:
    void runDiagnostics() {
        cout << "running ScreenSaverDiagnostics..." << "\n";
    }
    string getDiagnosticsResult() {
        return "ScreenSaverDiagnostics all tested Okay!";
    }
};

class HardwareDiagnostics : public DiagnosticCompositeInterface {
public:
    void runDiagnostics() {
        cout << "running HardwareDiagnostics..." << "\n";
        DiagnosticCompositeInterface::runDiagnostics();
    }
    string getDiagnosticsResult() {
        string ret = "HardwareDiagnostics all tested Okay!\n";
        ret += DiagnosticCompositeInterface::getDiagnosticsResult();
        return ret;
    }
};

class NetworkDiagnostics : public DiagnosticCompositeInterface {
public:
    void runDiagnostics() {
        cout << "running NetworkDiagnostics..." << "\n";
        DiagnosticCompositeInterface::runDiagnostics();
    }

    string getDiagnosticsResult() {
        string ret = "NetworkDiagnostics all tested Okay!\n";
        ret += DiagnosticCompositeInterface::getDiagnosticsResult();
        return ret;
    }
};

class TouchScreenDiagnostics : public DiagnosticInterface {
public:
    void runDiagnostics() {
        cout << "running TouchScreenDiagnostics..." << "\n";
    }
    string getDiagnosticsResult() {
        return "TouchScreenDiagnostics all tested Okay!";
    }
};

class OnOffKeyDiagnostics : public DiagnosticInterface {
public:
    void runDiagnostics() {
        cout << "running OnOffKeyDiagnostics..." << "\n";
    }
    string getDiagnosticsResult() {
        return "OnOffKeyDiagnostics all tested Okay!";
    }
};

class WifiDiagnostics : public DiagnosticInterface {
public:
    void runDiagnostics() {
        cout << "running WifiDiagnostics..." << "\n";
    }
    string getDiagnosticsResult() {
        return "WifiDiagnostics all tested Okay!";
    }
};

class ThreeGDiagnostics : public DiagnosticInterface {
public:
    void runDiagnostics() {
        cout << "running ThreeGDiagnostics..." << "\n";
    }
    string getDiagnosticsResult() {
        return "ThreeGDiagnostics all tested Okay!";
    }
};

int main()
{
    DiagnosticCompositeInterface *phoneDiagnostics = new PhoneDiagnostics();

    DiagnosticInterface *screenSaverDiagnostics = new ScreenSaverDiagnostics();
    DiagnosticCompositeInterface *hardwareDiagnostics = new HardwareDiagnostics();
    DiagnosticCompositeInterface *networkDiagnostics = new NetworkDiagnostics();

    DiagnosticInterface *touchScreenDiagnostics = new TouchScreenDiagnostics();
    DiagnosticInterface *onOffKeyDiagnostics = new OnOffKeyDiagnostics();

    DiagnosticInterface *wifiDiagnostics = new WifiDiagnostics();
    DiagnosticInterface *threeGDiagnostics = new ThreeGDiagnostics();

    hardwareDiagnostics->addChild(touchScreenDiagnostics);
    hardwareDiagnostics->addChild(onOffKeyDiagnostics);

    networkDiagnostics->addChild(wifiDiagnostics);
    networkDiagnostics->addChild(threeGDiagnostics);

    phoneDiagnostics->addChild(screenSaverDiagnostics);
    phoneDiagnostics->addChild(hardwareDiagnostics);
    phoneDiagnostics->addChild(networkDiagnostics);

    cout << "Run diagnostics on the phone!" << "\n";
    phoneDiagnostics->runDiagnostics();
    cout << "****\nGet the diagnostics result\n" << phoneDiagnostics->getDiagnosticsResult() << "\n";
    return 0;
}
