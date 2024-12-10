import os

from FindPython import FindPython

py = FindPython()

DEVKITPRO = os.environ['DEVKITPRO']

BF_PYTHON_ABI_FLAGS = py['ABI_FLAGS']
BF_PYTHON = py['PYTHON']
BF_PYTHON_LIBPATH = py['LIBPATH']
BF_PYTHON_LIBPATH_ARCH = py['LIBPATH_ARCH']
BF_PYTHON_CONFIG = py['CONFIG']
BF_PYTHON_VERSION = py['VERSION']
WITH_BF_STATICPYTHON = False
BF_PYTHON_INC = '#intern/python2.7-sup/ ${BF_PYTHON}/include/python${BF_PYTHON_VERSION}${BF_PYTHON_ABI_FLAGS} ' + BF_PYTHON_CONFIG
BF_PYTHON_BINARY = '${BF_PYTHON}/bin/python${BF_PYTHON_VERSION}'
BF_PYTHON_LIB = 'python${BF_PYTHON_VERSION}${BF_PYTHON_ABI_FLAGS}'
BF_PYTHON_LINKFLAGS = []
BF_PYTHON_LIB_STATIC = '${BF_PYTHON_LIBPATH_ARCH}/libpython${BF_PYTHON_VERSION}${BF_PYTHON_ABI_FLAGS}.a'

WITH_BF_OPENAL = False
WITH_BF_STATICOPENAL = False
BF_OPENAL = DEVKITPRO+'/portlibs/wii'
BF_OPENAL_INC = '${BF_OPENAL}/include'
BF_OPENAL_LIB = 'openal SDL2'

BF_CXX = DEVKITPRO+'/devkitPPC/powerpc-eabi'
WITH_BF_STATICCXX = False

WITH_BF_AUDASPACE = True

WITH_BF_JACK = False

WITH_BF_SNDFILE = True
BF_SNDFILE = DEVKITPRO+'/portlibs/wii'
BF_SNDFILE_INC = '${BF_SNDFILE}/include/sndfile'
BF_SNDFILE_LIB = 'sndfile opus vorbis vorbisenc FLAC ogg'
BF_SNDFILE_LIBPATH = '${BF_SNDFILE}/lib'

WITH_BF_SDL = True
WITH_GHOST_SDL = True
BF_SDL = DEVKITPRO+'/portlibs/wii' #$(shell sdl-config --prefix)
BF_SDL_INC = '${BF_SDL}/include/SDL2' #$(shell $(BF_SDL)/bin/sdl-config --cflags)
BF_SDL_LIB = 'SDL2' #BF_SDL #$(shell $(BF_SDL)/bin/sdl-config --libs) -lSDL_mixer

WITH_BF_OPENEXR = False
WITH_BF_STATICOPENEXR = False

WITH_BF_DDS = True

WITH_BF_JPEG = True
BF_JPEG = DEVKITPRO+'/portlibs/ppc'
BF_JPEG_INC = '${BF_JPEG}/include'
BF_JPEG_LIB = 'jpeg'

WITH_BF_PNG = True
BF_PNG = DEVKITPRO+'/portlibs/ppc'
BF_PNG_INC = '${BF_PNG}/include'
BF_PNG_LIB = 'png'

WITH_BF_TIFF = False

WITH_BF_ZLIB = True
BF_ZLIB = DEVKITPRO+'/portlibs/ppc'
BF_ZLIB_INC = '${BF_ZLIB}/include'
BF_ZLIB_LIB = 'z'

WITH_BF_INTERNATIONAL = False

WITH_BF_GAMEENGINE = True
WITH_BF_PLAYER = True
WITH_BF_NOBLENDER = True

WITH_BF_BULLET = True
BF_BULLET = '#extern/bullet2/src'
BF_BULLET_INC = '${BF_BULLET}'
BF_BULLET_LIB = 'extern_bullet'

WITH_BF_ELTOPO = False

BF_FREETYPE = DEVKITPRO+'/portlibs/ppc'
BF_FREETYPE_INC = '${BF_FREETYPE}/include ${BF_FREETYPE}/include/freetype2'
BF_FREETYPE_LIB = 'freetype'
#BF_FREETYPE_LIB_STATIC = '${BF_FREETYPE}/lib/libfreetype.a'

WITH_BF_ICONV = False

WITH_BF_BINRELOC = False

# enable ffmpeg  support
WITH_BF_FFMPEG = False

# enable ogg, vorbis and theora in ffmpeg
WITH_BF_OGG = True
BF_OGG = DEVKITPRO+'/portslibs/ppc'
BF_OGG_INC = '${BF_OGG}/include'
BF_OGG_LIB = 'ogg vorbis vorbisenc theoraenc theoradec'
BF_OGG_LIBPATH='${BF_OGG}/lib'

WITH_BF_OPENJPEG = True
BF_OPENJPEG = '#extern/libopenjpeg'
BF_OPENJPEG_LIB = ''
BF_OPENJPEG_INC = '${BF_OPENJPEG}'
BF_OPENJPEG_LIBPATH='${BF_OPENJPEG}/lib'

