#ifndef _DATAFORMAT_H
#define _DATAFORMAT_H

#include <cstdint>

// handy c-style error handling routines because we don't get exceptions
#define DE_BEGIN      dataformat::DataError err = dataformat::DataError::OK;
#define DE_END        done: return err;
#define DE_CHECK(x)   {err = (x); if (err != dataformat::DataError::OK) { goto done; }}

namespace dataformat {
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
    // delimitser for signaling end of list
    LIST_END,

    UNKNOWN
  };

  // convert integer to data type
  DataType data_type(int16_t const value);

  // simple buffer overlay
  class DataBuffer {
  protected:
    DataBuffer(uint8_t * buffer, uint16_t length);

    DataError checkRemaining(uint16_t const needed);
    void      advance(uint16_t const length);

    uint8_t * _buffer;
    uint16_t  _rem;
  };

  // simple data parser
  class DataParser : DataBuffer {
  public:
    DataParser(uint8_t * buffer, uint16_t length);

    // reads the next element into `type`
    DataError next(DataType & type);
    // reads the current value as an integer (widening as appropriate)
    DataError readInt(int32_t & value);
    // reads the current value as a string, returning pointers to internal buffer.
    DataError readStr(uint8_t *& start, uint16_t & length);

  private:
    DataError readByte(uint8_t & octet);
    DataError consumeU16(uint16_t & value);
    
    DataType  _type;
  };

  // simple data serializer
  class DataSerializer : DataBuffer {
  public:
    DataSerializer(uint8_t * buffer, uint16_t length);

    // writes a U16 integer
    DataError writeU16Int(uint16_t const value);
    // writes a U16 length string
    DataError writeU16Str(uint8_t const * start, uint16_t length);
    // writes the list start
    DataError writeListStart();
    // writes the list end
    DataError writeListEnd();
    // retrieves the encoded part of the buffer
    DataError buffer(uint8_t *& start, uint16_t & length);

  private:
    DataError writeU16(uint16_t const value);
    DataError writeByte(uint8_t const value);

    uint8_t * const _start;
  };
}

#endif
