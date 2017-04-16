#ifndef SERVICES_SAMPLE_XORSHIFT_TEST_HPP
#define SERVICES_SAMPLE_XORSHIFT_TEST_HPP

#include <stan/services/sample/splitmix64.hpp>
#include <stdint.h>
//#include <string.h>

/* This is a fast, top-quality generator. If 1024 bits of state are too
   much, try a xoroshiro128+ generator.

   Note that the three lowest bits of this generator are LSFRs, and thus
   they are slightly less random than the other bits. We suggest to use a
   sign test to extract a random Boolean value.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

class xorshift_1024 {
  uint64_t s[16]; 
  int p;


public:
  typedef uint32_t result_type;

  static uint32_t min BOOST_PREVENT_MACRO_SUBSTITUTION () { return 0; }
  static uint32_t max BOOST_PREVENT_MACRO_SUBSTITUTION () { return 0xFFFFFFFF; }

  xorshift_1024(unsigned int short_seed) : p(0) {
      //Use splitmix to initialize the long internal state
      splitmix64 helper((uint64_t)short_seed);
      for (int i = 0; i < 10; i++) {
        helper.next();
      }

      for (int i = 0; i < 16; i++) {
        s[i] = helper.next();
      }      
  }

  uint64_t next(void) {
    const uint64_t s0 = s[p];
    uint64_t s1 = s[p = (p + 1) & 15];
    s1 ^= s1 << 31; // a
    s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
    return s[p] * UINT64_C(1181783497276652981);
  }

  uint32_t operator()()
    {
      return (uint32_t) this->next();
    }

  /* This is the jump function for the generator. It is equivalent
    to 2^512 calls to next(); it can be used to generate 2^512
    non-overlapping subsequences for parallel computations. */

  void jump(void) {
    static const uint64_t JUMP[] = { 0x84242f96eca9c41d,
      0xa3c65b8776f96855, 0x5b34a39f070b5837, 0x4489affce4f31a1e,
      0x2ffeeb0a48316f40, 0xdc2d9891fe68c022, 0x3659132bb12fea70,
      0xaac17d8efa43cab8, 0xc4cb815590989b13, 0x5ee975283d71c93b,
      0x691548c86c1bd540, 0x7910c41d10a1e6a5, 0x0b5fc64563b3e2a8,
      0x047f7684e9fc949d, 0xb99181f2d8f685ca, 0x284600e3f30e38c3
    };

    uint64_t t[16] = { 0 };
    for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
      for(int b = 0; b < 64; b++) {
        if (JUMP[i] & 1ULL << b)
          for(int j = 0; j < 16; j++)
            t[j] ^= s[(j + p) & 15];
        next();
      }

    for(int j = 0; j < 16; j++)
      s[(j + p) & 15] = t[j];
  }

};

#endif