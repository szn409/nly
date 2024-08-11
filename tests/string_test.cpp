#include "gtest/gtest.h"
#include "nly/string.hpp"
#include <string>
#include <vector>

using std::string;
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

  nly::split("12;34;;56", vec, ';', true);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));
  nly::split_if("12;34;;56", vec, fun, true);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));

  vector<wstring> out;
  nly::split(L"12;34;56", out, L';');
  EXPECT_TRUE(out == (vector<wstring>{ L"12", L"34", L"56" }));
  nly::split_if(L"12;34;56", out, [](auto v) { return v == L';'; });
  EXPECT_TRUE(out == (vector<wstring>{ L"12", L"34", L"56" }));
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
  string str = "HeLLo 123，你好";

  auto tem = str;
  nly::to_upper(tem);
  EXPECT_TRUE(tem == "HELLO 123，你好");

  tem = str;
  nly::to_lower(tem);
  EXPECT_TRUE(tem == "hello 123，你好");

  str = "αβγδ";
  tem = str;
  nly::to_upper(tem);
  EXPECT_FALSE(tem == "ΑΒΓΔ");

  std::wstring wstr = L"αβγδ";
  auto         wtem = wstr;
  nly::to_upper(wtem);
  EXPECT_TRUE(wtem == L"ΑΒΓΔ");
}

TEST(String, StartsWithAndEndsWith)
{
  const char* str = "hello world 你好 abc";

  EXPECT_TRUE(nly::starts_with(str, "hello"));
  EXPECT_FALSE(nly::starts_with(str, "HELLO"));
  EXPECT_TRUE(nly::starts_with(str, "HELLO", false));

  EXPECT_TRUE(nly::ends_with(str, "你好 abc"));
  EXPECT_FALSE(nly::ends_with(str, "你好 ABC"));
  EXPECT_TRUE(nly::ends_with(str, "你好 ABC", false));
}

TEST(String, Trim)
{
  string str = "  hello world  ";

  auto tem = str;
  nly::trim(tem);
  EXPECT_TRUE(tem == "hello world");

  tem = str;
  nly::trim(tem, NLY_TRIM_RIGHT);
  EXPECT_TRUE(tem == "  hello world");

  tem = str;
  nly::trim(tem, NLY_TRIM_LEFT);
  EXPECT_TRUE(tem == "hello world  ");

  str = "--hello world@@";
  tem = str;
  nly::trim_if(tem, [](char value) { return value == '-' || value == '@'; });
  EXPECT_TRUE(tem == "hello world");

  tem = str;
  nly::trim_if(tem, [](char value) { return value == '-' || value == '@'; }, NLY_TRIM_RIGHT);
  EXPECT_TRUE(tem == "--hello world");

  tem = str;
  nly::trim_if(tem, [](char value) { return value == '-' || value == '@'; }, NLY_TRIM_LEFT);
  EXPECT_TRUE(tem == "hello world@@");
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
}

TEST(String, Equals)
{
  string str_0 = "abcd";
  string str_1 = "abcd";
  EXPECT_TRUE(nly::equals(str_0, str_1));
  EXPECT_FALSE(nly::equals("ABC", "abc"));
  EXPECT_TRUE(nly::equals("ABC", "abc", false));
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
