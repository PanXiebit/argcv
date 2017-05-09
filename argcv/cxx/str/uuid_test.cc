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
#include "argcv/cxx/str/uuid.h"

#include <iostream>

#include "gtest/gtest.h"

using argcv::str::UUID;

TEST(UUIDTest, Compare) {
  UUID u1;
  UUID u2;
  EXPECT_TRUE(u1 <= u2);
  EXPECT_TRUE(u1 < u2);
  EXPECT_TRUE(u2 >= u1);
  EXPECT_TRUE(u2 > u1);
}

TEST(UUIDTest, Copy) {
  UUID u1;
  UUID u2 = u1;
  EXPECT_TRUE(u1 == u2);
  //
  // std::cout << "u1.hex: " << u1.hex() << ":" << u1.hex().size() << std::endl;
  // int ix = 0;
  // for(char c: u1.hex()) {
  //   std::cout << "u1.hex:" << c << "/" << std::to_string(ix++)<< std::endl;
  // }
  // std::cout << "u1.str: " << u1.str() << ":" << u1.str().size() << std::endl;
  //
  // ix = 0;
  // for(char c: u1.str()) {
  //   std::cout << "u1.hex:" << c << "/" << std::to_string(ix++)<< std::endl;
  // }
  //
  // std::cout << "u1.pair (" << std::to_string(u1.pair().first) << "," <<
  // std::to_string(u1.pair().second) << ")"<<std::endl;

  UUID u3(u1.data());
  EXPECT_TRUE(u1 == u3);

  // std::cout << "u3.pair (" << std::to_string(u3.pair().first) << "," <<
  // std::to_string(u3.pair().second) << ")"<<std::endl;

  UUID u4(u1.hex());
  EXPECT_TRUE(u1 == u4);

  // std::cout << "u4.pair (" << std::to_string(u4.pair().first) << "," <<
  // std::to_string(u4.pair().second) << ")"<<std::endl;

  UUID u5(u1.str());
  EXPECT_TRUE(u1 == u5);

  // std::cout << "u5.pair (" << std::to_string(u5.pair().first) << "," <<
  // std::to_string(u5.pair().second) << ")"<<std::endl;

  std::pair<uint64_t, uint64_t> p = u1.pair();
  UUID u6(p.first, p.second);
  EXPECT_TRUE(u1 == u6);
}

TEST(UUIDTest, ToString) {
  UUID u1;
  EXPECT_EQ(u1.data().size(), sizeof(uint64_t) * 2);  // 16
  EXPECT_EQ(u1.hex().size(), 32);
  EXPECT_EQ(u1.str().size(), 36);
  // std::string s = u1.str();
  // for(int i = 0; i < s.size(); i++) {
  //   if(s[i] == '-') {
  //     std::cout << "XXX " << std::to_string(i) << std::endl;
  //   }
  // }
}
