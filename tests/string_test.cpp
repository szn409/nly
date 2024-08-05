#include "gtest/gtest.h"
#include "nly/string.hpp"
#include <string>
#include <vector>

TEST(String, Split)
{
  using std::string;
  using std::vector;
  using std::wstring;

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