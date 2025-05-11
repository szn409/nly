#ifndef NLY_STL_HELPER_ALGORITHM
#define NLY_STL_HELPER_ALGORITHM
#include <algorithm>
#include <numeric>
#include <execution>
#include <random>

namespace nly
{
/*
关于算法库中比较函数的说明
1. 其必须满足严格弱序的要求
2. 默认使用 <, 以 sort 举例, 对应的结果是非降序
3. 还是拿 sort 举例, 若实现的 比较函数 使用 >= 是非法的, 以下代码会崩溃
  std::vector<int> vec{ 1, 2, 2, 3 };
  std::sort(vec.begin(), vec.end(), [](int a, int b) { return a >= b; });
4. 正确的做法是使用 > 而不是 >=
*/

#ifndef NLY_RANGE
#define NLY_RANGE(container) std::begin(container), std::end(container)
#endif

#ifndef NLY_ALGO_ONE_CONTAINER
#define NLY_ALGO_ONE_CONTAINER(fun_name)                                                           \
  template<typename t_container>                                                                   \
  decltype(auto) fun_name(t_container& container, bool parallel = false)                           \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container));                             \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container));                                                  \
    }                                                                                              \
  }
#endif

#ifndef NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM
#define NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(fun_name)                                            \
  template<typename t_container, typename t_param>                                                 \
  decltype(auto) fun_name(t_container& container, t_param&& param, bool parallel = false)          \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container), param);                      \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container), param);                                           \
    }                                                                                              \
  }
#endif

#ifndef NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM
#define NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(fun_name)                                            \
  template<typename t_container, typename t_param_0, typename t_param_1>                           \
  decltype(auto) fun_name(                                                                         \
    t_container& container,                                                                        \
    t_param_0&&  param_0,                                                                          \
    t_param_1&&  param_1,                                                                          \
    bool         parallel = false)                                                                 \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container), param_0, param_1);           \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container), param_0, param_1);                                \
    }                                                                                              \
  }
#endif

#ifndef NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM
#define NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(fun_name)                                          \
  template<typename t_container, typename t_param_0, typename t_param_1, typename t_param_2>       \
  decltype(auto) fun_name(                                                                         \
    t_container& container,                                                                        \
    t_param_0&&  param_0,                                                                          \
    t_param_1&&  param_1,                                                                          \
    t_param_2&&  param_2,                                                                          \
    bool         parallel = false)                                                                 \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container), param_0, param_1, param_2);  \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container), param_0, param_1, param_2);                       \
    }                                                                                              \
  }
#endif

#ifndef NLY_ALGO_TWO_CONTAINER
#define NLY_ALGO_TWO_CONTAINER(fun_name)                                                           \
  template<typename t_container_0, typename t_container_1>                                         \
  decltype(auto) fun_name(                                                                         \
    t_container_0& container_0,                                                                    \
    t_container_1& container_1,                                                                    \
    bool           parallel = false)                                                               \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container_0), NLY_RANGE(container_1));   \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container_0), NLY_RANGE(container_1));                        \
    }                                                                                              \
  }
#endif

#ifndef NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM
#define NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(fun_name)                                            \
  template<typename t_container_0, typename t_container_1, typename t_param>                       \
  decltype(auto) fun_name(                                                                         \
    t_container_0& container_0,                                                                    \
    t_container_1& container_1,                                                                    \
    t_param&&      param,                                                                          \
    bool           parallel = false)                                                               \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(                                                                        \
        std::execution::par,                                                                       \
        NLY_RANGE(container_0),                                                                    \
        NLY_RANGE(container_1),                                                                    \
        param);                                                                                    \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container_0), NLY_RANGE(container_1), param);                 \
    }                                                                                              \
  }
#endif

#ifndef NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM
#define NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM(fun_name)                                            \
  template<typename t_container_0, typename t_container_1, typename t_param_0, typename t_param_1> \
  decltype(auto) fun_name(                                                                         \
    t_container_0& container_0,                                                                    \
    t_container_1& container_1,                                                                    \
    t_param_0&&    param_0,                                                                        \
    t_param_1&&    param_1,                                                                        \
    bool           parallel = false)                                                               \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(                                                                        \
        std::execution::par,                                                                       \
        NLY_RANGE(container_0),                                                                    \
        NLY_RANGE(container_1),                                                                    \
        param_0,                                                                                   \
        param_1);                                                                                  \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container_0), NLY_RANGE(container_1), param_0, param_1);      \
    }                                                                                              \
  }
