#pragma once

// Simple thread pool for the Box2D v3 task system.
// Header only. No external libraries.
//
// Use: set these fields in b2WorldDef before b2CreateWorld:
//   worldDef.workerCount     = pool->WorkerCount();
//   worldDef.enqueueTask     = &TaskPool::Enqueue;
//   worldDef.finishTask      = &TaskPool::Finish;
//   worldDef.userTaskContext = pool;
//
// The pool must stay alive for as long as the world exists.
// Box2D calls Enqueue and Finish from the thread that calls b2World_Step.

#include <box2d/box2d.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

namespace sg
{
    class TaskPool
    {
    public:
        // workerCount includes the calling thread.
        // The pool starts (workerCount - 1) background threads.
        explicit TaskPool(uint32_t workerCount)
            : workerCount_(std::max(1u, workerCount))
        {
            threads_.reserve(workerCount_ - 1);
            for (uint32_t i = 1; i < workerCount_; ++i)
            {
                threads_.emplace_back([this, i]() { WorkerLoop(i); });
            }
        }

        ~TaskPool()
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                stop_ = true;
            }
            condition_.notify_all();
            for (std::thread& thread : threads_)
            {
                thread.join();
            }
        }

        TaskPool(const TaskPool&) = delete;
        TaskPool& operator=(const TaskPool&) = delete;

        uint32_t WorkerCount() const { return workerCount_; }

        // Box2D callback: b2EnqueueTaskCallback
        static void* Enqueue(b2TaskCallback* task, int itemCount, int minRange,
                             void* taskContext, void* userContext)
        {
            TaskPool* pool = static_cast<TaskPool*>(userContext);
            return pool->EnqueueImpl(task, itemCount, minRange, taskContext);
        }

        // Box2D callback: b2FinishTaskCallback
        static void Finish(void* userTask, void* userContext)
        {
            if (userTask == nullptr)
            {
                return; // Enqueue ran the work inline.
            }
            TaskPool* pool = static_cast<TaskPool*>(userContext);
            pool->FinishImpl(static_cast<Task*>(userTask));
        }

    private:
        struct Task
        {
            std::atomic<int> remaining{0};
        };

        struct Job
        {
            b2TaskCallback* function = nullptr;
            int start = 0;
            int end = 0;
            void* context = nullptr;
            Task* task = nullptr;
        };

        void* EnqueueImpl(b2TaskCallback* function, int itemCount, int minRange,
                          void* taskContext)
        {
            if (itemCount <= 0)
            {
                return nullptr;
            }

            minRange = std::max(1, minRange);

            // Run small tasks on the calling thread (worker 0).
            // Run all tasks on the calling thread if there are no background threads.
            if (itemCount <= minRange || workerCount_ == 1)
            {
                function(0, itemCount, 0, taskContext);
                return nullptr;
            }

            int maxParts = (itemCount + minRange - 1) / minRange;
            int partCount = std::min<int>(static_cast<int>(workerCount_), maxParts);
            int baseSize = itemCount / partCount;
            int extra = itemCount % partCount;

            Task* task = new Task();
            task->remaining.store(partCount, std::memory_order_relaxed);

            {
                std::lock_guard<std::mutex> lock(mutex_);
                int start = 0;
                for (int i = 0; i < partCount; ++i)
                {
                    int size = baseSize + (i < extra ? 1 : 0);
                    jobs_.push_back(Job{function, start, start + size, taskContext, task});
                    start += size;
                }
            }
            condition_.notify_all();

            return task;
        }

        void FinishImpl(Task* task)
        {
            // The calling thread (worker 0) helps to run jobs while it waits.
            while (task->remaining.load(std::memory_order_acquire) > 0)
            {
                Job job;
                if (TryPop(job))
                {
                    Run(job, 0);
                }
                else
                {
                    std::this_thread::yield();
                }
            }
            delete task;
        }

        bool TryPop(Job& job)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (jobs_.empty())
            {
                return false;
            }
            job = jobs_.front();
            jobs_.pop_front();
            return true;
        }

        static void Run(const Job& job, uint32_t workerIndex)
        {
            job.function(job.start, job.end, workerIndex, job.context);
            // Do not use job.task after this line. The waiting thread can delete it.
            job.task->remaining.fetch_sub(1, std::memory_order_release);
        }

        void WorkerLoop(uint32_t workerIndex)
        {
            for (;;)
            {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    condition_.wait(lock, [this]() { return stop_ || !jobs_.empty(); });
                    if (stop_ && jobs_.empty())
                    {
                        return;
                    }
                    job = jobs_.front();
                    jobs_.pop_front();
                }
                Run(job, workerIndex);
            }
        }

        uint32_t workerCount_;
        std::vector<std::thread> threads_;
        std::deque<Job> jobs_;
        std::mutex mutex_;
        std::condition_variable condition_;
        bool stop_ = false;
    };
}