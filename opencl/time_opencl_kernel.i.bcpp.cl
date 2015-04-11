typedef ulong uint64_t;
typedef uint uint32_t;
typedef uchar uint8_t;
inline int haveAESNI()
{
    return 0;
}


struct r123array1x32{ uint32_t v[1]; };
struct r123array2x32{ uint32_t v[2]; };
struct r123array4x32{ uint32_t v[4]; };
struct r123array8x32{ uint32_t v[8]; };
struct r123array1x64{ uint64_t v[1]; };
struct r123array2x64{ uint64_t v[2]; };
struct r123array4x64{ uint64_t v[4]; };
struct r123array16x8{ uint8_t v[16]; };
inline uint32_t mulhilo32(uint32_t a, uint32_t b, uint32_t* hip){ uint64_t product = ((uint64_t)a)*((uint64_t)b); *hip = product>>32; return (uint32_t)product; }
inline uint64_t mulhilo64(uint64_t a, uint64_t b, uint64_t* hip){ *hip = mul_hi(a, b); return a*b; }
inline struct r123array1x32 _philox2x32bumpkey( struct r123array1x32 key) { key.v[0] += ((uint32_t)0x9E3779B9); return key; }
inline struct r123array2x32 _philox4x32bumpkey( struct r123array2x32 key) { key.v[0] += ((uint32_t)0x9E3779B9); key.v[1] += ((uint32_t)0xBB67AE85); return key; }
inline struct r123array2x32 _philox2x32round(struct r123array2x32 ctr, struct r123array1x32 key) __attribute__((always_inline)); inline struct r123array2x32 _philox2x32round(struct r123array2x32 ctr, struct r123array1x32 key)
{
    uint32_t hi; uint32_t lo = mulhilo32(((uint32_t)0xd256d193), ctr.v[0], &hi); struct r123array2x32 out =
    {
        {
            hi^key.v[0]^ctr.v[1], lo
        }
    };
    return out;
}


inline struct r123array4x32 _philox4x32round(struct r123array4x32 ctr, struct r123array2x32 key) __attribute__((always_inline)); inline struct r123array4x32 _philox4x32round(struct r123array4x32 ctr, struct r123array2x32 key)
{
    uint32_t hi0; uint32_t hi1; uint32_t lo0 = mulhilo32(((uint32_t)0xD2511F53), ctr.v[0], &hi0); uint32_t lo1 = mulhilo32(((uint32_t)0xCD9E8D57), ctr.v[2], &hi1); struct r123array4x32 out =
    {
        {
            hi1^ctr.v[1]^key.v[0], lo1, hi0^ctr.v[3]^key.v[1], lo0
        }
    };
    return out;
}


enum { philox2x32_rounds = 10 };
typedef struct r123array2x32 philox2x32_ctr_t; typedef struct r123array1x32 philox2x32_key_t; typedef struct r123array1x32 philox2x32_ukey_t; inline philox2x32_key_t philox2x32keyinit(philox2x32_ukey_t uk)
{
    return uk;
}


inline philox2x32_ctr_t philox2x32_R(unsigned int R, philox2x32_ctr_t ctr, philox2x32_key_t key) __attribute__((always_inline)); inline philox2x32_ctr_t philox2x32_R(unsigned int R, philox2x32_ctr_t ctr, philox2x32_key_t key)
{
    ; if(R>0)
    {
        ctr = _philox2x32round(ctr, key);
    }
    if(R>1)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>2)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>3)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>4)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>5)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>6)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>7)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>8)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>9)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>10)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>11)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>12)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>13)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>14)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    if(R>15)
    {
        key = _philox2x32bumpkey(key); ctr = _philox2x32round(ctr, key);
    }
    return ctr;
}


enum { philox4x32_rounds = 10 };
typedef struct r123array4x32 philox4x32_ctr_t; typedef struct r123array2x32 philox4x32_key_t; typedef struct r123array2x32 philox4x32_ukey_t; inline philox4x32_key_t philox4x32keyinit(philox4x32_ukey_t uk)
{
    return uk;
}


