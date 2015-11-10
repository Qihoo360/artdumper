// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if !defined(DISALLOW_COPY_AND_ASSIGN)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;  \
  void operator=(const TypeName&) = delete
#endif

typedef unsigned char uint8_t;
typedef unsigned __int32 uint32_t;
typedef __int32 int32_t;

typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;

// TODO: reference additional headers your program requires here
