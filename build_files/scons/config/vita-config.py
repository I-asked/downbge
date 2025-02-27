from FindPython import FindPython

py = FindPython()

BF_PYTHON_ABI_FLAGS = py['ABI_FLAGS']
BF_PYTHON = py['PYTHON']
BF_PYTHON_LIBPATH = py['LIBPATH']
BF_PYTHON_LIBPATH_ARCH = py['LIBPATH_ARCH']
BF_PYTHON_CONFIG = py['CONFIG']
BF_PYTHON_VERSION = py['VERSION']
WITH_BF_STATICPYTHON = False
BF_PYTHON_INC = '#python2.7/ ${BF_PYTHON}/include/python${BF_PYTHON_VERSION}${BF_PYTHON_ABI_FLAGS} ' + BF_PYTHON_CONFIG
BF_PYTHON_BINARY = '${BF_PYTHON}/bin/python${BF_PYTHON_VERSION}'
BF_PYTHON_LIB = 'python${BF_PYTHON_VERSION}${BF_PYTHON_ABI_FLAGS}'  # BF_PYTHON+'/lib/python'+BF_PYTHON_VERSION+'/config/libpython'+BF_PYTHON_VERSION+'.a'
BF_PYTHON_LINKFLAGS = []
BF_PYTHON_LIB_STATIC = '${BF_PYTHON_LIBPATH_ARCH}/libpython${BF_PYTHON_VERSION}${BF_PYTHON_ABI_FLAGS}.a'

WITH_BF_OPENAL = True
WITH_BF_STATICOPENAL = False
BF_OPENAL = '/opt/vitasdk/arm-vita-eabi'
BF_OPENAL_INC = '${BF_OPENAL}/include'
BF_OPENAL_LIB = 'openal'
BF_OPENAL_LIB_STATIC = '${BF_OPENAL}/lib/libopenal.a'

BF_CXX = '/opt/vitasdk/arm-vita-eabi'
WITH_BF_STATICCXX = False
BF_CXX_LIB_STATIC = '${BF_CXX}/lib/libstdc++.a'

WITH_BF_AUDASPACE = True

WITH_BF_JACK = False
BF_JACK = '/opt/vitasdk/arm-vita-eabi'
BF_JACK_INC = '${BF_JACK}/include/jack'
BF_JACK_LIB = 'jack'
BF_JACK_LIBPATH = '${BF_JACK}/lib'

WITH_BF_SNDFILE = False
WITH_BF_STATICSNDFILE = False
BF_SNDFILE = '/opt/vitasdk/arm-vita-eabi'
BF_SNDFILE_INC = '${BF_SNDFILE}/include/sndfile'
BF_SNDFILE_LIB = 'sndfile'
BF_SNDFILE_LIBPATH = '${BF_SNDFILE}/lib'
BF_SNDFILE_LIB_STATIC = '${BF_SNDFILE}/lib/libsndfile.a ${BF_OGG}/lib/libvorbis.a ${BF_OGG}/lib/libFLAC.a ${BF_OGG}/lib/libvorbisenc.a ${BF_OGG}/lib/libogg.a'

WITH_BF_SDL = True
BF_SDL = '/opt/vitasdk/arm-vita-eabi' #$(shell sdl-config --prefix)
BF_SDL_INC = '${BF_SDL}/include/SDL2' #$(shell $(BF_SDL)/bin/sdl-config --cflags)
BF_SDL_LIB = 'SDL2' #BF_SDL #$(shell $(BF_SDL)/bin/sdl-config --libs) -lSDL_mixer

WITH_BF_OPENEXR = False
WITH_BF_STATICOPENEXR = False
BF_OPENEXR = '/opt/vitasdk/arm-vita-eabi'
# when compiling with your own openexr lib you might need to set...
# BF_OPENEXR_INC = '${BF_OPENEXR}/include/OpenEXR ${BF_OPENEXR}/include'

BF_OPENEXR_INC = '${BF_OPENEXR}/include/OpenEXR'
BF_OPENEXR_LIB = 'Half IlmImf Iex Imath '
BF_OPENEXR_LIB_STATIC = '${BF_OPENEXR}/lib/libHalf.a ${BF_OPENEXR}/lib/libIlmImf.a ${BF_OPENEXR}/lib/libIex.a ${BF_OPENEXR}/lib/libImath.a ${BF_OPENEXR}/lib/libIlmThread.a'
# BF_OPENEXR_LIBPATH = '${BF_OPENEXR}/lib'


