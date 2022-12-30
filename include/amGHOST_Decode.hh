#pragma once

#define REY_InYourMind     amGHOST_DEC_PNG
#define amGHOST_DECODER_WUFFS  0
#define amGHOST_DECODER_STBI   2
#define amGHOST_DECODER_SPNG   3

/**
 * Only Supports RGBA for now... 
 * + We Prioratize WUFFs... [Wrangling Untrusted File Formats Safely] by GOOGLE
 * 
 * TODO: add RGB final Support
 * TODO: Check the full functionality of WUFFs.... see if you can replace miniz
 * TODO: I dont wanna keep adding SPNG in CMakeLists.... not ideal not ideal...
 *       First solution I could think of was amGHOST_V.c    and include sPNG.c inside that
 *       But actually single files like wuffs.c & stbi_image.h is a hell lot better idea
 *       in those you dont gotta care about external shits.... or even MAKE-FILE Implementation
 */
class amGHOST_DEC_PNG {
    public:
    uint8_t *m_ptr = nullptr;
    uint64_t m_size;
    uint32_t m_width;
    uint32_t m_height;

    static bool INITIALIZE_WUFFs(void);
    /** CONSTRUCTOR-1: Hardcoded uses WUFFs.... by Google 😜 */
    amGHOST_DEC_PNG(const char *pngPath);
    /** CONSTRUCTOR-2: You can choose STB/WUFFs.... more to be added soon */
    amGHOST_DEC_PNG(const char *pngPath, uint8_t DEC) {
        switch (DEC)
        {
            case amGHOST_DECODER_WUFFS:
                *this = amGHOST_DEC_PNG(pngPath);   /** I know this is not that Optimized.... but I just wanted to have the above COSTRUCTOR... as it is */
            case amGHOST_DECODER_STBI:
                decode_stb(pngPath);
            case amGHOST_DECODER_SPNG:
                decode_spng(pngPath);
        };
    }

    bool decode_stb(const char *pngPath);
    bool decode_spng(const char *pngPath);
};




#ifdef amGHOST_V_CPP


/* WUFFs Implementation in amGHOST_V.cpp */
#include "extern/decode/wuffs-v0.3.c"
#include "amVK_Logger.hh"
#include <string>
#include <fstream>

/**
 * TODO: option to Store notDecoded png data too...
 * TODO: amGHOST_File Opening Utility
 * TODO: Separate Contents in this functions
 * TODO: RETURN PATHS
 * 
 * as of wuffs-v0.3 ..... it seems that people uses 'wuffs_png__decoder__alloc' and the CONSTRUCTOR is deleted for this
 *                      also if WUFFS_IMPLEMENTATION is not defined.... theres a deadWeight of 123MB declared... inside
 */
static inline wuffs_png__decoder *G_PNG = wuffs_png__decoder__alloc();
bool amGHOST_DEC_PNG::INITIALIZE_WUFFs(void) {
    /** ---------- INITIALIZE -> ---------- */

    /**
    wuffs_png__decoder__initialize(
        &G_PNG, sizeof(G_PNG),
        WUFFS_VERSION, WUFFS_INITIALIZE__DEFAULT_OPTIONS
    );
    */
    wuffs_base__status res = G_PNG->initialize(
        sizeof__wuffs_png__decoder(), 
        WUFFS_VERSION, 
        WUFFS_INITIALIZE__DEFAULT_OPTIONS
    );
    if (res.repr) {amVK_LOG("wuffs initialization error: " << res.repr); return false;}
}



/** 
 * uses WUFFs [Wrangling Untrusted File Formats Safely] by GOOGLE 
 *      Faster than sPNG, WUFFs is like 🚄
 */
