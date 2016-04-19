/**
 * @file gin_std_types.h
 * 
 * @brief a small set of standard types
 */


#pragma once


typedef signed char	int8;	/**< 8 bit signed integer type */
typedef signed short	int16;	/**< 16 bit signed integer type */
typedef signed int	int32;	/**< 32 bit signed integer type */
typedef signed long	int64;	/**< 64 bit signed integer type */

typedef unsigned char	uint8;	/**< 8 bit unsigned integer type */
typedef unsigned short	uint16;	/**< 16 bit unsigned integer type */
typedef unsigned int	uint32;	/**< 32 bit unsigned integer type */
typedef unsigned long	uint64;	/**< 64 bit unsigned integer type */

typedef unsigned char	flag8;	/**< the 8 bit flag type which holds 8 flags */
typedef unsigned short	flag16;	/**< the 16 bit flag type which holds 16 flags */
typedef unsigned int	flag32;	/**< the 32 bit flag type which holds 32 flags */
typedef unsigned long	flag64;	/**< the 64 bit flag type which holds 64 flags */

typedef void*		anything;	/**< a pointer to any type of data */
typedef const void*	const_anything;	/**< a pointer to a constant of any type of data */


/* eof */