#pragma once

class amGHOST_DEC_PNG {
    public:
    uint8_t *m_ptr = nullptr;
    uint64_t m_size = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;

    amGHOST_DEC_PNG(const char *pngPath);
};




#ifdef amGHOST_V_CPP
/* WUFFs Implementation in amGHOST_V.cpp */
#include "wuffs-v0.3.c"
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



/** uses WUFFs [Wrangling Untrusted File Formats Safely] by GOOGLE */
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

        pngSize = static_cast<uint64_t> (pngFile.tellg()) + 1;    //Works cz of std::ios::ate
        pngData = static_cast<char *> (malloc(pngSize));

        pngData[pngSize-1] = '\0';
        pngFile.seekg(0);
        pngFile.read(pngData, pngSize);
        pngFile.close();

        /** TODO: Add .png extension header check support*/
    } else {amVK_LOG_EX("[param: String] pngPath isn't defined  (pngPath.size() <= 0) "); return;}

    /** ---------- Copied from amVK_Pipeline.cpp <- ---------- */


    /** ---------- INITIALIZE -> ---------- */

    /**
    wuffs_png__decoder__initialize(
        &G_PNG, sizeof(G_PNG),
        WUFFS_VERSION, WUFFS_INITIALIZE__DEFAULT_OPTIONS
    );
    */
    G_PNG->initialize(
        sizeof__wuffs_png__decoder(), 
        WUFFS_VERSION, 
        WUFFS_INITIALIZE__DEFAULT_OPTIONS
    );

    /** ---------- Data & STRUCTS -> ---------- */

/** wuffs_base__image_decoder *_IMG_DEC = wuffs_png__decoder__upcast_as__wuffs_base__image_decoder(&G_PNG); */
/** wuffs_base__image_decoder *_IMG_DEC = G_PNG.upcast_as__wuffs_base__image_decoder(); */
    wuffs_base__image_config   _IMG_CFG;
/** wuffs_base__pixel_config   _IMG_CFG.pixcfg; */
    wuffs_base__io_buffer      _IMG_BUF = {};                       /** Encoded Pixels */
    wuffs_base__pixel_buffer   _PIX_BUF;                      /** Decoded Pixel Buffer */

    _IMG_BUF.data.ptr = reinterpret_cast<uint8_t *> (pngData);
    _IMG_BUF.data.len = static_cast<size_t> (pngSize);
/** _IMG_BUF.meta.ri = 0; */
    _IMG_BUF.meta.wi = pngSize;
/** _IMG_BUF.meta.pos = 0; */
    _IMG_BUF.meta.closed = true;

    /** ---------- Data & STRUCTS <- ---------- */





    /** ---------- IMG_CONFIG -> ---------- */
    /**                                                        We are Skipping on some standard procedures.... (which is using the '_IMG_DEC') */
    G_PNG->decode_image_config(&_IMG_CFG, &_IMG_BUF);                               /** check impl     wuffs_png__decoder__decode_image_config */

    /** ---------- DestinationBuffer -> ---------- */
    _PIX_BUF.pixcfg = _IMG_CFG.pixcfg;
    _PIX_BUF.pixcfg.private_impl.pixfmt.repr = WUFFS_BASE__PIXEL_FORMAT__RGBX;      /**   see \fn wuffs_base__pixel_swizzler__prepare__rgb()   */
    
    m_size = _PIX_BUF.pixcfg.pixbuf_len();                                          /**                these are member variables              */
    m_ptr = static_cast<uint8_t *> (malloc(m_size));
    m_width = _IMG_CFG.pixcfg.private_impl.width;
    m_height = _IMG_CFG.pixcfg.private_impl.height;

    wuffs_base__table_u8* tab = &_PIX_BUF.private_impl.planes[0];
    tab->ptr = m_ptr;                                                               /**  IMPL: \fn wuffs_base__pixel_buffer__set_from_slice()  */
    tab->width  = m_width;                                                          /**          we need the last part from there....          */
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
#endif