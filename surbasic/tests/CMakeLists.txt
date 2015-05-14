add_executable(main_test main.cpp)
target_link_libraries(main_test surbasic borzoi pthread ssl jsoncpp curl)

add_executable(XmlHelperTests XmlHelperTests.cpp)
set_source_files_properties(XmlHelperTests.cpp PROPERTIES COMPILE_FLAGS "-Wno-error")
target_link_libraries(XmlHelperTests surbasic borzoi pthread ssl jsoncpp curl)

#add_executable(surbasic_op_tests surbasic_op_tests.cpp)
#target_link_libraries(surbasic_op_tests gwbase surbasic borzoi pthread ssl jsoncpp curl)

#add_executable(aes_file_tests aes_file_tests.cpp)
#target_link_libraries(aes_file_tests gwbase surbasic borzoi pthread ssl jsoncpp curl)


