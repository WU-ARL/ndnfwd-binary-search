/*
 *  Copyright (c) 2014-2015 Washington University in St. Louis.
 *  Comments: added the siphash_step() function
 *
 */

/*
   SipHash reference C implementation

   Copyright (c) 2012-2014 Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
   Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>

   To the extent possible under law, the author(s) have dedicated all copyright
   and related and neighboring rights to this software to the public domain
   worldwide. This software is distributed without any warranty.

   You should have received a copy of the CC0 Public Domain Dedication along with
   this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#ifndef SIPHASH24_H
#define SIPHASH24_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* default: SipHash-2-4 */
#define cROUNDS 2
#define dROUNDS 4
#define MAX_COMP_COUNT 16;

#define ROTL(x,b) (uint64_t)( ((x) << (b)) | ( (x) >> (64 - (b))) )

#define U32TO8_LE(p, v)                                         \
  (p)[0] = (uint8_t)((v)      ); (p)[1] = (uint8_t)((v) >>  8); \
  (p)[2] = (uint8_t)((v) >> 16); (p)[3] = (uint8_t)((v) >> 24);

#define U64TO8_LE(p, v)                        \
  U32TO8_LE((p),     (uint32_t)((v)      ));   \
  U32TO8_LE((p) + 4, (uint32_t)((v) >> 32));

#define U8TO64_LE(p)            \
  (((uint64_t)((p)[0])      ) | \
   ((uint64_t)((p)[1]) <<  8) | \
   ((uint64_t)((p)[2]) << 16) | \
   ((uint64_t)((p)[3]) << 24) | \
   ((uint64_t)((p)[4]) << 32) | \
   ((uint64_t)((p)[5]) << 40) | \
   ((uint64_t)((p)[6]) << 48) | \
   ((uint64_t)((p)[7]) << 56))

#define SIPROUND                                        \
  do {                                                  \
    v0 += v1; v1=ROTL(v1,13); v1 ^= v0; v0=ROTL(v0,32); \
    v2 += v3; v3=ROTL(v3,16); v3 ^= v2;                 \
    v0 += v3; v3=ROTL(v3,21); v3 ^= v0;                 \
    v2 += v1; v1=ROTL(v1,17); v1 ^= v2; v2=ROTL(v2,32); \
  } while(0)

#ifdef DEBUG
#define TRACE                                                       \
    do {                                                            \
    printf( "(%3d) v0 %08x %08x\n",                                 \
        ( int )inlen, ( uint32_t )( v0 >> 32 ), ( uint32_t )v0 );   \
    printf( "(%3d) v1 %08x %08x\n",                                 \
        ( int )inlen, ( uint32_t )( v1 >> 32 ), ( uint32_t )v1 );   \
    printf( "(%3d) v2 %08x %08x\n",                                 \
        ( int )inlen, ( uint32_t )( v2 >> 32 ), ( uint32_t )v2 );   \
    printf( "(%3d) v3 %08x %08x\n",                                 \
        ( int )inlen, ( uint32_t )( v3 >> 32 ), ( uint32_t )v3 );   \
    } while(0)
#else
#define TRACE
#endif

inline uint64_t
siphash(const uint8_t *in, uint64_t inlen, const uint8_t *k );

// Added in the ndnfwd-binary-search work
// Incrementally compute hash values for name prefixes
inline void
siphash_step(uint64_t * v0_in_p, uint64_t * v1_in_p, uint64_t * v2_in_p, uint64_t * v3_in_p,
                  uint8_t ** in_p, const uint8_t * end, uint64_t inlen, uint64_t * hash_value);

#endif
