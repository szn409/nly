#include "gtest/gtest.h"
#include "nly/network.hpp"
#include "nly/string.hpp"
#include <thread>
#include <map>

TEST(NetWork, MakeTcpEndPoint)
{
  auto addr = nly::make_tcp_endpoint("127.0.0.1", 1234);
  EXPECT_EQ(addr->address().to_string(), "127.0.0.1");
  EXPECT_EQ(addr->port(), 1234);
  EXPECT_TRUE(!nly::make_tcp_endpoint("127.0.0.666", 1234));
}

TEST(NetWork, MakeRequestMsg)
{
  nly::beast_http::fields header;
  header.insert(nly::beast_http::field::host, "www.szn.com");
  auto req = nly::make_request_msg(nly::beast_http::verb::get, "/hello", "bad world", header);
  EXPECT_EQ(req.method(), nly::beast_http::verb::get);
  EXPECT_EQ(req.target(), "/hello");
  EXPECT_EQ(req.body(), "bad world");
  EXPECT_EQ(req.at("host"), "www.szn.com");
  EXPECT_EQ(req.at(nly::beast_http::field::content_length), "9");
  EXPECT_EQ(req.version(), 11);
}

TEST(NetWork, MakeResponseMsg)
{
  nly::beast_http::fields header;
  header.insert(nly::beast_http::field::host, "www.szn.com");
  auto res = nly::make_response_msg("bad world", nly::beast_http::status::not_found, header);

  EXPECT_EQ(res.body(), "bad world");
  EXPECT_EQ(res.result(), nly::beast_http::status::not_found);
  EXPECT_EQ(res.at("host"), "www.szn.com");
  EXPECT_EQ(res.at(nly::beast_http::field::content_length), "9");
  EXPECT_EQ(res.version(), 11);
}

TEST(NetWork, MsgToString)
{
  nly::beast_http::fields header;
  header.insert(nly::beast_http::field::host, "www.szn.com");

  auto req = nly::make_request_msg(nly::beast_http::verb::get, "/hello", "bad world", header);
  std::string target = R"(GET /hello HTTP/1.1
Host: www.szn.com
Content-Length: 9

bad world)";
  nly::replace_all(target, "\n", "\r\n");
  EXPECT_EQ(nly::msg_to_string(req), target);

  auto res = nly::make_response_msg("bad world", nly::beast_http::status::not_found, header);
  target = R"(HTTP/1.1 404 Not Found
Host: www.szn.com
Content-Length: 9

bad world)";
  nly::replace_all(target, "\n", "\r\n");
  EXPECT_EQ(nly::msg_to_string(res), target);
}

TEST(NetWork, Resolve)
{
  auto re = nly::http_client::resolve("localhost");
  EXPECT_EQ(re.size(), 2);
  EXPECT_EQ(re[0].address().to_string(), "::1");
  EXPECT_EQ(re[0].port(), 80);
  EXPECT_EQ(re[1].address().to_string(), "127.0.0.1");
  EXPECT_EQ(re[1].port(), 80);

  re = nly::http_client::resolve("localhost", "https");
  EXPECT_EQ(re.size(), 2);
  EXPECT_EQ(re[0].address().to_string(), "::1");
  EXPECT_EQ(re[0].port(), 443);
  EXPECT_EQ(re[1].address().to_string(), "127.0.0.1");
  EXPECT_EQ(re[1].port(), 443);
}

TEST(NetWork, Http)
{
  std::thread thd(
    []()
    {
      boost::asio::io_context        cxt;
      boost::asio::ip::tcp::acceptor server(cxt);

      std::map<std::string, std::string> db;
      std::vector<unsigned char>         recv_buffer;

      server.open(boost::asio::ip::tcp::v4());
      server.bind(*nly::make_tcp_endpoint("127.0.0.1", 80));
      server.listen();
      auto client = server.accept();

      for (int i = 0; i < 3; ++i)
      {
        nly::beast_http::request<nly::beast_http::string_body, nly::beast_http::fields> req;
        nly::beast_http::read(client, boost::asio::dynamic_buffer(recv_buffer), req);

        auto res = nly::make_response_msg();
        if (req.method() == nly::beast_http::verb::get)
        {
          try
          {
            res.body() = db.at(req.target());
          }
          catch (...)
          {
            res.body() = "target not found";
          }
        }
        else
        {
          assert(req.method() == nly::beast_http::verb::post);
          db[req.target()] = req.body();
        }
        res.prepare_payload();
        nly::beast_http::write(client, res);
      }
    });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  nly::http_client client;
  EXPECT_FALSE(client.is_connected());
  EXPECT_TRUE(client.get_endpoints().empty());
  EXPECT_TRUE(client.set_host("localhost"));
  EXPECT_EQ(client.get_endpoints().size(), 2);
  EXPECT_TRUE(client.connect());
  EXPECT_TRUE(client.is_connected());

  auto req = nly::make_request_msg(nly::beast_http::verb::get, "hello");
  auto res = client.deal_command(req);
  EXPECT_EQ(res->body(), "target not found");

  req = nly::make_request_msg(nly::beast_http::verb::post, "hello", "hello world");
  client.deal_command(req);

  req = nly::make_request_msg(nly::beast_http::verb::get, "hello");
  res = client.deal_command(req);
  EXPECT_EQ(res->body(), "hello world");

  client.close();
  EXPECT_TRUE(!client.deal_command(req));

  thd.join();
}
