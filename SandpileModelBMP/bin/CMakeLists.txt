add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE sandpile)
target_link_libraries(${PROJECT_NAME} PRIVATE parsing)
target_link_libraries(${PROJECT_NAME} PRIVATE bmp)

