set(APP_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
        pcap_module.h
        pcap_module.cc
        socket_module.h
        socket_module.cc
        s1ap_module.h
        s1ap_module.cc
)

add_executable("Trace" ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(
    ${EXECUTABLE_NAME}
    PRIVATE
#            "LibSocket_mdl"
)


if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${EXECUTABLE_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${EXECUTABLE_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${EXECUTABLE_NAME})
endif()
