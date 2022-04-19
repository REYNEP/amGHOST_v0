# Currently only ninja backend works.... cz of 'defs_post' [cpp-args]... and IDK... ninja seems good
# TODO: Add support for OpenGL stuffs too... which I can't cz.... glew needs a meson build system too.... otherwise going furthur wouldn't make much of sense

# REF: https://mesonbuild.com/Reference-manual_functions.html#project
# 0.53.0: fs
# 0.54.0: message with multiple params
project(
    'amGHOST_V0.1beta', 'cpp',
    meson_version: '>=0.54.0',
    version: '0.1beta',
    default_options: ['c_std=c17', 'cpp_std=c++17']
)

# REF: https://github.com/pitivi/pitivi/blob/4b4ac33b2f79d0c8bd1bd49937d991334843ce7c/meson.build#L2
# host_system = host_machine.system()
current_source_dir = meson.current_source_dir()
fs = import('fs')

# TODO Add support for is_REL_EXTERN
summary(
    {
        '[option] EXTERN': get_option('EXTERN'),
        '[option] is_REL_EXTERN': get_option('is_REL_EXTERN'),
        '[option] amGHOST_VULKAN_WindowSurface': get_option('amGHOST_VULKAN_WindowSurface')
    },
    section: 'extern OPTIONS....'
)


# EXTERN
is_REL_EXTERN = get_option('is_REL_EXTERN')
EXTERN = get_option('EXTERN')

    if EXTERN[0] == '/' and not fs.is_dir(EXTERN)

            warning('check [option] EXTERN... trying with removing the first \'/\'')
        _xD = EXTERN.substring(1)

        if (fs.is_dir(_xD))
            EXTERN = _xD
            warning('check [option] EXTERN... remove the first \'/\'')
        endif

    endif
    if (not fs.isdir(EXTERN))
        warning('[option] EXTERN set a dir that doesnt exist')
    endif
# EXTERN Section END


# VULKAN
    amGHOST_VULKAN_WindowSurface = get_option('amGHOST_VULKAN_WindowSurface')
    VAR_VULKAN_DIR = get_option('VAR_VULKAN_DIR')
    VAR_VULKAN_INC = ''
    _is_VULKAN_DIR = false

    VAR_VULKAN_LIB = ''
    VAR_VULKAN_LIB_HINTS = get_option('VAR_VULKAN_LIB_HINTS')


        # if '/path/to/dir' instead of 'path/to/dir'
        if VAR_VULKAN_DIR[0] == '/' and not fs.is_dir(VAR_VULKAN_DIR)

                warning('check [option] VAR_VULKAN_DIR... trying with removing the first \'/\'')
            _xD = VAR_VULKAN_DIR.substring(1)

            if (fs.is_dir(_xD))
                VAR_VULKAN_DIR = _xD
                warning('check [option] VAR_VULKAN_DIR... remove the first \'/\'')
            endif

        endif

        if (is_REL_EXTERN)
            if (fs.isdir(EXTERN / VAR_VULKAN_DIR))
                VAR_VULKAN_DIR = EXTERN / VAR_VULKAN_DIR
            endif
        endif


    # The actual Stuffs
    if fs.is_dir(VAR_VULKAN_DIR)
        # Find_File( vulkan/vulkan.h )
        foreach _dir : [VAR_VULKAN_DIR / 'include', VAR_VULKAN_DIR]

            if fs.is_file(_dir / 'vulkan/vulkan.h')
                VAR_VULKAN_INC = _dir
                break
            endif

        endforeach
        
        _is_VULKAN_DIR = true

        # Find_File( vulkan-1.lib)
        N = ['vulkan-1.lib', 'vulkan.lib']
        foreach _N : N
            foreach _dir : VAR_VULKAN_LIB_HINTS.split(';')

                # String PATH Building: https://mesonbuild.com/Syntax.html#string-path-building
                _xD = VAR_VULKAN_DIR / _dir / _N
                if fs.is_file(_xD)
                    VAR_VULKAN_LIB = _xD
                    break
                endif

            endforeach

            if VAR_VULKAN_LIB != ''
                break
            endif

        endforeach
    endif

    summary(
        {
            '[option] amGHOST_VULKAN_WindowSurface': get_option('amGHOST_VULKAN_WindowSurface'),
            '[option] VAR_VULKAN_DIR': get_option('VAR_VULKAN_DIR'),
            ' actual  VAR_VULKAN_DIR': VAR_VULKAN_DIR,
            '         _is_VULKAN_DIR': _is_VULKAN_DIR,
            '         VAR_VULKAN_INC': VAR_VULKAN_INC + '   (has vulkan/vulkan.h)',
            '         VAR_VULKAN_LIB': VAR_VULKAN_LIB
        },
        section: 'VkSurfaceKHR (Vulkan Window Surface)'
    )

    if amGHOST_VULKAN_WindowSurface and (not _is_VULKAN_DIR == true or VAR_VULKAN_INC == '' or VAR_VULKAN_LIB == '')
        warning('couldn\'t find vulkan/vulkan.h   or   vulkan-1.lib (or vulkan.lib)')
        warning('')
        warning('check VkSurfaceKHR (Vulkan Window Surface) options.... [VAR_VULKAN_DIR etc.]')
    endif
# VULKAN Section END













summary(
    {
        '[option] amGHOST_CPP_ONLY': get_option('amGHOST_CPP_ONLY'),
        '[option] amGHOST_USE_TSuccess': get_option('amGHOST_USE_TSuccess'),
        '[option] amVK_LoggerNUtis': get_option('amVK_LoggerNUtils'),
        '[option] amGHOST_V': get_option('amGHOST_V')
    },
    section: 'GENERAL OPTIONS....'
)



src = [
    'intern/amGHOST_System.cpp'
]
inc = [
    '.',
    'intern/'
]
def = []
lib = []

if (host_machine.system() == 'windows')
    src += [
        'intern/amGHOST_WindowWIN32.cpp',
        'intern/amGHOST_SystemWIN32.cpp'
    ]
endif

# There's a Section for finding VULKAN stuffs above....
if (get_option('amGHOST_VULKAN_WindowSurface') == true)
    def += 'amGHOST_BUILD_VULKAN'
    inc += VAR_VULKAN_INC
    lib += VAR_VULKAN_LIB
endif

if (get_option('amGHOST_CPP_ONLY') == true)
    src += 'intern/amGHOST_Capi.cpp'
else
    def += 'amGHOST_BUILD_CPP_ONLY'
endif

if (get_option('amGHOST_USE_TSuccess') == true)
    def += 'amGHOST_BUILD_TSuccess'
endif

if (get_option('amGHOST_V') == false)
    def += 'amGHOST_V_DISABLED'
endif


# amVK_LoggerNUtils
    amVK_LoggerNUtils = get_option('amVK_LoggerNUtils')

    if fs.is_dir(amVK_LoggerNUtils)
        if fs.is_file(amVK_LoggerNUtils / 'amVK_Logger.hh')
            inc += amVK_LoggerNUtils
        else
            warning('[option] amVK_LoggerNUtils, is set. But the directory doesnt have amVK_Logger.hh')
        endif
    else
        warning('check [option] amVK_LoggerNUtils...')
    endif
# amVK_LoggerNUtils Section END


defs_post = []
foreach _def : def
    defs_post += ('-D' + _def)  #right-side is str
endforeach



#meson adds 'lib-' (e.g. lib-amGHOST) by default
#static_library('amGHOST', sources : src) also works....
static_library('amGHOST', src, include_directories: inc, cpp_args: defs_post)