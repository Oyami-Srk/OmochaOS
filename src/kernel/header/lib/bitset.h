#ifndef __BITSET_H__
#define __BITSET_H__

// typedef unsigned int bitset;

#define BITS_PER_UINT (8 * sizeof(unsigned int))

static inline void set_bit(unsigned int *bitarray, unsigned int i) {
    bitarray[i / BITS_PER_UINT] |= (1 << (i % BITS_PER_UINT));
}

static inline void clear_bit(unsigned int *bitarray, unsigned int i) {
    bitarray[i / BITS_PER_UINT] &= ~(1 << (i % BITS_PER_UINT));
}

static inline int check_bit(unsigned int *bitarray, unsigned int i) {
    return bitarray[i / BITS_PER_UINT] & (1 << (i % BITS_PER_UINT)) ? 1 : 0;
}

static inline int find_first_unset_bit(unsigned int *bitarray,
                                       unsigned int  array_size) {
    // basicly from http://graphics.stanford.edu/~seander/bithacks.html
    for (unsigned int i = 0; i < array_size; i++)
        if (bitarray[i] == 0xFFFFFFFF)
            continue;
        else {
            unsigned int     c                               = ~bitarray[i];
            static const int MultiplyDeBruijnBitPosition[32] = {
                0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
                31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
            return MultiplyDeBruijnBitPosition
                       [((unsigned int)((c & -c) * 0x077CB531U)) >> 27] +
                   i * 32;
        }
    return -1;
}

static inline int find_first_unset_bit_group(unsigned int *bitarray,
                                             unsigned int  array_size,
                                             unsigned int  group_size) {
    return 0;
}

#endif // __BITSET_H__
