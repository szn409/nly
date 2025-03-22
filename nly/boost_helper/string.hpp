#ifndef NLY_BOOST_HELPER_STRING
#define NLY_BOOST_HELPER_STRING

#include "boost/algorithm/string.hpp"

namespace nly
{
class string_algo
{
public:
  template<typename t_input, typename t_output, typename t_predicate>
  static void split_if(
    t_input&&   input,
    t_output&   output,
    t_predicate pred,
    bool        compress_adjacent_token = false)
  {
    boost::split(
      output,
      input,
      pred,
      compress_adjacent_token ? boost::algorithm::token_compress_on
                              : boost::algorithm::token_compress_off);
  }

  template<typename t_input, typename t_output, typename t_input_element>
  static void split(
    t_input&&               input,
    t_output&               output,
    const t_input_element&& separator,
    bool                    compress_adjacent_token = false)

  {
    split_if(
      input,
      output,
      [&separator](const t_input_element& value) { return value == separator; },
      compress_adjacent_token);
  }
};

} // namespace nly

#endif
