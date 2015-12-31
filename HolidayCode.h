#ifndef _HOLIDAYCODE_H
#define _HOLIDAYCODE_H

//
// This module defines a very simple portable machine code (p-code)
// to control our window panels.  The goal is to make something relatively
// efficient and simple to implement.
//
// In particular, this p-code engine is implemented as a simple token threaded
// code interpreter.
//
// The format of the p-code is represented as dfmt U16
//

#include "DataFormat.h"

namespace hcode {

  // 'opcode' function pointer type for our p-code implementation
  typedef void(*Operation)(dfmt::DataParser &in, dfmt::DataSerializer &out);
}

#endif