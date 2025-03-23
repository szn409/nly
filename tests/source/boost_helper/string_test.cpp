#include "gtest/gtest.h"
#include "nly/boost_helper/string.hpp"
#include <string>
#include <vector>

using std::string;
using std::u16string;
using std::u32string;
using std::vector;
using std::wstring;

TEST(String, Split)
{
  auto fun = [](const char value) { return value == ';'; };

  vector<string> vec;
  nly::split("12;34;56", vec, ';');
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));
  nly::split_if("12;34;56", vec, fun);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));

  nly::split("12;34;56;", vec, ';');
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56", "" }));
  nly::split_if("12;34;56;", vec, fun);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56", "" }));

  nly::split("12;34;;56", vec, ';');
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "", "56" }));
  nly::split_if("12;34;;56", vec, fun);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "", "56" }));

  nly::split("12;34;;;;56", vec, ';', true);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));
  nly::split_if("12;34;;;;56", vec, fun, true);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));

  vector<wstring> out_w;
  nly::split(L"12;34;56", out_w, L';');
  EXPECT_TRUE(out_w == (vector<wstring>{ L"12", L"34", L"56" }));
  nly::split_if(L"12;34;56", out_w, [](auto v) { return v == L';'; });
  EXPECT_TRUE(out_w == (vector<wstring>{ L"12", L"34", L"56" }));

  vector<u16string> out_16;
  nly::split(u"12;34;56", out_16, u';');
  EXPECT_TRUE(out_16 == (vector<u16string>{ u"12", u"34", u"56" }));
  nly::split_if(u"12;34;56", out_16, [](auto v) { return v == u';'; });
  EXPECT_TRUE(out_16 == (vector<u16string>{ u"12", u"34", u"56" }));

  vector<u32string> out_32;
  nly::split(U"12;34;56", out_32, U';');
  EXPECT_TRUE(out_32 == (vector<u32string>{ U"12", U"34", U"56" }));
  nly::split_if(U"12;34;56", out_32, [](auto v) { return v == U';'; });
  EXPECT_TRUE(out_32 == (vector<u32string>{ U"12", U"34", U"56" }));
}

TEST(String, SplitFast)
{
  const char* value = "123-456--789";

  auto                                        fun = [](const char t) { return t == '-'; };
  nly::split_fast<const char*, decltype(fun)> fast(value, fun, true);

  EXPECT_TRUE(fast.get()->begin() == value);
  EXPECT_TRUE(fast.get()->end() == value + 3);

  vector<string> vec;

  while (!fast.is_stop())
  {
    vec.emplace_back(string(fast.get()->begin(), fast.get()->end()));
    fast.next();
  }

  auto re = fast.get();
  EXPECT_TRUE(re->empty() && fast.is_stop());
  EXPECT_TRUE(vec == (vector<string>{ "123", "456", "789" }));
}

TEST(String, ToUpperAndToLow)
{
  string str = "HeLLo 123";
  nly::to_upper(str);
  EXPECT_TRUE(str == "HELLO 123");
  nly::to_lower(str);
  EXPECT_TRUE(str == "hello 123");

  str = "αβγδ";
  nly::to_upper(str);
  EXPECT_FALSE(str == "ΑΒΓΔ");

  std::wstring wstr = L"αβγδ";
  nly::to_upper(wstr);
  EXPECT_TRUE(wstr == L"ΑΒΓΔ");
  nly::to_lower(wstr);
  EXPECT_TRUE(wstr == L"αβγδ");

  std::u16string u16str = u"αβγδ";
  nly::to_upper(u16str);
  EXPECT_FALSE(u16str == u"ΑΒΓΔ");

  u16str = u"hello";
  nly::to_upper(u16str);
  EXPECT_TRUE(u16str == u"HELLO");
  nly::to_lower(u16str);
  EXPECT_TRUE(u16str == u"hello");

  std::u32string u32str = U"αβγδ";
  nly::to_upper(u32str);
  EXPECT_FALSE(u32str == U"ΑΒΓΔ");

  u32str = U"hello";
  nly::to_upper(u32str);
  EXPECT_TRUE(u32str == U"HELLO");
  nly::to_lower(u32str);
  EXPECT_TRUE(u32str == U"hello");
}

TEST(String, StartsWithAndEndsWith)
{
  std::string str = "hello world 你好 abc";

  EXPECT_TRUE(nly::starts_with(str, "hello"));
  EXPECT_FALSE(nly::starts_with(str, "HELLO"));
  EXPECT_TRUE(nly::starts_with(str, "HELLO", false));

  EXPECT_TRUE(nly::ends_with(str, "你好 abc"));
  EXPECT_FALSE(nly::ends_with(str, "你好 ABC"));
  EXPECT_TRUE(nly::ends_with(str, "你好 ABC", false));

  str = "αβγδ";
  EXPECT_TRUE(nly::starts_with(str, "αβ"));
  EXPECT_TRUE(nly::ends_with(str, "γδ"));

  EXPECT_FALSE(nly::starts_with(str, "ΑΒ", false));

  std::wstring wstr = L"αβγδ";
  EXPECT_TRUE(nly::starts_with(wstr, L"ΑΒ", false));
}