WITH_BF_DDS = False

WITH_BF_JPEG = False
BF_JPEG = '/opt/vitasdk/arm-vita-eabi'
BF_JPEG_INC = '${BF_JPEG}/include'
BF_JPEG_LIB = 'jpeg'

WITH_BF_PNG = True
BF_PNG = '/opt/vitasdk/arm-vita-eabi'
BF_PNG_INC = '${BF_PNG}/include'
BF_PNG_LIB = 'png'

WITH_BF_TIFF = False
BF_TIFF = '/opt/vitasdk/arm-vita-eabi'
BF_TIFF_INC = '${BF_TIFF}/include'
BF_TIFF_LIB = 'tiff'

WITH_BF_ZLIB = True
BF_ZLIB = '/opt/vitasdk/arm-vita-eabi'
BF_ZLIB_INC = '${BF_ZLIB}/include'
BF_ZLIB_LIB = 'z'

WITH_BF_INTERNATIONAL = True

WITH_BF_GAMEENGINE = True
WITH_BF_PLAYER = True
WITH_BF_NOBLENDER = True

WITH_BF_BULLET = True
BF_BULLET = '#extern/bullet2/src'
BF_BULLET_INC = '${BF_BULLET}'
BF_BULLET_LIB = 'extern_bullet'

WITH_BF_ELTOPO = False
BF_LAPACK = '/opt/vitasdk/arm-vita-eabi'
BF_LAPACK_LIB = 'lapack3gf blas clapack'
BF_LAPACK_LIBPATH = '${BF_LAPACK}/lib'

BF_FREETYPE = '/opt/vitasdk/arm-vita-eabi'
BF_FREETYPE_INC = '${BF_FREETYPE}/include ${BF_FREETYPE}/include/freetype2'
BF_FREETYPE_LIB = 'freetype'
#BF_FREETYPE_LIB_STATIC = '${BF_FREETYPE}/lib/libfreetype.a'

WITH_BF_ICONV = False
BF_ICONV = "/opt/vitasdk/arm-vita-eabi"
BF_ICONV_INC = '${BF_ICONV}/include'
BF_ICONV_LIB = 'iconv'
BF_ICONV_LIBPATH = '${BF_ICONV}/lib'

WITH_BF_BINRELOC = True

# enable ffmpeg  support
WITH_BF_FFMPEG = False
BF_FFMPEG = '/opt/vitasdk/arm-vita-eabi'
BF_FFMPEG_LIB = 'avformat avcodec swscale avutil avdevice'
BF_FFMPEG_INC = '${BF_FFMPEG}/include'
BF_FFMPEG_LIBPATH='${BF_FFMPEG}/lib'
#WITH_BF_STATICFFMPEG = True
#BF_FFMPEG_LIB_STATIC = '${BF_FFMPEG_LIBPATH}/libavformat.a ${BF_FFMPEG_LIBPATH/libavcodec.a ${BF_FFMPEG_LIBPATH}/libswscale.a ${BF_FFMPEG_LIBPATH}/libavutil.a ${BF_FFMPEG_LIBPATH}/libavdevice.a'

# enable ogg, vorbis and theora in ffmpeg
WITH_BF_OGG = False
BF_OGG = '/opt/vitasdk/arm-vita-eabi'
BF_OGG_INC = '${BF_OGG}/include'
BF_OGG_LIB = 'ogg vorbis vorbisenc theoraenc theoradec'

WITH_BF_OPENJPEG = True
BF_OPENJPEG = '#extern/libopenjpeg'
BF_OPENJPEG_LIB = ''
BF_OPENJPEG_INC = '${BF_OPENJPEG}'
BF_OPENJPEG_LIBPATH='${BF_OPENJPEG}/lib'

WITH_BF_FFTW3 = False
WITH_BF_STATICFFTW3 = False
BF_FFTW3 = '/opt/vitasdk/arm-vita-eabi'
BF_FFTW3_INC = '${BF_FFTW3}/include'
BF_FFTW3_LIB = 'fftw3'
BF_FFTW3_LIBPATH = '${BF_FFTW3}/lib'
BF_FFTW3_LIB_STATIC = '${BF_FFTW3_LIBPATH}/libfftw3.a'

WITH_BF_REDCODE = False  
BF_REDCODE = '#extern/libredcode'
BF_REDCODE_LIB = ''
# BF_REDCODE_INC = '${BF_REDCODE}/include'
BF_REDCODE_INC = '${BF_REDCODE}/../' #C files request "libredcode/format.h" which is in "#extern/libredcode/format.h", stupid but compiles for now.
BF_REDCODE_LIBPATH='${BF_REDCODE}/lib'

