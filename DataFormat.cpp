#include "DataFormat.h"

#include <algorithm>

namespace dfmt {

  //
  // Top-level Functions
  //

  // convert integer to data type
  DataType data_type(int16_t const value) {
    if (value < 0) {
      return DataType::NONE;
    }

    DataType type(static_cast<DataType>(value));
    if (value >= static_cast<int16_t>(DataType::UNKNOWN)) {
      type = DataType::UNKNOWN;
    }
    return type;
  }

  //
  // DataBuffer Methods
  //
  
  inline void DataBuffer::advance(uint16_t const length) {
    _buffer += length;
    _rem -= length;
  }

  inline DataBuffer::DataBuffer(uint8_t * buffer, uint16_t length) : _buffer(buffer), _rem(length) {}

  inline DataError DataBuffer::checkRemaining(uint16_t const needed) {
    if (needed > _rem) {
      return DataError::END_OF_BUFFER;
    }
    return DataError::OK;
  }

  //
  // DataParser Methods
  //

  DataParser::DataParser(uint8_t * buffer, uint16_t length) : DataBuffer(buffer, length), _type(DataType::NONE) {}

  DataError DataParser::next(DataType & type) {
    DE_BEGIN;

    uint8_t octet;
    DE_CHECK(readByte(octet));

    DE_END;
  }

  DataError DataParser::readInt(int32_t & value) {
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

  DataError DataParser::readStr(uint8_t *& start, uint16_t & length) {
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

  inline DataError DataParser::readByte(uint8_t & octet) {
    if (_rem < 1) {
      return DataError::END_OF_BUFFER;
    }

    octet = *_buffer;
    ++_buffer;
    --_rem;

    return DataError::OK;
  }

  inline DataError DataParser::consumeU16(uint16_t & value) {
    DE_BEGIN;
    DE_CHECK(checkRemaining(2));
    value = _buffer[0] << 8 | _buffer[1];
    advance(2);
    DE_END;
  }

  //
  // DataSerializer Methods
  //

  DataSerializer::DataSerializer(uint8_t * buffer, uint16_t length) : DataBuffer(buffer, length), _start(buffer) {}

  DataError DataSerializer::writeU16Int(uint16_t const value)
  {
    DE_BEGIN;
    DE_CHECK(writeU16(value));
    DE_END;
  }

  DataError DataSerializer::writeU16Str(uint8_t const * start, uint16_t length)
  {
    DE_BEGIN;
    // make sure we have enough room to write everything before starting
    DE_CHECK(checkRemaining(2 + length));
    DE_CHECK(writeU16(length));
    std::copy(start, start + length, _buffer);
    DE_END;
  }

  DataError DataSerializer::writeListStart()
  {
    DE_BEGIN;
    DE_CHECK(writeByte(static_cast<uint8_t>(DataType::LIST_START)));
    DE_END;
  }

  DataError DataSerializer::writeListEnd()
  {
    DE_BEGIN;
    DE_CHECK(writeByte(static_cast<uint8_t>(DataType::LIST_START)));
    DE_END;
  }

  DataError DataSerializer::buffer(uint8_t *& start, uint16_t & length)
  {
    start = _start;
    length = _buffer - _start;

    return DataError::OK;
  }

  DataError DataSerializer::writeU16(uint16_t const value)
  {
    DE_BEGIN;
    DE_CHECK(checkRemaining(2));
    _buffer[0] = value >> 8;
    _buffer[1] = value & 0xFF;
    advance(2);
    DE_END;
  }

  DataError DataSerializer::writeByte(uint8_t const value)
  {
    DE_BEGIN;
    DE_CHECK(checkRemaining(1));
    *_buffer = value;
    advance(1);
    DE_END;
  }
}