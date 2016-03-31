#pragma once


/**
 * @flag.h
 * 
 * The flag type is using individual bits for use as boolean values.
 * Bits can be checked, set, removed, and toggled.
 */


/**
 * the flag data type
 */
typedef unsigned int flag;


/**
 * @brief checks if a bit is set a flag
 * 
 * @param flag the flag to use
 * @param bit the bit to check
 * 
 * @return 1 if the bit is set, 0 if it isn't
 */
#define check_bit( flag, bit )	( flag & bit )

/**
 * @brief sets a bit to 1 in a flag
 * 
 * @param flag the flag to use
 * @param bit the bit to set
 */
#define set_bit( flag, bit )	( flag |= bit )

/**
 * @brief sets a bit to 0 in a flag
 * 
 * @param flag the flag to use
 * @param bit the bit to remove
 */
#define remove_bit( flag, bit )	( flag &= ~bit )

/**
 * @brief toggles between a bit being 1 and 0
 * 
 * @param flag the flag to use
 * @param bit the bit to toggle
 */
#define toggle_bit( flag, bit )	( flag ^= bit )


/* eof */