# Mesa Libs should go here if you're using them as well....
WITH_BF_STATICOPENGL = False
BF_OPENGL = '/opt/vitasdk/arm-vita-eabi'
BF_OPENGL_INC = '${BF_OPENGL}/include'
BF_OPENGL_LIB = 'GL GLU'
BF_OPENGL_LIBPATH = '/opt/vitasdk/arm-vita-eabi/X11R6/lib'
BF_OPENGL_LIB_STATIC = '${BF_OPENGL_LIBPATH}/libGL.a ${BF_OPENGL_LIBPATH}/libGLU.a ${BF_OPENGL_LIBPATH}/libXxf86vm.a ${BF_OPENGL_LIBPATH}/libX11.a ${BF_OPENGL_LIBPATH}/libXi.a ${BF_OPENGL_LIBPATH}/libXext.a ${BF_OPENGL_LIBPATH}/libXxf86vm.a'

WITH_BF_COLLADA = False
BF_COLLADA = '#source/blender/collada'
BF_COLLADA_INC = '${BF_COLLADA}'
BF_COLLADA_LIB = 'bf_collada'
BF_OPENCOLLADA = '/opt/vitasdk/arm-vita-eabi'
BF_OPENCOLLADA_INC = '${BF_OPENCOLLADA}/include/opencollada'
BF_OPENCOLLADA_LIB = 'OpenCOLLADAStreamWriter OpenCOLLADASaxFrameworkLoader OpenCOLLADAFramework OpenCOLLADABaseUtils GeneratedSaxParser UTF MathMLSolver pcre buffer ftoa'
BF_OPENCOLLADA_LIBPATH = '${BF_OPENCOLLADA}/lib/opencollada'
BF_PCRE = ''
BF_PCRE_LIB = 'pcre'
BF_PCRE_LIBPATH = '/opt/vitasdk/arm-vita-eabi/lib'
BF_EXPAT = '/opt/vitasdk/arm-vita-eabi'
BF_EXPAT_LIB = 'expat'
BF_EXPAT_LIBPATH = '/opt/vitasdk/arm-vita-eabi/lib'

WITH_BF_JEMALLOC = False
WITH_BF_STATICJEMALLOC = False
BF_JEMALLOC = '/opt/vitasdk/arm-vita-eabi'
BF_JEMALLOC_INC = '${BF_JEMALLOC}/include'
BF_JEMALLOC_LIBPATH = '${BF_JEMALLOC}/lib'
BF_JEMALLOC_LIB = 'jemalloc'
BF_JEMALLOC_LIB_STATIC = '${BF_JEMALLOC_LIBPATH}/libjemalloc.a'

WITH_BF_OIIO = False
WITH_BF_STATICOIIO = False
BF_OIIO = '/opt/vitasdk/arm-vita-eabi'
BF_OIIO_INC = '${BF_OIIO}/include'
BF_OIIO_LIB = 'OpenImageIO'
BF_OIIO_LIB_STATIC = '${BF_OIIO_LIBPATH}/libOpenImageIO.a ${BF_OPENEXR}/lib/libIlmImf.a ${BF_JPEG}/lib/libjpeg.a'
BF_OIIO_LIBPATH = '${BF_OIIO}/lib'

WITH_BF_OCIO = False
WITH_BF_STATICOCIO = False
BF_OCIO = '/opt/vitasdk/arm-vita-eabi'
BF_OCIO_INC = '${BF_OCIO}/include'
BF_OCIO_LIB = 'OpenColorIO yaml-cpp tinyxml'
BF_OCIO_LIB_STATIC = '${BF_OCIO_LIBPATH}/libOpenColorIO.a ${BF_OCIO_LIBPATH}/libtinyxml.a ${BF_OCIO_LIBPATH}/libyaml-cpp.a'
BF_OCIO_LIBPATH = '${BF_OCIO}/lib'

WITH_BF_BOOST = False
WITH_BF_STATICBOOST = False
BF_BOOST = '/opt/vitasdk/arm-vita-eabi'
BF_BOOST_INC = '${BF_BOOST}/include'
BF_BOOST_LIB = 'boost_filesystem boost_regex boost_system boost_thread boost_date_time'
BF_BOOST_LIB_STATIC = '${BF_BOOST_LIBPATH}/libboost_filesystem.a ${BF_BOOST_LIBPATH}/libboost_date_time.a ' + \
    '${BF_BOOST_LIBPATH}/libboost_regex.a ${BF_BOOST_LIBPATH}/libboost_locale.a ${BF_BOOST_LIBPATH}/libboost_system.a ' + \
    '${BF_BOOST_LIBPATH}/libboost_thread.a'
