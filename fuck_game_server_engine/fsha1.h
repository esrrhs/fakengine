#pragma once

#define SHA1_BLOCK_SIZE  64
#define SHA1_DIGEST_SIZE 20
#define SHA2_GOOD         0
#define SHA2_BAD          1

#define rotl32(x,n) (((x) << n) | ((x) >> (32 - n)))

#if (FPLATFORM_BYTE_ORDER == F_BIG_ENDIAN)
#define swap_b32(x) (x)
#else
#define swap_b32(x) ((rotl32((x), 8) & 0x00ff00ff) | (rotl32((x), 24) & 0xff00ff00))
#endif

#define SHA1_MASK   (SHA1_BLOCK_SIZE - 1)

/* reverse byte order in 32-bit words   */

#define ch(x,y,z)       (((x) & (y)) ^ (~(x) & (z)))
#define parity(x,y,z)   ((x) ^ (y) ^ (z))
#define maj(x,y,z)      (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/* A normal version as set out in the FIPS. This version uses   */
/* partial loop unrolling and is optimised for the Pentium 4    */

#define rnd(f,k)    \
    t = a; a = rotl32(a,5) + f(b,c,d) + e + k + w[i]; \
    e = d; d = c; c = rotl32(b, 30); b = t

#define sha1_compile()\
{   \
	uint32_t    w[80], i, a, b, c, d, e, t;\
	\
    /* note that words are compiled from the buffer into 32-bit */\
    /* words in big-endian order so an order reversal is needed */\
    /* here on little endian machines                           */\
    for(i = 0; i < SHA1_BLOCK_SIZE / 4; ++i)\
        w[i] = swap_b32(ctx_wbuf[i]);\
		\
    for(i = SHA1_BLOCK_SIZE / 4; i < 80; ++i)\
        w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);\
		\
    a = ctx_hash[0];\
    b = ctx_hash[1];\
    c = ctx_hash[2];\
    d = ctx_hash[3];\
    e = ctx_hash[4];\
	\
    for(i = 0; i < 20; ++i)\
    {\
        rnd(ch, 0x5a827999);  \
    }\
		\
    for(i = 20; i < 40; ++i)\
    {\
        rnd(parity, 0x6ed9eba1);\
    }\
	\
    for(i = 40; i < 60; ++i)\
    {\
        rnd(maj, 0x8f1bbcdc);\
    }\
	\
    for(i = 60; i < 80; ++i)\
    {\
        rnd(parity, 0xca62c1d6);\
    }\
	\
    ctx_hash[0] += a; \
    ctx_hash[1] += b; \
    ctx_hash[2] += c; \
    ctx_hash[3] += d; \
    ctx_hash[4] += e;\
}

#define sha1_begin()\
{\
    ctx_count[0] = ctx_count[1] = 0;\
    ctx_hash[0] = 0x67452301;\
    ctx_hash[1] = 0xefcdab89;\
    ctx_hash[2] = 0x98badcfe;\
    ctx_hash[3] = 0x10325476;\
    ctx_hash[4] = 0xc3d2e1f0;\
}

/* SHA1 hash data in an array of bytes into hash buffer and call the        */
/* hash_compile function as required.                                       */

#define sha1_hash(data, len)\
{   \
	uint32_t pos = (uint32_t)(ctx_count[0] & SHA1_MASK), \
             space = SHA1_BLOCK_SIZE - pos;\
    const uint8_t *sp = data;\
	\
    if((ctx_count[0] += len) < len)\
        ++(ctx_count[1]);\
		\
    while(len >= space)     /* tranfer whole blocks while possible  */\
    {\
        memcpy(((uint8_t*)ctx_wbuf) + pos, sp, space);\
        sp += space; len -= space; space = SHA1_BLOCK_SIZE; pos = 0; \
        sha1_compile();\
    }\
	\
    memcpy(((uint8_t*)ctx_wbuf) + pos, sp, len);\
}

/* SHA1 final padding and digest calculation  */

#if (FPLATFORM_BYTE_ORDER == F_LITTLE_ENDIAN)
static const uint32_t  mask[4] = 
	{   0x00000000, 0x000000ff, 0x0000ffff, 0x00ffffff };
static const uint32_t  bits[4] = 
	{   0x00000080, 0x00008000, 0x00800000, 0x80000000 };
#else
static const uint32_t  mask[4] = 
	{   0x00000000, 0xff000000, 0xffff0000, 0xffffff00 };
static const uint32_t  bits[4] = 
	{   0x80000000, 0x00800000, 0x00008000, 0x00000080 };
#endif

#define sha1_end(hval)\
{   \
	uint32_t    i = (uint32_t)(ctx_count[0] & SHA1_MASK);\
	\
    /* mask out the rest of any partial 32-bit word and then set    */\
    /* the next byte to 0x80. On big-endian machines any bytes in   */\
    /* the buffer will be at the top end of 32 bit words, on little */\
    /* endian machines they will be at the bottom. Hence the AND    */\
    /* and OR masks above are reversed for little endian systems    */\
	/* Note that we can always add the first padding byte at this	*/\
	/* because the buffer always contains at least one empty slot	*/ \
    ctx_wbuf[i >> 2] = (ctx_wbuf[i >> 2] & mask[i & 3]) | bits[i & 3];\
	\
    /* we need 9 or more empty positions, one for the padding byte  */\
    /* (above) and eight for the length count.  If there is not     */\
    /* enough space pad and empty the buffer                        */\
    if(i > SHA1_BLOCK_SIZE - 9)\
    {\
        if(i < 60) ctx_wbuf[15] = 0;\
        sha1_compile();\
        i = 0;\
    }\
    else    /* compute a word index for the empty buffer positions  */\
        i = (i >> 2) + 1;\
		\
    while(i < 14) /* and zero pad all but last two positions      */ \
        ctx_wbuf[i++] = 0;\
    \
    /* assemble the eight byte counter in in big-endian format		*/\
    ctx_wbuf[14] = swap_b32((ctx_count[1] << 3) | (ctx_count[0] >> 29));\
    ctx_wbuf[15] = swap_b32(ctx_count[0] << 3);\
	\
    sha1_compile();\
	\
    /* extract the hash value as bytes in case the hash buffer is   */\
    /* misaligned for 32-bit words                                  */\
    for(i = 0; i < SHA1_DIGEST_SIZE; ++i)\
        hval[i] = (uint8_t)(ctx_hash[i >> 2] >> 8 * (~i & 3));\
}

static force_inline stringc fsha1(const uint8_t * data, uint32_t len)
{   
	uint8_t hval[SHA1_DIGEST_SIZE]; 

	uint32_t ctx_count[2];
	uint32_t ctx_hash[5];
	uint32_t ctx_wbuf[16];

    sha1_begin(); 
	sha1_hash(data, len); 
	sha1_end(hval);

	stringc ret;
	for ( int32_t i = 0 ; i < SHA1_DIGEST_SIZE ; i++ )
	{
		stringc rettmp(hval[i], 16);

		if (rettmp.size() == 1)
		{
			ret += "0";
			ret += rettmp;
		}
		else
		{
			ret += rettmp;
		}
	}

	return ret;
}
