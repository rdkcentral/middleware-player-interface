/*
* If not stated otherwise in this file or this component's license file the
* following copyright and licenses apply:
*
* Copyright 2022 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "PlayerTelemetry2.hpp"
#include "FakeTelemetry.h"
#include <gtest/gtest.h>
#include <atomic>
#include <future>

using namespace std::chrono_literals;

namespace
{
template <typename Pred> bool waitFor(Pred pred, std::chrono::milliseconds timeout = 2000ms)
{
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (!pred())
    {
        if (std::chrono::steady_clock::now() > deadline)
        {
            return false;
        }
        std::this_thread::sleep_for(5ms);
    }
    return true;
}

size_t fakeRecordCount()
{
    std::lock_guard<std::mutex> lock(fakeTelemetry().mutex);
    return fakeTelemetry().markers.size();
}

struct Recorder
{
    std::mutex mutex;
    std::vector<std::string> markers;
    size_t size()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return markers.size();
    }
};
} // namespace

TEST(PlayerTelemetryWorkerTest, EnqueueDoesNotWaitForSlowSender)
{
    Recorder recorder;
    PlayerTelemetryWorker worker(
        [&](const PlayerTelemetryWorker::Item &item)
        {
            std::this_thread::sleep_for(300ms);
            std::lock_guard<std::mutex> lock(recorder.mutex);
            recorder.markers.push_back(item.marker);
            return true;
        },
        nullptr, nullptr);

    const auto start = std::chrono::steady_clock::now();
    EXPECT_TRUE(worker.enqueue({"BUFFERING_STARTED", "{}", false}));
    EXPECT_LT(std::chrono::steady_clock::now() - start, 50ms);
    EXPECT_TRUE(waitFor([&] { return recorder.size() == 1; }));
}

TEST(PlayerTelemetryWorkerTest, SendsInOrder)
{
    Recorder recorder;
    PlayerTelemetryWorker worker(
        [&](const PlayerTelemetryWorker::Item &item)
        {
            std::lock_guard<std::mutex> lock(recorder.mutex);
            recorder.markers.push_back(item.marker);
            return true;
        },
        nullptr, nullptr);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(worker.enqueue({"M" + std::to_string(i), "{}", false}));
    }
    ASSERT_TRUE(waitFor([&] { return recorder.size() == 5; }));
    EXPECT_EQ(recorder.markers, (std::vector<std::string>{"M0", "M1", "M2", "M3", "M4"}));
}

TEST(PlayerTelemetryWorkerTest, DisablesAfterConsecutiveSlowCalls)
{
    std::atomic<int> calls{0};
    PlayerTelemetryWorker worker(
        [&](const PlayerTelemetryWorker::Item &)
        {
            std::this_thread::sleep_for(80ms);
            ++calls;
            return true;
        },
        nullptr, nullptr, 64, 50ms, 2);
    EXPECT_TRUE(worker.enqueue({"A", "{}", false}));
    EXPECT_TRUE(worker.enqueue({"B", "{}", false}));
    ASSERT_TRUE(waitFor([&] { return worker.isDisabled(); }));
    EXPECT_EQ(calls.load(), 2);
    EXPECT_FALSE(worker.enqueue({"C", "{}", false}));
    std::this_thread::sleep_for(150ms);
    EXPECT_EQ(calls.load(), 2);
}

TEST(PlayerTelemetryWorkerTest, FastCallResetsSlowCounter)
{
    std::atomic<int> calls{0};
    PlayerTelemetryWorker worker(
        [&](const PlayerTelemetryWorker::Item &item)
        {
            if (item.marker == "slow")
            {
                std::this_thread::sleep_for(80ms);
            }
            ++calls;
            return true;
        },
        nullptr, nullptr, 64, 50ms, 2);
    EXPECT_TRUE(worker.enqueue({"slow", "{}", false}));
    EXPECT_TRUE(worker.enqueue({"fast", "{}", false}));
    EXPECT_TRUE(worker.enqueue({"slow", "{}", false}));
    ASSERT_TRUE(waitFor([&] { return calls.load() == 3; }));
    EXPECT_FALSE(worker.isDisabled());
    EXPECT_TRUE(worker.enqueue({"fast", "{}", false}));
}

TEST(PlayerTelemetryWorkerTest, DropsWhenQueueIsFull)
{
    std::promise<void> started;
    std::promise<void> release;
    std::shared_future<void> releaseFuture = release.get_future().share();
    std::atomic<bool> first{true};
    PlayerTelemetryWorker worker(
        [&](const PlayerTelemetryWorker::Item &)
        {
            if (first.exchange(false))
            {
                started.set_value();
                releaseFuture.wait();
            }
            return true;
        },
        nullptr, nullptr, 2, 10000ms, 2);
    EXPECT_TRUE(worker.enqueue({"blocking", "{}", false}));
    started.get_future().wait();
    EXPECT_TRUE(worker.enqueue({"q1", "{}", false}));
    EXPECT_TRUE(worker.enqueue({"q2", "{}", false}));
    EXPECT_FALSE(worker.enqueue({"dropped", "{}", false}));
    release.set_value();
}

TEST(PlayerTelemetryWorkerTest, InitAndUninitRunOnWorkerThread)
{
    std::promise<std::thread::id> initThread;
    std::future<std::thread::id> initThreadFuture = initThread.get_future();
    std::atomic<bool> uninitCalled{false};
    {
        PlayerTelemetryWorker worker([](const PlayerTelemetryWorker::Item &) { return true; },
                                     [&] { initThread.set_value(std::this_thread::get_id()); },
                                     [&] { uninitCalled = true; });
        EXPECT_TRUE(worker.enqueue({"A", "{}", false}));
        ASSERT_EQ(initThreadFuture.wait_for(2s), std::future_status::ready);
    }
    EXPECT_NE(initThreadFuture.get(), std::this_thread::get_id());
    EXPECT_TRUE(uninitCalled.load());
}

TEST(PlayerTelemetry2Test, SendReturnsBeforeSlowT2Call)
{
    fakeTelemetry().blockMs = 300;
    PlayerTelemetry2 telemetry;
    std::map<std::string, int> intData{{"frames", 3}};
    std::map<std::string, std::string> stringData{{"state", "started"}};
    std::map<std::string, float> floatData;
    const size_t initialCount = fakeRecordCount();

    const auto start = std::chrono::steady_clock::now();
    EXPECT_TRUE(telemetry.send("BUFFERING_STARTED", intData, stringData, floatData));
    EXPECT_LT(std::chrono::steady_clock::now() - start, 50ms);

    ASSERT_TRUE(waitFor(
        [&]
        {
            std::lock_guard<std::mutex> lock(fakeTelemetry().mutex);
            return fakeTelemetry().markers.size() > initialCount;
        }));
    std::lock_guard<std::mutex> lock(fakeTelemetry().mutex);
    EXPECT_EQ(fakeTelemetry().markers.back(), "BUFFERING_STARTED");
    EXPECT_NE(fakeTelemetry().values.back().find("\"state\":\"started\""), std::string::npos) << fakeTelemetry().values.back();
    fakeTelemetry().blockMs = 0;
}

TEST(PlayerTelemetry2Test, SendEventUsesEventApi)
{
    PlayerTelemetry2 telemetry;
    const size_t initialCount = fakeRecordCount();
    telemetry.sendEvent("INTERFACE_INITIALIZED");
    ASSERT_TRUE(waitFor(
        [&]
        {
            std::lock_guard<std::mutex> lock(fakeTelemetry().mutex);
            return fakeTelemetry().markers.size() > initialCount;
        }));
    std::lock_guard<std::mutex> lock(fakeTelemetry().mutex);
    EXPECT_EQ(fakeTelemetry().markers.back(), "INTERFACE_INITIALIZED");
    EXPECT_EQ(fakeTelemetry().values.back(), "1");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
