#ifndef NLY_BOOST_HELPER_ASIO_STREAMBUF
#define NLY_BOOST_HELPER_ASIO_STREAMBUF
#include "boost/asio.hpp"
#include <algorithm>
#include <limits>

namespace nly
{

/*
A. 本类封装了 asio::streambuf, 本质上未对其进行任何功能更改
B. 关于 asio::streambuf
  1. 继承自 std::streambuf, 丰富了其接口, 包括
    a. 支持访问底层数据
    b. 支持预分配大小
  2. 其底层使用单个 vector 来充当输入输出队列
  3. 其输入队列:
    a. 外部的输出流, 将数据输出到 asio::streambuf 的输入队列末尾
    b. 外部的输入流, 从 asio::streambuf 输入队列的开头, 获取数据
  4. 其输出队列
    a. 外部流不直接操作 asio::streambuf 的输出队列
    b. 可以通过 prepare 来扩展输出队列
    c. 可以通过 commit 将输出队列中的数据转入输入队列中
*/
class streambuf
{
public:
  /*
  maximum_size
    1. 为输入输出序列之和的最大值
    2. 查看源码可得, 底层缓冲区会被 resize 为 std::min(maximum_size, 128)
  */
  streambuf(std::size_t maximum_size = (std::numeric_limits<std::size_t>::max)())
    : m_buf(maximum_size)
  {
  }

public:
  // 返回底层缓冲区(vector)的 capacity
  size_t capacity() const
  {
    return m_buf.capacity();
  }

  // 返回底层缓冲区(vector)的最大允许 size
  size_t max_size() const
  {
    return m_buf.max_size();
  }

  // 获取 输入队列
  // 任何修改 输入输出序列 的函数的调用, 都会使得返回的对象无效
  boost::asio::const_buffer get_input() const
  {
    return m_buf.data();
  }

  // 从 输入队列 的开头, 移除 count 个字符
  // 若 count 大于 输入序列 中的数据量, 则移除 输入队列 中所有数据
  void consume_input(std::size_t count)
  {
    if (0 == count)
    {
      return;
    }

    while (true)
    {
      auto   old_size = m_buf.size();
      size_t value = (std::min)(count, static_cast<size_t>(std::numeric_limits<int>::max()));
      m_buf.consume(value);
      count -= value;
      if (0 == count || m_buf.size() == old_size)
      {
        return;
      }
    }
  }

  // 确保输出序列的大小, 至少是 count 个字节
  // 会根据需要进行内存分配
  // 返回输出序列的 buffer
  // 若 get_input().size() + count > max_size() 则会抛出 std::length_error
  // 任何修改 输入输出序列 的函数的调用, 都会使得返回的对象无效
  // 由于 asio::streambuf 内部使用 vector 作为缓冲区, 所以此函数可以防止底层 vector 的反复分配内存
  boost::asio::mutable_buffer prepare_output(size_t count)
  {
    return m_buf.prepare(count);
  }

  // 从输出序列的开头, 移动 n 个字符到输入序列的末尾
  // 若 count 大于输出序列的大小, 则移动整个输出序列
  void commit_output(size_t count)
  {
    if (0 == count)
    {
      return;
    }

    while (true)
    {
      auto   old_size = m_buf.size();
      size_t value = (std::min)(count, static_cast<size_t>(std::numeric_limits<int>::max()));
      m_buf.commit(value);
      count -= value;
      if (0 == count || m_buf.size() == old_size)
      {
        return;
      }
    }
  }

  // 获取底层数据
  boost::asio::streambuf& get()
  {
    return m_buf;
  }

private:
  boost::asio::streambuf m_buf;
};
} // namespace nly

#endif