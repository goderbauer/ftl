// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FTL_INTTYPES_H_
#define LIB_FTL_INTTYPES_H_

#include "lib/ftl/build_config.h"

#include <stdint.h>

#if defined(OS_WIN)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#endif  // LIB_FTL_INTTYPES_H_
