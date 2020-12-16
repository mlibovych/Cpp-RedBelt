#include "test_runner.h"

#include <numeric>
#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(move(initial)) {

    }

    struct Access {
        lock_guard<mutex> guard;
        T& ref_to_value;
        
    };

    Access GetAccess() {
        return {lock_guard(m), value};
    }
private:
    T value;
    mutex m;
};