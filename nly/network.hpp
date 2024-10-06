#ifndef NLY_NETWORK
#define NLY_NETWORK

#include "boost/asio.hpp"
#include "boost/beast.hpp"
#include "nly/time/time_count.hpp"
#include <optional>
#include <cassert>
#include <sstream>
#include <memory>
#include <vector>
#include <chrono>

namespace nly
{
namespace beast_http = boost::beast::http;

inline std::optional<boost::asio::ip::tcp::endpoint> make_tcp_endpoint(
  const std::string& ip,
  const int          port)
{
  try
  {
    return boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip), port);
  }
  catch (...)
  {
    return {};
  }
}

// Note: If the body of the return value is modified, remember to call prepare_payload() again.
template<typename Body = beast_http::string_body, class Fields = beast_http::fields>
inline beast_http::request<Body, Fields> make_request_msg(
  beast_http::verb                 method,
  const std::string&               target,
  const typename Body::value_type& body = {},
  const Fields&                    fields = {},
  unsigned int                     version = 11)
{
  auto result = beast_http::request<Body, Fields>(method, target, version, body, fields);
  result.prepare_payload();
  return result;
}

// Note: If the body of the return value is modified, remember to call prepare_payload() again.
template<typename Body = beast_http::string_body, class Fields = beast_http::fields>
inline beast_http::response<Body, Fields> make_response_msg(
  const typename Body::value_type& body = {},
  beast_http::status               status = beast_http::status::ok,
  const Fields&                    fields = {},
  unsigned int                     version = 11)
{
  auto result = beast_http::response<Body, Fields>(status, version, body, fields);
  result.prepare_payload();
  return result;
}

template<typename T>
inline std::string msg_to_string(const T& msg)
{
  std::ostringstream osm;
  osm << msg;
  return osm.str();
}

class http_client
{
public:
  static std::vector<boost::asio::ip::tcp::endpoint> resolve(
    const std::string&           host,
    const std::string&           service = "http",
    std::chrono::milliseconds    max_wait_time = std::chrono::milliseconds(5000),
    const std::function<bool()>& early_terminate = nullptr)
  {
    auto                                        start_time = nly::now();
    boost::asio::io_context                     cxt;
    boost::asio::ip::tcp::resolver              rv(cxt);
    std::vector<boost::asio::ip::tcp::endpoint> result;
    bool                                        finish = false;

    rv.async_resolve(
      host,
      service,
      [&result,
       &finish](boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type re)
      {
        finish = true;

        if (ec)
        {
          return;
        }

        for (auto it = re.begin(); it != re.end(); ++it)
        {
          result.emplace_back(std::move(it->endpoint()));
        }
      });

    http_client::wait(finish, cxt, start_time, max_wait_time, early_terminate);

    if (!finish)
    {
      rv.cancel();
      http_client::wait(finish, cxt);
    }

    return result;
  }

private:
  static void wait(
    bool&                                                finish,
    boost::asio::io_context&                             cxt,
    std::optional<std::chrono::steady_clock::time_point> startTime = {},
    std::optional<std::chrono::milliseconds>             max_wait_time = {},
    const std::function<bool()>&                         early_terminate = nullptr)
  {
    while (!finish)
    {
      if (!cxt.poll())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      if (
        early_terminate && early_terminate() ||
        startTime && max_wait_time && nly::time_diff_ms(*startTime) >= max_wait_time->count())
      {
        break;
      }
    }
  }

public:
  http_client()
    : m_sock(m_cxt)
    , m_guard(
        std::make_shared<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
          boost::asio::make_work_guard(m_cxt)))
  {
  }

public:
  // Use this function with caution.
  boost::asio::ip::tcp::socket& get_socket()
  {
    return m_sock;
  }

  const std::vector<boost::asio::ip::tcp::endpoint>& get_endpoints() const
  {
    return m_endpoints;
  }

  const bool is_connected() const
  {
    return m_connected;
  }

