#include <gtest/gtest.h>

#include <iostream>

class HelloTest : public ::testing::Test{

};

TEST_F(HelloTest, Hello){
    std::cout << "Hello world, this is a unit test!\n";
}
