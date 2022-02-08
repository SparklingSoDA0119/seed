#include <Common/Seed_Endian.h>

namespace Seed {

    namespace Endian {

        ByteOrder systemByteOrder()
        {
            int32 i = 0x00000001;
            return (0 == *(int8*)&i) ? SEED_BIG_ENDIAN : SEED_LITTLE_ENDIAN;
        }

        int16 swapInt16(int16 i)
        {
            return static_cast<int16>(swapUint16(static_cast<uint16>(i)));
        }

        uint16 swapUint16(uint16 i)
        {
#if defined(__i386__) && defined(__GNUC__)
            __asm__("xchgb %b0, %h0" : "+q" (i));
            return i;
#elif defined(__ppc__) && defined(__GNUC__)
            uint16 result;
            __asm__("lhbrx %0,0,%1" : "=r" (result) : "r" (&i), "m" (i));
            return result;
#else
            return ((i >> 8) & 0x00FF) | ((i << 8) & 0xFF00);
#endif
        }

        int32 swapInt32(int32 i)
        {
            return static_cast<int32>(swapUint32(static_cast<uint32>(i)));
        }

        uint32 swapUint32(uint32 i)
        {
#if defined(__i386__) && defined(__GNUC__)
            __asm__("bswap %0" : "+r" (i));
            return i;
#elif defined(__ppc__) && defined(__GNUC__)
            uint32 result;
            __asm__("lwbrx %0,0,%1" : "=r" (result) : "r" (&i), "m" (i));
            return result;
#else
            return ((i >> 24) & 0x000000FF) | ((i << 24) & 0xFF000000) |
                ((i >> 8) & 0x0000FF00) | ((i << 8) & 0x00FF0000);
#endif
        }

        int64 swapInt64(int64 i)
        {
            return static_cast<int64>(swapUint64(static_cast<uint64>(i)));
        }

        uint64 swapUint64(uint64 i)
        {
            union Swap {
                uint64 sv;
                uint32 ul[2];
            } tmp, result;

            tmp.sv = i;
            result.ul[0] = swapUint32(tmp.ul[1]);
            result.ul[1] = swapUint32(tmp.ul[0]);
            return result.sv;
        }

        float32 swapFloat32(float32 f)
        {
            float32 result;
            uint8* r = reinterpret_cast<uint8*>(&result);
            uint8* p = reinterpret_cast<uint8*>(&f);
            r[0] = p[3], r[1] = p[2], r[2] = p[1], r[3] = p[0];
            return result;
        }

        float64 swapFloat64(float64 f)
        {
            float64 result;
            uint8* r = reinterpret_cast<uint8*>(&result);
            uint8* p = reinterpret_cast<uint8*>(&f);
            r[0] = p[7], r[1] = p[6], r[2] = p[5], r[3] = p[4];
            r[4] = p[3], r[5] = p[2], r[6] = p[1], r[7] = p[0];
            return result;
        }

    }   // namespace Endian

}   // namespace Seed
