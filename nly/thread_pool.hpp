#ifndef NLY_THREAD_POOL
#define NLY_THREAD_POOL
#include "./upstream/BS_thread_pool.hpp"

namespace nly
{

// 提供了对 BS::thread_pool 的简单包装(尚有很多接口没暴露在此类中)
// 线程安全
class thread_pool
{
public:
  thread_pool(int thread_count = std::thread::hardware_concurrency())
    : m_pool(thread_count)
  {
  }

  // BS::thread_pool 的析构函数内部, 会自动调用其 wait 函数
  ~thread_pool()
  {
  }

public:
  // 以指定的优先级(priority 越大标识优先级越高), 向线程池提交一个任务
  // 返回一个 future, 可用于等待任务完成
  template<typename t_fun, typename t_result = std::invoke_result_t<std::decay_t<t_fun>>>
  [[nodiscard]] std::future<t_result> submit_task(t_fun&& task, std::uint8_t priority = 0)
  {
    return m_pool.submit_task(std::forward<t_fun>(task), priority);
  }

  // 以指定的优先级(priority 越大标识优先级越高), 向线程池提交一个任务
  // 不同于 submit_task, 该函数不会生成 future
  template<typename t_fun>
  void detach_task(t_fun&& task, const std::uint8_t priority = 0)
  {
    m_pool.detach_task(std::forward<t_fun>(task), priority);
  }

  // 等待所有已提交的任务完成
  // 注意: 不要在线程池的任务中调用此函数, 否则会造成死锁(会抛出异常)
  // 底层库还提供了 wait_for 和 wait_until
  void wait()
  {
    m_pool.wait();
  }

  // 清除所有处于等待执行的任务
  // 当前正在执行的任务不受影响
  void purge()
  {
    m_pool.purge();
  }

  // 获取处于等待状态的任务个数
  [[nodiscard]] std::size_t get_tasks_queued() const
  {
    return m_pool.get_tasks_queued();
  }

  // 获取正被执行的任务个数
  [[nodiscard]] std::size_t get_tasks_running() const
  {
    return m_pool.get_tasks_running();
  }

  // 获取未完成任务总数, 等于 get_tasks_queued() + get_tasks_running()
  [[nodiscard]] std::size_t get_tasks_total() const
  {
    return m_pool.get_tasks_total();
  }

  // 获取线程池底层线程个数
  [[nodiscard]] std::size_t get_thread_count() const noexcept
  {
    return m_pool.get_thread_count();
  }

private:
  // 目前不启用暂停线程池相关的功能
  BS::thread_pool<BS::tp::priority | BS::tp::wait_deadlock_checks> m_pool;
};

} // namespace nly

#endif // !NLY_THREAD_POOL
