/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ref: https://github.com/yuikns/unpbt/blob/master/lib/argcvnw/argcvnw.c
 * ref: https://github.com/yuikns/unpbt/blob/master/src/nwtest.c
 **/
#include "argcv/cxx/nio/tcp_listen_pool.h"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>  // malloc
#include <cstring>
// #include <future>  // NOLINT(build/c++11)
#include <thread>  // NOLINT(build/c++11)

#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::nio::TcpListenPool;
using argcv::nio::Conn;
using argcv::nio::sockStatusExplain;

typedef struct sockaddr SA;

static const int kListenPort = 19527;

void echo_server_test(int *status) {
  TcpListenPool pool(kListenPort, 3);
  size_t sz_min_sleep = 100;
  size_t sz_max_sleep = 300000;
  size_t sz_sleep = sz_min_sleep;
  EXPECT_EQ(0, pool.error_no());
  LOG(INFO) << "LISTENING...";
  (*status) = 1;
  LOG(INFO) << "status =>" << (*status);
  for (;;) {
    int id = pool.poll(0);
    if (id != -1) {
      sz_sleep = sz_min_sleep;
      LOG(INFO) << "ACCEPT, ID:" << id;
      Conn &c = pool[id];
      ssize_t st = pool.pull(id, 1024);
      LOG(INFO) << "ACCEPT, STATUS:" << st;
      if (st > 0) {
        std::string s = c.to_str();
        LOG(INFO) << "LENGTH:" << c.to_str().length() << " DATA[" << c.to_str()
                  << "]";
        c.write(c.to_str(), c.to_str().length());
        c.clear();
      } else {
        if (c.closed()) {
          LOG(INFO) << "is closed";
          break;
        } else {
          LOG(WARNING) << "unknown error:" << sockStatusExplain(c.status());
          break;
        }
      }
      c.flush();
    } else {
      // printf("sleep ...[%lu] time %lu\n",loop++,sz_sleep);
      // fflush(NULL);
      sz_sleep *= 2;
      if (sz_sleep > sz_max_sleep) {
        sz_sleep = sz_max_sleep;
      }
      usleep(sz_sleep);
    }
  }
  LOG(INFO) << "stop service";
}

/**
 * host: host name or ip
 * serv: port or service
 */
int tcp_connect(const char *host, const char *serv) {
  int sockfd;
  int n;
  struct addrinfo hints;
  struct addrinfo *res;
  struct addrinfo *ressave;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
    LOG(FATAL) << "tcp_connect error for " << gai_strerror(n) << ", " << host
               << ":" << serv;
    return 0;
  }

  ressave = res;

  // try to connect
  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // error , try next one
    if (sockfd < 0) continue;

    // connect , if return zero ,
    // it will be judged as success and break the loop
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;

    // connect error ,
    // close this one and try next
    close(sockfd);
  } while ((res = res->ai_next) != NULL);

  // errno from final socket() or connect()
  if (res == NULL) {
    fprintf(stderr, "tcp_connect error for %s:%s \n", host, serv);
    return 0;
  }

  freeaddrinfo(ressave);
  return sockfd;
}

void send_echo_request(const int &sock_fd, const char *buff) {
  EXPECT_LT(0, sock_fd);
  struct sockaddr_storage ss;
  socklen_t len = sizeof(ss);
  getpeername(sock_fd, reinterpret_cast<SA *>(&ss), &len);
  LOG(INFO) << "[client] connected";
  char send_buff[1024];
  char recv_buff[1024];
  memset(send_buff, 0, 1024);
  memset(recv_buff, 0, 1024);
  snprintf(send_buff, sizeof(send_buff), "%s", buff);
  ssize_t send_len = write(sock_fd, send_buff, strlen(send_buff));
  // ASSERT_EQ(send_len, strlen(send_buff));
  ssize_t recv_len = read(sock_fd, recv_buff, 1024);
  recv_buff[recv_len] = '\0';
  LOG(INFO) << "[echo result] " << recv_buff;
  ASSERT_EQ(recv_len, send_len);
  ASSERT_EQ(std::string(recv_buff), std::string(send_buff));
}

TEST(thread_pool, echo) {
  google::InitGoogleLogging("thread_pool::echo");

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  int serve_status = 0;

  std::thread my_server(echo_server_test, &serve_status);
  while (serve_status == 0) {
    usleep(333);
    LOG(INFO) << "waiting...";
  }
  LOG(INFO) << "starting...";
  int sock_fd = tcp_connect("127.0.0.1", "19527");

  send_echo_request(sock_fd, "this is a request");
  send_echo_request(sock_fd, "this is another request");

  usleep(1 * 1000 * 1000);
  LOG(INFO) << "START CLOSE FD";
  close(sock_fd);
  LOG(INFO) << "END CLOSE FD";
  my_server.join();
}
