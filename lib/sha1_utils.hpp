
#pragma once

#include <array>
#include <cstdint>
#include <cstring>

namespace pwned {

using text_sha1_t = std::array<char, 40>;
using binary_sha1_t = std::array<uint8_t, 20u>;
using sha1_t = binary_sha1_t;

using sha1_tail_t = std::array<uint8_t, 19u>;

inline bool operator<(const sha1_t& lhs, const sha1_t& rhs)
{
  return std::memcmp(lhs.data(), rhs.data(), sizeof(sha1_t));
}

inline constexpr uint8_t char_to_index(char c)
{
  switch (c) {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      c -= 'A' - ':';
      [[fallthrough]];
    default:
      c -= '0';
  }

  return static_cast<uint8_t>(c);
}

inline constexpr uint8_t chars_to_byte(const char* cs)
{
  uint8_t byte = static_cast<uint8_t>(char_to_index(cs[0])) << 4;
  byte |= char_to_index(cs[1]);
  return byte;
}

inline sha1_t to_sha1(std::string_view sha1_text)
{
  pwned::sha1_t sha1;

  for (auto i = 0; i < 40; i += 2) {
    sha1[i / 2] = pwned::chars_to_byte(sha1_text.data() + i);
  }

  //  std::memcpy(&sha1[0], sha1_txt.data(), 40);

  return sha1;
}

inline std::string binary_sha1_to_string(const binary_sha1_t& sha1)
{
  std::string result;

  const auto to_char = [](uint8_t value) -> char {
    return value < 10 ? value + '0' : (value - 10 + 'A');
  };

  for (uint8_t byte : sha1) {
    const uint8_t first_char = (byte & 0xf0) >> 4;
    const uint8_t second_char = byte & 0x0f;

    result += to_char(first_char);
    result += to_char(second_char);
  }

  return result;
}
}