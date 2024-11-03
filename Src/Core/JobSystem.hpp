#pragma once
#include <functional>

namespace HAIEngine::JobSystem
{
	struct JobDispatchArgs
	{
		uint32_t jobIndex;   // 任务索引
		uint32_t groupIndex; // 组索引 
	};
	
	void Initialize();

	// 添加任务
	void Execute(const std::function<void()>& job);

	/// <summary>
	/// 将一个的任务拆分成多个小任务
	/// </summary>
	/// <param name="jobCount">指定任务数量</param>
	/// <param name="groupSize">指定每个线程要执行的任务数量</param>
	/// <param name="job"></param>
	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);
	
	bool IsBusy();
	
	void Wait();
}