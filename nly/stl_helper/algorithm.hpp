#ifndef NLY_STL_HELPER_ALGORITHM
#define NLY_STL_HELPER_ALGORITHM
#include <algorithm>
#include <execution>

namespace nly
{
#ifndef NLY_RANGE
#define NLY_RANGE(container) std::begin(container), std::end(container)
#endif

// 作用于单个容器, 无额外参数
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

// 作用于单个容器, 并传入单个额外参数
#ifndef NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM
#define NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(fun_name)                                            \
  template<typename t_container, typename t_param>                                                 \
  decltype(auto) fun_name(t_container& container, t_param param, bool parallel = false)            \
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

// 作用于单个容器, 并传入两个额外参数
#ifndef NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM
#define NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(fun_name)                                            \
  template<typename t_container, typename t_param_0, typename t_param_1>                           \
  decltype(auto) fun_name(                                                                         \
    t_container& container,                                                                        \
    t_param_0    param_0,                                                                          \
    t_param_1    param_1,                                                                          \
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

// 作用于单个容器, 并传入三个额外参数
#ifndef NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM
#define NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(fun_name)                                          \
  template<typename t_container, typename t_param_0, typename t_param_1, typename t_param_2>       \
  decltype(auto) fun_name(                                                                         \
    t_container& container,                                                                        \
    t_param_0    param_0,                                                                          \
    t_param_1    param_1,                                                                          \
    t_param_2    param_2,                                                                          \
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

// 作用于两个容器, 无额外参数
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

// 作用于两个容器, 并传入单个额外参数
#ifndef NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM
#define NLY_ALGO_TWO_CONTAINER_WITH_ONE_PARAM(fun_name)                                            \
  template<typename t_container_0, typename t_container_1, typename t_param>                       \
  decltype(auto) fun_name(                                                                         \
    t_container_0& container_0,                                                                    \
    t_container_1& container_1,                                                                    \
    t_param        param,                                                                          \
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

// 适用于: 一个容器 + 一个输出迭代器
#ifndef NLY_ALGO_ONE_CONTAINER_TO_OUTPUT
#define NLY_ALGO_ONE_CONTAINER_TO_OUTPUT(fun_name)                                                 \
  template<typename t_container, typename t_output>                                                \
  decltype(auto) fun_name(t_container& container, t_output output, bool parallel = false)          \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container), output);                     \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container), output);                                          \
    }                                                                                              \
  }
#endif

// 适用于: 一个容器 + 一个输出迭代器, 并接受一个额外参数
#ifndef NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM
#define NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM(fun_name)                                  \
  template<typename t_container, typename t_output, typename t_param>                              \
  decltype(auto) fun_name(                                                                         \
    t_container& container,                                                                        \
    t_output     output,                                                                           \
    t_param      param,                                                                            \
    bool         parallel = false)                                                                 \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container), output, param);              \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container), output, param);                                   \
    }                                                                                              \
  }
#endif

// 适用于: 一个容器 + 一个输出迭代器, 并接受两个额外参数
#ifndef NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_TWO_PARAM
#define NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_TWO_PARAM(fun_name)                                  \
  template<typename t_container, typename t_output, typename t_param_0, typename t_param_1>        \
  decltype(auto) fun_name(                                                                         \
    t_container& container,                                                                        \
    t_output     output,                                                                           \
    t_param_0    param_0,                                                                          \
    t_param_1    param_1,                                                                          \
    bool         parallel = false)                                                                 \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(std::execution::par, NLY_RANGE(container), output, param_0, param_1);   \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container), output, param_0, param_1);                        \
    }                                                                                              \
  }
#endif

// 适用于: 两个容器 + 一个输出迭代器, 并接受一个额外参数
#ifndef NLY_ALGO_TWO_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM
#define NLY_ALGO_TWO_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM(fun_name)                                  \
  template<typename t_container_0, typename t_container_1, typename t_output, typename t_param>    \
  decltype(auto) fun_name(                                                                         \
    t_container_0& container_0,                                                                    \
    t_container_1& container_1,                                                                    \
    t_output       output,                                                                         \
    t_param        param,                                                                          \
    bool           parallel = false)                                                               \
  {                                                                                                \
    if (parallel)                                                                                  \
    {                                                                                              \
      return std::fun_name(                                                                        \
        std::execution::par,                                                                       \
        NLY_RANGE(container_0),                                                                    \
        std::begin(container_1),                                                                   \
        output,                                                                                    \
        param);                                                                                    \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
      return std::fun_name(NLY_RANGE(container_0), std::begin(container_1), output, param);        \
    }                                                                                              \
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

// 在容器中, 搜索 count 个元素, 其值等于 value 或使得二元谓语成立
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(search_n)
NLY_ALGO_ONE_CONTAINER_WITH_THREE_PARAM(search_n)

/*
1. 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
2. 当重叠且目标位于源的左侧, 此时可以使用 copy, 不可以使用 copy_backward
3. 此 hpp 声明的函数, 均操作整个容器, 所以 copy_n 未纳入
4. copy_backward 亦未纳入, 在源与目标存在重叠, 且目标位于源的右侧时, 应该使用此函数而不是 copy
5. move_backward 亦未纳入
*/
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT(copy)
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM(copy_if)
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT(move)

using std::iter_swap;
using std::swap;
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT(swap_ranges)

// 将一个或两个容器中的元素, 经过转换, 输出到指定的输出迭代器中
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM(transform)
NLY_ALGO_TWO_CONTAINER_TO_OUTPUT_WITH_ONE_PARAM(transform)

// 由于并行要求, 对于输出使用 std::back_inserter 会导致编译错误, 所以需要自行保证输出容器 size 够大
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(replace)
NLY_ALGO_ONE_CONTAINER_WITH_TWO_PARAM(replace_if)
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_TWO_PARAM(replace_copy)
NLY_ALGO_ONE_CONTAINER_TO_OUTPUT_WITH_TWO_PARAM(replace_copy_if)

// 此 hpp 声明的函数, 均操作整个容器, 所以 fill_n, generate_n 未纳入
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(fill)     // 使用一个固定值进行填充
NLY_ALGO_ONE_CONTAINER_WITH_ONE_PARAM(generate) // 使用一个生成器进行填充

} // namespace nly

#endif