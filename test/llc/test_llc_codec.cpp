#include "dlms/llc/llc_codec.hpp"

#include "dlms/llc/llc_types.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

namespace {

using dlms::llc::EncodeDlmsRequest;
using dlms::llc::EncodeDlmsResponse;
using dlms::llc::EncodeLpdu;
using dlms::llc::EncodeLpduToBuffer;
using dlms::llc::kLlcControlDefault;
using dlms::llc::kLlcDsapBroadcast;
using dlms::llc::kLlcDsapDlms;
using dlms::llc::kLlcHeaderSize;
using dlms::llc::kLlcSsapClient;
using dlms::llc::kLlcSsapServer;
using dlms::llc::LlcHeader;
using dlms::llc::LlcStatus;

TEST(LlcCodecTest, EncodesDlmsRequest)
{
  const std::uint8_t apdu[] = {0xC0, 0x01, 0x81, 0x00};

  std::vector<std::uint8_t> output;
  EXPECT_EQ(LlcStatus::Ok, EncodeDlmsRequest(apdu, sizeof(apdu), output));

  const std::uint8_t expected[] = {
    kLlcDsapDlms,
    kLlcSsapClient,
    kLlcControlDefault,
    0xC0,
    0x01,
    0x81,
    0x00
  };
  EXPECT_EQ(std::vector<std::uint8_t>(expected, expected + sizeof(expected)),
            output);
}

TEST(LlcCodecTest, EncodesDlmsResponse)
{
  const std::uint8_t apdu[] = {0xC4, 0x01, 0x09};

  std::vector<std::uint8_t> output;
  EXPECT_EQ(LlcStatus::Ok, EncodeDlmsResponse(apdu, sizeof(apdu), output));

  ASSERT_EQ(kLlcHeaderSize + sizeof(apdu), output.size());
  EXPECT_EQ(kLlcDsapDlms, output[0]);
  EXPECT_EQ(kLlcSsapServer, output[1]);
  EXPECT_EQ(kLlcControlDefault, output[2]);
  EXPECT_EQ(std::vector<std::uint8_t>(apdu, apdu + sizeof(apdu)),
            std::vector<std::uint8_t>(output.begin() + kLlcHeaderSize,
                                      output.end()));
}

TEST(LlcCodecTest, EncodesIntoCallerProvidedBuffer)
{
  const LlcHeader header = {
    kLlcDsapDlms,
    kLlcSsapClient,
    kLlcControlDefault
  };
  const std::uint8_t apdu[] = {0x01, 0x02};
  std::uint8_t output[5] = {};
  std::size_t writtenSize = 99;

  EXPECT_EQ(LlcStatus::Ok,
            EncodeLpduToBuffer(
              header, apdu, sizeof(apdu), output, sizeof(output), writtenSize));

  EXPECT_EQ(sizeof(output), writtenSize);
  EXPECT_EQ(kLlcDsapDlms, output[0]);
  EXPECT_EQ(kLlcSsapClient, output[1]);
  EXPECT_EQ(kLlcControlDefault, output[2]);
  EXPECT_EQ(0x01, output[3]);
  EXPECT_EQ(0x02, output[4]);
}

TEST(LlcCodecTest, ReportsSmallOutputBuffer)
{
  const LlcHeader header = {
    kLlcDsapDlms,
    kLlcSsapClient,
    kLlcControlDefault
  };
  const std::uint8_t apdu[] = {0x01, 0x02};
  std::uint8_t output[4] = {};
  std::size_t writtenSize = 99;

  EXPECT_EQ(LlcStatus::OutputBufferTooSmall,
            EncodeLpduToBuffer(
              header, apdu, sizeof(apdu), output, sizeof(output), writtenSize));
  EXPECT_EQ(0u, writtenSize);
}

TEST(LlcCodecTest, RejectsNullLsduWithNonZeroSize)
{
  const LlcHeader header = {
    kLlcDsapDlms,
    kLlcSsapClient,
    kLlcControlDefault
  };
  std::uint8_t output[kLlcHeaderSize] = {};
  std::size_t writtenSize = 99;

  EXPECT_EQ(LlcStatus::InvalidArgument,
            EncodeLpduToBuffer(header, 0, 1, output, sizeof(output), writtenSize));
  EXPECT_EQ(0u, writtenSize);
}

TEST(LlcCodecTest, AllowsNullLsduForEmptyPayload)
{
  const LlcHeader header = {
    kLlcDsapDlms,
    kLlcSsapClient,
    kLlcControlDefault
  };
  std::vector<std::uint8_t> output;

  EXPECT_EQ(LlcStatus::Ok, EncodeLpdu(header, 0, 0, output));

  const std::uint8_t expected[] = {
    kLlcDsapDlms,
    kLlcSsapClient,
    kLlcControlDefault
  };
  EXPECT_EQ(std::vector<std::uint8_t>(expected, expected + sizeof(expected)),
            output);
}

TEST(LlcCodecTest, RejectsBroadcastDestinationForEncode)
{
  const LlcHeader header = {
    kLlcDsapBroadcast,
    kLlcSsapClient,
    kLlcControlDefault
  };
  const std::uint8_t apdu[] = {0x01};
  std::vector<std::uint8_t> output(1, 0xAA);

  EXPECT_EQ(LlcStatus::BroadcastEncodeForbidden,
            EncodeLpdu(header, apdu, sizeof(apdu), output));
  EXPECT_TRUE(output.empty());
}

TEST(LlcCodecTest, DoesNotModifyInputApdu)
{
  std::uint8_t apdu[] = {0x7E, 0xA0, 0x03, 0x00};
  const std::vector<std::uint8_t> before(apdu, apdu + sizeof(apdu));

  std::vector<std::uint8_t> output;
  EXPECT_EQ(LlcStatus::Ok, EncodeDlmsRequest(apdu, sizeof(apdu), output));

  EXPECT_EQ(before, std::vector<std::uint8_t>(apdu, apdu + sizeof(apdu)));
}

} // namespace
