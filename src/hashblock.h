#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "sph_blake.h"
#include "sph_groestl.h"
#include "sph_jh.h"
#include "sph_keccak.h"
#include "sph_skein.h"

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

inline uint256 HashSHA3V2(char * input, unsigned int round_mask) {

    sph_blake512_context     ctx_blake;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;

    uint512 hash;

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, input, 80);
    sph_keccak512_close(&ctx_keccak, (&hash));

    unsigned int round;
    for (round = 0; round < 3; round++) {
        if (hash.GetUInt32(0) & 0x01) {
           sph_groestl512_init(&ctx_groestl);
           sph_groestl512 (&ctx_groestl, (&hash), 64);
           sph_groestl512_close(&ctx_groestl, (&hash));
        }
        else {
           sph_skein512_init(&ctx_skein);
           sph_skein512 (&ctx_skein, (&hash), 64);
           sph_skein512_close(&ctx_skein, (&hash));
        }
        if (hash.GetUInt32(0) & 0x01) {
           sph_blake512_init(&ctx_blake);
           sph_blake512 (&ctx_blake, (&hash), 64);
           sph_blake512_close(&ctx_blake, (&hash));
        }
        else {
           sph_jh512_init(&ctx_jh);
           sph_jh512 (&ctx_jh, (&hash), 64);
           sph_jh512_close(&ctx_jh, (&hash));
        }
    }

    return hash.trim256();

}

template<typename T1>
inline uint256 HashSHA3(const T1 pbegin, const T1 pend) {

    char * ptr = (char*)(&pbegin[0]);

    if (fDebug) {
       printf("Hash Input \n");
       for (unsigned int i = 0; i < 88; i++) {
           printf ("%.2x", (unsigned char)ptr[i]);
       }
       printf("\n");
    }

    unsigned int round_mask = *(unsigned int *)(&pbegin[84]);
    return HashSHA3V2(ptr, round_mask);
}


#endif // HASHBLOCK_H