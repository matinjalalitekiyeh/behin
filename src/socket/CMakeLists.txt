set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/socket_mdl.c")
set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/socket_mdl.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/sock_context.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library("LibSocket_mdl" STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories("LibSocket_mdl" PUBLIC ${LIBRARY_INCLUDES})
#target_link_libraries(
#    "LibSocket_mdl"
#    PRIVATE nlohmann_json::nlohmann_json
#            fmt::fmt
#            spdlog::spdlog
#            cxxopts::cxxopts)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        "LibSocket_mdl"
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        "LibSocket_mdl"
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target("LibSocket_mdl")
endif()