inline philox4x32_ctr_t philox4x32_R(unsigned int R, philox4x32_ctr_t ctr, philox4x32_key_t key) __attribute__((always_inline)); inline philox4x32_ctr_t philox4x32_R(unsigned int R, philox4x32_ctr_t ctr, philox4x32_key_t key)
{
    ; if(R>0)
    {
        ctr = _philox4x32round(ctr, key);
    }
    if(R>1)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>2)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>3)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>4)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>5)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>6)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>7)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>8)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>9)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>10)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>11)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>12)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>13)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>14)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    if(R>15)
    {
        key = _philox4x32bumpkey(key); ctr = _philox4x32round(ctr, key);
    }
    return ctr;
}


inline struct r123array1x64 _philox2x64bumpkey( struct r123array1x64 key) { key.v[0] += ((ulong)(0x9E3779B97F4A7C15UL)); return key; }
inline struct r123array2x64 _philox4x64bumpkey( struct r123array2x64 key) { key.v[0] += ((ulong)(0x9E3779B97F4A7C15UL)); key.v[1] += ((ulong)(0xBB67AE8584CAA73BUL)); return key; }
inline struct r123array2x64 _philox2x64round(struct r123array2x64 ctr, struct r123array1x64 key) __attribute__((always_inline)); inline struct r123array2x64 _philox2x64round(struct r123array2x64 ctr, struct r123array1x64 key)
{
    uint64_t hi; uint64_t lo = mulhilo64(((ulong)(0xD2B74407B1CE6E93UL)), ctr.v[0], &hi); struct r123array2x64 out =
    {
        {
            hi^key.v[0]^ctr.v[1], lo
        }
    };
    return out;
}


inline struct r123array4x64 _philox4x64round(struct r123array4x64 ctr, struct r123array2x64 key) __attribute__((always_inline)); inline struct r123array4x64 _philox4x64round(struct r123array4x64 ctr, struct r123array2x64 key)
{
    uint64_t hi0; uint64_t hi1; uint64_t lo0 = mulhilo64(((ulong)(0xD2E7470EE14C6C93UL)), ctr.v[0], &hi0); uint64_t lo1 = mulhilo64(((ulong)(0xCA5A826395121157UL)), ctr.v[2], &hi1); struct r123array4x64 out =
    {
        {
            hi1^ctr.v[1]^key.v[0], lo1, hi0^ctr.v[3]^key.v[1], lo0
        }
    };
    return out;
}


enum { philox2x64_rounds = 10 };
typedef struct r123array2x64 philox2x64_ctr_t; typedef struct r123array1x64 philox2x64_key_t; typedef struct r123array1x64 philox2x64_ukey_t; inline philox2x64_key_t philox2x64keyinit(philox2x64_ukey_t uk)
{
    return uk;
}


inline philox2x64_ctr_t philox2x64_R(unsigned int R, philox2x64_ctr_t ctr, philox2x64_key_t key) __attribute__((always_inline)); inline philox2x64_ctr_t philox2x64_R(unsigned int R, philox2x64_ctr_t ctr, philox2x64_key_t key)
{
    ; if(R>0)
    {
        ctr = _philox2x64round(ctr, key);
    }
    if(R>1)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>2)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>3)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>4)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>5)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>6)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>7)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>8)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>9)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>10)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>11)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>12)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>13)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>14)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    if(R>15)
    {
        key = _philox2x64bumpkey(key); ctr = _philox2x64round(ctr, key);
    }
    return ctr;
}


enum { philox4x64_rounds = 10 };
typedef struct r123array4x64 philox4x64_ctr_t; typedef struct r123array2x64 philox4x64_key_t; typedef struct r123array2x64 philox4x64_ukey_t; inline philox4x64_key_t philox4x64keyinit(philox4x64_ukey_t uk)
{
    return uk;
}


inline philox4x64_ctr_t philox4x64_R(unsigned int R, philox4x64_ctr_t ctr, philox4x64_key_t key) __attribute__((always_inline)); inline philox4x64_ctr_t philox4x64_R(unsigned int R, philox4x64_ctr_t ctr, philox4x64_key_t key)
{
    ; if(R>0)
    {
        ctr = _philox4x64round(ctr, key);
    }
    if(R>1)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>2)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>3)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>4)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>5)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>6)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>7)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>8)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>9)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>10)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>11)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>12)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>13)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>14)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    if(R>15)
    {
        key = _philox4x64bumpkey(key); ctr = _philox4x64round(ctr, key);
    }
    return ctr;
}


