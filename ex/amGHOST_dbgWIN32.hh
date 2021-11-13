/** 
 * Based on [https://github.com/blender/blender/blob/master/source/blender/blenlib/intern/system_win32.c]
 *      and [https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64]       (win32 dbghelp.h)
 * 
 * Blender Does StackTrace with   BLI_ASSERT   https://github.com/blender/blender/blob/master/source/blender/blenlib/BLI_assert.h
 *      \see blenlin/intern/BLI_assert.c
 *      \see the txt file in the dir    [From: https://developer.blender.org/T90999]
 *      \see also what they see as StackTrace: https://developer.blender.org/T90248     and    [https://developer.blender.org/T90620]
 */