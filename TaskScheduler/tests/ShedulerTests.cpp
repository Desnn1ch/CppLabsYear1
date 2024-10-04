#include <lib/Sheduler.hpp>
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>

TEST(TaskSchedulerTest, WithoutArgs) {
    TaskScheduler scheduler;
    auto id = scheduler.Add([]() { return 42; });
    EXPECT_EQ(scheduler.GetResult<int>(id), 42);
}

TEST(TaskSchedulerTest, WithOneArg) {
    TaskScheduler scheduler;
    float arg = 3.14;
    auto id = scheduler.Add([](float x) { return x * x; }, arg);
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id), arg * arg);
}

TEST(TaskSchedulerTest, WithTwoArgs) {
    TaskScheduler scheduler;
    float arg1 = 2.5;
    float arg2 = 3.5;
    auto id = scheduler.Add([](float x, float y) { return x + y; }, arg1, arg2);
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id), arg1 + arg2);
}

TEST(TaskSchedulerTest, ExecuteAllTasks) {
    TaskScheduler scheduler;
    auto id1 = scheduler.Add([]() { return 42; });
    auto id2 = scheduler.Add([]() { return 3.14f; });
    auto id3 = scheduler.Add([]() { return "Hello world"; });
    scheduler.ExecuteAll();
    EXPECT_EQ(scheduler.GetResult<int>(id1), 42);
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id2), 3.14f);
    EXPECT_STREQ(scheduler.GetResult<const char*>(id3), "Hello world");
}

TEST(TaskSchedulerTest, GetFutureResult1) {
    TaskScheduler scheduler;
    float a = 2.0f;
    float b = 3.0f;
    auto id1 = scheduler.Add([](float a, float b) { return a + b; }, a, b);
    auto id2 = scheduler.Add([](float a) { return a * 2; }, scheduler.GetFutureResult<float>(id1));
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id1), a + b);
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id2), (a + b) * 2);
}

TEST(TaskSchedulerTest, GetFutureResult2) {
    TaskScheduler scheduler;
    float a = 2;
    float b = 10;
    auto id1 = scheduler.Add([](float a, float b){return a * b;}, a, b);
    auto id2 = scheduler.Add([](float b, float v){return b + v;}, b, scheduler.GetFutureResult<float>(id1));
    auto id3 = scheduler.Add([](float b, float v){return b + v;}, scheduler.GetFutureResult<float>(id2), scheduler.GetFutureResult<float>(id1));

    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id3), b + a * b + a * b);
}

TEST(TaskSchedulerTest, GetFutureResult3) {
    TaskScheduler scheduler;
    auto id1 = scheduler.Add([]() { return 42; });
    auto id2 = scheduler.Add([](int x) { return x * 2; }, scheduler.GetFutureResult<int>(id1));
    EXPECT_EQ(scheduler.GetResult<int>(id2), 84);
}

TEST(TaskSchedulerTest, ExecuteAllWithoutTasks) {
    TaskScheduler scheduler;
    scheduler.ExecuteAll();
    EXPECT_TRUE(true);
}

TEST(TaskSchedulerTest, QuadraticEquationSolver) {
    TaskScheduler scheduler;
    float a = 1;
    float b = -2;
    float c = 0;

    auto id1 = scheduler.Add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.Add([](float b, float v){return b * b + v;}, b, scheduler.GetFutureResult<float>(id1));
    auto id3 = scheduler.Add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));
    auto id4 = scheduler.Add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.GetFutureResult<float>(id2));
    auto id5 = scheduler.Add([](float a, float v){return v / (2 * a);}, a, scheduler.GetFutureResult<float>(id3));
    auto id6 = scheduler.Add([](float a, float v){return v / (2 * a);}, a, scheduler.GetFutureResult<float>(id4));

    scheduler.ExecuteAll();
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id6), 0.0f);
    EXPECT_FLOAT_EQ(scheduler.GetResult<float>(id5), 2.0f);
}

TEST(TaskSchedulerTest, ArithmeticOperations) {
    TaskScheduler scheduler;
    int a = 10;
    int b = 5;

    auto id1 = scheduler.Add([](int a, int b){ return a + b; }, a, b);
    auto id2 = scheduler.Add([](int a, int b){ return a - b; }, a, b);
    auto id3 = scheduler.Add([](int a, int b){ return a * b; }, a, b);
    auto id4 = scheduler.Add([](int a, int b){ return a / b; }, a, b);
    scheduler.ExecuteAll();
    EXPECT_EQ(scheduler.GetResult<int>(id1), 15);
    EXPECT_EQ(scheduler.GetResult<int>(id2), 5);
    EXPECT_EQ(scheduler.GetResult<int>(id3), 50);
    EXPECT_EQ(scheduler.GetResult<int>(id4), 2);
}

TEST(TaskSchedulerTest, GetFutureResultExample) {
    TaskScheduler scheduler;
    int a = 5;
    int b = 7;
    auto id1 = scheduler.Add([](int a, int b){ return a + b; }, a, b);
    auto id2 = scheduler.Add([](int sum){ return sum * sum; }, scheduler.GetFutureResult<int>(id1));
    scheduler.ExecuteAll();
    EXPECT_EQ(scheduler.GetResult<int>(id2), 144);
}