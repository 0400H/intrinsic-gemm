#ifndef INTRINSIC_TYPES_HPP
#define INTRINSIC_TYPES_HPP

#include "intrinsic_utils.h"

namespace IceSword {

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

template <DataType datatype>
struct DataTrait {};

template <>
struct DataTrait<DT_HALF> {
    typedef short Dtype;
    typedef short* PtrDtype;
};

template <>
struct DataTrait<DT_FLOAT> {
    typedef float Dtype;
    typedef float* PtrDtype;
};

template <>
struct DataTrait<DT_DOUBLE> {
    typedef double Dtype;
    typedef double* PtrDtype;
};

template <>
struct DataTrait<DT_INT8> {
    typedef char Dtype;
    typedef char* PtrDtype;
};

template <>
struct DataTrait<DT_INT16> {
    typedef short Dtype;
    typedef short* PtrDtype;
};

template <>
struct DataTrait<DT_INT32> {
    typedef int Dtype;
    typedef int* PtrDtype;
};

template <>
struct DataTrait<DT_INT64> {
    typedef long Dtype;
    typedef long* PtrDtype;
};

template <>
struct DataTrait<DT_UINT8> {
    typedef unsigned char Dtype;
    typedef unsigned char* PtrDtype;
};

template <>
struct DataTrait<DT_UINT16> {
    typedef unsigned short Dtype;
    typedef unsigned short* PtrDtype;
};

template <>
struct DataTrait<DT_UINT32> {
    typedef unsigned int Dtype;
    typedef unsigned int* PtrDtype;
};

}

#endif //INTRINSIC_TYPES_HPP