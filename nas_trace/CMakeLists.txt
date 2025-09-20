set(APP_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/s1ap_packet_parser.c"
)

set(APP_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/s1ap_packet_parser.h"
    )

add_executable("s1ap_trace" ${APP_SOURCES} ${APP_HEADERS} ${APP_HEADERS})

target_link_libraries(
    "s1ap_trace"
    PRIVATE
            "LibUtilities"
            "LibSocket_mdl"
            "LibPcap_mdl"
)


if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        "s1ap_trace"
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        "s1ap_trace"
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target("s1ap_trace")
endif()
