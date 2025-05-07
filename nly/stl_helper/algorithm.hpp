#ifndef NLY_STL_HELPER_ALGORITHM
#define NLY_STL_HELPER_ALGORITHM
#include <algorithm>
#include <execution>

namespace nly
{

#define NLY_ALGORITHM_HELPER(fun_name, container, fun, parallel, return_mark)                      \
  if (parallel)                                                                                    \
  {                                                                                                \
    return_mark std::fun_name(std::execution::par, container.begin(), container.end(), fun);       \
  }                                                                                                \
  else                                                                                             \
  {                                                                                                \
    return_mark std::fun_name(container.begin(), container.end(), fun);                            \
  }

#define NLY_ALGORITHM_HELPER_NOT_RETURN(fun_name, container, fun, parallel)                        \
  NLY_ALGORITHM_HELPER(fun_name, container, fun, parallel, (void))

#define NLY_ALGORITHM_HELPER_NEED_RETURN(fun_name, container, fun, parallel)                       \
  NLY_ALGORITHM_HELPER(fun_name, container, fun, parallel, return)

template<typename t_container, typename t_fun>
void for_each(t_container& container, t_fun fun, bool parallel = false)
{
  NLY_ALGORITHM_HELPER_NOT_RETURN(for_each, container, fun, parallel);
}

// container 为空, 则返回 true
template<typename t_container, typename t_fun>
bool all_of(t_container& container, t_fun fun, bool parallel = false)
{
  NLY_ALGORITHM_HELPER_NEED_RETURN(all_of, container, fun, parallel);
}

// container 为空, 则返回 false
template<typename t_container, typename t_fun>
bool any_of(t_container& container, t_fun fun, bool parallel = false)
{
  NLY_ALGORITHM_HELPER_NEED_RETURN(any_of, container, fun, parallel);
}

// container 为空, 则返回 true
template<typename t_container, typename t_fun>
bool none_of(t_container& container, t_fun fun, bool parallel = false)
{
  NLY_ALGORITHM_HELPER_NEED_RETURN(none_of, container, fun, parallel);
}

template<typename t_container, typename t_value>
typename t_container::iterator find(
  t_container&   container,
  const t_value& value,
  bool           parallel = false)
{
  NLY_ALGORITHM_HELPER_NEED_RETURN(find, container, value, parallel);
}
template<typename t_container, typename t_value>
typename t_container::const_iterator find(
  const t_container& container,
  const t_value&     value,
  bool               parallel = false)
{
  NLY_ALGORITHM_HELPER_NEED_RETURN(find, container, value, parallel);
}

} // namespace nly

#endif