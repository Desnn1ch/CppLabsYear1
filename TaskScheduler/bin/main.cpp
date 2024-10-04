#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <lib/Sheduler.hpp>

int main() {

    float a = 1;
    float b = -2;
    float c = 0;
    int g = 4;

    TaskScheduler scheduler;

    auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.Add([](float b, float v){return b * b + v;}, b, scheduler.GetFutureResult<float>(id1));
    auto id3 = scheduler.Add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));
    auto id4 = scheduler.Add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));
    auto id5 = scheduler.Add([](float a, float v) {return v/(2*a);}, a, scheduler.GetFutureResult<float>(id3));
    auto id6 = scheduler.Add([](float a, float v) {return v/(2*a);},a, scheduler.GetFutureResult<float>(id4));
    auto id7 = scheduler.Add([](float a, float v) {return v/(a);},scheduler.GetFutureResult<float>(id5), scheduler.GetFutureResult<float>(id2));
    auto id8 = scheduler.Add([](float a) {return 11*a;},scheduler.GetFutureResult<float>(id5));
    auto id10 = scheduler.Add([](int a) {return 45*a;},10);

    std::cout << scheduler.GetResult<float>(id5) << '\n';
    scheduler.ExecuteAll();
    std::cout << "id5 = " << scheduler.GetResult<float>(id5) << std::endl;
    std::cout << "id2 = " << scheduler.GetResult<float>(id2) << std::endl;
    std::cout << "id7 = " << scheduler.GetResult<float>(id7) << std::endl;
    std::cout << "id7 = " << scheduler.GetResult<float>(id7) << std::endl;
    std::cout << "id8 = " << scheduler.GetResult<float>(id8) << std::endl;
    std::cout << "id10 = " << scheduler.GetResult<int>(id10) << std::endl;

    return 0;
}
