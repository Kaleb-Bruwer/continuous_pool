#include "forwardlevel.h"

void ForwardLevel::runToStop(){
    do{
        logic();
    }while(moving_state);
}