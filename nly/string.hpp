#ifndef NLY_STRING
#define NLY_STRING
#include "boost/algorithm/string.hpp"

namespace nly
{
// compress: if true then adjacent separators are merged together.
template<typename t_input, typename t_output, typename t_predicate>
t_output& split_if(t_input&& input, t_output& output, t_predicate pred, bool compress = false)
{
  return boost::split(
    output,
    input,
    pred,
    compress ? boost::algorithm::token_compress_on : boost::algorithm::token_compress_off);
}

// compress: if true then adjacent separators are merged together.
template<typename t_input, typename t_output, typename t_input_element>
t_output& split(
  t_input&&               input,
  t_output&               output,
  const t_input_element&& separator,
  bool                    compress = false)
{
  return split_if(
    input,
    output,
    [&separator](const t_input_element& value) { return value == separator; },
    compress);
}

} // namespace nly

#endif