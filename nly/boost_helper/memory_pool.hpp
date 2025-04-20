#ifndef NLY_BOOST_HELPER_MEMORY_POOL
#define NLY_BOOST_HELPER_MEMORY_POOL
#include "boost/pool/pool.hpp"
#include "boost/signals2/dummy_mutex.hpp"

namespace nly
{
template<typename t_mutex = boost::signals2::dummy_mutex>
class memory_pool
{
public:
  /*
  形参说明:
    node_size: 单个内存块大小
    next_size: 内存池首次分配时, 分配的内存块个数, 该值不得为 0
    max_size:
      申请新内存时, 使用加倍算法
      第一次分配是 next_size, 接下来是 next_size * 2, 不断翻倍，直到达到 max_size
      该值为 0 标识不进行限制
  */
  memory_pool(size_t node_size, size_t next_size = 32, size_t max_size = 0)
    : m_pool(node_size, next_size, max_size)
  {
  }

public:
  // 内存耗尽时, 返回 0
  void* malloc()
  {
    std::lock_guard<t_mutex> guard(this->m_mutex);
    return m_pool.malloc();
  }

  void free(void* p)
  {
    std::lock_guard<t_mutex> guard(this->m_mutex);
    return m_pool.free(p);
  }

  /*
  功能: 若存在未使用的内存块, 则进行释放
  返回值: 若至少释放了一个内存块, 则返回 true
  */
  bool release_unused()
  {
    std::lock_guard<t_mutex> guard(this->m_mutex);
    return m_pool.release_memory();
  }

  /*
  功能: 释放所有内存块
  返回值: 若至少释放了一个内存块, 则返回 true
  */
  bool release_all()
  {
    std::lock_guard<t_mutex> guard(this->m_mutex);
    return m_pool.purge_memory();
  }

  // 返回单个内存块的大小
  size_t node_size()
  {
    std::lock_guard<t_mutex> guard(this->m_mutex);
    return m_pool.get_requested_size();
  }

private:
  boost::pool<> m_pool;
  t_mutex       m_mutex;
};

// 线程安全的内存池
typedef memory_pool<std::mutex> memory_pool_s;

} // namespace nly

#endif