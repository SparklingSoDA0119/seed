#ifndef _SEED_ENDIAN_H
#define _SEED_ENDIAN_H

#include <Common/Seed_Type.h>

namespace Seed {

    typedef bool ByteOrder;
    const ByteOrder SEED_BIG_ENDIAN = true;
    const ByteOrder SEED_LITTLE_ENDIAN = false;

    namespace Endian {
        ByteOrder systemByteOrder();

        int16 swapInt16(int16 i);
        uint16 swapUint16(uint16 i);
        int32 swapInt32(int32 i);
        uint32 swapUint32(uint32 i);
        int64 swapInt64(int64 i);
        uint64 swapUint64(uint64 i);
        float32 swapFloat32(float32 f);
        float64 swapFloat64(float64 f);
    }   // namespace Endian

}   // namespace Seed

#endif  // _SEED_ENDIAN_H
