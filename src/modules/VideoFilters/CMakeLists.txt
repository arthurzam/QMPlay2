cmake_minimum_required(VERSION 3.16)
project(VideoFilters)

set(VideoFilters_HDR
    VFilters.hpp
    BobDeint.hpp
    BlendDeint.hpp
    DiscardDeint.hpp
    YadifDeint.hpp
    FPSDoubler.hpp
)

set(VideoFilters_SRC
    VFilters.cpp
    BobDeint.cpp
    BlendDeint.cpp
    DiscardDeint.cpp
    YadifDeint.cpp
    FPSDoubler.cpp
)

if(FALSE)
    list(APPEND VideoFilters_HDR
        MotionBlur.hpp
    )
    list(APPEND VideoFilters_SRC
        MotionBlur.cpp
    )
    add_definitions(-DMOTION_BLUR)
endif()

set(VideoFilters_RESOURCES
    icon.qrc
)

add_library(${PROJECT_NAME} MODULE
    ${VideoFilters_HDR}
    ${VideoFilters_SRC}
    ${VideoFilters_RESOURCES}
)

if(USE_PCH)
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        ${VideoFilters_HDR}
    )
endif()

libqmplay2_set_target_params()

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${MODULES_INSTALL_PATH})