enum
{
    R_64x4_0_0=14, R_64x4_0_1=16,
    R_64x4_1_0=52, R_64x4_1_1=57,
    R_64x4_2_0=23, R_64x4_2_1=40,
    R_64x4_3_0= 5, R_64x4_3_1=37,
    R_64x4_4_0=25, R_64x4_4_1=33,
    R_64x4_5_0=46, R_64x4_5_1=12,
    R_64x4_6_0=58, R_64x4_6_1=22,
    R_64x4_7_0=32, R_64x4_7_1=32
};
enum
{
    R_64x2_0_0=16,
    R_64x2_1_0=42,
    R_64x2_2_0=12,
    R_64x2_3_0=31,
    R_64x2_4_0=16,
    R_64x2_5_0=32,
    R_64x2_6_0=24,
    R_64x2_7_0=21
};
enum
{
    R_32x4_0_0=10, R_32x4_0_1=26,
    R_32x4_1_0=11, R_32x4_1_1=21,
    R_32x4_2_0=13, R_32x4_2_1=27,
    R_32x4_3_0=23, R_32x4_3_1= 5,
    R_32x4_4_0= 6, R_32x4_4_1=20,
    R_32x4_5_0=17, R_32x4_5_1=11,
    R_32x4_6_0=25, R_32x4_6_1=10,
    R_32x4_7_0=18, R_32x4_7_1=20
};
enum
{
    R_32x2_0_0=13,
    R_32x2_1_0=15,
    R_32x2_2_0=26,
    R_32x2_3_0= 6,
    R_32x2_4_0=17,
    R_32x2_5_0=29,
    R_32x2_6_0=16,
    R_32x2_7_0=24
};
enum
{
    WCNT2=2,
    WCNT4=4
};
inline uint64_t RotL_64(uint64_t x, unsigned int N) __attribute__((always_inline));
inline uint64_t RotL_64(uint64_t x, unsigned int N)
{
    return (x << (N & 63)) | (x >> ((64-N) & 63));
}


inline uint32_t RotL_32(uint32_t x, unsigned int N) __attribute__((always_inline));
inline uint32_t RotL_32(uint32_t x, unsigned int N)
{
    return (x << (N & 31)) | (x >> ((32-N) & 31));
}


typedef struct r123array2x64 threefry2x64_ctr_t; typedef struct r123array2x64 threefry2x64_key_t; typedef struct r123array2x64 threefry2x64_ukey_t; inline threefry2x64_key_t threefry2x64keyinit(threefry2x64_ukey_t uk) { return uk; }
inline threefry2x64_ctr_t threefry2x64_R(unsigned int Nrounds, threefry2x64_ctr_t in, threefry2x64_key_t k) __attribute__((always_inline)); inline threefry2x64_ctr_t threefry2x64_R(unsigned int Nrounds, threefry2x64_ctr_t in, threefry2x64_key_t k)
{
    threefry2x64_ctr_t X; uint64_t ks[WCNT2+1]; int i; ; ks[WCNT2] = ((0xA9FC1A22) + (((uint64_t) (0x1BD11BDA)) << 32)); for (i=0;i < WCNT2; i++)
    {
        ks[i] = k.v[i]; X.v[i] = in.v[i]; ks[WCNT2] ^= k.v[i];
    }
    X.v[0] += ks[0]; X.v[1] += ks[1]; if(Nrounds>0)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>1)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>2)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>3)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>3)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[1] += 1;
    }
    if(Nrounds>4)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_4_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>5)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_5_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>6)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_6_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>7)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_7_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>7)
    {
        X.v[0] += ks[2]; X.v[1] += ks[0]; X.v[1] += 2;
    }
    if(Nrounds>8)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>9)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>10)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>11)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>11)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[1] += 3;
    }
    if(Nrounds>12)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_4_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>13)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_5_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>14)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_6_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>15)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_7_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>15)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[1] += 4;
    }
    if(Nrounds>16)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>17)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>18)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>19)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>19)
    {
        X.v[0] += ks[2]; X.v[1] += ks[0]; X.v[1] += 5;
    }
    if(Nrounds>20)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>21)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>22)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>23)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>23)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[1] += 6;
    }
    if(Nrounds>24)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_4_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>25)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_5_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>26)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_6_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>27)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_7_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>27)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[1] += 7;
    }
    if(Nrounds>28)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>29)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>30)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>31)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>31)
    {
        X.v[0] += ks[2]; X.v[1] += ks[0]; X.v[1] += 8;
    }
    return X;
}


