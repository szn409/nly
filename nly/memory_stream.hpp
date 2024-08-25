#ifndef NLY_MEMORY_STREAM
#define NLY_MEMORY_STREAM

#include <deque>
#include <memory>
#include <cassert>
#include <functional>

namespace nly
{

// clang-format off
static const unsigned char bit_set_count[256] =
{
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};
// clang-format on

// Returns whether the number of differing bits between the first byte_len bytes of input and base
// is less than or equal to allow_error_bit_count.
inline const bool bit_cmp(
  const void*  input,
  const void*  base,
  const size_t byte_len,
  const size_t allow_error_bit_count = 0)
{
  if (!allow_error_bit_count)
  {
    return !memcmp(input, base, byte_len);
  }
  else
  {
    auto base_begin = static_cast<const unsigned char*>(base);
    auto input_begin = static_cast<const unsigned char*>(input);
    auto input_end = input_begin + byte_len;

    size_t count = 0;
    while (input_begin != input_end)
    {
      count += bit_set_count[(*input_begin++) ^ (*base_begin++)];

      if (count > allow_error_bit_count)
      {
        return false;
      }
    }
  }

  return true;
}

class memory_stream
{
public:
  typedef std::pair<const void*, size_t>          memory_type;
  typedef std::function<void(const memory_type&)> release_type;

public:
  memory_stream(release_type f)
    : m_release(f)
  {
  }

  memory_stream(const memory_stream&) = delete;
  memory_stream& operator=(const memory_stream&) = delete;

  ~memory_stream()
  {
    if (m_release)
    {
      for (const auto& item : m_memory_chunk)
      {
        m_release(item);
      }
    }
  }

public:
  void add(const void* data, const size_t len)
  {
    m_memory_chunk.emplace_back(std::make_pair(data, len));
    m_available_byte += len;
  }

  // Move the stream forward by the specified number of bytes from the current position.
  // Returns: The actual number of bytes moved forward.
  // Note: If the current valid data in the stream is less than len, all valid data is discarded.
  size_t slide(size_t len)
  {
    len = (std::min)(len, m_available_byte);
    auto out = len;

    m_available_byte -= len;
    m_already_slide_byte += len;

    while (len)
    {
      assert(!m_memory_chunk.empty());
      assert(m_first_chunk_useful_pos <= m_memory_chunk.front().second);

      auto left = m_memory_chunk.front().second - m_first_chunk_useful_pos;

      if (left > len)
      {
        m_first_chunk_useful_pos += len;
        break;
      }
      else
      {
        len -= left;
        m_first_chunk_useful_pos = 0;
        if (m_release)
        {
          m_release(m_memory_chunk.front());
        }
        m_memory_chunk.pop_front();
      }
    }

    return out;
  }

  // Find the target and return its pos.
  // Return -1 if the target is not found.
  const size_t find(
    const void*  target,
    const size_t target_byte,
    const size_t allow_error_bit_count = 0) const
  {
    if (!target_byte)
    {
      return 0;
    }

    size_t chunk_index = 0;
    size_t move_byte_count = 0;
    auto   first_chunk_useful_pos = m_first_chunk_useful_pos;

    std::unique_ptr<unsigned char[]> head_buff;

    while (m_available_byte - move_byte_count >= target_byte)
    {
      assert(chunk_index < m_memory_chunk.size());
      assert(m_available_byte > move_byte_count);
      assert(m_memory_chunk.size() > chunk_index);

      auto& data = m_memory_chunk[chunk_index];
      if (data.second - first_chunk_useful_pos >= target_byte)
      {
        if (bit_cmp(
              static_cast<const unsigned char*>(data.first) + first_chunk_useful_pos,
              target,
              target_byte,
              allow_error_bit_count))
        {
          return move_byte_count;
        }

        ++first_chunk_useful_pos;
        ++move_byte_count;
        continue;
      }

      if (!head_buff)
      {
        head_buff.reset(new unsigned char[target_byte]);
      }

      while (data.second > first_chunk_useful_pos)
      {
        if (peek(head_buff.get(), target_byte, move_byte_count) == target_byte)
        {
          if (bit_cmp(head_buff.get(), target, target_byte, allow_error_bit_count))
          {
            return move_byte_count;
          }
          else
          {
            ++first_chunk_useful_pos;
            ++move_byte_count;
            continue;
          }
        }
        else
        {
          return static_cast<size_t>(-1);
        }
      }

      ++chunk_index;
      first_chunk_useful_pos = 0;
    }

    return static_cast<size_t>(-1);
  }

  // Read the specified bytes of data from the stream at the given offset.
  // Return value: The actual number of bytes read.
  size_t peek(void* output, const size_t need_byte, const size_t offset_byte = 0) const
  {
    if (m_available_byte <= offset_byte)
    {
      return 0;
    }

    const size_t chunk_size = m_memory_chunk.size();
    const auto   final_len = (std::min)(need_byte, m_available_byte - offset_byte);

    size_t already_get = 0;
    size_t already_offset = 0;
    auto   first_chunk_useful_pos = m_first_chunk_useful_pos;

    for (size_t i = 0; i < chunk_size; ++i)
    {
      auto& data = m_memory_chunk[i];
      assert(data.second >= first_chunk_useful_pos);

      if (already_offset != offset_byte)
      {
        auto offset =
          (std::min)(offset_byte - already_offset, data.second - first_chunk_useful_pos);
        first_chunk_useful_pos += offset;
        already_offset += offset;
      }

      if (already_offset == offset_byte)
      {
        auto copy_len = (std::min)(final_len - already_get, data.second - first_chunk_useful_pos);
        memcpy(
          static_cast<unsigned char*>(output) + already_get,
          static_cast<const unsigned char*>(data.first) + first_chunk_useful_pos,
          copy_len);

        already_get += copy_len;
        if (already_get == final_len)
        {
          break;
        }
      }

      first_chunk_useful_pos = 0;
    }

    return final_len;
  }

  size_t already_slide_byte() const
  {
    return m_already_slide_byte;
  }

  size_t available_byte() const
  {
    return m_available_byte;
  }

private:
  release_type            m_release;
  std::deque<memory_type> m_memory_chunk;

  size_t m_first_chunk_useful_pos = 0;
  size_t m_already_slide_byte{ 0 };
  size_t m_available_byte{ 0 };
};

} // namespace nly

#endif // NLY_MEMORY_STREAM