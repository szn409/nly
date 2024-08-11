#ifndef NLY_MEMORY_POOL
#define NLY_MEMORY_POOL
#include "boost/pool/pool.hpp"
#include "boost/signals2/dummy_mutex.hpp"

namespace nly
{
template<typename t_mutex = boost::signals2::dummy_mutex>
class memory_pool
{
public:
  /**
   * @param node_size The size of a single memory block requested.
   *
   * @param next_size The number of memory blocks requested from the system when the memory pool is
   * first allocated, this value cannot be 0.
   *
   * @param max_size The maximum number of memory blocks allocated at a time when the memory pool
   * allocates new memory, a value of 0 indicates that there is no limit.
   *
   * @note The new memory allocation strategy uses a doubling algorithm: the first allocation is
   * next_size, the next allocation is next_size * 2, and it continues doubling until it reaches
   * max_size.
   */
  memory_pool(size_t node_size, size_t next_size = 32, size_t max_size = 0)
    : pool_(node_size, next_size, max_size)
  {
  }

public:
  // returns 0 if out-of-memory.
  void* malloc()
  {
    std::lock_guard<t_mutex> guard(this->mutex_);
    return pool_.malloc();
  }

  void free(void* p)
  {
    std::lock_guard<t_mutex> guard(this->mutex_);
    return pool_.free(p);
  }

  // frees every memory block that doesn't have any allocated chunks.
  // return: true if at least one memory block was freed.
  bool release_unused()
  {
    std::lock_guard<t_mutex> guard(this->mutex_);
    return pool_.release_memory();
  }

  // frees every memory block.
  // true if at least one memory block was freed.
  bool release_all()
  {
    std::lock_guard<t_mutex> guard(this->mutex_);
    return pool_.purge_memory();
  }

  // the size of a single memory block requested.
  size_t node_size()
  {
    std::lock_guard<t_mutex> guard(this->mutex_);
    return pool_.get_requested_size();
  }

private:
  boost::pool<> pool_;
  t_mutex       mutex_;
};

// thread safe memory pool.
typedef memory_pool<std::mutex> memory_pool_s;

} // namespace nly

#endif