amGHOST_DEC_PNG::amGHOST_DEC_PNG(const char *pngPath) {
    /** ---------- Copied from amVK_Pipeline.cpp -> ---------- */
    uint64_t pngSize; char *pngData;

    //READ
    if (strlen(pngPath) > 0) {
        /** open the file. With cursor at the end     TODO: Erorr Checking in DEBUG */
        std::ifstream pngFile(pngPath, std::ios::ate | std::ios::binary);

        if (!pngFile.is_open()) {
            amVK_LOG_EX("FAILED to OPEN FILE: " << pngPath);
            return;
        }

        pngSize = static_cast<uint64_t> (pngFile.tellg()) + 1;    // Works cz of std::ios::ate
        pngData = static_cast<char *> (malloc(pngSize));

        pngData[pngSize-1] = '\0';
        pngFile.seekg(0);
        pngFile.read(pngData, pngSize);
        pngFile.close();

        /** TODO: Add .png extension header check support*/
    } else {amVK_LOG_EX("[param: String] pngPath isn't defined  (pngPath.size() <= 0) "); return;}

    /** ---------- Copied from amVK_Pipeline.cpp <- ---------- */


    

    


    /** ---------- Data & STRUCTS -> ---------- */

/** wuffs_base__image_decoder *_IMG_DEC = wuffs_png__decoder__upcast_as__wuffs_base__image_decoder(&G_PNG); */
/** wuffs_base__image_decoder *_IMG_DEC = G_PNG.upcast_as__wuffs_base__image_decoder(); */
    wuffs_base__image_config   _IMG_CFG;
/** wuffs_base__pixel_config   _IMG_CFG.pixcfg; */
    wuffs_base__io_buffer      _IMG_BUF;                    /** Encoded Pixels */   /**     Zero-Initialization takes 8 CPU Instructions....   */
    wuffs_base__pixel_buffer   _PIX_BUF;              /** Decoded Pixel Buffer */   /**           Dont need this Zero-Initialized....          */
                                                /** Default-Instructor won't be called.... czzz it doesn't have anything to get initialized....*/
                                          /** \see ex/Google's_Ways_Are_Cool.hh::#1     ---  Run that in GodBolt */

    _IMG_BUF.data.ptr = reinterpret_cast<uint8_t *> (pngData);                      /**  These gets compiled into pretty 1 CPU Instruction.... */
    _IMG_BUF.data.len = static_cast<size_t> (pngSize);                              /**    Alternatively,   \fn 'wuffs_base__ptr_u8__reader()' */
    _IMG_BUF.meta.wi  = static_cast<size_t> (pngSize);
    _IMG_BUF.meta.ri  = 0;
    _IMG_BUF.meta.pos = 0;
    _IMG_BUF.meta.closed = true;                                                    /**  All Elements of _IMG_BUF & members of members covered */

    /** ---------- Data & STRUCTS <- ---------- */







    /** ---------- IMG_CONFIG -> ---------- */
    /**                                                        We are Skipping on some standard procedures.... (which is using the '_IMG_DEC') */
    G_PNG->decode_image_config(&_IMG_CFG, &_IMG_BUF);                               /** check impl     wuffs_png__decoder__decode_image_config */

    /** ---------- DestinationBuffer -> ---------- */
    _PIX_BUF.pixcfg = _IMG_CFG.pixcfg;
    _PIX_BUF.pixcfg.private_impl.pixfmt.repr = WUFFS_BASE__PIXEL_FORMAT__RGBX;      /**   see \fn wuffs_base__pixel_swizzler__prepare__rgb()   */
    uint8_t bytes_per_pixel = 4;                                                    /**        8-bit per R/G/B/X channel, 4 channels-RGBX      */
    
    /** ---------- Member Variables -> ---------- */
    m_size = _PIX_BUF.pixcfg.pixbuf_len();
    m_ptr = static_cast<uint8_t *> (malloc(m_size));
    m_width = _IMG_CFG.pixcfg.private_impl.width;
    m_height = _IMG_CFG.pixcfg.private_impl.height;

    /** ---------- MemTable of PIX_BUF -> ---------- */
    wuffs_base__table_u8* tab = &_PIX_BUF.private_impl.planes[0];                   /**          Maybe use the  'plane()' function...          */
    tab->ptr = m_ptr;                                                               /**  IMPL: \fn wuffs_base__pixel_buffer__set_from_slice()  */
    tab->width  = m_width * bytes_per_pixel;                                        /**          we need the last part from there....          */
    tab->height = m_height;                                                         /** these're set assuming, PIX_FMT is not gonna be indexed */
    tab->stride = tab->width;                                                       /**                 So this works for PNG                  */

    /** ---------- WorkBuffer -> ---------- */
    size_t _WRK_LEN = G_PNG->workbuf_len().max_incl;  /** check 'f_overall_workbuf_length' inside function 'wuffs_png__decoder__workbuf_len()' */
    wuffs_base__slice_u8 _WRK_BUF = {static_cast<uint8_t *> (malloc(_WRK_LEN)), _WRK_LEN};







    /** ---------- DECODE IMAGE (FINALLY !!!!) -> ---------- */

    wuffs_base__status res = G_PNG->decode_frame(
        &_PIX_BUF, &_IMG_BUF,
        WUFFS_BASE__PIXEL_BLEND__SRC, /** other option: SRC_OVER.... means to ONLY change NOT-TRANSPARENT Pixels or change new data if PNG-Animation */
        _WRK_BUF,
        nullptr
    );
    if (res.repr) amVK_LOG(res.repr);

    /** ---------- DECODE IMAGE (FINALLY !!!!) <- ---------- */
};

























