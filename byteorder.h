
#define ___constant_swab16(x) ((uint16_t)(                         \
  (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |                  \
  (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define ___constant_swab32(x) ((uint32_t)(                         \
  (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |            \
  (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |            \
  (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |            \
  (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))


#if __BYTE_ORDER == __LITTLE_ENDIAN

#define cpu_to_le32(x) ((uint32_t)(x))
#define le32_to_cpu(x) ((uint32_t)(x))
#define cpu_to_le16(x) ((uint16_t)(x))
#define le16_to_cpu(x) ((uint16_t)(x))

#define cpu_to_be32(x) (__swab32((x)))
#define be32_to_cpu(x) __swab32((uint32_t)(x))
#define cpu_to_be16(x) (__swab16((x)))
#define be16_to_cpu(x) __swab16((uint16_t)(x))

#else

#define cpu_to_le32(x) (__swab32((x)))
#define le32_to_cpu(x) __swab32((uint32_t)(x))
#define cpu_to_le16(x) (__swab16((x)))
#define le16_to_cpu(x) __swab16((uint16_t)(x))

#define cpu_to_be32(x) ((uint32_t)(x))
#define be32_to_cpu(x) ((uint32_t)(x))
#define cpu_to_be16(x) ((uint16_t)(x))
#define be16_to_cpu(x) ((uint16_t)(x))

#endif
