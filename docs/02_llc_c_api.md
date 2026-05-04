# 02. LLC C API

## 1. Goals

The C ABI is a stable wrapper over the C++ implementation.

Rules:

- `extern "C"`.
- No C++ types in public C headers.
- Fixed-width integer types.
- Caller-provided buffers.
- Status codes only.
- Stable enum values.
- No exceptions crossing the C ABI.

## 2. Header

The C ABI header is:

```text
include/dlms/llc/llc_c_api.h
```

The implementation file is:

```text
src/llc/llc_c_api.cpp
```

## 3. Status Enum

```c
typedef enum dlms_llc_status_t
{
  DLMS_LLC_STATUS_OK = 0,
  DLMS_LLC_STATUS_NEED_MORE_DATA = 1,
  DLMS_LLC_STATUS_OUTPUT_BUFFER_TOO_SMALL = 2,
  DLMS_LLC_STATUS_INVALID_ARGUMENT = 3,
  DLMS_LLC_STATUS_INVALID_HEADER = 4,
  DLMS_LLC_STATUS_INVALID_DSAP = 5,
  DLMS_LLC_STATUS_INVALID_SSAP = 6,
  DLMS_LLC_STATUS_INVALID_CONTROL = 7,
  DLMS_LLC_STATUS_INVALID_LPDU_LENGTH = 8,
  DLMS_LLC_STATUS_LSDU_TOO_LARGE = 9,
  DLMS_LLC_STATUS_BROADCAST_ENCODE_FORBIDDEN = 10,
  DLMS_LLC_STATUS_UNSUPPORTED_ADDRESS = 11,
  DLMS_LLC_STATUS_UNSUPPORTED_CONTROL = 12,
  DLMS_LLC_STATUS_UNSUPPORTED_FEATURE = 13,
  DLMS_LLC_STATUS_INTERNAL_ERROR = 14
} dlms_llc_status_t;
```

Enum values must remain stable after public release.

## 4. C Structs

```c
typedef struct dlms_llc_header_t
{
  uint8_t dsap;
  uint8_t ssap;
  uint8_t control;
} dlms_llc_header_t;
```

```c
typedef struct dlms_llc_lpdu_view_t
{
  dlms_llc_header_t header;
  const uint8_t* lsdu;
  size_t lsdu_size;
} dlms_llc_lpdu_view_t;
```

The `lsdu` pointer in `dlms_llc_lpdu_view_t` points into the caller-provided
input buffer.

## 5. Encode API

```c
dlms_llc_status_t dlms_llc_encode_lpdu(
  const dlms_llc_header_t* header,
  const uint8_t* lsdu,
  size_t lsdu_size,
  uint8_t* output,
  size_t output_size,
  size_t* written_size);
```

```c
dlms_llc_status_t dlms_llc_encode_request(
  const uint8_t* apdu,
  size_t apdu_size,
  uint8_t* output,
  size_t output_size,
  size_t* written_size);
```

```c
dlms_llc_status_t dlms_llc_encode_response(
  const uint8_t* apdu,
  size_t apdu_size,
  uint8_t* output,
  size_t output_size,
  size_t* written_size);
```

## 6. Decode API

```c
dlms_llc_status_t dlms_llc_decode_lpdu(
  const uint8_t* input,
  size_t input_size,
  int allow_broadcast_destination,
  dlms_llc_lpdu_view_t* lpdu);
```

## 7. Validation API

```c
dlms_llc_status_t dlms_llc_validate_header(
  const dlms_llc_header_t* header,
  int allow_broadcast_destination);
```

## 8. Null Argument Policy

Functions must return `DLMS_LLC_STATUS_INVALID_ARGUMENT` for invalid pointers.

For zero-length LSDU:

- `lsdu == NULL` is allowed when `lsdu_size == 0`.
- `lsdu == NULL` is invalid when `lsdu_size > 0`.

## 9. Header Compile Test

The C header must compile as C and C++.

The test suite must include a `.c` translation unit that includes
`dlms/llc/llc_c_api.h`.