enum
{
    threefry2x64_rounds = 20
};
inline threefry2x64_ctr_t threefry2x64(threefry2x64_ctr_t in, threefry2x64_key_t k) __attribute__((always_inline)); inline threefry2x64_ctr_t threefry2x64(threefry2x64_ctr_t in, threefry2x64_key_t k)
{
    return threefry2x64_R(threefry2x64_rounds, in, k);
}


typedef struct r123array2x32 threefry2x32_ctr_t; typedef struct r123array2x32 threefry2x32_key_t; typedef struct r123array2x32 threefry2x32_ukey_t; inline threefry2x32_key_t threefry2x32keyinit(threefry2x32_ukey_t uk) { return uk; }
inline threefry2x32_ctr_t threefry2x32_R(unsigned int Nrounds, threefry2x32_ctr_t in, threefry2x32_key_t k) __attribute__((always_inline)); inline threefry2x32_ctr_t threefry2x32_R(unsigned int Nrounds, threefry2x32_ctr_t in, threefry2x32_key_t k)
{
    threefry2x32_ctr_t X; uint32_t ks[WCNT2+1]; int i; ; ks[WCNT2] = 0x1BD11BDA; for (i=0;i < WCNT2; i++)
    {
        ks[i] = k.v[i]; X.v[i] = in.v[i]; ks[WCNT2] ^= k.v[i];
    }
    X.v[0] += ks[0]; X.v[1] += ks[1]; if(Nrounds>0)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>1)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>2)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>3)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>3)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[1] += 1;
    }
    if(Nrounds>4)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_4_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>5)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_5_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>6)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_6_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>7)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_7_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>7)
    {
        X.v[0] += ks[2]; X.v[1] += ks[0]; X.v[1] += 2;
    }
    if(Nrounds>8)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>9)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>10)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>11)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>11)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[1] += 3;
    }
    if(Nrounds>12)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_4_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>13)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_5_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>14)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_6_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>15)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_7_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>15)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[1] += 4;
    }
    if(Nrounds>16)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>17)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>18)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>19)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>19)
    {
        X.v[0] += ks[2]; X.v[1] += ks[0]; X.v[1] += 5;
    }
    if(Nrounds>20)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>21)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>22)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>23)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>23)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[1] += 6;
    }
    if(Nrounds>24)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_4_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>25)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_5_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>26)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_6_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>27)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_7_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>27)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[1] += 7;
    }
    if(Nrounds>28)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_0_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>29)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_1_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>30)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_2_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>31)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x2_3_0); X.v[1] ^= X.v[0];
    }
    if(Nrounds>31)
    {
        X.v[0] += ks[2]; X.v[1] += ks[0]; X.v[1] += 8;
    }
    return X;
}


enum
{
    threefry2x32_rounds = 20
};
inline threefry2x32_ctr_t threefry2x32(threefry2x32_ctr_t in, threefry2x32_key_t k) __attribute__((always_inline)); inline threefry2x32_ctr_t threefry2x32(threefry2x32_ctr_t in, threefry2x32_key_t k)
{
    return threefry2x32_R(threefry2x32_rounds, in, k);
}


