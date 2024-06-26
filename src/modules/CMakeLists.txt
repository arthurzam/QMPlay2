cmake_minimum_required(VERSION 3.16)

if(WIN32)
    set(MODULES_INSTALL_PATH "modules")
    set(CMAKE_SHARED_MODULE_PREFIX "") # remove lib prefix
elseif(APPLE)
    set(MODULES_INSTALL_PATH "${CMAKE_INSTALL_LIBDIR}/modules")
elseif(HAIKU)
    set(MODULES_INSTALL_PATH "modules")
else()
    set(MODULES_INSTALL_PATH "${CMAKE_INSTALL_LIBDIR}/qmplay2/modules")
endif()

add_subdirectory(FFmpeg)

if(USE_INPUTS)
    add_subdirectory(Inputs)
endif()

if(USE_MODPLUG)
    add_subdirectory(Modplug)
endif()

if(USE_EXTENSIONS)
    add_subdirectory(Extensions)
endif()

if(USE_VISUALIZATIONS)
    add_subdirectory(Visualizations)
endif()

if(USE_AUDIOFILTERS)
    add_subdirectory(AudioFilters)
endif()

if(USE_VIDEOFILTERS)
    add_subdirectory(VideoFilters)
endif()

if(USE_AUDIOCD)
    add_subdirectory(AudioCD)
endif()

if(USE_ALSA)
    add_subdirectory(ALSA)
endif()

if(USE_PORTAUDIO)
    add_subdirectory(PortAudio)
endif()

if(USE_PULSEAUDIO)
    add_subdirectory(PulseAudio)
endif()

if(USE_PIPEWIRE)
    add_subdirectory(PipeWire)
endif()

if(USE_XVIDEO)
    add_subdirectory(XVideo)
endif()

if(USE_CHIPTUNE_GME OR USE_CHIPTUNE_SID)
    add_subdirectory(Chiptune)
endif()

add_subdirectory(Playlists)
add_subdirectory(Subtitles)
add_subdirectory(QPainter)

if(WIN32)
    if(NOT BUILD_WITH_QT6)
        add_subdirectory(DirectX)
    endif()
    add_subdirectory(FileAssociation)
endif()

if(USE_CUVID)
    add_subdirectory(CUVID)
endif()

if(USE_NOTIFY)
    add_subdirectory(Notify)
endif()

if(ANDROID)
    add_subdirectory(OpenSLES)
endif()

if(HAIKU)
    add_subdirectory(MediaKit)
endif()
