#ifndef NLY_POOL
#define NLY_POOL
#include "./utility.hpp"
#include <algorithm>
#include <cassert>
#include <vector>
#include <mutex>
#include <new>

namespace nly
{

/*
功能: 实现了一个对象池子, 可高效复用对象
注意:
  1. 可以被作为内存池, 输入的类型为 std::array<char, N> 即可
  2. 默认使用 nly::dummy_mutex, 不提供线程安全支持
  3. t_object 必须支持默认构造
*/
template<typename t_object, typename t_mutex = nly::dummy_mutex>
class pool : private nly::noncopyable
{
public:
  // 默认构造的池子不会预分配任何资源, 可以后续调用 resize 或 get_one 来获取资源
  pool()
  {
  }

  ~pool()
  {
    {
      std::lock_guard guard(m_lock);

      std::for_each(m_all.begin(), m_all.end(), [](t_object* object) { delete object; });
      m_all.clear();
      m_available.clear();
    }
  }

public:
  /*
  功能: 获取一个可用对象, 若失败则返回 nullptr
  形参:
    allow_new: 是否允许在资源不足时, 内部使用 new 来分配一个新对象
  */
  t_object* get_one(bool allow_new = true)
  {
    std::lock_guard guard(m_lock);

    if (!m_available.empty())
    {
      auto output = m_available.back();
      m_available.pop_back();
      return output;
    }

    if (allow_new)
    {
      auto output = new (std::nothrow) t_object;
      if (output)
      {
        m_all.emplace_back(output);
      }

      return output;
    }

    return nullptr;
  }

  /*
  功能: 回收传入的对象
  注意:
    1. 出于效率考虑, 不会对输入的对象进行验证,
    2. 传入的对象必须来自 get_one, 否则行为未定义
  */
  void release_one(const t_object* object)
  {
    std::lock_guard guard(m_lock);
    m_available.emplace_back(const_cast<t_object*>(object));
  }

  /*
  功能: 回收所有已使用的对象
  注意: 任何来自 get_one 的对象均不应该再被使用, 否则行为是未定义的
  */
  void release_all()
  {
    std::lock_guard guard(m_lock);
    m_available = m_all;
  }

  /*
  返回值说明:
    1. 当扩大池子时, 除非内存分配失败, 否则总是返回 true
    2. 当缩小池子时
         A. 出于效率考虑, 若用户仍持有来自 get_one 的资源未归还, 则固定返回 false
         B. 若用户已归还了所有资源, 则固定返回 true
  */
  bool resize(size_t size)
  {
    std::lock_guard guard(m_lock);

    if (size >= m_all.size())
    {
      m_available.reserve(size);
      m_all.reserve(size);

      while (m_all.size() < size)
      {
        auto object = new (std::nothrow) t_object;
        if (!object)
        {
          return false;
        }
        m_available.emplace_back(object);
        m_all.emplace_back(object);
      }

      return true;
    }
    else
    {
      if (m_available.size() != m_all.size())
      {
        return false;
      }

      while (m_all.size() != size)
      {
        auto object = m_all.back();
        delete object;
        m_all.pop_back();
      }

      m_available = m_all;
      return true;
    }
  }

  // 返回可用的资源个数
  size_t get_available_size() const
  {
    std::lock_guard guard(m_lock);
    return m_available.size();
  }

  // 返回未归还的资源个数
  size_t get_used_size() const
  {
    std::lock_guard guard(m_lock);
    assert(m_all.size() >= m_available.size());
    return m_all.size() - m_available.size();
  }

  // 返回所有资源个数(包括已使用的和未使用的)
  size_t get_total_size() const
  {
    std::lock_guard guard(m_lock);
    return m_all.size();
  }

private:
  mutable t_mutex m_lock;

  // 可用资源
  std::vector<t_object*> m_available;

  // 已分配的所有资源
  std::vector<t_object*> m_all;
};

// 提供了线程安全的 pool
template<typename t_object>
using pool_s = class pool<t_object, std::mutex>;

} // namespace nly

#endif