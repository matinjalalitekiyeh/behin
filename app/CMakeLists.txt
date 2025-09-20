set(APP_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"

        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_app.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_parser.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_history_manager.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_input_handler.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_command_executer.cc"
)
set(APP_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/ver/version.hxx"

        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_app.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_parser.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_history_manager.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_input_handler.hxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/cli/cli_command_executer.hxx"

)

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${EXECUTABLE_NAME} ${APP_SOURCES} ${APP_HEADERS})

target_link_libraries(
    ${EXECUTABLE_NAME}
    PRIVATE "LibBoost_sokcet_client_async"
            "LibSocket_mdl" #remove me
            "LibPcap_mdl" #remove me
            "LibUtilities" #remove me
            ${Boost_LIBRARIES} readline
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
