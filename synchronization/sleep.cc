// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/ftl/synchronization/sleep.h"

#include "lib/ftl/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#else
#include <time.h>
#endif // defined(OS_WIN)

namespace ftl {

void SleepFor(TimeDelta duration) {
#if defined(OS_WIN)
  LARGE_INTEGER duration_time;
  duration_time.QuadPart = (duration.ToNanoseconds() / 100) * -1;
  HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &duration_time, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
#else
  struct timespec ts = duration.ToTimespec();
  nanosleep(&ts, nullptr);
#endif // defined(OS_WIN)
}

}  // namespace ftl
