set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/boost_client_socket_async.cc")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/boost_client_socket_async.hxx")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library("LibBoost_sokcet_client_async" STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories("LibBoost_sokcet_client_async" PUBLIC ${LIBRARY_INCLUDES})
#target_link_libraries(
#    "LibFoo"
#    PRIVATE nlohmann_json::nlohmann_json
#            fmt::fmt
#            spdlog::spdlog
#            cxxopts::cxxopts)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        "LibBoost_sokcet_client_async"
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        "LibBoost_sokcet_client_async"
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target("LibBoost_sokcet_client_async")
endif()
