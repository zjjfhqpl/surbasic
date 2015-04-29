add_executable(main_test main.cpp)
target_link_libraries(main_test surbasic borzoi pthread ssl jsoncpp curl)

add_executable(surbasic_op_tests surbasic_op_tests.cpp)
target_link_libraries(surbasic_op_tests gwbase surbasic borzoi pthread ssl jsoncpp curl)
