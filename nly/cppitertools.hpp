#ifndef NLY_CPPITERTOOLS
#define NLY_CPPITERTOOLS
#include "cppitertools/itertools.hpp"

namespace nly
{

/*
1. cppitertools 受 python 启发, 实现了很多其内置函数

2. 迭代过程不涉及底层元素的 复制 或 移动(参考测试用例中的 Zip)

3. 典型用法一(注意范围 for 中的右值引用)
  std::vector<int>         v0{ 1, 2, 3, 4 };
  std::set<std::string>    v1{ "hello", "world" };
  std::vector<std::string> result;

  for (auto&& [tmp0, tmp1] : nly::zip(v0, v1))
  {
    result.emplace_back(fmt::format("{}, {}", tmp0, tmp1));
  }

  EXPECT_TRUE(result == (std::vector<std::string>{ "1, hello", "2, world" }));

4. 典型用法二: 作用于临时对象
  int sum = 0;
  for (auto&& [v0, v1] : nly::zip(std::vector<int>{ 2, 3, 4 }, std::vector<int>{ 4, 5 }))
  {
    sum += v0 * v1;
  }
  EXPECT_TRUE(sum == 2 * 4 + 3 * 5);
*/

using iter::zip;

// 注意: 若迭代步长为 0, 将产生空范围(python 会抛出异常)
// 该函数还支持浮点数类型
using iter::range;

using iter::enumerate;

/*
1. 只会产生使得谓语返回 true 的值, 形式为
  filter(pred, container);
2. 若不传递谓语, 则直接测试元素自身是否为 true, 形式为
  filter(container);
*/
using iter::filter;

// 类似 iter::filter, 但过滤条件相反
using iter::filterfalse;

/*
1. 从可迭代对象中返回元素, 直到遇到第一个不满足谓语的元素, 形式为
  takewhile(pred, container);
2. 若不传递谓语, 则直接测试元素自身是否为 true, 形式为
  takewhile(container);
*/
using iter::takewhile;

/*
1. 得到第一个使得谓语不成立的元素, 返回自该元素(含)起及其后所有的元素, 形式为
  dropwhile(pred, container);
2. 若不传递谓语, 则直接测试元素自身是否为 true, 形式为
  dropwhile(container);
*/
using iter::dropwhile;

// 生成可迭代对象的所有值, 并无限循环
using iter::cycle;

} // namespace nly

#endif
