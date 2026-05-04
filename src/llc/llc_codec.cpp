#include "dlms/llc/llc_codec.hpp"

#include "dlms/llc/llc_types.hpp"

#include <algorithm>
#include <limits>
#include <new>

namespace dlms {
namespace llc {

namespace {

bool HasRepresentableLpduSize(std::size_t lsduSize)
{
  return lsduSize <=
         (std::numeric_limits<std::size_t>::max() - kLlcHeaderSize);
}

} // namespace

LlcStatus EncodeLpduToBuffer(
  const LlcHeader& header,
  const std::uint8_t* lsdu,
  std::size_t lsduSize,
  std::uint8_t* output,
  std::size_t outputSize,
  std::size_t& writtenSize)
{
  writtenSize = 0;

  if (lsdu == 0 && lsduSize != 0) {
    return LlcStatus::InvalidArgument;
  }
  if (output == 0) {
    return LlcStatus::InvalidArgument;
  }

  const LlcStatus headerStatus = ValidateLlcHeader(header, false);
  if (headerStatus != LlcStatus::Ok) {
    return headerStatus;
  }

  if (!HasRepresentableLpduSize(lsduSize)) {
    return LlcStatus::LsduTooLarge;
  }

  const std::size_t requiredSize = kLlcHeaderSize + lsduSize;
  if (outputSize < requiredSize) {
    return LlcStatus::OutputBufferTooSmall;
  }

  output[0] = header.dsap;
  output[1] = header.ssap;
  output[2] = header.control;
  if (lsduSize != 0) {
    std::copy(lsdu, lsdu + lsduSize, output + kLlcHeaderSize);
  }

  writtenSize = requiredSize;
  return LlcStatus::Ok;
}

LlcStatus EncodeLpdu(
  const LlcHeader& header,
  const std::uint8_t* lsdu,
  std::size_t lsduSize,
  std::vector<std::uint8_t>& output)
{
  output.clear();

  if (!HasRepresentableLpduSize(lsduSize)) {
    return LlcStatus::LsduTooLarge;
  }

  try {
    output.resize(kLlcHeaderSize + lsduSize);
  } catch (const std::bad_alloc&) {
    output.clear();
    return LlcStatus::InternalError;
  }

  std::size_t writtenSize = 0;
  const LlcStatus status = EncodeLpduToBuffer(
    header, lsdu, lsduSize, output.data(), output.size(), writtenSize);
  if (status != LlcStatus::Ok) {
    output.clear();
    return status;
  }

  output.resize(writtenSize);
  return LlcStatus::Ok;
}

LlcStatus EncodeDlmsRequest(
  const std::uint8_t* apdu,
  std::size_t apduSize,
  std::vector<std::uint8_t>& output)
{
  return EncodeLpdu(
    MakeLlcHeader(LlcDirection::ClientToServer), apdu, apduSize, output);
}

LlcStatus EncodeDlmsResponse(
  const std::uint8_t* apdu,
  std::size_t apduSize,
  std::vector<std::uint8_t>& output)
{
  return EncodeLpdu(
    MakeLlcHeader(LlcDirection::ServerToClient), apdu, apduSize, output);
}

} // namespace llc
} // namespace dlms
