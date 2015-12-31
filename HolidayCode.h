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
// The format of the p-code is represented as 'dfmt' U16_INT followed by any parameters
// required in a list.
//
// For example:
//   
//   MARQUEE [ 60 /*ms delay*/ "..." /*image 1 in RGB octets, column major*/ "..." "..." ]
//
// Also part of this module is a simple command interface for storing winow matrix 'programs'
// and configuring the micro-controller that can be tunneled through serial or as a simeple
// RPC protocol over the WiFi interface.  The format is a 'dfmt' message in the p-code form above,
// but has a synchronous response element that is a 'dfmt' message in the p-code form.
//

#include "DataFormat.h"

namespace hcode {
  
  
}

#endif