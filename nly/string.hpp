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

// no copying occurs during execution.
template<typename t_input, typename t_predicate>
class split_fast
{
private:
  typedef boost::algorithm::split_iterator<typename boost::range_iterator<t_input>::type>
    t_split_iterator;

public:
  // compress: if true then adjacent separators are merged together.
  split_fast(const t_input& input, t_predicate pred, bool compress = false)
  {
    this->iter_ = boost::algorithm::make_split_iterator(
      input,
      boost::algorithm::token_finder(
        pred,
        compress ? boost::algorithm::token_compress_on : boost::algorithm::token_compress_off));
  }

  // get current split result.
  const t_split_iterator& get() const
  {
    return this->iter_;
  }

  // get next split result
  const t_split_iterator& next()
  {
    ++this->iter_;
    return this->get();
  }

  const bool is_stop() const
  {
    return this->iter_.eof();
  }

private:
  t_split_iterator iter_;
};

// for converting characters like "αβγδ" that are non-ASCII, use wstring instead of string.
// this rule applies to all functions involving case conversion.
template<typename T>
void to_upper(T& input, const std::locale& loc = std::locale("zh_CN.utf8"))
{
  boost::algorithm::to_upper(input, loc);
}

// for converting characters like "αβγδ" that are non-ASCII, use wstring instead of string.
// this rule applies to all functions involving case conversion.
template<typename T>
void to_lower(T& input, const std::locale& loc = std::locale("zh_CN.utf8"))
{
  boost::algorithm::to_lower(input, loc);
}

template<typename t_input, typename t_match>
bool starts_with(
  const t_input&     input,
  const t_match&     match,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::starts_with(input, match);
  }

  return boost::algorithm::istarts_with(input, match, loc);
}

template<typename t_input, typename t_match>
bool ends_with(
  const t_input&     input,
  const t_match&     match,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::ends_with(input, match);
  }

  return boost::algorithm::iends_with(input, match, loc);
}

#define NLY_TRIM_ALL 0
#define NLY_TRIM_RIGHT 1
#define NLY_TRIM_LEFT 2

template<typename t_input>
void trim(t_input& input, int mode = NLY_TRIM_ALL)
{
  if (NLY_TRIM_ALL == mode)
  {
    boost::algorithm::trim(input);
  }
  else if (NLY_TRIM_LEFT == mode)
  {
    boost::algorithm::trim_left(input);
  }
  else
  {
    assert(mode == NLY_TRIM_RIGHT);
    boost::algorithm::trim_right(input);
  }
}

template<typename t_input, typename t_predicate>
void trim_if(t_input& input, t_predicate pred, int mode = NLY_TRIM_ALL)
{
  if (NLY_TRIM_ALL == mode)
  {
    boost::algorithm::trim_if(input, pred);
  }
  else if (NLY_TRIM_LEFT == mode)
  {
    boost::algorithm::trim_left_if(input, pred);
  }
  else
  {
    assert(mode == NLY_TRIM_RIGHT);
    boost::algorithm::trim_right_if(input, pred);
  }
}

template<typename t_input, typename t_join>
auto join(const t_input& input, const t_join& join_str)
{
  return boost::algorithm::join(input, join_str);
}

template<typename t_input, typename t_join, typename t_predicate>
auto join_if(const t_input& input, const t_join& join_str, t_predicate pred)
{
  return boost::algorithm::join_if(input, join_str, pred);
}

template<typename t_input, typename t_test>
bool contains(
  const t_input&     input,
  const t_test&      test,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::contains(input, test);
  }

  return boost::algorithm::icontains(input, test, loc);
}

template<typename t_input_1, typename t_input_2>
bool equals(
  const t_input_1&   input_1,
  const t_input_2&   input_2,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::equals(input_1, input_2);
  }

  return boost::algorithm::iequals(input_1, input_2, loc);
}

// std::lexicographical_compare is called inside the function.
template<typename t_input_1, typename t_input_2>
bool lexicographical_compare(
  const t_input_1&   input_1,
  const t_input_2&   input_2,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::lexicographical_compare(input_1, input_2);
  }

  return boost::algorithm::ilexicographical_compare(input_1, input_2, loc);
}

template<typename t_input, typename t_match>
auto find_first(
  const t_input&     input,
  const t_match&     match,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::find_first(input, match);
  }

  return boost::algorithm::ifind_first(input, match, loc);
}

template<typename t_input, typename t_match>
auto find_last(
  const t_input&     input,
  const t_match&     match,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::find_last(input, match);
  }

  return boost::algorithm::ifind_last(input, match, loc);
}

template<typename t_input, typename t_match, typename t_replace>
void replace_first(
  t_input&           input,
  const t_match&     match,
  const t_replace&   replace,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::replace_first(input, match, replace);
  }

  return boost::algorithm::ireplace_first(input, match, replace, loc);
}

template<typename t_input, typename t_match, typename t_replace>
void replace_last(
  t_input&           input,
  const t_match&     match,
  const t_replace&   replace,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::replace_last(input, match, replace);
  }

  return boost::algorithm::ireplace_last(input, match, replace, loc);
}

// nth starts from 0, 1 identifies the second item, -1 identifies the last item, -2 identifies the
// second-to-last item, and so on.
template<typename t_input, typename t_match, typename t_replace>
void replace_nth(
  t_input&           input,
  const t_match&     match,
  int                nth,
  const t_replace&   replace,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::replace_nth(input, match, nth, replace);
  }

  return boost::algorithm::ireplace_nth(input, match, nth, replace, loc);
}

template<typename t_input, typename t_match, typename t_replace>
void replace_all(
  t_input&           input,
  const t_match&     match,
  const t_replace&   replace,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::replace_all(input, match, replace);
  }

  return boost::algorithm::ireplace_all(input, match, replace, loc);
}

template<typename t_input, typename t_replace>
void replace_head(t_input& input, int replace_len, const t_replace& replace)
{
  return boost::algorithm::replace_head(input, replace_len, replace);
}

template<typename t_input, typename t_replace>
void replace_tail(t_input& input, int replace_len, const t_replace& replace)
{
  return boost::algorithm::replace_tail(input, replace_len, replace);
}

template<typename t_input, typename t_erase>
void erase_first(
  t_input&           input,
  const t_erase&     erase,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::erase_first(input, erase);
  }

  return boost::algorithm::ierase_first(input, erase, loc);
}

template<typename t_input, typename t_erase>
void erase_last(
  t_input&           input,
  const t_erase&     erase,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::erase_last(input, erase);
  }

  return boost::algorithm::ierase_last(input, erase, loc);
}

// nth starts from 0, 1 identifies the second item, -1 identifies the last item, -2 identifies the
// second-to-last item, and so on.
template<typename t_input, typename t_erase>
void erase_nth(
  t_input&           input,
  const t_erase&     erase,
  int                nth,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::erase_nth(input, erase, nth);
  }

  return boost::algorithm::ierase_nth(input, erase, nth, loc);
}

template<typename t_input, typename t_erase>
void erase_all(
  t_input&           input,
  const t_erase&     erase,
  bool               case_sensitive = true,
  const std::locale& loc = std::locale("zh_CN.utf8"))
{
  if (case_sensitive)
  {
    return boost::algorithm::erase_all(input, erase);
  }

  return boost::algorithm::ierase_all(input, erase, loc);
}

template<typename t_input>
void erase_head(t_input& input, int erase_len)
{
  return boost::algorithm::erase_head(input, erase_len);
}

template<typename t_input>
void erase_tail(t_input& input, int erase_len)
{
  return boost::algorithm::erase_tail(input, erase_len);
}

} // namespace nly

#endif