TEST(String, Trim)
{
  string str = "  hello world  ";
  nly::trim(str);
  EXPECT_TRUE(str == "hello world");

  str = "  hello world  ";
  nly::trim(str, nly::string_trim_type::right);
  EXPECT_TRUE(str == "  hello world");

  str = "  hello world  ";
  nly::trim(str, nly::string_trim_type::left);
  EXPECT_TRUE(str == "hello world  ");

  auto fun = [](char value) { return value == '-' || value == '@'; };

  str = "--hello world@@";
  nly::trim_if(str, fun);
  EXPECT_TRUE(str == "hello world");

  str = "--hello world@@";
  nly::trim_if(str, fun, nly::string_trim_type::right);
  EXPECT_TRUE(str == "--hello world");

  str = "--hello world@@";
  nly::trim_if(str, fun, nly::string_trim_type::left);
  EXPECT_TRUE(str == "hello world@@");
}

TEST(String, Join)
{
  vector<string> vec = { "123", "456", "789" };
  EXPECT_TRUE(nly::join(vec, " - ") == "123 - 456 - 789");

  vec = {};
  EXPECT_TRUE(nly::join(vec, " - ") == "");

  vec = { "123" };
  EXPECT_TRUE(nly::join(vec, " - ") == "123");

  vec = { "1", "23", "4", "56" };
  EXPECT_TRUE(nly::join_if(vec, "-", [](const auto& str) { return str.size() > 1; }) == "23-56");
}

TEST(String, Contains)
{
  EXPECT_TRUE(nly::contains("abcd", "bc"));
  EXPECT_FALSE(nly::contains("abcd", "BC"));
  EXPECT_TRUE(nly::contains("abcd", "BC", false));

  EXPECT_TRUE(nly::contains("αβγδ", "αβ"));
  EXPECT_FALSE(nly::contains("αβγδ", "ΑΒ", false));

  EXPECT_TRUE(nly::contains(L"αβγδ", L"αβ"));
  EXPECT_TRUE(nly::contains(L"αβγδ", L"ΑΒ", false));
}

TEST(String, Equals)
{
  string str_0 = "abcd";
  string str_1 = "abcd";
  EXPECT_TRUE(nly::equals(str_0, str_1));
  EXPECT_FALSE(nly::equals("ABC", "abc"));
  EXPECT_TRUE(nly::equals("ABC", "abc", false));

  EXPECT_FALSE(nly::equals("αβγδ", "ΑΒΓΔ", false));
  EXPECT_TRUE(nly::equals(L"αβγδ", L"ΑΒΓΔ", false));
}

TEST(String, LexicographicalCompare)
{
  EXPECT_FALSE(nly::lexicographical_compare("abc", "BCD"));
  EXPECT_TRUE(nly::lexicographical_compare("abc", "BCD", false));
  EXPECT_TRUE(nly::lexicographical_compare("AB", "B"));
}

TEST(StringAlgo, Find)
{
  string str("ABC abc");

  auto re = nly::find_first(str, "bc");
  EXPECT_TRUE(re.begin() - str.begin() == 5);

  re = nly::find_first(str, "bc", false);
  EXPECT_TRUE(re.begin() - str.begin() == 1);

  re = nly::find_first(str, "hello");
  EXPECT_TRUE(re.begin() == str.end() && re.end() == str.end());

  re = nly::find_last(str, "BC");
  EXPECT_TRUE(re.begin() - str.begin() == 1);

  re = nly::find_last(str, "BC", false);
  EXPECT_TRUE(re.begin() - str.begin() == 5);

  re = nly::find_last(str, "hello");
  EXPECT_TRUE(re.begin() == str.end() && re.end() == str.end());
}