typedef struct r123array4x64 threefry4x64_ctr_t; typedef struct r123array4x64 threefry4x64_key_t; typedef struct r123array4x64 threefry4x64_ukey_t; inline threefry4x64_key_t threefry4x64keyinit(threefry4x64_ukey_t uk) { return uk; }
inline threefry4x64_ctr_t threefry4x64_R(unsigned int Nrounds, threefry4x64_ctr_t in, threefry4x64_key_t k) __attribute__((always_inline)); inline threefry4x64_ctr_t threefry4x64_R(unsigned int Nrounds, threefry4x64_ctr_t in, threefry4x64_key_t k)
{
    threefry4x64_ctr_t X; uint64_t ks[WCNT4+1]; int i; ; ks[WCNT4] = ((0xA9FC1A22) + (((uint64_t) (0x1BD11BDA)) << 32)); for (i=0;i < WCNT4; i++)
    {
        ks[i] = k.v[i]; X.v[i] = in.v[i]; ks[WCNT4] ^= k.v[i];
    }
    X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; if(Nrounds>0)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>1)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>2)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>3)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>3)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 1;
    }
    if(Nrounds>4)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>5)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>6)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>7)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>7)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 2;
    }
    if(Nrounds>8)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>9)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>10)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>11)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>11)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 3;
    }
    if(Nrounds>12)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>13)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>14)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>15)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>15)
    {
        X.v[0] += ks[4]; X.v[1] += ks[0]; X.v[2] += ks[1]; X.v[3] += ks[2]; X.v[WCNT4-1] += 4;
    }
    if(Nrounds>16)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>17)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>18)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>19)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>19)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; X.v[WCNT4-1] += 5;
    }
    if(Nrounds>20)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>21)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>22)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>23)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>23)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 6;
    }
    if(Nrounds>24)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>25)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>26)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>27)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>27)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 7;
    }
    if(Nrounds>28)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>29)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>30)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>31)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>31)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 8;
    }
    if(Nrounds>32)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>33)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>34)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>35)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>35)
    {
        X.v[0] += ks[4]; X.v[1] += ks[0]; X.v[2] += ks[1]; X.v[3] += ks[2]; X.v[WCNT4-1] += 9;
    }
    if(Nrounds>36)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>37)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>38)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>39)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>39)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; X.v[WCNT4-1] += 10;
    }
    if(Nrounds>40)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>41)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>42)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>43)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>43)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 11;
    }
    if(Nrounds>44)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>45)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>46)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>47)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>47)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 12;
    }
    if(Nrounds>48)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>49)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>50)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>51)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>51)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 13;
    }
    if(Nrounds>52)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>53)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>54)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>55)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>55)
    {
        X.v[0] += ks[4]; X.v[1] += ks[0]; X.v[2] += ks[1]; X.v[3] += ks[2]; X.v[WCNT4-1] += 14;
    }
    if(Nrounds>56)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>57)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>58)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>59)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>59)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; X.v[WCNT4-1] += 15;
    }
    if(Nrounds>60)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>61)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>62)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>63)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>63)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 16;
    }
    if(Nrounds>64)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>65)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>66)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>67)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>67)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 17;
    }
    if(Nrounds>68)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>69)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>70)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>71)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_64(X.v[3],R_64x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_64(X.v[1],R_64x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>71)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 18;
    }
    return X;
}


enum
{
    threefry4x64_rounds = 20
};
inline threefry4x64_ctr_t threefry4x64(threefry4x64_ctr_t in, threefry4x64_key_t k) __attribute__((always_inline)); inline threefry4x64_ctr_t threefry4x64(threefry4x64_ctr_t in, threefry4x64_key_t k)
{
    return threefry4x64_R(threefry4x64_rounds, in, k);
}


