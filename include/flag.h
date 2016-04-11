#pragma once


/**
 * @file flag.h
 * 
 * @brief A file for controlling bit flags.
 * 
 * @note The flag type for using individual bits as boolean values.
 * @note Bits can be checked, set, removed, and toggled.
 * @note The macros work with any integer type.
 */


typedef unsigned char	flag8;	/**< the 8 bit flag type which holds 8 flags */
typedef unsigned short	flag16;	/**< the 16 bit flag type which holds 16 flags */
typedef unsigned int	flag32;	/**< the 32 bit flag type which holds 32 flags */
typedef unsigned long	flag64;	/**< the 64 bit flag type which holds 64 flags */


/**
 * @brief A macro to check if a bit in a flag is set.
 * 
 * @param flag the flag to use
 * @param bit the bit to check
 * 
 * @return 1 if the bit is set, 0 if it isn't
 */
#define check_bit( flag, bit )	( flag & bit )

/**
 * @brief A macro to set a bit in a flag.
 * 
 * @param flag the flag to use
 * @param bit the bit to set
 */
#define set_bit( flag, bit )	( flag |= bit )

/**
 * @brief A macro to remove a bit from a flag.
 * 
 * @param flag the flag to use
 * @param bit the bit to remove
 */
#define remove_bit( flag, bit )	( flag &= ~bit )

/**
 * @brief A macro to toggle a bit in a flag.
 * 
 * @param flag the flag to use
 * @param bit the bit to toggle
 */
#define toggle_bit( flag, bit )	( flag ^= bit )


/* eof */