#ifndef NLY_STL_HELPER_ALGORITHM
#define NLY_STL_HELPER_ALGORITHM
#include <algorithm>
#include <execution>
#include <numeric>
#include <random>
#include <vector>
#include <limits>

namespace nly
{
/*
关于算法库中比较函数的说明
1. 其必须满足严格弱序的要求
2. 默认使用 <, 以 sort 举例, 对应的结果是非降序
3. 还是拿 sort 举例, 若实现的 比较函数 使用 >= 是非法的, 以下代码会崩溃(msvc 2019 debug环境测试)
  std::vector<int> vec{ 1, 2, 2, 3 };
  std::sort(vec.begin(), vec.end(), [](int a, int b) { return a >= b; });
4. 上述例子, 正确的做法是使用 > 而不是 >=
*/

// 注意, 虽然很多函数接受并行参数, 但是底层不一定真的实现(比如 msvc 2019 有些实现直接忽略了并行参数)

#ifndef NLY_RANGE
#define NLY_RANGE(container) std::begin(container), std::end(container)
#endif

#ifndef NLY_EXECUTION_PAR
#define NLY_EXECUTION_PAR std::execution::par
#endif // !NLY_EXECUTION_PAR

template<typename t_container, typename t_unary_pred>
void for_each(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    std::for_each(NLY_EXECUTION_PAR, NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    std::for_each(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// for_each_n 未纳入

// 仅在谓语对范围中的所有元素返回 true 时，才返回 true
// 若 container 为空, 将返回 true
template<typename t_container, typename t_unary_pred>
bool all_of(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::all_of(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::all_of(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// 在谓语对范围中的至少一个元素返回 true 时，就返回 true
// 若 container 为空, 将返回 false
template<typename t_container, typename t_unary_pred>
bool any_of(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::any_of(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::any_of(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// 仅在谓语对范围中的所有元素返回 false 时，才返回 true
// 若 container 为空, 将返回 true
template<typename t_container, typename t_unary_pred>
bool none_of(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::none_of(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::none_of(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

template<typename t_container, typename t_value>
decltype(auto) find(t_container&& container, t_value&& value, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find(NLY_EXECUTION_PAR, NLY_RANGE(container), std::forward<t_value>(value));
  }
  else
  {
    return std::find(NLY_RANGE(container), std::forward<t_value>(value));
  }
}

template<typename t_container, typename t_unary_pred>
decltype(auto) find_if(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::find_if(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// 搜索第一个使得谓语返回 false 的元素
template<typename t_container, typename t_unary_pred>
decltype(auto) find_if_not(
  t_container&&  container,
  t_unary_pred&& unary_pred,
  bool           parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find_if_not(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::find_if_not(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

/*
在第一个容器中, 寻找第二个容器元素出现的最后位置
无谓语版本: 使用 operator =
经测试, msvc 2019 是从后向前找的，相对于标准库的要求，还是做了点优化的
*/
template<typename t_container_base, typename t_container_target>
decltype(auto) find_end(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find_end(NLY_EXECUTION_PAR, NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
  else
  {
    return std::find_end(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
decltype(auto) find_end(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find_end(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::find_end(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
在第一个容器中, 搜索第二个容器中任意元素首次出现(满足条件)的位置
无谓语版本: 使用 operator =
*/
template<typename t_container_base, typename t_container_target>
decltype(auto) find_first_of(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find_first_of(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target));
  }
  else
  {
    return std::find_first_of(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
decltype(auto) find_first_of(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::find_first_of(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::find_first_of(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
在容器中搜索两个连续满足条件的元素
无谓语版本: 使用 operator =
*/
template<typename t_container>
decltype(auto) adjacent_find(t_container&& container, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::adjacent_find(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::adjacent_find(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
decltype(auto) adjacent_find(
  t_container&&   container,
  t_binary_pred&& binary_pred,
  bool            parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::adjacent_find(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::adjacent_find(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

/*
返回容器中, 满足特定条件的元素个数
count: 使用 operator =
count_if: 使用一元谓语
*/
template<typename t_container, typename t_value>
size_t count(t_container&& container, t_value&& value, bool parallel = false)
{
  if (parallel)
  {
    return std::count(NLY_EXECUTION_PAR, NLY_RANGE(container), std::forward<t_value>(value));
  }
  else
  {
    return std::count(NLY_RANGE(container), std::forward<t_value>(value));
  }
}
template<typename t_container, typename t_unary_pred>
size_t count_if(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::count_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::count_if(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

/*
返回来自两个范围的首个不匹配元素对
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
注意:
  1. 此函数有其他版本的重载, 支持第二个容器仅传入起始迭代器, 此版本不进行兼容
  2. 目前兼容的版本, 支持两个容器 size 不一致
*/
template<typename t_container_base, typename t_container_target>
decltype(auto) mismatch(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");
  static_assert(
    std::is_lvalue_reference<decltype(container_target)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::mismatch(NLY_EXECUTION_PAR, NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
  else
  {
    return std::mismatch(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
decltype(auto) mismatch(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");
  static_assert(
    std::is_lvalue_reference<decltype(container_target)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::mismatch(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::mismatch(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
注意:
  1. 此函数有其他版本的重载, 支持第二个容器仅传入起始迭代器, 此版本不进行兼容
  2. 目前兼容的版本, 支持两个容器 size 不一致(返回 false)
*/
template<typename t_container_base, typename t_container_target>
bool equal(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    return std::equal(NLY_EXECUTION_PAR, NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
  else
  {
    return std::equal(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
bool equal(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    return std::equal(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::equal(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
将第二个容器视为子序列, 在第一个容器中进行搜索, 返回指向其首次出现的位置的迭代器
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
注意: 存在其他重载版本, 可指定不同的搜索算法, 未进行兼容
*/
template<typename t_container_base, typename t_container_target>
decltype(auto) search(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::search(NLY_EXECUTION_PAR, NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
  else
  {
    return std::search(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
decltype(auto) search(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container_base)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::search(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::search(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
在容器中, 搜索连续 count 个元素, 其值等于 value 或使得二元谓语成立
对于二元谓语
  第一个参数: 来自容器
  第二个参数: value
*/
template<typename t_container, typename t_value>
decltype(auto) search_n(
  t_container&& container,
  size_t        count,
  t_value&&     value,
  bool          parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (count > static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max()))
  {
    throw std::length_error("count too large for search_n");
  }

  if (parallel)
  {
    return std::search_n(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      static_cast<ptrdiff_t>(count),
      std::forward<t_value>(value));
  }
  else
  {
    return std::search_n(
      NLY_RANGE(container),
      static_cast<ptrdiff_t>(count),
      std::forward<t_value>(value));
  }
}
template<typename t_container, typename t_value, typename t_binary_pred>
decltype(auto) search_n(
  t_container&&   container,
  size_t          count,
  t_value&&       value,
  t_binary_pred&& binary_pred,
  bool            parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (count > static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max()))
  {
    throw std::length_error("count too large for search_n");
  }

  if (parallel)
  {
    return std::search_n(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      static_cast<ptrdiff_t>(count),
      std::forward<t_value>(value),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::search_n(
      NLY_RANGE(container),
      static_cast<ptrdiff_t>(count),
      std::forward<t_value>(value),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
注意: 当使用 std::copy, std::copy_backward 时
  A. 当目标位于源的左侧, 此时可以使用 copy, 不可以使用 copy_backward
  B. 当目标位于源的右侧, 应该使用 copy_backward, 而不是 copy
  C. move 和 move_backward 参考上述两条规则
*/
template<typename t_container_base, typename t_container_target, typename t_unary_pred>
void copy_if(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_unary_pred&&       unary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base));

    auto it = std::copy_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_unary_pred>(unary_pred));
    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::copy_if(
      NLY_RANGE(container_base),
      std::back_inserter(container_target),
      std::forward<t_unary_pred>(unary_pred));
  }
}

using std::iter_swap;
using std::swap;

template<typename t_iter_0, typename t_iter_1>
decltype(auto) swap_ranges(
  t_iter_0&& iter_0_first,
  t_iter_0&& iter_0_last,
  t_iter_1&& iter_1_first,
  bool       parallel = false)
{
  if (parallel)
  {
    return std::swap_ranges(
      NLY_EXECUTION_PAR,
      std::forward<t_iter_0>(iter_0_first),
      std::forward<t_iter_0>(iter_0_last),
      std::forward<t_iter_1>(iter_1_first));
  }
  else
  {
    return std::swap_ranges(
      std::forward<t_iter_0>(iter_0_first),
      std::forward<t_iter_0>(iter_0_last),
      std::forward<t_iter_1>(iter_1_first));
  }
}

template<typename t_container_base, typename t_container_target, typename t_unary_pred>
void transform(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_unary_pred&&       unary_pred,
  bool                 parallel = false)
{
  container_target.resize(std::size(container_base));

  if (parallel)
  {
    std::transform(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    std::transform(
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_unary_pred>(unary_pred));
  }
}

// 不同于 std::transform, 此函数支持两个容器大小不同
template<
  typename t_container_base_0,
  typename t_container_base_1,
  typename t_container_target,
  typename t_binary_pred>
void transform(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  container_target.resize((std::min)(std::size(container_base_0), std::size(container_base_1)));

  auto it = std::begin(container_base_0);
  std::advance(it, container_target.size());

  if (parallel)
  {
    std::transform(
      NLY_EXECUTION_PAR,
      std::begin(container_base_0),
      it,
      std::begin(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    std::transform(
      std::begin(container_base_0),
      it,
      std::begin(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));
  }
}

template<typename t_container, typename t_old_value, typename t_new_value>
void replace(
  t_container&& container,
  t_old_value&& old_value,
  t_new_value&& new_value,
  bool          parallel = false)
{
  if (parallel)
  {
    std::replace(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_old_value>(old_value),
      std::forward<t_new_value>(new_value));
  }
  else
  {
    std::replace(
      NLY_RANGE(container),
      std::forward<t_old_value>(old_value),
      std::forward<t_new_value>(new_value));
  }
}

template<typename t_container, typename t_unary_pred, typename t_new_value>
void replace_if(
  t_container&&  container,
  t_unary_pred&& unary_pred,
  t_new_value&&  new_value,
  bool           parallel = false)
{
  if (parallel)
  {
    std::replace_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred),
      std::forward<t_new_value>(new_value));
  }
  else
  {
    std::replace_if(
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred),
      std::forward<t_new_value>(new_value));
  }
}

template<
  typename t_container_base,
  typename t_container_target,
  typename t_old_value,
  typename t_new_value>
void replace_copy(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_old_value&&        old_value,
  t_new_value&&        new_value,
  bool                 parallel = false)
{
  container_target.resize(std::size(container_base));

  if (parallel)
  {
    std::replace_copy(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_old_value>(old_value),
      std::forward<t_new_value>(new_value));
  }
  else
  {
    std::replace_copy(
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_old_value>(old_value),
      std::forward<t_new_value>(new_value));
  }
}

template<
  typename t_container_base,
  typename t_container_target,
  typename t_unary_pred,
  typename t_new_value>
void replace_copy_if(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_unary_pred&&       unary_pred,
  t_new_value&&        new_value,
  bool                 parallel = false)
{
  container_target.resize(std::size(container_base));

  if (parallel)
  {
    std::replace_copy_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_unary_pred>(unary_pred),
      std::forward<t_new_value>(new_value));
  }
  else
  {
    std::replace_copy_if(
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_unary_pred>(unary_pred),
      std::forward<t_new_value>(new_value));
  }
}

// 此 hpp 声明的函数, 均操作整个容器, 所以 fill_n, generate_n 未纳入

// 使用一个固定值进行填充
template<typename t_container, typename t_value>
void fill(t_container&& container, t_value&& value, bool parallel = false)
{
  if (parallel)
  {
    std::fill(NLY_EXECUTION_PAR, NLY_RANGE(container), std::forward<t_value>(value));
  }
  else
  {
    std::fill(NLY_RANGE(container), std::forward<t_value>(value));
  }
}

// 使用一个固定值进行填充
template<typename t_container, typename t_unary_pred>
void generate(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    std::generate(NLY_EXECUTION_PAR, NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    std::generate(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// 会实际改变容器大小, 并返回实际移除的元素个数
template<typename t_container, typename t_value>
size_t remove(t_container&& container, t_value&& value, bool parallel = false)
{
  auto it = container.end();

  if (parallel)
  {
    it = std::remove(NLY_EXECUTION_PAR, NLY_RANGE(container), std::forward<t_value>(value));
  }
  else
  {
    it = std::remove(NLY_RANGE(container), std::forward<t_value>(value));
  }

  auto count = std::distance(it, container.end());
  container.erase(it, container.end());
  return count;
}

// 会实际改变容器大小, 并返回实际移除的元素个数
template<typename t_container, typename t_unary_pred>
size_t remove_if(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  auto it = container.end();

  if (parallel)
  {
    it = std::remove_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    it = std::remove_if(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }

  auto count = std::distance(it, container.end());
  container.erase(it, container.end());
  return count;
}

template<typename t_container_base, typename t_container_target, typename t_value>
void remove_copy(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_value&&            value,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base));

    auto it = std::remove_copy(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_value>(value));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::remove_copy(
      NLY_RANGE(container_base),
      std::back_inserter(container_target),
      std::forward<t_value>(value));
  }
}

template<typename t_container_base, typename t_container_target, typename t_unary_pred>
void remove_copy_if(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_unary_pred&&       unary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base));

    auto it = std::remove_copy_if(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_unary_pred>(unary_pred));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::remove_copy_if(
      NLY_RANGE(container_base),
      std::back_inserter(container_target),
      std::forward<t_unary_pred>(unary_pred));
  }
}

// 消除容器中，相继等价元素中(或使得一元谓语返回 true)，除首元素外的元素
// 会实际改变容器大小
template<typename t_container>
void unique(t_container&& container, bool parallel = false)
{
  auto it = container.end();
  if (parallel)
  {
    it = std::unique(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    it = std::unique(NLY_RANGE(container));
  }

  container.erase(it, container.end());
}
template<typename t_container, typename t_binary_pred>
void unique(t_container&& container, t_binary_pred&& binary_pred, bool parallel = false)
{
  auto it = container.end();
  if (parallel)
  {
    it = std::unique(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    it = std::unique(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }

  container.erase(it, container.end());
}

template<typename t_container_base, typename t_container_target>
void unique_copy(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base));

    auto it =
      std::unique_copy(NLY_EXECUTION_PAR, NLY_RANGE(container_base), container_target.begin());

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::unique_copy(NLY_RANGE(container_base), std::back_inserter(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
void unique_copy(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base));

    auto it = std::unique_copy(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::unique_copy(
      NLY_RANGE(container_base),
      std::back_inserter(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

// 反转容器中的元素
template<typename t_container>
void reverse(t_container&& container, bool parallel = false)
{
  if (parallel)
  {
    std::reverse(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    std::reverse(NLY_RANGE(container));
  }
}
template<typename t_container_base, typename t_container_target>
void reverse_copy(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  container_target.resize(std::size(container_base));

  if (parallel)
  {
    std::reverse_copy(NLY_EXECUTION_PAR, NLY_RANGE(container_base), container_target.begin());
  }
  else
  {
    std::reverse_copy(NLY_RANGE(container_base), container_target.begin());
  }
}

// rotate, rotate_copy 未纳入

/*
无谓语版本: 默认以非降序排序, 不保证相等元素的相对顺序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container>
void sort(t_container&& container, bool parallel = false)
{
  if (parallel)
  {
    return std::sort(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::sort(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
void sort(t_container&& container, t_binary_pred&& binary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::sort(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::sort(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

// sort 的稳定版本
template<typename t_container>
void stable_sort(t_container&& container, bool parallel = false)
{
  if (parallel)
  {
    return std::stable_sort(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::stable_sort(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
void stable_sort(t_container&& container, t_binary_pred&& binary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::stable_sort(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::stable_sort(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

// 检查容器中的元素，是否已排序
// 若容器为空, 则返回 true
template<typename t_container>
bool is_sorted(t_container&& container, bool parallel = false)
{
  if (parallel)
  {
    return std::is_sorted(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::is_sorted(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
bool is_sorted(t_container&& container, t_binary_pred&& binary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::is_sorted(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::is_sorted(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

// partial_sort, is_sorted_until, nth_element, 未纳入

/*
将两个有序范围，进行合并输出
该函数稳定，即在结果中，对于来自两个序列的等价元素，来自第一个范围的元素将先于来自第二个范围的元素
无谓语版本: 使用 operator < 进行排序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container_base_0, typename t_container_base_1, typename t_container_target>
void merge(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  container_target.resize(std::size(container_base_0) + std::size(container_base_1));

  if (parallel)
  {
    std::merge(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin());
  }
  else
  {
    std::merge(NLY_RANGE(container_base_0), NLY_RANGE(container_base_1), container_target.begin());
  }
}
template<
  typename t_container_base_0,
  typename t_container_base_1,
  typename t_container_target,
  typename t_binary_pred>
void merge(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  container_target.resize(std::size(container_base_0) + std::size(container_base_1));

  if (parallel)
  {
    std::merge(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    std::merge(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));
  }
}

// inplace_merge 未纳入

// random_shuffle 自 C++17 起移除, 故而未纳入

// 重排容器中的元素
// 由于使用了 std::random_device, 对于相同的输入, 每次的输出均会不同
template<typename t_container>
void shuffle(t_container&& container)
{
  std::random_device rd;
  std::mt19937       mt(rd());
  std::shuffle(NLY_RANGE(container), mt);
}

// 从容器中不重复的选择 sample_count 个元素, 使得每个元素有相同出现概率, 输出到 container_target
// 若 sample_count 大于容器的 size, 则容器中所有元素都将被选择
// 由于使用了 std::random_device, 对于相同的输入, 每次的输出均会不同
template<typename t_container_base, typename t_container_target>
void sample(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  size_t               sample_count)
{
  container_target.resize((std::min)(std::size(container_base), sample_count));
  std::random_device rd;
  std::mt19937       mt(rd());
  std::sample(NLY_RANGE(container_base), std::begin(container_target), sample_count, mt);
}

/*
不接受谓语的版本: 随机选择一个元素, 并返回其迭代器
接受谓语的版本: 将所有使得谓语返回 true 的元素视为一个集合, 从该集合随机选取一个元素, 并返回其迭代器
*/
template<typename t_container>
decltype(auto) random_pick(t_container&& container)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (std::empty(container))
  {
    return std::end(container);
  }

  std::random_device e;
  auto ui = std::uniform_int_distribution<size_t>(static_cast<size_t>(0), std::size(container) - 1);
  auto select_index = ui(e);

  auto it = std::begin(container);
  std::advance(it, select_index);
  return it;
}
template<typename t_container, typename t_unary_pred>
decltype(auto) random_pick(t_container&& container, t_unary_pred&& unary_pred)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  auto it_begin = std::begin(container);
  auto it_end = std::end(container);

  if (std::empty(container))
  {
    return it_end;
  }

  std::vector<decltype(it_begin)> vec;

  for (auto it = it_begin; it != it_end; ++it)
  {
    if (unary_pred(*it))
    {
      vec.emplace_back(it);
    }
  }

  if (vec.empty())
  {
    return it_end;
  }

  auto result = *random_pick(vec);
  return result;
}

// 重排序容器中的元素，使得谓语返回 true 的元素, 位于使得谓语返回 false 的元素之前（不保持相对顺序）
// 返回值：指向划分点的迭代器, 即第一个不满足一元谓语的元素
template<typename t_container, typename t_unary_pred>
decltype(auto) partition(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::partition(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::partition(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// partition 的稳定版本
template<typename t_container, typename t_unary_pred>
decltype(auto) stable_partition(
  t_container&&  container,
  t_unary_pred&& unary_pred,
  bool           parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::stable_partition(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::stable_partition(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

/*
参数说明:
  第一个参数: 待划分的容器
  第二个参数: 保存使得谓语为 true 的元素
  第三个参数: 保存使得谓语为 false 的元素
  第四个参数: 一元谓语
*/
template<
  typename t_container_base,
  typename t_container_for_true,
  typename t_container_for_false,
  typename t_unary_pred>
void partition_copy(
  t_container_base&&      container_base,
  t_container_for_true&&  container_for_true,
  t_container_for_false&& container_for_false,
  t_unary_pred&&          unary_pred,
  bool                    parallel = false)
{
  if (parallel)
  {
    container_for_true.resize(std::size(container_base));
    container_for_false.resize(std::size(container_base));
    auto [it0, it1] = std::partition_copy(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      container_for_true.begin(),
      container_for_false.begin(),
      std::forward<t_unary_pred>(unary_pred));
    container_for_true.erase(it0, container_for_true.end());
    container_for_false.erase(it1, container_for_false.end());
  }
  else
  {
    container_for_true.clear();
    container_for_false.clear();
    std::partition_copy(
      NLY_RANGE(container_base),
      std::back_inserter(container_for_true),
      std::back_inserter(container_for_false),
      std::forward<t_unary_pred>(unary_pred));
  }
}

// 返回容器是否已划分, 即使得谓语返回 true 的元素, 位于使得谓语返回 false 的元素之前
// 容器为空也返回 true
template<typename t_container, typename t_unary_pred>
bool is_partitioned(t_container&& container, t_unary_pred&& unary_pred, bool parallel = false)
{
  if (parallel)
  {
    return std::is_partitioned(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_unary_pred>(unary_pred));
  }
  else
  {
    return std::is_partitioned(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
  }
}

// 返回已划分容器的划分点, 即指向第一个不满足一元谓语的元素
// 若所有元素均满足谓语, 则返回容器的尾后迭代器
// 注意: 输入的容器, 一定要是已划分的
template<typename t_container, typename t_unary_pred>
decltype(auto) partition_point(t_container&& container, t_unary_pred&& unary_pred)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::partition_point(NLY_RANGE(container), std::forward<t_unary_pred>(unary_pred));
}

/*
二分搜索相关

注意:
  1. 下面这些函数使用时, 输入的容器必须是根据输入值划分好的
  2. 进一步说, 实际使用时, 输入的容器, 最好是有序的, 否则很容易拿到错误的结果
*/
template<typename t_container, typename t_value>
decltype(auto) lower_bound(t_container&& container, t_value&& value)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::lower_bound(NLY_RANGE(container), std::forward<t_value>(value));
}
template<typename t_container, typename t_value, typename t_binary_pred>
decltype(auto) lower_bound(t_container&& container, t_value&& value, t_binary_pred&& binary_pred)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::lower_bound(
    NLY_RANGE(container),
    std::forward<t_value>(value),
    std::forward<t_binary_pred>(binary_pred));
}

// 注意点参考 lower_bound
template<typename t_container, typename t_value>
decltype(auto) upper_bound(t_container&& container, t_value&& value)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::upper_bound(NLY_RANGE(container), std::forward<t_value>(value));
}
template<typename t_container, typename t_value, typename t_binary_pred>
decltype(auto) upper_bound(t_container&& container, t_value&& value, t_binary_pred&& binary_pred)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::upper_bound(
    NLY_RANGE(container),
    std::forward<t_value>(value),
    std::forward<t_binary_pred>(binary_pred));
}

// 注意点参考 lower_bound
template<typename t_container, typename t_value>
decltype(auto) equal_range(t_container&& container, t_value&& value)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::equal_range(NLY_RANGE(container), std::forward<t_value>(value));
}
template<typename t_container, typename t_value, typename t_binary_pred>
decltype(auto) equal_range(t_container&& container, t_value&& value, t_binary_pred&& binary_pred)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  return std::equal_range(
    NLY_RANGE(container),
    std::forward<t_value>(value),
    std::forward<t_binary_pred>(binary_pred));
}

// 注意点参考 lower_bound
template<typename t_container, typename t_value>
bool binary_search(t_container&& container, t_value&& value)
{
  return std::binary_search(NLY_RANGE(container), std::forward<t_value>(value));
}
template<typename t_container, typename t_value, typename t_binary_pred>
bool binary_search(t_container&& container, t_value&& value, t_binary_pred&& binary_pred)
{
  return std::binary_search(
    NLY_RANGE(container),
    std::forward<t_value>(value),
    std::forward<t_binary_pred>(binary_pred));
}

/*
注意:
1. 若输出中存在多个最大值, 则返回最左侧的那个
2. 此函数返回的是输入的引用, 所以使用时小心悬垂引用
  int n = 1;
  const int& r = std::max(n - 1, n + 1); // r 悬垂
  auto t = std::max(n - 1, n + 1); // safe
*/
using std::max;

// 若输出中存在多个最大值, 则返回最左侧的那个
template<typename t_container>
decltype(auto) max_element(t_container&& container, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::max_element(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::max_element(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
decltype(auto) max_element(
  t_container&&   container,
  t_binary_pred&& binary_pred,
  bool            parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::max_element(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::max_element(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

// 注意点参考 max
using std::min;

// 若输出中存在多个最小值, 则返回最左侧的那个
template<typename t_container>
decltype(auto) min_element(t_container&& container, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::min_element(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::min_element(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
decltype(auto) min_element(
  t_container&&   container,
  t_binary_pred&& binary_pred,
  bool            parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::min_element(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::min_element(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

// <最小值(若存在多个则取最左侧的), 最大值(若存在多个则取最右边的)>
// 同样要小心悬垂引用, 参考 max
using std::minmax;

// <最小值(若存在多个则取最左侧的), 最大值(若存在多个则取最右边的)>
template<typename t_container>
decltype(auto) minmax_element(t_container&& container, bool parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::minmax_element(NLY_EXECUTION_PAR, NLY_RANGE(container));
  }
  else
  {
    return std::minmax_element(NLY_RANGE(container));
  }
}
template<typename t_container, typename t_binary_pred>
decltype(auto) minmax_element(
  t_container&&   container,
  t_binary_pred&& binary_pred,
  bool            parallel = false)
{
  static_assert(
    std::is_lvalue_reference<decltype(container)>::value,
    "Parameter must be an lvalue");

  if (parallel)
  {
    return std::minmax_element(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::minmax_element(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
  }
}

/*
1. 此函数返回的是输入的引用, 所以使用时小心悬垂引用(参考 max 说明)
2. 此函数有谓语版本, 未纳入
*/
template<typename t_value>
const t_value& clamp(const t_value& input_value, const t_value& min_value, const t_value& max_value)
{
  return std::clamp(input_value, min_value, max_value);
}

/*
检查 第一个容器 是否按字典序小于 第二个容器
  1. 逐元素比较两个范围
  2. 首个不匹配元素, 定义一个范围是否按字典序 小于 或 大于 另一个
  3. 若一个范围是另一个范围的前缀，则较短的范围小于另一个
  4. 空范围小于任何非空范围
  5. 两个空范围字典序相等
*/
template<typename t_container_base, typename t_container_target>
bool lexicographical_compare(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{

  if (parallel)
  {
    return std::lexicographical_compare(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target));
  }
  else
  {
    return std::lexicographical_compare(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
bool lexicographical_compare(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    return std::lexicographical_compare(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::lexicographical_compare(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

template<typename t_container, typename t_init_value>
void iota(t_container&& container, t_init_value&& init_value)
{
  std::iota(NLY_RANGE(container), std::forward<t_init_value>(init_value));
}

// 还有一个接受二元谓语的版本, 未进行适配
template<typename t_container, typename t_init_value>
decltype(auto) accumulate(t_container&& container, t_init_value&& init_value)
{
  return std::accumulate(NLY_RANGE(container), std::forward<t_init_value>(init_value));
}

/*
若 第二个容器(必须有序) 是 第一个容器(必须有序) 的子序列(不必是连续子序列), 则返回 true
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container_base, typename t_container_target>
bool includes(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    return std::includes(NLY_EXECUTION_PAR, NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
  else
  {
    return std::includes(NLY_RANGE(container_base), NLY_RANGE(container_target));
  }
}
template<typename t_container_base, typename t_container_target, typename t_binary_pred>
bool includes(
  t_container_base&&   container_base,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    return std::includes(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
  else
  {
    return std::includes(
      NLY_RANGE(container_base),
      NLY_RANGE(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
求两个有序容器的并集
若两个容器存在相等元素, 优先选取来自第一个容器的元素
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container_base_0, typename t_container_base_1, typename t_container_target>
void set_union(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_union(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin());

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_union(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target));
  }
}
template<
  typename t_container_base_0,
  typename t_container_base_1,
  typename t_container_target,
  typename t_binary_pred>
void set_union(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_union(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_union(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
求两个有序容器的交集
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container_base_0, typename t_container_base_1, typename t_container_target>
void set_intersection(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_intersection(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin());

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_intersection(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target));
  }
}
template<
  typename t_container_base_0,
  typename t_container_base_1,
  typename t_container_target,
  typename t_binary_pred>
void set_intersection(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_intersection(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_intersection(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
求两个有序容器的 差集, 第一个容器 - 第二个容器
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container_base_0, typename t_container_base_1, typename t_container_target>
void set_difference(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_difference(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin());

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_difference(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target));
  }
}
template<
  typename t_container_base_0,
  typename t_container_base_1,
  typename t_container_target,
  typename t_binary_pred>
void set_difference(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_difference(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_difference(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
求两个有序容器的 对称差集: 将处于任一容器，但未在两个容器中均被找到的元素
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
*/
template<typename t_container_base_0, typename t_container_base_1, typename t_container_target>
void set_symmetric_difference(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_symmetric_difference(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin());

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_symmetric_difference(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target));
  }
}
template<
  typename t_container_base_0,
  typename t_container_base_1,
  typename t_container_target,
  typename t_binary_pred>
void set_symmetric_difference(
  t_container_base_0&& container_base_0,
  t_container_base_1&& container_base_1,
  t_container_target&& container_target,
  t_binary_pred&&      binary_pred,
  bool                 parallel = false)
{
  if (parallel)
  {
    container_target.resize(std::size(container_base_0) + std::size(container_base_1));

    auto it = std::set_symmetric_difference(
      NLY_EXECUTION_PAR,
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      container_target.begin(),
      std::forward<t_binary_pred>(binary_pred));

    container_target.erase(it, container_target.end());
  }
  else
  {
    container_target.clear();
    std::set_symmetric_difference(
      NLY_RANGE(container_base_0),
      NLY_RANGE(container_base_1),
      std::back_inserter(container_target),
      std::forward<t_binary_pred>(binary_pred));
  }
}

/*
将给定的容器中的元素, 变为其下一个排列
无谓语版本: 使用 operator <
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
返回值:
 1. 若存在这种排列, 则返回 true
 2. 否则返回 false, 且将给定的范围变换为首个排列, 如同调用了 std::sort() 一样
*/
template<typename t_container>
bool next_permutation(t_container&& container)
{
  return std::next_permutation(NLY_RANGE(container));
}
template<typename t_container, typename t_binary_pred>
bool next_permutation(t_container&& container, t_binary_pred&& binary_pred)
{
  return std::next_permutation(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
}

/*
将给定范围，变为上个排列
无谓语版本: 使用 operator <
谓语版本: 注意严格弱序语义(参考文件起使位置附近的说明)
返回值:
 1. 若存在这种排列, 则返回 true
 2. 否则返回 false, 且将给定的范围变换为首个排列, 如同调用了 std::sort() + std::reverse() 一样
*/
template<typename t_container>
bool prev_permutation(t_container&& container)
{
  return std::prev_permutation(NLY_RANGE(container));
}
template<typename t_container, typename t_binary_pred>
bool prev_permutation(t_container&& container, t_binary_pred&& binary_pred)
{
  return std::prev_permutation(NLY_RANGE(container), std::forward<t_binary_pred>(binary_pred));
}

/*
若 第一个容器 进行重排, 能使其等于 第二个容器, 则返回 true
备注: 存在其他重载版本, 不进行纳入
*/
template<typename t_container_0, typename t_container_1>
bool is_permutation(t_container_0&& container_0, t_container_1&& container_1)
{
  return std::is_permutation(NLY_RANGE(container_0), NLY_RANGE(container_1));
}

} // namespace nly

#endif