public:
  bool set_host(const std::vector<boost::asio::ip::tcp::endpoint>& endpoints)
  {
    if (endpoints.empty())
    {
      return false;
    }

    close();
    m_endpoints = endpoints;
    return true;
  }

  bool set_host(const boost::asio::ip::tcp::endpoint& endpoint)
  {
    return set_host(std::vector<boost::asio::ip::tcp::endpoint>{ endpoint });
  }

  bool set_host(
    const std::string&           host,
    const std::string&           service = "http",
    std::chrono::milliseconds    max_wait_time = std::chrono::milliseconds(5000),
    const std::function<bool()>& early_terminate = nullptr)
  {
    return set_host(http_client::resolve(host, service, max_wait_time, early_terminate));
  }

  // You should call set_host first.
  bool connect(
    std::chrono::milliseconds    max_wait_time_for_every_endpoint = std::chrono::milliseconds(5000),
    const std::function<bool()>& early_terminate = nullptr)
  {
    if (m_connected)
    {
      return true;
    }

    if (m_endpoints.empty())
    {
      return false;
    }

    auto fun = [this, &max_wait_time_for_every_endpoint, &early_terminate](
                 const boost::asio::ip::tcp::endpoint& addr)
    {
      auto start_time = std::chrono::steady_clock::now();
      bool finish = false;

      if (m_sock.is_open())
      {
        m_sock.close();
      }

      m_sock.async_connect(
        addr,
        [this, &finish](boost::system::error_code ec)
        {
          finish = true;

          if (!ec)
          {
            assert(!m_connected);
            m_connected = true;
          }
        });

      http_client::wait(
        finish,
        m_cxt,
        start_time,
        max_wait_time_for_every_endpoint,
        early_terminate);

      if (!finish)
      {
        m_sock.close();
        http_client::wait(finish, m_cxt);
        return false;
      }

      return m_connected;
    };

    for (auto& endpoint : m_endpoints)
    {
      if (fun(endpoint))
      {
        return true;
      }
    }

    return false;
  }

  // You should call connect first.
  template<
    typename ReqBody,
    typename ReqFields,
    typename RepBody = beast_http::string_body,
    typename RepFields = beast_http::fields>
  std::optional<beast_http::response<RepBody, RepFields>> deal_command(
    const beast_http::request<ReqBody, ReqFields>& req,
    std::chrono::milliseconds                      max_wait_time = std::chrono::milliseconds(5000),
    const std::function<bool()>&                   early_terminate = nullptr)
  {
    auto start_time = nly::now();

    auto deal_wait =
      [this, &start_time, &max_wait_time, &early_terminate](bool& finish, bool& success)
    {
      finish = false;
      success = false;

      http_client::wait(finish, m_cxt, start_time, max_wait_time, early_terminate);

      if (!finish)
      {
        m_sock.cancel();
        http_client::wait(finish, m_cxt);
        close();
        return false;
      }

      assert(finish);
      if (!success)
      {
        close();
        return false;
      }

      return true;
    };

    if (!m_connected)
    {
      return {};
    }

    bool finish = false;
    bool success = false;

    beast_http::async_write(
      m_sock,
      req,
      [&finish, &success](boost::system::error_code ec, std::size_t)
      {
        finish = true;
        success = !ec;
      });
    if (!deal_wait(finish, success))
    {
      return {};
    }

    beast_http::response<RepBody, RepFields> rep;
    beast_http::async_read(
      m_sock,
      boost::asio::dynamic_buffer(m_recv_buffer),
      rep,
      [&finish, &success](boost::system::error_code ec, std::size_t)
      {
        finish = true;
        success = !ec;
      });
    if (!deal_wait(finish, success))
    {
      return {};
    }

    return rep;
  }

  void close()
  {
    m_recv_buffer.clear();
    m_connected = false;
    m_sock.close();
  }

private:
  boost::asio::io_context                     m_cxt;
  boost::asio::ip::tcp::socket                m_sock;
  std::vector<boost::asio::ip::tcp::endpoint> m_endpoints;
  bool                                        m_connected{ false };
  std::vector<unsigned char>                  m_recv_buffer;

  std::shared_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> m_guard;
};

} // namespace nly

#endif // !NLY_NETWORK