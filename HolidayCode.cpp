#include "HolidayCode.h"

#include <cstdint>

// handy c-style error handling routines because we don't get exceptions
#define DE_BEGIN      DataError err = DataError::OK;
#define DE_END        done: return err;
#define DE_CHECK(x)   {err = (x); if (err != DataError::OK) { goto done; }}

namespace {
  // error code for data hanlding routines
  enum class DataError : uint8_t {
    OK = 0,
    END_OF_BUFFER,
    BAD_TYPE,
  };

  // Type codes for a trivially simple data format that is easy to decode.
  // Really, there are good data formats out there (e.g. CBOR, MsgPack), but this is dead simple.
  // E.g.
  //
  //  [ 1, "hi" ]
  //
  // Could be encoded as:
  //
  //  0x02 | 0x00 : 0x00 0x01 | 0x01 : 0x00 0x02 : 0x68 0x69 | 0x03
  //
  enum class DataType : int16_t {
    NONE = -0x01,

    // unsigned 16-bit integer follows, big-endian
    U16_INT,
    // unsigned 16-bit integer length follows, then string of that length
    U16_STR,
    // delimiter for signaling start of list
    LIST_START,
    // delimiter for signaling end of list
    LIST_END,

    UNKNOWN
  };

  // convert integer to data type
  inline DataType data_type(int16_t const value) {
    if (value < 0) {
      return DataType::NONE;
    }

    DataType type(static_cast<DataType>(value));
    if (value >= static_cast<int16_t>(DataType::UNKNOWN)) {
      type = DataType::UNKNOWN;
    }
    return type;
  }

  class DataParser {
  public:
    DataParser(uint8_t *buffer, uint16_t length) : _type(DataType::NONE), _buffer(buffer), _rem(length) {}

    // reads the next element into `type`
    DataError next(DataType &type);
    // reads the current value as an integer (widening as appropriate)
    DataError readInt(int32_t &value);
    // reads the current value as a string, returning pointers to internal buffer.
    DataError readStr(uint8_t *&start, uint16_t &length);

  private:
    DataError readByte(uint8_t &octet) {
      if (_rem < 1) {
        return DataError::END_OF_BUFFER;
      }

      octet = *_buffer;
      ++_buffer;
      --_rem;

      return DataError::OK;
    }

    DataError checkRemaining(uint16_t needed) {
      if (needed > _rem) {
        return DataError::END_OF_BUFFER;
      }
      return DataError::OK;
    }

    void advance(uint16_t length) {
      _buffer += length;
      _rem -= length;
    }

    DataError consumeU16(uint16_t &value) {
      DE_BEGIN;
      DE_CHECK(checkRemaining(2));
      value = _buffer[0] << 8 | _buffer[1];
      advance(2);
      DE_END;
    }

    DataType  _type;
    uint8_t * _buffer;
    uint16_t  _rem;
  };

  DataError DataParser::next(DataType &type) {
    DE_BEGIN;

    uint8_t octet;
    DE_CHECK(readByte(octet));

    DE_END;
  }

  DataError DataParser::readInt(int32_t &value) {
    DE_BEGIN;

    uint16_t u16_value(0);
    switch (_type) {
    case DataType::U16_INT:
      DE_CHECK(consumeU16(u16_value));
      value = u16_value;
      break;
    default:
      return DataError::BAD_TYPE;
      break;
    }

    DE_END;
  }

  DataError DataParser::readStr(uint8_t *&start, uint16_t &length) {
    DE_BEGIN;
    switch (_type) {
    case DataType::U16_STR:
      DE_CHECK(consumeU16(length));
      start = _buffer;
      DE_CHECK(checkRemaining(length));
      advance(length);
      break;
    default:
      return DataError::BAD_TYPE;
      break;
    }
    DE_END;
  }


  //void op_info(uint8_t *in, uint16_t in_len, uint8_t *out, uint16_t out_len) {

  //}

  //typedef void(*opcode)(uint8_t *in, uint16_t in_len, uint8_t *out, uint16_t out_len);
}