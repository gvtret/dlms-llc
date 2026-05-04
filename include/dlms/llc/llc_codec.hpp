#pragma once

#include "dlms/llc/llc_error.hpp"
#include "dlms/llc/llc_header.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace dlms {
namespace llc {

/**
 * @brief Non-owning view of a decoded LLC LPDU.
 *
 * `lsduData` points into the input buffer passed to `DecodeLpduFromBuffer`.
 * The caller must keep that input buffer alive while using the view.
 */
struct LlcLpdu
{
  LlcHeader header;
  const std::uint8_t* lsduData;
  std::size_t lsduSize;
};

/**
 * @brief Owning decoded LLC LPDU container.
 */
struct LlcLpduBuffer
{
  LlcHeader header;
  std::vector<std::uint8_t> lsdu;
};

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
 * @brief Decode an LLC LPDU from a caller-provided input buffer.
 *
 * The returned `lpdu` is a view into `input`. Broadcast destination LSAP is
 * accepted only when `allowBroadcastDestination` is true.
 */
LlcStatus DecodeLpduFromBuffer(
  const std::uint8_t* input,
  std::size_t inputSize,
  bool allowBroadcastDestination,
  LlcLpdu& lpdu);

/**
 * @brief Decode an LLC LPDU into an owned container.
 */
LlcStatus DecodeLpdu(
  const std::uint8_t* input,
  std::size_t inputSize,
  bool allowBroadcastDestination,
  LlcLpduBuffer& lpdu);

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
