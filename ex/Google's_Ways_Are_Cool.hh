/**

            ██╗ ██╗  ██╗
           ████████╗███║
           ╚██╔═██╔╝╚██║
           ████████╗ ██║
           ╚██╔═██╔╝ ██║
           ╚═╝ ╚═╝  ╚═╝
             
 * 
 * 
 * 
 * Google's Ways are Pretty COOL
 * PROOF: #1 - Classes.... but without Default-Constructor....




// WUFFS_BASE__TABLE is a 2-dimensional buffer.
//
// width, height and stride measure a number of elements, not necessarily a
// size in bytes.
//
// A value with all fields NULL or zero is a valid, empty table.

#ifndef _MSC_VER
    typedef unsigned long long int size_t;
#endif
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

// ---------------- Slices and Tables

// WUFFS_BASE__SLICE is a 1-dimensional buffer.
//
// len measures a number of elements, not necessarily a size in bytes.
//
// A value with all fields NULL or zero is a valid, empty slice.
#define WUFFS_BASE__SLICE(T) \
  struct {                   \
    T* ptr;                  \
    size_t len;              \
  }

#define WUFFS_BASE__TABLE(T) \
  struct {                   \
    T* ptr;                  \
    size_t width;            \
    size_t height;           \
    size_t stride;           \
  }

typedef WUFFS_BASE__SLICE(uint8_t) wuffs_base__slice_u8;
typedef WUFFS_BASE__SLICE(uint16_t) wuffs_base__slice_u16;
typedef WUFFS_BASE__SLICE(uint32_t) wuffs_base__slice_u32;
typedef WUFFS_BASE__SLICE(uint64_t) wuffs_base__slice_u64;

typedef WUFFS_BASE__TABLE(uint8_t) wuffs_base__table_u8;
typedef WUFFS_BASE__TABLE(uint16_t) wuffs_base__table_u16;
typedef WUFFS_BASE__TABLE(uint32_t) wuffs_base__table_u32;
typedef WUFFS_BASE__TABLE(uint64_t) wuffs_base__table_u64;










typedef struct wuffs_base__pixel_format__struct {
  uint32_t repr;

#ifdef __cplusplus
  inline bool is_valid() const;
  inline uint32_t bits_per_pixel() const;
  inline bool is_direct() const;
  inline bool is_indexed() const;
  inline bool is_interleaved() const;
  inline bool is_planar() const;
  inline uint32_t num_planes() const;
#endif  // __cplusplus

} wuffs_base__pixel_format;

// --------

// wuffs_base__pixel_subsampling encodes whether sample values cover one pixel
// or cover multiple pixels.
//
// See https://github.com/google/wuffs/blob/main/doc/note/pixel-subsampling.md
//
// Do not manipulate its bits directly; they are private implementation
// details. Use methods such as wuffs_base__pixel_subsampling__bias_x instead.
typedef struct wuffs_base__pixel_subsampling__struct {
  uint32_t repr;

#ifdef __cplusplus
  inline uint32_t bias_x(uint32_t plane) const;
  inline uint32_t denominator_x(uint32_t plane) const;
  inline uint32_t bias_y(uint32_t plane) const;
  inline uint32_t denominator_y(uint32_t plane) const;
#endif  // __cplusplus

} wuffs_base__pixel_subsampling;





// --------

// See https://github.com/google/wuffs/blob/main/doc/note/statuses.md
typedef struct wuffs_base__status__struct {
  const char* repr;

#ifdef __cplusplus
  inline bool is_complete() const;
  inline bool is_error() const;
  inline bool is_note() const;
  inline bool is_ok() const;
  inline bool is_suspension() const;
  inline const char* message() const;
#endif  // __cplusplus

} wuffs_base__status;






typedef struct wuffs_base__pixel_config__struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    wuffs_base__pixel_format pixfmt;
    wuffs_base__pixel_subsampling pixsub;
    uint32_t width;
    uint32_t height;
  } private_impl;

#ifdef __cplusplus
  inline void set(uint32_t pixfmt_repr,
                  uint32_t pixsub_repr,
                  uint32_t width,
                  uint32_t height);
  inline void invalidate();
  inline bool is_valid() const;
  inline wuffs_base__pixel_format pixel_format() const;
  inline wuffs_base__pixel_subsampling pixel_subsampling() const;
  inline uint32_t width() const;
  inline uint32_t height() const;
  inline uint64_t pixbuf_len() const;
#endif  // __cplusplus

} wuffs_base__pixel_config;





// ---------------- Images

// wuffs_base__color_u32_argb_premul is an 8 bit per channel premultiplied
// Alpha, Red, Green, Blue color, as a uint32_t value. Its value is always
// 0xAARRGGBB (Alpha most significant, Blue least), regardless of endianness.
typedef uint32_t wuffs_base__color_u32_argb_premul;

// wuffs_base__color_u32_argb_premul__is_valid returns whether c's Red, Green
// and Blue channels are all less than or equal to its Alpha channel. c uses
// premultiplied alpha, so 50% opaque 100% saturated red is 0x7F7F_0000 and a
// value like 0x7F80_0000 is invalid.

typedef struct wuffs_base__pixel_buffer__struct {
  wuffs_base__pixel_config pixcfg;
  int x, y, z;
  int hoga = 0;
  int hofa = 100000;
  int hoga2 = 64;

  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    wuffs_base__table_u8 planes[4];
    // TODO: color spaces.
  } private_impl;

#ifdef __cplusplus
  inline wuffs_base__status set_interleaved(
      const wuffs_base__pixel_config* pixcfg,
      wuffs_base__table_u8 primary_memory,
      wuffs_base__slice_u8 palette_memory);
  inline wuffs_base__status set_from_slice(
      const wuffs_base__pixel_config* pixcfg,
      wuffs_base__slice_u8 pixbuf_memory);
  inline wuffs_base__status set_from_table(
      const wuffs_base__pixel_config* pixcfg,
      wuffs_base__table_u8 primary_memory);
  inline wuffs_base__slice_u8 palette();
  inline wuffs_base__slice_u8 palette_or_else(wuffs_base__slice_u8 fallback);
  inline wuffs_base__pixel_format pixel_format() const;
  inline wuffs_base__table_u8 plane(uint32_t p);
  inline wuffs_base__color_u32_argb_premul color_u32_at(uint32_t x,
                                                        uint32_t y) const;
  inline wuffs_base__status set_color_u32_at(
      uint32_t x,
      uint32_t y,
      wuffs_base__color_u32_argb_premul color);
#endif  // __cplusplus

} wuffs_base__pixel_buffer;




int main(void) {
    wuffs_base__pixel_buffer   _PIX_BUF;

     _PIX_BUF.pixcfg.private_impl.pixfmt.repr = 4;
    uint8_t bytes_per_pixel = 4;
    
    size_t m_size = _PIX_BUF.pixcfg.pixbuf_len();
};

*/