#include "../../header/Game.hpp"
#include <gtest/gtest.h>
#include <string_view>
int main(int argc, char* argv[]){ 
    if (argc >= 2 && std::string_view{argv[1]} == "run-tests") { 
        ::testing::InitGoogleTest(&argc, argv); 
        return RUN_ALL_TESTS(); 
    } 
    while(rungame()); 
    return 0; 
}