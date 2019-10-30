#ifndef BLARGG_ENDIAN
#define BLARGG_ENDIAN
// CPU Byte Order Utilities

// snes_spc 0.9.0

#include "blargg_common.h"

// BLARGG_CPU_CISC: Defined if CPU has very few general-purpose registers (< 16)
#if defined (_M_IX86) || defined (_M_IA64) || defined (__i486__) || \
		defined (__x86_64__) || defined (__ia64__) || defined (__i386__)
	#define BLARGG_CPU_X86 1
	#define BLARGG_CPU_CISC 1
#endif

#if defined (__powerpc__) || defined (__ppc__) || defined (__POWERPC__) || defined (__powerc)
	#define BLARGG_CPU_POWERPC 1
	#define BLARGG_CPU_RISC 1
#endif

// BLARGG_BIG_ENDIAN, BLARGG_LITTLE_ENDIAN: Determined automatically, otherwise only
// one may be #defined to 1. Only needed if something actually depends on byte order.
#if !defined (BLARGG_BIG_ENDIAN) && !defined (BLARGG_LITTLE_ENDIAN)
#ifdef __GLIBC__
	// GCC handles this for us
	#include <endian.h>
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define BLARGG_LITTLE_ENDIAN 1
	#elif __BYTE_ORDER == __BIG_ENDIAN
		#define BLARGG_BIG_ENDIAN 1
	#endif
#else

#if defined (LSB_FIRST) || defined (__LITTLE_ENDIAN__) || BLARGG_CPU_X86 || \
		(defined (LITTLE_ENDIAN) && LITTLE_ENDIAN+0 != 1234)
	#define BLARGG_LITTLE_ENDIAN 1
#endif

#if defined (MSB_FIRST)     || defined (__BIG_ENDIAN__) || defined (WORDS_BIGENDIAN) || \
	defined (__sparc__)     ||  BLARGG_CPU_POWERPC || \
	(defined (BIG_ENDIAN) && BIG_ENDIAN+0 != 4321)
	#define BLARGG_BIG_ENDIAN 1
#elif !defined (__mips__)
	// No endian specified; assume little-endian, since it's most common
	#define BLARGG_LITTLE_ENDIAN 1
#endif
#endif
#endif

#if BLARGG_LITTLE_ENDIAN && BLARGG_BIG_ENDIAN
	#undef BLARGG_LITTLE_ENDIAN
	#undef BLARGG_BIG_ENDIAN
#endif

#ifndef NDEBUG
inline void blargg_verify_byte_order()
{
		#if BLARGG_BIG_ENDIAN
			volatile int i = 1;
			assert( *(volatile char*) &i == 0 );
		#elif BLARGG_LITTLE_ENDIAN
			volatile int i = 1;
			assert( *(volatile char*) &i != 0 );
		#endif
}
#endif

inline unsigned get_le16( void const* p )
{
	return  (unsigned) ((unsigned char const*) p) [1] << 8 |
			(unsigned) ((unsigned char const*) p) [0];
}

inline void set_le16( void* p, unsigned n )
{
	((unsigned char*) p) [1] = (unsigned char) (n >> 8);
	((unsigned char*) p) [0] = (unsigned char) n;
}

#if BLARGG_NONPORTABLE
	// Optimized implementation if byte order is known
	#if BLARGG_LITTLE_ENDIAN
		#define GET_LE16( addr )        (*(BOOST::uint16_t*) (addr))
		#define GET_LE32( addr )        (*(BOOST::uint32_t*) (addr))
		#define SET_LE16( addr, data )  (void) (*(BOOST::uint16_t*) (addr) = (data))
		#define SET_LE32( addr, data )  (void) (*(BOOST::uint32_t*) (addr) = (data))
	#elif BLARGG_BIG_ENDIAN
		#define GET_BE16( addr )        (*(BOOST::uint16_t*) (addr))
		#define GET_BE32( addr )        (*(BOOST::uint32_t*) (addr))
		#define SET_BE16( addr, data )  (void) (*(BOOST::uint16_t*) (addr) = (data))
		#define SET_BE32( addr, data )  (void) (*(BOOST::uint32_t*) (addr) = (data))
		
		#if BLARGG_CPU_POWERPC
			// PowerPC has special byte-reversed instructions
			#if defined (__MWERKS__)
				#define GET_LE16( addr )        (__lhbrx( addr, 0 ))
				#define GET_LE32( addr )        (__lwbrx( addr, 0 ))
				#define SET_LE16( addr, in )    (__sthbrx( in, addr, 0 ))
				#define SET_LE32( addr, in )    (__stwbrx( in, addr, 0 ))
			#elif defined (__GNUC__)
				#define GET_LE16( addr )        ({unsigned ppc_lhbrx_; asm( "lhbrx %0,0,%1" : "=r" (ppc_lhbrx_) : "r" (addr), "0" (ppc_lhbrx_) ); ppc_lhbrx_;})
				#define GET_LE32( addr )        ({unsigned ppc_lwbrx_; asm( "lwbrx %0,0,%1" : "=r" (ppc_lwbrx_) : "r" (addr), "0" (ppc_lwbrx_) ); ppc_lwbrx_;})
				#define SET_LE16( addr, in )    ({asm( "sthbrx %0,0,%1" : : "r" (in), "r" (addr) );})
				#define SET_LE32( addr, in )    ({asm( "stwbrx %0,0,%1" : : "r" (in), "r" (addr) );})
			#endif
		#endif
	#endif
#endif

#ifndef GET_LE16
	#define GET_LE16( addr )        get_le16( addr )
	#define SET_LE16( addr, data )  set_le16( addr, data )
#endif

#ifndef GET_LE32
	#define GET_LE32( addr )        get_le32( addr )
	#define SET_LE32( addr, data )  set_le32( addr, data )
#endif

#ifndef GET_BE16
	#define GET_BE16( addr )        get_be16( addr )
	#define SET_BE16( addr, data )  set_be16( addr, data )
#endif

#ifndef GET_BE32
	#define GET_BE32( addr )        get_be32( addr )
	#define SET_BE32( addr, data )  set_be32( addr, data )
#endif

#endif
