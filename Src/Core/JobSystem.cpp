#include "JobSystem.hpp"    // include our interface

#include <algorithm>    // std::max
#include <atomic>    // to use std::atomic<uint64_t>
#include <thread>    // to use std::thread
#include <condition_variable>    // to use std::condition_variable
#include <sstream>
#include <assert.h>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

// 线程安全的环形容器（双端队列）
template <typename T, size_t capacity>
class ThreadSafeRingBuffer
{
public:

	inline bool push_back(const T& item)
	{
		bool result = false;
		lock.lock();
		size_t next = (head + 1) % capacity;
		if (next != tail)
		{
			data[head] = item;
			head = next;
			result = true;
		}
		lock.unlock();
		return result;
	}

	inline bool pop_front(T& item)
	{
		bool result = false;
		lock.lock();
		if (tail != head)
		{
			item = data[tail];
			tail = (tail + 1) % capacity;
			result = true;
		}
		lock.unlock();
		return result;
	}

private:
	T data[capacity];
	size_t head = 0;
	size_t tail = 0;
	std::mutex lock;
};

namespace HAIEngine::JobSystem
{
	uint32_t numThreads = 0;
	ThreadSafeRingBuffer<std::function<void()>, 256> jobPool;
	std::condition_variable wakeCondition;
	std::mutex wakeMutex;
	uint64_t currentLabel = 0;
	std::atomic<uint64_t> finishedLabel;


	void Initialize()
	{
		finishedLabel.store(0);

		auto numCores = std::thread::hardware_concurrency();

		numThreads = std::max(1u, numCores);

		for (uint32_t threadID = 0; threadID < numThreads; ++threadID)
		{
			std::thread worker([] {

				std::function<void()> job;

				while (true)
				{
					if (jobPool.pop_front(job))
					{
						job();
						finishedLabel.fetch_add(1);
					}
					else
					{
						std::unique_lock<std::mutex> lock(wakeMutex);
						wakeCondition.wait(lock);
					}
				}

				});

#ifdef _WIN32
			// Do Windows-specific thread setup:
			HANDLE handle = (HANDLE)worker.native_handle();

			// Put each thread on to dedicated core
			DWORD_PTR affinityMask = 1ull << threadID;
			DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
			assert(affinity_result > 0);

			//// Increase thread priority:
			//BOOL priority_result = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
			//assert(priority_result != 0);

			// Name the thread:
			std::wstringstream wss;
			wss << "JobSystem_" << threadID;
			HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
			assert(SUCCEEDED(hr));
#endif // _WIN32

			worker.detach(); // forget about this thread, let it do it's job in the infinite loop that we created above
		}
	}


	inline void poll()
	{
		wakeCondition.notify_one();
		std::this_thread::yield();
	}

	void Execute(const std::function<void()>& job)
	{
		currentLabel += 1;

		while (!jobPool.push_back(job)) { poll(); }

		wakeCondition.notify_one();
	}

	bool IsBusy()
	{
		return finishedLabel.load() < currentLabel;
	}

	void Wait()
	{
		while (IsBusy()) { poll(); }
	}

	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job)
	{
		if (jobCount == 0 || groupSize == 0)
		{
			return;
		}

		const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

		currentLabel += groupCount;

		for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex)
		{
			const auto& jobGroup = [jobCount, groupSize, job, groupIndex]() {

				const uint32_t groupJobOffset = groupIndex * groupSize;
				const uint32_t groupJobEnd = std::min(groupJobOffset + groupSize, jobCount);

				JobDispatchArgs args;
				args.groupIndex = groupIndex;

				for (uint32_t i = groupJobOffset; i < groupJobEnd; ++i)
				{
					args.jobIndex = i;
					job(args);
				}
				};

			while (!jobPool.push_back(jobGroup)) { poll(); }

			wakeCondition.notify_one();
		}
	}
}