WITH_BF_FFTW3 = False
WITH_BF_STATICFFTW3 = False

WITH_BF_REDCODE = False  

# Mesa Libs should go here if you're using them as well....
WITH_BF_GLEW_ES = True
WITH_BF_STATICOPENGL = False
BF_OPENGL = DEVKITPRO+'/portlibs/wii'
BF_OPENGL_INC = '${BF_OPENGL}/include'
BF_OPENGL_LIB = 'SDL2 aesnd wiiuse bte wiikeyboard opengx GLU ogc fat'
BF_OPENGL_LIBPATH = DEVKITPRO+'/portlibs/wii/lib'

WITH_BF_COLLADA = False
BF_COLLADA = '#source/blender/collada'
BF_COLLADA_INC = '${BF_COLLADA}'
BF_COLLADA_LIB = 'bf_collada'
BF_PCRE = ''
BF_PCRE_LIB = 'pcre'
BF_PCRE_LIBPATH = DEVKITPRO+'/lib'
BF_EXPAT = DEVKITPRO+''
BF_EXPAT_LIB = 'expat'
BF_EXPAT_LIBPATH = DEVKITPRO+'/lib'

WITH_BF_JEMALLOC = False
WITH_BF_STATICJEMALLOC = False

WITH_BF_OIIO = False
WITH_BF_STATICOIIO = False

WITH_BF_OCIO = False
WITH_BF_STATICOCIO = False

WITH_BF_BOOST = False
WITH_BF_STATICBOOST = False

WITH_BF_CYCLES = False

WITH_BF_CYCLES_CUDA_BINARIES = False

WITH_BF_OPENMP = False

#Ray trace optimization
WITH_BF_RAYOPTIMIZATION = False
BF_RAYOPTIMIZATION_SSE_FLAGS = []

#SpaceNavigator and friends
WITH_BF_3DMOUSE = False
WITH_BF_STATIC3DMOUSE = False

#Freestyle
WITH_BF_FREESTYLE = False

WITH_BF_OPENSUBDIV = False
WITH_BF_STATICOPENSUBDIV = False

WITH_BF_BOOLEAN=False
WITH_BF_REMESH=False

##
CC = 'powerpc-eabi-gcc'
CXX = 'powerpc-eabi-g++'

CCFLAGS = ['-pipe', '-mrvl','-funsigned-char','-fno-strict-aliasing', '-D_LARGEFILE_SOURCE', '-D_FILE_OFFSET_BITS=64','-D_LARGEFILE64_SOURCE', '-Iintern/ogc-sup', '-I'+DEVKITPRO+'/portlibs/wii/SDL2', '-I'+DEVKITPRO+'/portlibs/wii/include', '-I'+DEVKITPRO+'/portlibs/ppc/include', '-I'+DEVKITPRO+'/libogc/include/ogc', '-I'+DEVKITPRO+'/libogc/include', '-L'+DEVKITPRO+'/portlibs/wii/lib', '-L'+DEVKITPRO+'/portlibs/ppc/lib', '-L'+DEVKITPRO+'/libogc/lib/wii']
CFLAGS = ['-std=gnu89']
CXXFLAGS = []

CPPFLAGS = []
# g++ 4.6, only needed for bullet
CXXFLAGS += ['-fpermissive']
if WITH_BF_FFMPEG:
    # libavutil needs UINT64_C()
    CXXFLAGS += ['-D__STDC_CONSTANT_MACROS', ]
REL_CFLAGS = []
REL_CXXFLAGS = []
REL_CCFLAGS = ['-Os']

C_WARN = ['-Wno-char-subscripts', '-Wdeclaration-after-statement', '-Wunused-parameter', '-Wstrict-prototypes', '-Werror=declaration-after-statement', '-Werror=implicit-function-declaration', '-Werror=return-type']
CC_WARN = ['-Wall']
CXX_WARN = ['-Wno-invalid-offsetof', '-Wno-sign-compare']

LLIBS = ['util', 'bz2', 'ogc', 'm']
LINKFLAGS = ['-mrvl', '-L'+DEVKITPRO+'/portlibs/wii/lib', '-L'+DEVKITPRO+'/portlibs/ppc/lib', '-L'+DEVKITPRO+'/libogc/lib/wii']

BF_PROFILE = False
BF_PROFILE_CCFLAGS = ['-pg','-g']
BF_PROFILE_LINKFLAGS = ['-pg']

BF_DEBUG = False
BF_DEBUG_CCFLAGS = ['-g']

BF_BUILDDIR = '../build/wii'
BF_INSTALLDIR='../install/wii'

PLATFORM_LINKFLAGS = ['-mrvl', '-L'+DEVKITPRO+'/portlibs/wii/lib', '-L'+DEVKITPRO+'/portlibs/ppc/lib', '-L'+DEVKITPRO+'/libogc/lib/wii']
