/**
 * @file gin_std_macros.h
 * 
 * @brief a small set of standard macros
 */


#pragma once


#define GIN_STMT_START	do{
#define GIN_STMT_END	}while(0)


/* bit macros */

/**
 * @brief checks if a bit(s) is set
 * 
 * @param bit_sequence the bits to check against
 * @param bit the bit(s) to check in the bit_sequence
 * 
 * @return 1 if the bit(s) is set, 0 if it isn't
 */
#define check_bit( bit_sequence, bit )	( bit_sequence & bit )

/**
 * @brief sets a bit(s)
 * 
 * @param bit_sequence the bits to modify
 * @param bit the bit(s) to set
 */
#define set_bit( bit_sequence, bit )	( bit_sequence |= bit )

/**
 * @brief removes a bit(s)
 * 
 * @param bit_sequence the bits to modify
 * @param bit the bit(s) to remove
 */
#define remove_bit( bit_sequence, bit )	( bit_sequence &= ~bit )

/**
 * @brief toggles a bit(s)
 * 
 * @param bit_sequence the bits to modify
 * @param bit the bit(s) to toggle
 */
#define toggle_bit( bit_sequence, bit )	( bit_sequence ^= bit )


/* return macros */

/**< returns a function if an expression succeeds */
#define return_if_success( expr )	\
    GIN_STMT_START		\
    if( expr ) {return;};	\
    GIN_STMT_END
    
/**< returns a function if an expression fails */
#define return_if_fail( expr )	\
    GIN_STMT_START		\
    if( !expr ) {return;};	\
    GIN_STMT_END

/**< returns a function with a value if an expression succeeds */
#define return_value_if_success( expr, value )	\
    GIN_STMT_START				\
    if( expr ) {return value;};			\
    GIN_STMT_END

/**< returns a function with a value if an expression fails */
#define return_value_if_fail( expr, value )	\
    GIN_STMT_START				\
    if( !expr ) {return value;};		\
    GIN_STMT_END


/* eof */