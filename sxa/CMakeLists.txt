set(APP_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/pfcp_packet_parser.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/pcap_file_initialize.c"
)

set(APP_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/pfcp_packet_parser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/pcap_file_initialize.h"
    )

add_executable("sxa" ${APP_SOURCES} ${APP_HEADERS} ${APP_HEADERS})

target_link_libraries(
    "sxa"
    PRIVATE
            "LibUtilities"
            "LibSniff_sock"
            "LibPcap_mdl"
)


if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        "sxa"
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        "sxa"
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target("sxa")
endif()