#define STB_IMAGE_IMPLEMENTATION
#include "extern/decode/stb_image.h"

/**
 * Well, STBI is pretty straight-forward ⏩ 
 * \also \see STBI_FAILURE_USERMSG 
 */
bool amGHOST_DEC_PNG::decode_stb(const char *pngPath) {
        /** STBI calls it 'comp' if RGBA... 'components number' will be 4, 3 if RGB, 1 if only Grayscale i guess.... 
         *    [What the Image was originally saved at... not the one that we 'Desire'] */
    int OG_channels_per_pixel;

        /** 'STBI_rgb_alpha': Desired_Channels */
    m_ptr = stbi_load(pngPath, (int*)&m_width, (int*)&m_height, &OG_channels_per_pixel, STBI_rgb_alpha);
    if (!m_ptr) {
        amVK_LOG("stbi Error: " << stbi_failure_reason());
        amVK_LOG_EX("stbi_load couldn't find/load/read image: " << pngPath);
        return false; 
    }

    m_size = m_height * m_width * 4;
    return true;
}


#define SPNG_STATIC
#include "extern/decode/spng.h"

/**
 * Well, sPNG is pretty fast 🏎️
 */
bool amGHOST_DEC_PNG::decode_spng(const char *pngPath) {
    /** ---------- Copied from amVK_Pipeline.cpp -> ---------- */
    uint64_t pngSize; char *pngData;

    //READ
    if (strlen(pngPath) > 0) {
        /** open the file. With cursor at the end     TODO: Erorr Checking in DEBUG */
        std::ifstream pngFile(pngPath, std::ios::ate | std::ios::binary);

        if (!pngFile.is_open()) {
            amVK_LOG_EX("FAILED to OPEN FILE: " << pngPath);
            return false;
        }

        pngSize = static_cast<uint64_t> (pngFile.tellg()) + 1;    // Works cz of std::ios::ate
        pngData = static_cast<char *> (malloc(pngSize));

        pngData[pngSize-1] = '\0';
        pngFile.seekg(0);
        pngFile.read(pngData, pngSize);
        pngFile.close();

        /** TODO: Add .png extension header check support*/
    } else {amVK_LOG_EX("[param: String] pngPath isn't defined  (pngPath.size() <= 0) "); return false;}

    /** ---------- Copied from amVK_Pipeline.cpp <- ---------- */




    /* Create a context */
    spng_ctx *ctx = spng_ctx_new(0);
    if (!ctx) {amVK_LOG_EX("[amGHOST] Couldn't Create sPNG Context...."); return false;}

    /* Set an input buffer */
    int err = spng_set_png_buffer(ctx, pngData, pngSize);
    if (err) {amVK_LOG_EX("spng_set_png_buffer: " << spng_strerror(err));}

    /* Determine output image size */
    err = spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &m_size);
    if (err) {amVK_LOG_EX("spng_decoded_image_size: " << spng_strerror(err));}

    spng_ihdr ihdr;
    spng_get_ihdr(ctx, &ihdr);
    m_width  = ihdr.width;
    m_height = ihdr.height;
    m_ptr = static_cast<uint8_t *> (malloc(m_size));

    /* Decode to 8-bit RGBA */
    err = spng_decode_image(ctx, m_ptr, m_size, SPNG_FMT_RGBA8, 0);
    if (err) {amVK_LOG_EX("spng_decode_image: " << spng_strerror(err));}

    /* Free context memory */
    spng_ctx_free(ctx);

    return true;
}

#endif