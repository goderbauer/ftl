// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/ftl/strings/ascii.h"

namespace ftl {

bool EqualsCaseInsensitiveASCII(ftl::StringView v1, ftl::StringView v2) {
  if (v1.size() != v2.size())
    return false;
  for (size_t i = 0; i < v1.size(); ++i) {
    if (ToLowerASCII(v1[i]) != ToLowerASCII(v2[i]))
      return false;
  }
  return true;
}

}  // namespace ftl
