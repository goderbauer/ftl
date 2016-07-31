// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/ftl/files/path.h"

#include "string.h"

namespace files {
namespace {

size_t ResolveParentDirectoryTraversal(const std::string& path, size_t put) {
  if (put >= 2) {
    size_t previous_separator = path.rfind('/', put - 2);
    if (previous_separator != std::string::npos)
      return previous_separator + 1;
  }
  if (put == 1 && path[0] == '/') {
    return put;
  }
  return 0;
}

}  // namespace

std::string SimplifyPath(std::string path) {
  if (path.empty())
    return ".";

  size_t put = 0;
  size_t get = 0;
  size_t traversal_root = 0;
  size_t component_start = 0;

  if (path[0] == '/') {
    put = 1;
    get = 1;
    component_start = 1;
  }

  while (get < path.size()) {
    char c = path[get];

    if (c == '.' && (get == component_start || get == component_start + 1)) {
      // We've seen "." or ".." so far in this component. We need to continue
      // searching.
      ++get;
      continue;
    }

    if (c == '/') {
      if (get == component_start || get == component_start + 1) {
        // We've found a "/" or a "./", which we can elide.
        ++get;
        component_start = get;
        continue;
      }
      if (get == component_start + 2) {
        // We've found a "../", which means we need to remove the previous
        // component.
        if (put == traversal_root) {
          path[put++] = '.';
          path[put++] = '.';
          path[put++] = '/';
          traversal_root = put;
        } else {
          put = ResolveParentDirectoryTraversal(path, put);
        }
        ++get;
        component_start = get;
        continue;
      }
    }

    size_t next_separator = path.find('/', get);
    if (next_separator == std::string::npos) {
      // We've reached the last component.
      break;
    }
    size_t next_component_start = next_separator + 1;
    ++next_separator;
    size_t component_size = next_component_start - component_start;
    if (put != component_start && component_size > 0)
      path.copy(&path[put], component_size, component_start);
    put += component_size;
    get = next_component_start;
    component_start = next_component_start;
  }

  size_t last_component_size = path.size() - component_start;
  if (last_component_size == 1 && path[component_start] == '.') {
    // The last component is ".", which we can elide.
  } else if (last_component_size == 2 && path[component_start] == '.' &&
             path[component_start + 1] == '.') {
    // The last component is "..", which means we need to remove the previous
    // component.
    if (put == traversal_root) {
      path[put++] = '.';
      path[put++] = '.';
      path[put++] = '/';
      traversal_root = put;
    } else {
      put = ResolveParentDirectoryTraversal(path, put);
    }
  } else {
    // Otherwise, we need to copy over the last component.
    if (put != component_start && last_component_size > 0)
      path.copy(&path[put], last_component_size, component_start);
    put += last_component_size;
  }

  if (put >= 2 && path[put - 1] == '/')
    --put;  // Trim trailing /
  else if (put == 0)
    return ".";  // Use . for otherwise empty paths to treat them as relative.

  path.resize(put);
  return path;
}

// Returns the directory name component of the given path.
std::string GetDirectoryName(std::string path) {
  size_t separator = path.rfind('/');
  if (separator == 0u)
    return "/";
  if (separator == std::string::npos)
    return std::string();
  return path.substr(0, separator);
}

}  // namespace files
