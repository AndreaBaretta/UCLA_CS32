#include <iostream>
#include <string>
#include <chrono>
#include <functional>
#include <utility>
#include <vector>

template<typename Type, typename Ret, typename... Args>
struct member_func {
    template<Ret(Type::*func)(Args...)>
    static Ret func(Type& obj, Args&&... args) {
        return (obj.*func)(std::forward<args>...);
    }
};

struct A {
    A() {}
    bool doSomething(int i) {
        std::cout << "Bazinga: " << i << std::endl;
        return true;
    }
};

template <typename RetType, typename... Args>
static RetType run(std::function<RetType(Args... args)> f, Args... args_in) {
    return f(args_in...);
}

int main(){
    A a;
    static_assert(&member_func<A, bool, int>::func<A::doSomething>);
    run(&member_func<A, bool, int>::func<A::doSomething>, a, 10);
}