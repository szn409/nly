#ifndef NLY_UTILITY
#define NLY_UTILITY
#include <functional>

namespace nly
{

// 功能: 保证在对象析构时, 调用一次传入的可执行对象
class release_guard
{
public:
  release_guard(std::function<void()> fun)
    : m_fun(fun)
  {
  }

  ~release_guard()
  {
    if (m_fun)
    {
      m_fun();
    }
  }

private:
  std::function<void()> m_fun;
};

} // namespace nly

#endif