#endif

// 不支持并发
#ifndef NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER
#define NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER(fun_name)                                              \
  template<typename t_container>                                                                   \
  decltype(auto) fun_name(t_container& container)                                                  \
  {                                                                                                \
    return std::fun_name(NLY_RANGE(container));                                                    \
  }
#endif

// 不支持并发
#ifndef NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM
#define NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(fun_name)                               \
  template<typename t_container, typename t_param>                                                 \
  decltype(auto) fun_name(t_container& container, t_param&& param)                                 \
  {                                                                                                \
    return std::fun_name(NLY_RANGE(container), param);                                             \
  }
#endif

// 不支持并发
#ifndef NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_TWO_PARAM
#define NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_TWO_PARAM(fun_name)                               \
  template<typename t_container, typename t_param_0, typename t_param_1>                           \
  decltype(auto) fun_name(t_container& container, t_param_0&& param_0, t_param_1&& param_1)        \
  {                                                                                                \
    return std::fun_name(NLY_RANGE(container), param_0, param_1);                                  \
  }
#endif

template<typename t_container, typename t_param>
void for_each(t_container& container, t_param param, bool parallel = false)
{
  if (parallel)
  {
    std::for_each(std::execution::par, NLY_RANGE(container), param);
  }
  else
  {
    std::for_each(NLY_RANGE(container), param);
  }
}

// 此 hpp 声明的函数, 均操作整个容器, 所以 for_each_n 未纳入

// 仅在谓语对范围中的所有元素返回 true 时，才返回 true
// container 为空, 则返回 true
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(all_of)

// 在谓语对范围中的至少一个元素返回 true 时，就返回 true
// container 为空, 则返回 false
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(any_of)

// 仅在谓语对范围中的所有元素返回 false 时，才返回 true
// container 为空, 则返回 true
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(none_of)

NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(find)

NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(find_if)

// 搜索使得谓语返回 false 的元素
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(find_if_not)

/*
在第一个容器中, 寻找第二个容器元素出现的最后位置
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
经测试, msvc 2019 是从后向前找的，相对于标准库的要求，还是做了点优化的
*/
NLY_ALGO_TWO_CONTAINER(find_end)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(find_end)

/*
在第一个容器中, 搜索第二个容器中任意元素首次出现的位置
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
*/
NLY_ALGO_TWO_CONTAINER(find_first_of)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(find_first_of)

/*
在容器中搜索两个连续满足条件的元素
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
*/
NLY_ALGO_ONE_CONTAINER(adjacent_find)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(adjacent_find)

/*
返回容器中, 满足特定条件的元素个数
count: 使用 operator =
count_if: 使用一元谓语
*/
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(count)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(count_if)

/*
返回来自两个范围的首个不匹配元素对
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
注意:
  1. 此函数有其他版本的重载, 支持第二个容器仅传入起始迭代器, 此版本不进行兼容
  2. 目前兼容的版本, 支持两个容器 size 不一致
*/
NLY_ALGO_TWO_CONTAINER(mismatch)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(mismatch)

/*
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
注意:
  1. 此函数有其他版本的重载, 支持第二个容器仅传入起始迭代器, 此版本不进行兼容
  2. 目前兼容的版本, 支持两个容器 size 不一致
*/
NLY_ALGO_TWO_CONTAINER(equal)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(equal)

/*
在第一个容器中, 搜索第二个容器元素首次出现的位置
无谓语版本: 使用 operator =
谓语版本: 使用二元谓语
注意: 存在其他重载版本, 可指定不同的搜索算法, 未进行兼容
*/
NLY_ALGO_TWO_CONTAINER(search)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(search)

// 在容器中, 搜索连续 count 个元素, 其值等于 value 或使得二元谓语成立
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(search_n)
NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(search_n)

/*
1. 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
2. 当重叠且目标位于源的左侧, 此时可以使用 copy, 不可以使用 copy_backward
3. 此 hpp 声明的函数, 均操作整个容器, 所以 copy_n 未纳入
4. copy_backward 亦未纳入, 在源与目标存在重叠, 且目标位于源的右侧时, 应该使用此函数而不是 copy
5. move_backward 亦未纳入
*/
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(copy)
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(copy_if)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(move)

