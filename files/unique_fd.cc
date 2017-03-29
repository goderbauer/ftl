// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/ftl/files/unique_fd.h"

#include "lib/ftl/build_config.h"

#if defined(OS_WIN)
#include <io.h>
#else
#include <unistd.h>
#endif // defined(OS_WIN)

namespace ftl {
namespace internal {

void UniqueFDTraits::Free(int fd) {
  close(fd);
}

}  // namespace internal
}  // namespace ftl
