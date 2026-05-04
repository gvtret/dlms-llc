#pragma once

#include <cstddef>
#include <cstdint>

namespace dlms {
namespace llc {

static const std::uint8_t kLlcDsapDlms = 0xE6;
static const std::uint8_t kLlcSsapClient = 0xE6;
static const std::uint8_t kLlcSsapServer = 0xE7;
static const std::uint8_t kLlcDsapBroadcast = 0xFF;
static const std::uint8_t kLlcControlDefault = 0x00;
static const std::size_t kLlcHeaderSize = 3;

} // namespace llc
} // namespace dlms
