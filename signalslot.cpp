// https://aboutcpp.wordpress.com/2014/12/20/a-simple-signal-system/

// signal.h --------------------------------------------------------------------
//#pragma once
// #ifndef SIGNAL_SIGNAL_H_
// #define SIGNAL_SIGNAL_H_

#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename EmitterType, int type, typename... Args>
class signal_can_be_sent : public std::false_type {};

// verification is done only at Object level, not at All or Type
#define DECLARE_SIGNAL(...) \
    template <> \
    class signal_can_be_sent<__VA_ARGS__> : public std::true_type {};

namespace Signal {

#define CLASS_DECLARATION(name, ...) \
    protected: \
        static std::vector<std::pair<unsigned int, std::function<void (__VA_ARGS__)> > > name; \
    \
    public: \
        static ID connect(std::function<void (__VA_ARGS__)> function); \
        static void disconnect(ID i);

#define CONNECT_IMPL(name) \
    if (name.empty()) \
        name.emplace_back(0u, std::move(function)); \
    else \
        name.emplace_back(name.back().first + 1u, std::move(function)); \
    \
    return name.back().first;

#define DISCONNECT_IMPL(name) \
    auto const end = name.cend(); \
    \
    for (auto iter = name.cbegin(); iter != end; ++iter) { \
        if (iter->first == i) { \
            name.erase(iter); \
            return; \
        } \
    } \
    std::cout << "Signal was not connected " << i << "\n";

    using ID = unsigned int;

    // catch every signal
    class All {
        CLASS_DECLARATION(m_allReceivers, int)
    };

    std::vector<std::pair<unsigned int, std::function<void (int)> > > All::m_allReceivers;

    // catch every signal of given type
    template <int type>
    class Type : public All {
        CLASS_DECLARATION(m_typeReceivers, int)
    };

    template <int type>
    std::vector<std::pair<unsigned int, std::function<void (int)> > > Type<type>::m_typeReceivers;

    template <typename EmitterType, int type, typename... Args>
    class Object : public Type<type> {

        static_assert(signal_can_be_sent<EmitterType, type, Args...>::value, "The signal has not been declared.");

    private:
        static std::unordered_map<EmitterType const *, std::vector<std::pair<unsigned int, std::function<void (int, EmitterType *, Args...)> > > > m_objectReceivers;

    public:
        // connect the function to receive Type events from the EmitterType
        // returns the id used to be able to disconnect
        static ID connect(std::function<void (int, EmitterType *, Args...)> function);
        static ID connect(EmitterType const * instance, std::function<void (int, EmitterType *, Args...)> function);

        static void disconnect(ID i);
        static void disconnect(EmitterType const * instance, ID i);

        static void send(EmitterType * instance, Args const &... args);
    };

    template <typename EmitterType, int type, typename... Args>
    std::unordered_map<EmitterType const *, std::vector<std::pair<unsigned int, std::function<void (int, EmitterType *, Args...)> > > > Object<EmitterType, type, Args...>::m_objectReceivers;

    template <int type, typename EmitterType, typename... Args>
    inline void send(EmitterType * emitter, Args const &... args) {

        Object<EmitterType, type, Args...>::send(emitter, args...);

    }

    //// Public /////////////////////////////////////////////////////////////////

    ID All::connect(std::function<void (int)> function) {

        CONNECT_IMPL (m_allReceivers);

    }

    void All::disconnect(ID i) {

        DISCONNECT_IMPL (m_allReceivers);

    }

    template <int type>
    ID Type<type>::connect(std::function<void (int)> function) {

        CONNECT_IMPL (m_typeReceivers);

    }

    template <int type>
    void Type<type>::disconnect(ID i) {

        DISCONNECT_IMPL (m_typeReceivers);

    }

    template <typename EmitterType, int type, typename... Args>
    inline ID Object<EmitterType, type, Args...>::connect(std::function<void (int, EmitterType *, Args...)> function) {

        return connect(nullptr, std::move(function));

    }