typedef struct r123array4x32 threefry4x32_ctr_t; typedef struct r123array4x32 threefry4x32_key_t; typedef struct r123array4x32 threefry4x32_ukey_t; inline threefry4x32_key_t threefry4x32keyinit(threefry4x32_ukey_t uk) { return uk; }
inline threefry4x32_ctr_t threefry4x32_R(unsigned int Nrounds, threefry4x32_ctr_t in, threefry4x32_key_t k) __attribute__((always_inline)); inline threefry4x32_ctr_t threefry4x32_R(unsigned int Nrounds, threefry4x32_ctr_t in, threefry4x32_key_t k)
{
    threefry4x32_ctr_t X; uint32_t ks[WCNT4+1]; int i; ; ks[WCNT4] = 0x1BD11BDA; for (i=0;i < WCNT4; i++)
    {
        ks[i] = k.v[i]; X.v[i] = in.v[i]; ks[WCNT4] ^= k.v[i];
    }
    X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; if(Nrounds>0)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>1)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>2)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>3)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>3)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 1;
    }
    if(Nrounds>4)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>5)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>6)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>7)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>7)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 2;
    }
    if(Nrounds>8)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>9)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>10)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>11)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>11)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 3;
    }
    if(Nrounds>12)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>13)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>14)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>15)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>15)
    {
        X.v[0] += ks[4]; X.v[1] += ks[0]; X.v[2] += ks[1]; X.v[3] += ks[2]; X.v[WCNT4-1] += 4;
    }
    if(Nrounds>16)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>17)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>18)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>19)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>19)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; X.v[WCNT4-1] += 5;
    }
    if(Nrounds>20)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>21)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>22)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>23)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>23)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 6;
    }
    if(Nrounds>24)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>25)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>26)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>27)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>27)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 7;
    }
    if(Nrounds>28)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>29)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>30)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>31)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>31)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 8;
    }
    if(Nrounds>32)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>33)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>34)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>35)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>35)
    {
        X.v[0] += ks[4]; X.v[1] += ks[0]; X.v[2] += ks[1]; X.v[3] += ks[2]; X.v[WCNT4-1] += 9;
    }
    if(Nrounds>36)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>37)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>38)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>39)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>39)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; X.v[WCNT4-1] += 10;
    }
    if(Nrounds>40)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>41)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>42)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>43)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>43)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 11;
    }
    if(Nrounds>44)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>45)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>46)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>47)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>47)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 12;
    }
    if(Nrounds>48)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>49)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>50)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>51)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>51)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 13;
    }
    if(Nrounds>52)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>53)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>54)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>55)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>55)
    {
        X.v[0] += ks[4]; X.v[1] += ks[0]; X.v[2] += ks[1]; X.v[3] += ks[2]; X.v[WCNT4-1] += 14;
    }
    if(Nrounds>56)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>57)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>58)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>59)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>59)
    {
        X.v[0] += ks[0]; X.v[1] += ks[1]; X.v[2] += ks[2]; X.v[3] += ks[3]; X.v[WCNT4-1] += 15;
    }
    if(Nrounds>60)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>61)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>62)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>63)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>63)
    {
        X.v[0] += ks[1]; X.v[1] += ks[2]; X.v[2] += ks[3]; X.v[3] += ks[4]; X.v[WCNT4-1] += 16;
    }
    if(Nrounds>64)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_0_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_0_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>65)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_1_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_1_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>66)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_2_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_2_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>67)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_3_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_3_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>67)
    {
        X.v[0] += ks[2]; X.v[1] += ks[3]; X.v[2] += ks[4]; X.v[3] += ks[0]; X.v[WCNT4-1] += 17;
    }
    if(Nrounds>68)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_4_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_4_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>69)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_5_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_5_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>70)
    {
        X.v[0] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_6_0); X.v[1] ^= X.v[0]; X.v[2] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_6_1); X.v[3] ^= X.v[2];
    }
    if(Nrounds>71)
    {
        X.v[0] += X.v[3]; X.v[3] = RotL_32(X.v[3],R_32x4_7_0); X.v[3] ^= X.v[0]; X.v[2] += X.v[1]; X.v[1] = RotL_32(X.v[1],R_32x4_7_1); X.v[1] ^= X.v[2];
    }
    if(Nrounds>71)
    {
        X.v[0] += ks[3]; X.v[1] += ks[4]; X.v[2] += ks[0]; X.v[3] += ks[1]; X.v[WCNT4-1] += 18;
    }
    return X;
}


enum
{
    threefry4x32_rounds = 20
};
inline threefry4x32_ctr_t threefry4x32(threefry4x32_ctr_t in, threefry4x32_key_t k) __attribute__((always_inline)); inline threefry4x32_ctr_t threefry4x32(threefry4x32_ctr_t in, threefry4x32_key_t k)
{
    return threefry4x32_R(threefry4x32_rounds, in, k);
}