using std::iter_swap;
using std::swap;
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(swap_ranges)

// 将一个或两个容器中的元素, 经过转换, 输出到指定的输出迭代器中
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(transform)
NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(transform)

// 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(replace)
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(replace_if)
NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(replace_copy)
NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(replace_copy_if)

// 此 hpp 声明的函数, 均操作整个容器, 所以 fill_n, generate_n 未纳入
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(fill)     // 使用一个固定值进行填充
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(generate) // 使用一个生成器进行填充

// 移除所有满足特定判别条件的元素，并返回新范围的尾后迭代器, 容器大小不会发生变化
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(remove)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(remove_if)

// 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(remove_copy)
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(remove_copy_if)

// 消除容器中，相继等价元素中，除首元素外的元素，并返回新范围的尾后迭代器
// 注意: 不会改变容器大小
NLY_ALGO_ONE_CONTAINER(unique)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(unique)

// 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(unique_copy)
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(unique_copy)

// 反转容器中的元素
NLY_ALGO_ONE_CONTAINER(reverse)

// 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(reverse_copy)

// 此 hpp 声明的函数, 均操作整个容器, 所以 rotate, rotate_copy 未纳入

// 以非降序排序, 不保证相等元素的相对顺序
NLY_ALGO_ONE_CONTAINER(sort)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(sort)

// sort 的稳地版本
NLY_ALGO_ONE_CONTAINER(stable_sort)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(stable_sort)

// 检查容器中的元素，是否以非降序排序
// 若容器为空, 则返回 true
NLY_ALGO_ONE_CONTAINER(is_sorted)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(is_sorted)

// 此 hpp 声明的函数, 均操作整个容器, 所以 partial_sort, is_sorted_until nth_element, 未纳入

/*
将两个有序范围，进行合并输出
该函数稳定，即在结果中，对于来自两个序列的等价元素，来自第一个范围的元素将先于来自第二个范围的元素
由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 符合 使用二元谓语 进行排序
*/
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(merge)
NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM(merge)

// 此 hpp 声明的函数, 均操作整个容器, 所以 inplace_merge 未纳入

// random_shuffle 自 C++17 起移除, 故而未纳入

// 重排容器中的元素
// 由于使用了 std::random_device, 对于相同的输入, 每次的输出均会不同
template<typename t_container>
void shuffle(t_container& container)
{
  std::random_device rd;
  std::mt19937       mt(rd());
  std::shuffle(NLY_RANGE(container), mt);
}

// 从容器中, 不重复的选择 sample_count 个元素, 使得每个元素有相同出现概率, 输出到 out_iterator 中
// 若 sample_count 大于容器的 size, 则容器中所有元素都将被选择
// 由于使用了 std::random_device, 对于相同的输入, 每次的输出均会不同
template<typename t_container, typename t_out>
void sample(t_container& container, t_out out_iterator, size_t sample_count)
{
  std::random_device rd;
  std::mt19937       mt(rd());
  std::sample(NLY_RANGE(container), out_iterator, sample_count, mt);
}

// 重排序容器中的元素，使得谓语返回 true 的元素, 位于使得谓语返回 false 的元素之前（不保持相对顺序）
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(partition)

// partition 的稳定版本
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(stable_partition)

/*
参数说明:
  第一个参数: 被操纵的容器
  第二个参数: 保存使得谓语为 true 的元素
  第三个参数: 保存使得谓语为 false 的元素
  第四个参数: 一元谓语

注意: 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size
够大
*/
NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(partition_copy)

// 返回容器是否已划分, 即使得谓语返回 true 的元素, 位于使得谓语返回 false 的元素之前
// 容器为空也返回 true
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(is_partitioned)

// 返回已划分容器的划分点, 即第一个不满足一元谓语的元素
// 若所有元素均满足谓语, 则返回容器的尾后迭代器
// 注意: 输入的容器, 一定要是已划分的
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(partition_point)

/*
二分搜索相关

注意:
  1. 下面这些函数使用时, 输入的容器必须是根据输入值划分好的
  2. 进一步说, 实际使用时, 输入的容器, 最好是有序的, 否则很容易拿到错误的结果
*/
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(lower_bound)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_TWO_PARAM(lower_bound)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(upper_bound)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_TWO_PARAM(upper_bound)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(equal_range)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_TWO_PARAM(equal_range)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(binary_search)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_TWO_PARAM(binary_search)

