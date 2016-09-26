// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "gtest/gtest.h"
#include "lib/ftl/strings/split_string.h"
#include "lib/ftl/strings/string_view.h"

namespace ftl {
namespace {

TEST(StringUtil, SplitString) {
  StringView sw = "First,\tSecond,Third\t ,, ";
  std::vector<std::string> r1 = {"First", "\tSecond", "Third\t ", "", " "};
  std::vector<std::string> r2 = {"First", "Second", "Third", "", ""};
  std::vector<std::string> r3 = {"First", "Second", "Third"};
  std::vector<std::string> r4 = {"First", "\tSecond", "Third\t ", " "};

  EXPECT_EQ(r1, SplitStringCopy(sw, ",", kKeepWhitespace, kSplitWantAll));
  EXPECT_EQ(r2, SplitStringCopy(sw, ",", kTrimWhitespace, kSplitWantAll));
  EXPECT_EQ(r3, SplitStringCopy(sw, ",", kTrimWhitespace, kSplitWantNonEmpty));
  EXPECT_EQ(r4, SplitStringCopy(sw, ",", kKeepWhitespace, kSplitWantNonEmpty));
}

}  // namespace
}  // namespace ftl