__kernel void test_philox4x32_7(uint64_t n64, philox4x32_ukey_t uk, philox4x32_ctr_t ctrinit, __global philox4x32_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; philox4x32_ctr_t c, v=
    {
        {
            0
        }
    };
    philox4x32_key_t k=philox4x32keyinit(uk); c = ctrinit; if( 7 == philox4x32_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x32_R(philox4x32_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x32_R(7, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_philox4x32_10(uint64_t n64, philox4x32_ukey_t uk, philox4x32_ctr_t ctrinit, __global philox4x32_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; philox4x32_ctr_t c, v=
    {
        {
            0
        }
    };
    philox4x32_key_t k=philox4x32keyinit(uk); c = ctrinit; if( 10 == philox4x32_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x32_R(philox4x32_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x32_R(10, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_philox2x64_6(uint64_t n64, philox2x64_ukey_t uk, philox2x64_ctr_t ctrinit, __global philox2x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; philox2x64_ctr_t c, v=
    {
        {
            0
        }
    };
    philox2x64_key_t k=philox2x64keyinit(uk); c = ctrinit; if( 6 == philox2x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = philox2x64_R(philox2x64_rounds, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = philox2x64_R(6, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_philox2x64_10(uint64_t n64, philox2x64_ukey_t uk, philox2x64_ctr_t ctrinit, __global philox2x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; philox2x64_ctr_t c, v=
    {
        {
            0
        }
    };
    philox2x64_key_t k=philox2x64keyinit(uk); c = ctrinit; if( 10 == philox2x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = philox2x64_R(philox2x64_rounds, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = philox2x64_R(10, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_philox4x64_7(uint64_t n64, philox4x64_ukey_t uk, philox4x64_ctr_t ctrinit, __global philox4x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; philox4x64_ctr_t c, v=
    {
        {
            0
        }
    };
    philox4x64_key_t k=philox4x64keyinit(uk); c = ctrinit; if( 7 == philox4x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x64_R(philox4x64_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x64_R(7, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_philox4x64_10(uint64_t n64, philox4x64_ukey_t uk, philox4x64_ctr_t ctrinit, __global philox4x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; philox4x64_ctr_t c, v=
    {
        {
            0
        }
    };
    philox4x64_key_t k=philox4x64keyinit(uk); c = ctrinit; if( 10 == philox4x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x64_R(philox4x64_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = philox4x64_R(10, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry2x64_13(uint64_t n64, threefry2x64_ukey_t uk, threefry2x64_ctr_t ctrinit, __global threefry2x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry2x64_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry2x64_key_t k=threefry2x64keyinit(uk); c = ctrinit; if( 13 == threefry2x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry2x64_R(threefry2x64_rounds, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry2x64_R(13, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry2x64_20(uint64_t n64, threefry2x64_ukey_t uk, threefry2x64_ctr_t ctrinit, __global threefry2x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry2x64_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry2x64_key_t k=threefry2x64keyinit(uk); c = ctrinit; if( 20 == threefry2x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry2x64_R(threefry2x64_rounds, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry2x64_R(20, c, k); do
            {
                if (2==4) if(!(v.v[2>2?3:0]^v.v[2>2?2:0]^v.v[2>1?1:0]^v.v[0])) ++i; if (2==2) if(!(v.v[2>1?1:0]^v.v[0])) ++i; if (2==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry4x64_12(uint64_t n64, threefry4x64_ukey_t uk, threefry4x64_ctr_t ctrinit, __global threefry4x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry4x64_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry4x64_key_t k=threefry4x64keyinit(uk); c = ctrinit; if( 12 == threefry4x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x64_R(threefry4x64_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x64_R(12, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry4x64_20(uint64_t n64, threefry4x64_ukey_t uk, threefry4x64_ctr_t ctrinit, __global threefry4x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry4x64_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry4x64_key_t k=threefry4x64keyinit(uk); c = ctrinit; if( 20 == threefry4x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x64_R(threefry4x64_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x64_R(20, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry4x32_12(uint64_t n64, threefry4x32_ukey_t uk, threefry4x32_ctr_t ctrinit, __global threefry4x32_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry4x32_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry4x32_key_t k=threefry4x32keyinit(uk); c = ctrinit; if( 12 == threefry4x32_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x32_R(threefry4x32_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x32_R(12, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry4x32_20(uint64_t n64, threefry4x32_ukey_t uk, threefry4x32_ctr_t ctrinit, __global threefry4x32_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry4x32_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry4x32_key_t k=threefry4x32keyinit(uk); c = ctrinit; if( 20 == threefry4x32_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x32_R(threefry4x32_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x32_R(20, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}


__kernel void test_threefry4x64_72(uint64_t n64, threefry4x64_ukey_t uk, threefry4x64_ctr_t ctrinit, __global threefry4x64_ctr_t *ctr)
{
    uint n = (uint)n64; unsigned tid = get_global_id(0); uint i; threefry4x64_ctr_t c, v=
    {
        {
            0
        }
    };
    threefry4x64_key_t k=threefry4x64keyinit(uk); c = ctrinit; if( 72 == threefry4x64_rounds )
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x64_R(threefry4x64_rounds, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    else
    {
        for (i = 0; i < n; ++i)
        {
            v = threefry4x64_R(72, c, k); do
            {
                if (4==4) if(!(v.v[4>2?3:0]^v.v[4>2?2:0]^v.v[4>1?1:0]^v.v[0])) ++i; if (4==2) if(!(v.v[4>1?1:0]^v.v[0])) ++i; if (4==1) if(!(v.v[0])) ++i;
            }
            while(0); c.v[0]++;
        }
    }
    ctr[tid] = v;
}
