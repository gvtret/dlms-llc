#pragma once

#include "dlms/llc/llc_error.hpp"
#include "dlms/llc/llc_header.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace dlms {
namespace llc {

/**
 * @brief Encode an LLC LPDU into a caller-provided output buffer.
 *
 * `lsdu` may be null only when `lsduSize` is zero. `output` and `writtenSize`
 * must be valid. Broadcast destination LSAP is rejected for encoding.
 */
LlcStatus EncodeLpduToBuffer(
  const LlcHeader& header,
  const std::uint8_t* lsdu,
  std::size_t lsduSize,
  std::uint8_t* output,
  std::size_t outputSize,
  std::size_t& writtenSize);

/**
 * @brief Encode an LLC LPDU into an owned byte vector.
 */
LlcStatus EncodeLpdu(
  const LlcHeader& header,
  const std::uint8_t* lsdu,
  std::size_t lsduSize,
  std::vector<std::uint8_t>& output);

/**
 * @brief Encode a client-to-server DLMS/COSEM APDU as an LLC LPDU.
 */
LlcStatus EncodeDlmsRequest(
  const std::uint8_t* apdu,
  std::size_t apduSize,
  std::vector<std::uint8_t>& output);

/**
 * @brief Encode a server-to-client DLMS/COSEM APDU as an LLC LPDU.
 */
LlcStatus EncodeDlmsResponse(
  const std::uint8_t* apdu,
  std::size_t apduSize,
  std::vector<std::uint8_t>& output);

} // namespace llc
} // namespace dlms
