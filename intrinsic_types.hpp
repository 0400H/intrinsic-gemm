#ifndef INTRINSIC_TYPES_H
#define INTRINSIC_TYPES_H

namespace IceSword {

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

enum DataType {
    DT_INVALID      =       -1,
    DT_HALF         =       0,
    DT_FLOAT        =       1,
    DT_DOUBLE       =       2,
    DT_INT8         =       3,
    DT_INT16        =       4,
    DT_INT32        =       5,
    DT_INT64        =       6,
    DT_UINT8        =       7,
    DT_UINT16       =       8,
    DT_UINT32       =       9,
    DT_UINT64       =       10,
    DT_STRING       =       11,
    DT_BOOL         =       12,
    DT_SHAPE        =       13,
    DT_TENSOR       =       14
};

typedef enum {
    IceSwordSuccess         = -1,                                /*!< No errors */
    IceSwordNotInitialized  = 1,                                 /*!< Data not initialized. */
    IceSwordInvalidValue    = (1 << 1) + IceSwordNotInitialized, /*!< Incorrect variable value. */
    IceSwordMemAllocFailed  = (1 << 2) + IceSwordInvalidValue,   /*!< Memory allocation error. */
    IceSwordUnKownError     = (1 << 3) + IceSwordMemAllocFailed, /*!< Unknown error. */
    IceSwordOutOfAuthority  = (1 << 4) + IceSwordUnKownError,    /*!< Try to modified data not your own*/
    IceSwordOutOfMem        = (1 << 5) + IceSwordOutOfAuthority, /*!< OOM error*/
    IceSwordUnImplError     = (1 << 6) + IceSwordOutOfMem,       /*!< Unimplement error. */
    IceSwordWrongDevice     = (1 << 7) + IceSwordUnImplError     /*!< un-correct device. */
} IceSwordStatus;

}

#endif //INTRINSIC_TYPES_H