TEST(StringAlgo, Replace)
{
  string str = "abc ABc 123";
  auto   tem = str;
  nly::replace_first(tem, "AB", "---");
  EXPECT_TRUE(tem == "abc ---c 123");

  tem = str;
  nly::replace_first(tem, "AB", "---", false);
  EXPECT_TRUE(tem == "---c ABc 123");

  nly::replace_last(tem, "ab", "---");
  EXPECT_TRUE(tem == "---c ABc 123");

  tem = str;
  nly::replace_last(tem, "ab", "---", false);
  EXPECT_TRUE(tem == "abc ---c 123");

  str = "abc ABC ab 123";
  tem = str;
  nly::replace_nth(tem, "AB", 0, "-");
  EXPECT_TRUE(tem == "abc -C ab 123");

  tem = str;
  nly::replace_nth(tem, "AB", 0, "-", false);
  EXPECT_TRUE(tem == "-c ABC ab 123");

  tem = str;
  nly::replace_nth(tem, "AB", 2, "-", false);
  EXPECT_TRUE(tem == "abc ABC - 123");

  tem = str;
  nly::replace_nth(tem, "AB", -1, "-", false);
  EXPECT_TRUE(tem == "abc ABC - 123");

  tem = str;
  nly::replace_nth(tem, "AB", -2, "-", false);
  EXPECT_TRUE(tem == "abc -C ab 123");

  tem = str;
  nly::replace_nth(tem, "AB", -2048, "-", false);
  EXPECT_TRUE(tem == "abc ABC ab 123");

  tem = str;
  nly::replace_nth(tem, "AB", 2048, "-", false);
  EXPECT_TRUE(tem == "abc ABC ab 123");

  tem = str;
  nly::replace_all(tem, "AB", "-", false);
  EXPECT_TRUE(tem == "-c -C - 123");

  tem = str;
  nly::replace_all(tem, "AB", "-");
  EXPECT_TRUE(tem == "abc -C ab 123");

  str = "abc ABC ab 123";
  tem = str;
  nly::replace_head(tem, 3, "hello");
  EXPECT_TRUE(tem == "hello ABC ab 123");

  tem = str;
  nly::replace_head(tem, 30000, "hello");
  EXPECT_TRUE(tem == "hello");

  tem = str;
  nly::replace_head(tem, 0, "hello");
  EXPECT_TRUE(tem == str);

  tem = str;
  nly::replace_head(tem, -2, "hello-");
  EXPECT_TRUE(tem == "hello-23");

  tem = str;
  nly::replace_head(tem, -20000, "hello-");
  EXPECT_TRUE(tem == str);

  str = "abc ABC ab 123";
  tem = str;
  nly::replace_tail(tem, 3, "hello");
  EXPECT_TRUE(tem == "abc ABC ab hello");

  tem = str;
  nly::replace_tail(tem, 30000, "hello");
  EXPECT_TRUE(tem == "hello");

  tem = str;
  nly::replace_tail(tem, 0, "hello");
  EXPECT_TRUE(tem == str);

  tem = str;
  nly::replace_tail(tem, -2, "-hello");
  EXPECT_TRUE(tem == "ab-hello");

  tem = str;
  nly::replace_tail(tem, -20000, "hello-");
  EXPECT_TRUE(tem == str);
}

TEST(StringAlgo, Erase)
{
  string str = "abc ABC 123";

  auto tem = str;
  nly::erase_first(tem, "AB");
  EXPECT_TRUE(tem == "abc C 123");

  tem = str;
  nly::erase_first(tem, "AB", false);
  EXPECT_TRUE(tem == "c ABC 123");

  tem = str;
  nly::erase_last(tem, "ab");
  EXPECT_TRUE(tem == "c ABC 123");

  tem = str;
  nly::erase_last(tem, "ab", false);
  EXPECT_TRUE(tem == "abc C 123");

  str = "abc ABC abc 123";

  tem = str;
  nly::erase_nth(tem, "ab", 0);
  EXPECT_TRUE(tem == "c ABC abc 123");

  tem = str;
  nly::erase_nth(tem, "ab", 1, false);
  EXPECT_TRUE(tem == "abc C abc 123");

  tem = str;
  nly::erase_nth(tem, "ab", 1024, false);
  EXPECT_TRUE(tem == str);

  tem = str;
  nly::erase_nth(tem, "ab", -1, false);
  EXPECT_TRUE(tem == "abc ABC c 123");

  tem = str;
  nly::erase_nth(tem, "ab", -1024, false);
  EXPECT_TRUE(tem == str);

  str = "abc ABC abc 123";

  tem = str;
  nly::erase_all(tem, "ab");
  EXPECT_TRUE(tem == "c ABC c 123");

  nly::erase_all(tem, "ab", false);
  EXPECT_TRUE(tem == "c C c 123");

  str = "0123456789";
  tem = str;
  nly::erase_head(tem, 2);
  EXPECT_TRUE(tem == "23456789");

  tem = str;
  nly::erase_head(tem, 200);
  EXPECT_TRUE(tem == "");

  tem = str;
  nly::erase_head(tem, 0);
  EXPECT_TRUE(tem == str);

  tem = str;
  nly::erase_head(tem, -2);
  EXPECT_TRUE(tem == "89");

  str = "0123456789";
  tem = str;
  nly::erase_tail(tem, 2);
  EXPECT_TRUE(tem == "01234567");

  tem = str;
  nly::erase_tail(tem, 200);
  EXPECT_TRUE(tem == "");

  tem = str;
  nly::erase_tail(tem, 0);
  EXPECT_TRUE(tem == str);

  tem = str;
  nly::erase_tail(tem, -2);
  EXPECT_TRUE(tem == "01");
}