    template <typename EmitterType, int type, typename... Args>
    ID Object<EmitterType, type, Args...>::connect(EmitterType const * instance, std::function<void (int, EmitterType *, Args...)> function) {

        auto & vector = m_objectReceivers[instance];
        CONNECT_IMPL(vector)

    }

    template <typename EmitterType, int type, typename... Args>
    inline void Object<EmitterType, type, Args...>::disconnect(ID i) {

        disconnect(nullptr, i);

    }

    template <typename EmitterType, int type, typename... Args>
    inline void Object<EmitterType, type, Args...>::disconnect(EmitterType const * instance, ID i) {

        auto const iter = m_objectReceivers.find(instance);

        if (iter != m_objectReceivers.end()) {
            auto & vector = iter->second;
            DISCONNECT_IMPL(vector);
        }

    }

    template <typename EmitterType, int type, typename... Args>
    void Object<EmitterType, type, Args...>::send(EmitterType * instance, Args const &... args) {

        for (auto const & function: All::m_allReceivers)
            function.second(type);
        for (auto const & function: Type<type>::m_typeReceivers)
            function.second(type);

        auto const end = m_objectReceivers.cend();

        // dispatch the signal for all those that do not care about the instance
        auto iter = m_objectReceivers.find(nullptr);
        if (iter != end) {
            for (auto const & function: iter->second)
                function.second(type, instance, args...);
        }

        // then for those that do
        iter = m_objectReceivers.find(instance);
        if (iter != end) {
            for (auto const & function: iter->second)
                function.second(type, instance, args...);
        }

    }

    //// Protected //////////////////////////////////////////////////////////////

    //// Private ////////////////////////////////////////////////////////////////

    //// Functions //////////////////////////////////////////////////////////////

}

// #endif // SIGNAL_SIGNAL_H_
// signal.h --------------------------------------------------------------------


enum : int {
    EXEMPLE_SIGNAL = 0,
    EXEMPLE_SIGNAL_2,
    NEVER_SENT_EXEMPLE
};
class Exemple {};

DECLARE_SIGNAL(Exemple, EXEMPLE_SIGNAL, int)
DECLARE_SIGNAL(Exemple, EXEMPLE_SIGNAL_2, float)

int main()
{
   Exemple ex1;
   Exemple ex2;
   
   Signal::Object<Exemple, EXEMPLE_SIGNAL, int>::connect(&ex1, [] (int type, Exemple *, int value) {
       std::cout << "1 - Received object int " << type << " " << value << "\n";
   });
   Signal::Object<Exemple, EXEMPLE_SIGNAL_2, float>::connect(&ex1, [] (int type, Exemple *, float value) {
       std::cout << "2 - Received object float " << type << " " << value << "\n";
   });
   Signal::Object<Exemple, EXEMPLE_SIGNAL, int>::connect([] (int type, Exemple *, int value) {
       std::cout << "3 - Received object float " << type << " " << value << "\n";
   });
   Signal::Type<EXEMPLE_SIGNAL>::connect([] (int type) {
       std::cout << "4 - Received type " << type << "\n";
   });
   Signal::All::connect([] (int type) {
       std::cout << "5 - Received all " << type << "\n";
   });
   
   // uncomment for error
//   Signal::Object<Exemple, EXEMPLE_SIGNAL, float>::connect(&ex1, [] (int type, Exemple *, float value) {
//       std::cout << "Never received " << type << " " << value << "\n";
//   });
   
   Signal::Type<NEVER_SENT_EXEMPLE>::connect([] (int type) {
       std::cout << "Never received type " << type << "\n";
   });
   
   Signal::send<EXEMPLE_SIGNAL>(&ex1, 42); // received by all except 2
   std::cout << "\n";
   Signal::send<EXEMPLE_SIGNAL>(&ex2, 43); // received by 3, 4 and 5
   std::cout << "\n";
   Signal::send<EXEMPLE_SIGNAL_2>(&ex1, 44.4f); // received by 2 and 5
//   Signal::send<EXEMPLE_SIGNAL_2>(&ex1, 44); // error
   
   return 0;
}