BF_BOOST_LIB_INTERNATIONAL = 'boost_locale'
BF_BOOST_LIBPATH = '${BF_BOOST}/lib'

WITH_BF_CYCLES = WITH_BF_OIIO and WITH_BF_BOOST

WITH_BF_CYCLES_CUDA_BINARIES = False
BF_CYCLES_CUDA_NVCC = '/opt/vitasdk/arm-vita-eabi/local/cuda/bin/nvcc'
BF_CYCLES_CUDA_BINARIES_ARCH = ['sm_20', 'sm_21', 'sm_30', 'sm_35', 'sm_50', 'sm_52']

WITH_BF_OPENMP = False

#Ray trace optimization
WITH_BF_RAYOPTIMIZATION = False
BF_RAYOPTIMIZATION_SSE_FLAGS = []

#SpaceNavigator and friends
WITH_BF_3DMOUSE = False
WITH_BF_STATIC3DMOUSE = False
BF_3DMOUSE = '/opt/vitasdk/arm-vita-eabi'
BF_3DMOUSE_INC = '${BF_3DMOUSE}/include'
BF_3DMOUSE_LIBPATH = '${BF_3DMOUSE}/lib'
BF_3DMOUSE_LIB = 'spnav'
BF_3DMOUSE_LIB_STATIC = '${BF_3DMOUSE_LIBPATH}/libspnav.a'

#Freestyle
WITH_BF_FREESTYLE = False

WITH_BF_OPENSUBDIV = False
WITH_BF_STATICOPENSUBDIV = False
BF_OPENSUBDIV = '/opt/vitasdk/arm-vita-eabi'
BF_OPENSUBDIV_INC = '${BF_OPENSUBDIV}/include'
BF_OPENSUBDIV_LIB = 'osdCPU osdGPU'
BF_OPENSUBDIV_LIB_STATIC = '${BF_OPENSUBDIV_LIBPATH}/libosdGPU.a ${BF_OPENSUBDIV_LIBPATH}/libosdCPU.a'
BF_OPENSUBDIV_LIBPATH = '${BF_OPENSUBDIV}/lib'

##
CC = 'arm-vita-eabi-gcc'
CXX = 'arm-vita-eabi-g++'

CCFLAGS = ['-pipe','-fPIC','-funsigned-char','-fno-strict-aliasing', '-D_LARGEFILE_SOURCE', '-D_FILE_OFFSET_BITS=64','-D_LARGEFILE64_SOURCE']
CFLAGS = ['-std=gnu89']
CXXFLAGS = []

CPPFLAGS = ['-D__vita__']
# g++ 4.6, only needed for bullet
CXXFLAGS += ['-fpermissive']
if WITH_BF_FFMPEG:
    # libavutil needs UINT64_C()
    CXXFLAGS += ['-D__STDC_CONSTANT_MACROS', ]
REL_CFLAGS = []
REL_CXXFLAGS = []
REL_CCFLAGS = ['-Oz']

C_WARN = ['-Wno-char-subscripts', '-Wdeclaration-after-statement', '-Wunused-parameter', '-Wstrict-prototypes', '-Werror=declaration-after-statement', '-Werror=implicit-function-declaration', '-Werror=return-type']
CC_WARN = ['-Wall']
CXX_WARN = ['-Wno-invalid-offsetof', '-Wno-sign-compare']

LLIBS = ['util', 'c', 'm', 'dl', 'pthread']

BF_PROFILE = False
BF_PROFILE_CCFLAGS = ['-pg','-g']
BF_PROFILE_LINKFLAGS = ['-pg']

BF_DEBUG = False
BF_DEBUG_CCFLAGS = ['-g']

BF_BUILDDIR = '../build/vita'
BF_INSTALLDIR='../install/vita'

#Link against pthread
PLATFORM_LINKFLAGS = ['-static']

#Fix for LLVM conflict with Mesa llvmpipe, SDL dynload also requires symbols to be hidden.
# TODO(sergey): Move this to SConstruct, so we can have this line depended on user config.
#PLATFORM_LINKFLAGS += ['-Wl,--version-script=source/creator/blender.map']
