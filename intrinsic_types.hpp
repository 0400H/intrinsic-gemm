#ifndef INTRINSIC_TYPES_HPP
#define INTRINSIC_TYPES_HPP

#include "intrinsic_utils.h"
#include <string>

namespace IceSword {

typedef enum {
    S_Success         = -1,                                /*!< No errors */
    S_NotInitialized  = 1,                                 /*!< Data not initialized. */
    S_InvalidValue    = (1 << 1) + S_NotInitialized, /*!< Incorrect variable value. */
    S_MemAllocFailed  = (1 << 2) + S_InvalidValue,   /*!< Memory allocation error. */
    S_UnKownError     = (1 << 3) + S_MemAllocFailed, /*!< Unknown error. */
    S_OutOfAuthority  = (1 << 4) + S_UnKownError,    /*!< Try to modified data not your own*/
    S_OutOfMem        = (1 << 5) + S_OutOfAuthority, /*!< OOM error*/
    S_UnImplError     = (1 << 6) + S_OutOfMem,       /*!< Unimplement error. */
    S_WrongDevice     = (1 << 7) + S_UnImplError     /*!< un-correct device. */
} S_Status;

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
    DT_TENSOR       =       14,
};

template <DataType datatype>
struct DataTrait {};

template <>
struct DataTrait<DT_HALF> {
    typedef short Dtype;
    typedef short* PtrDtype;
    std::string DtypeStr = "DT_HALF";
};

template <>
struct DataTrait<DT_FLOAT> {
    typedef float Dtype;
    typedef float* PtrDtype;
    std::string DtypeStr = "DT_FLOAT";
};

template <>
struct DataTrait<DT_DOUBLE> {
    typedef double Dtype;
    typedef double* PtrDtype;
    std::string DtypeStr = "DT_DOUBLE";
};

template <>
struct DataTrait<DT_INT8> {
    typedef char Dtype;
    typedef char* PtrDtype;
    std::string DtypeStr = "DT_INT8";
};

template <>
struct DataTrait<DT_INT16> {
    typedef short Dtype;
    typedef short* PtrDtype;
    std::string DtypeStr = "DT_INT16";
};

template <>
struct DataTrait<DT_INT32> {
    typedef int Dtype;
    typedef int* PtrDtype;
    std::string DtypeStr = "DT_INT32";
};

template <>
struct DataTrait<DT_INT64> {
    typedef long Dtype;
    typedef long* PtrDtype;
    std::string DtypeStr = "DT_INT64";
};

template <>
struct DataTrait<DT_UINT8> {
    typedef unsigned char Dtype;
    typedef unsigned char* PtrDtype;
    std::string DtypeStr = "DT_UINT8";
};

template <>
struct DataTrait<DT_UINT16> {
    typedef unsigned short Dtype;
    typedef unsigned short* PtrDtype;
    std::string DtypeStr = "DT_UINT16";
};

template <>
struct DataTrait<DT_UINT32> {
    typedef unsigned int Dtype;
    typedef unsigned int* PtrDtype;
    std::string DtypeStr = "DT_UINT32";
};

template <DataType datatype>
std::string GetDtypeStr() {
    return DataTrait<datatype>().DtypeStr;
}

}

#endif //INTRINSIC_TYPES_HPP