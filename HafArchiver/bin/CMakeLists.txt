add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE argparse)
target_link_libraries(${PROJECT_NAME} PRIVATE archive)
target_link_libraries(${PROJECT_NAME} PRIVATE hamming)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})