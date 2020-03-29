#ifndef __BITSET_H__
#define __BITSET_H__

typedef unsigned int bitset;

#define BITS_PER_BITSET (8 * sizeof(bitset))

static inline void set_bit(bitset *bitarray, unsigned int i) {
    bitarray[i / BITS_PER_BITSET] |= (1 << (i % BITS_PER_BITSET));
}

static inline void clear_bit(bitset *bitarray, unsigned int i) {
    bitarray[i / BITS_PER_BITSET] &= ~(1 << (i % BITS_PER_BITSET));
}

static inline int check_bit(bitset *bitarray, unsigned int i) {
    return bitarray[i / BITS_PER_BITSET] & (1 << (i % BITS_PER_BITSET)) ? 1 : 0;
}

static inline int xor_bit(bitset *bitarray, unsigned int i, unsigned int x) {
    return (bitarray[i / BITS_PER_BITSET] ^= (1 << (i % BITS_PER_BITSET)));
}

static inline unsigned int find_first_unset_bit(bitset *     bitarray,
                                                unsigned int array_size) {
    // basicly from http://graphics.stanford.edu/~seander/bithacks.html
    for (bitset i = 0; i < array_size; i++)
        if (bitarray[i] == 0xFFFFFFFF)
            continue;
        else {
            bitset       c = ~bitarray[i];
            unsigned int r;
            asm volatile("bsfl %0, %0" : "=a"(r) : "0"(c));
            return r;
            // static const int MultiplyDeBruijnBitPosition[32] = {
            // 0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
            // 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
            // return MultiplyDeBruijnBitPosition
            // [((bitset)((c & -c) * 0x077CB531U)) >> 27] +
            // i * 32;
        }
    return 0xFFFFFFFF;
}

#endif // __BITSET_H__