/*
注意:
1. 若输出中存在多个最大值, 则返回最左侧的那个
2. 此函数返回的是输入的引用, 所以使用时小心悬垂引用
  int n = 1;
  const int& r = std::max(n - 1, n + 1); // r 悬垂
*/
using std::max;

// 若输出中存在多个最大值, 则返回最左侧的那个
NLY_ALGO_ONE_CONTAINER(max_element)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(max_element)

// 注意点参考 max
using std::min;

// 若输出中存在多个最小值, 则返回最左侧的那个
NLY_ALGO_ONE_CONTAINER(min_element)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(min_element)

// <最小值(若存在多个则取最左侧的), 最大值(若存在多个则取最右边的)>
// 同样要小心悬垂引用, 参考 max
using std::minmax;

// <最小值(若存在多个则取最左侧的), 最大值(若存在多个则取最右边的)>
NLY_ALGO_ONE_CONTAINER(minmax_element)
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(minmax_element)

/*
此函数返回的是输入的引用, 所以使用时小心悬垂引用
  int n = -1;
  const int& r = std::clamp(n, 0, 255); // r 悬垂
*/
using std::clamp;

/*
检查 第一个容器 是否按字典序小于 第二个容器
  1. 逐元素比较两个范围
  2. 首个不匹配元素, 定义一个范围是否按字典序 小于 ﻿或 大于 ﻿另一个
  3. 若一个范围是另一个范围的前缀，则较短的范围小于另一个
  4. 空范围小于任何非空范围
  5. 两个空范围字典序相等
*/
NLY_ALGO_TWO_CONTAINER(lexicographical_compare)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(lexicographical_compare)

NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(iota)

// 还有一个接受二元谓语的版本, 未进行适配
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(accumulate)

/*
若 第二个容器(必须有序) 是 第一个容器(必须有序) 的子序列(不必是连续子序列), 则返回 true
无谓语版本: 符合使用 operator < 进行排序
谓语版本: 符合 使用二元谓语 进行排序
*/
NLY_ALGO_TWO_CONTAINER(includes)
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(includes)

/*
求两个有序容器的 并集
注意: 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size
够大 若两个容器存在相等元素, 优先选取来自第一个容器的元素
*/
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(set_union)
NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM(set_union)

/*
求两个有序容器的 交集
注意: 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size
够大
*/
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(set_intersection)
NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM(set_intersection)

/*
求两个有序容器的 差集, 第一个容器 - 第二个容器
注意: 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size
够大
*/
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(set_difference)
NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM(set_difference)

/*
求两个有序容器的 对称差集: 将处于任一容器，但未在两个容器中均被找到的元素
注意: 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size
够大
*/
NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(set_symmetric_difference)
NLY_ALGO_TWO_CONTAINER_WITH_TWO_PARAM(set_symmetric_difference)

/*
将给定的容器中的元素, 变为其下一个排列
无谓语版本: 使用 operator <
谓语版本: 使用二元谓语
返回值:
 1. 若存在这种排列, 则返回 true
 2. 否则返回 false, 且将给定的范围变换为首个排列, 如同调用了 std::sort() 一样
*/
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER(next_permutation)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(next_permutation)

/*
将给定范围，变为上个排列
无谓语版本: 使用 operator <
谓语版本: 使用二元谓语
返回值:
 1. 若存在这种排列, 则返回 true
 2. 否则返回 false, 且将给定的范围变换为首个排列, 如同调用了 std::sort() + std::reverse() 一样
*/
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER(prev_permutation)
NLY_ALGO_NOT_PARALLEL_ONE_CONTAINER_WITH_ONE_PARAM(prev_permutation)

/*
若 第一个容器 进行重排, 能使其等于 第二个容器, 则返回 true
备注: 存在其他重载版本, 不进行纳入
*/
template<typename t_container_0, typename t_container_1>
decltype(auto) is_permutation(t_container_0& container_0, t_container_1& container_1)
{
  return std::is_permutation(NLY_RANGE(container_0), NLY_RANGE(container_1));
}

} // namespace nly

#endif