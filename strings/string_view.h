// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FTL_STRINGS_STRING_VIEW_H_
#define LIB_FTL_STRINGS_STRING_VIEW_H_

#include <iosfwd>
#include <string>
#include <type_traits>

#include "lib/ftl/logging.h"

namespace ftl {
namespace {
  int constexpr length(const char* str) {
    return *str ? 1 + length(str + 1) : 0;
  }
} // namespace

// A string-like object that points to a sized piece of memory.
class StringView {
 public:
  // Types.
  using const_iterator = const char*;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;

  constexpr static size_t npos = static_cast<size_t>(-1);

  // Constructors.
  constexpr StringView() : data_(""), size_(0u) {}

  constexpr StringView(const StringView& string_view)
      : data_(string_view.data_), size_(string_view.size_) {}

  constexpr StringView(const char* str, size_t len) : data_(str), size_(len) {}

  // strlen is not a constexpr function, using length from above.
  explicit constexpr StringView(const char* str)
      : data_(str), size_(length(str)) {}

  // Implicit constructor for constant C strings.
  template<size_t N>
  constexpr StringView(const char(&str)[N])
      : data_(str), size_(length(str)) {}

  // Implicit constructor.
  StringView(const std::string& str) : data_(str.data()), size_(str.size()) {}

  // Copy operators.
  StringView& operator=(const StringView& other) {
    data_ = other.data_;
    size_ = other.size_;
    return *this;
  }

  // Capacity methods.
  constexpr size_t size() const { return size_; }
  constexpr bool empty() const { return size_ == 0u; }

  // Element access methods.
  constexpr char operator[](size_t pos) const { return data_[pos]; };
  constexpr char at(size_t pos) const { return data_[pos]; };
  constexpr char front() const { return data_[0]; };
  constexpr char back() const { return data_[size_ - 1]; };
  constexpr const char* data() const { return data_; }

  // Iterators.
  constexpr const_iterator begin() const { return cbegin(); }
  constexpr const_iterator end() const { return cend(); }
  constexpr const_iterator cbegin() const { return data_; }
  constexpr const_iterator cend() const { return data_ + size_; }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(cbegin());
  }
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  // Modifier methods.
  void clear() {
    data_ = "";
    size_ = 0;
  }
  void remove_prefix(size_t n) {
    FTL_DCHECK(n <= size_);
    data_ += n;
    size_ -= n;
  }
  void remove_suffix(size_t n) {
    FTL_DCHECK(n <= size_);
    size_ -= n;
  }
  void swap(StringView& other) {
    const char* data = data_;
    data_ = other.data_;
    other.data_ = data;

    size_t size = size_;
    size_ = other.size_;
    other.size_ = size;
  }

  // String conversion.
  std::string ToString() const { return std::string(data_, size_); }

  // String operations.
  constexpr StringView substr(size_t pos = 0, size_t n = npos) const {
    return StringView(data_ + pos, min(n, size_ - pos));
  }

  int compare(StringView other);
  size_t find(StringView s, size_t pos = 0) const;
  size_t find(char c, size_t pos = 0) const;
  size_t rfind(StringView s, size_t pos = npos) const;
  size_t rfind(char c, size_t pos = npos) const;
  size_t find_first_of(StringView s, size_t pos = 0) const;
  size_t find_last_of(StringView s, size_t pos = npos) const;
  size_t find_first_not_of(StringView s, size_t pos = 0) const;
  size_t find_last_not_of(StringView s, size_t pos = npos) const;

 private:
  constexpr static size_t min(size_t v1, size_t v2) {
    return v1 < v2 ? v1 : v2;
  }

  const char* data_;
  size_t size_;
};

// Comparison.

bool operator==(StringView lhs, StringView rhs);
bool operator!=(StringView lhs, StringView rhs);
bool operator<(StringView lhs, StringView rhs);
bool operator>(StringView lhs, StringView rhs);
bool operator<=(StringView lhs, StringView rhs);
bool operator>=(StringView lhs, StringView rhs);

// IO.
std::ostream& operator<<(std::ostream& o, StringView string_view);

}  // namespace ftl

#endif  // LIB_FTL_STRINGS_STRING_VIEW_H_
