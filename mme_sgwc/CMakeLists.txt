set(APP_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/gtpv2_packet_parser.c"
)

set(APP_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/gtpv2_packet_parser.h"
    )

add_executable("gtpv2" ${APP_SOURCES} ${APP_HEADERS} ${APP_HEADERS})

target_link_libraries(
    "gtpv2"
    PRIVATE
            "LibUtilities"
            "LibSocket_mdl"
            "LibPcap_mdl"
)


if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        "gtpv2"
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        "gtpv2"
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target("gtpv2")
endif()
