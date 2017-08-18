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
 **/
#include "argcv/cxx/nio/tcp/tcp_listen_pool.h"

namespace argcv {
namespace nio {
namespace tcp {

const std::string SockStatusExplain(SockStatus status) {
  switch (status) {
    case SockStatus::kSockInvalid:
      return "SockStatus::kSockInvalid";
    case SockStatus::kSockClosed:
      return "SockStatus::kSockClosed";
    case SockStatus::kSockSuspend:
      return "SockStatus::kSockSuspend||SockStatus::kSockAlive";
    // case SockStatus::kSockRead:
    // return "SockStatus::kSockRead";
    case SockStatus::kSockPollin:
      return "SockStatus::kSockPollin";
    default:
      return "UNKNOWN";
  }
}
}  // namespace tcp
}  // namespace nio
}  // namespace argcv
