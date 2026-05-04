#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum dlms_llc_status_t
{
  DLMS_LLC_STATUS_OK,
  DLMS_LLC_STATUS_NEED_MORE_DATA,
  DLMS_LLC_STATUS_OUTPUT_BUFFER_TOO_SMALL,
  DLMS_LLC_STATUS_INVALID_ARGUMENT,
  DLMS_LLC_STATUS_INVALID_HEADER,
  DLMS_LLC_STATUS_INVALID_DSAP,
  DLMS_LLC_STATUS_INVALID_SSAP,
  DLMS_LLC_STATUS_INVALID_CONTROL,
  DLMS_LLC_STATUS_INVALID_LPDU_LENGTH,
  DLMS_LLC_STATUS_LSDU_TOO_LARGE,
  DLMS_LLC_STATUS_BROADCAST_ENCODE_FORBIDDEN,
  DLMS_LLC_STATUS_UNSUPPORTED_ADDRESS,
  DLMS_LLC_STATUS_UNSUPPORTED_CONTROL,
  DLMS_LLC_STATUS_UNSUPPORTED_FEATURE,
  DLMS_LLC_STATUS_INTERNAL_ERROR
} dlms_llc_status_t;

typedef struct dlms_llc_header_t
{
  uint8_t dsap;
  uint8_t ssap;
  uint8_t control;
} dlms_llc_header_t;

typedef struct dlms_llc_lpdu_view_t
{
  dlms_llc_header_t header;
  const uint8_t* lsdu;
  size_t lsdu_size;
} dlms_llc_lpdu_view_t;

dlms_llc_status_t dlms_llc_encode_lpdu(
  const dlms_llc_header_t* header,
  const uint8_t* lsdu,
  size_t lsdu_size,
  uint8_t* output,
  size_t output_size,
  size_t* written_size);

dlms_llc_status_t dlms_llc_encode_request(
  const uint8_t* apdu,
  size_t apdu_size,
  uint8_t* output,
  size_t output_size,
  size_t* written_size);

dlms_llc_status_t dlms_llc_encode_response(
  const uint8_t* apdu,
  size_t apdu_size,
  uint8_t* output,
  size_t output_size,
  size_t* written_size);

dlms_llc_status_t dlms_llc_decode_lpdu(
  const uint8_t* input,
  size_t input_size,
  int allow_broadcast_destination,
  dlms_llc_lpdu_view_t* lpdu);

dlms_llc_status_t dlms_llc_validate_header(
  const dlms_llc_header_t* header,
  int allow_broadcast_destination);

#ifdef __cplusplus
} // extern "C"
#endif
