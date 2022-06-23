#include "searchTable.h"

#include <float.h>
#include <iostream>

#include "subject.h"
#include "ball.h"
#include "collision.h"
#include "level.h"

using namespace std;

void SearchTable::populate(const std::vector<Subject*>& subjects, Ball* cueball){
    int b_index = 0;
    int s_index = 0;

    for(int i=0; i<subjects.size(); i++){
        Ball* b = dynamic_cast<Ball*>(subjects[i]);
        if(b && b->is_movable){
            if(b == cueball)
                cueball_index = b_index;
            balls[b_index++] = b;
        }
        else
            non_balls[s_index++] = subjects[i];
    }

    for(int b=0; b<NUM_BALLS; b++){
        for(int s=0; s<(NUM_BALLS + NUM_NON_BALLS); s++){
            time_safe[b][s] = 0;
        }
    }
}

void SearchTable::applyCue(vec2d vel, double time){
    // This may be overkill, but now it should work even if balls were moving

    if(time == -1)
        time = Level::get_time();

    if(cueball_index == -1){
        cout << "cueball not found\n";
        throw 0;
    }

    Ball& cueball = *balls[cueball_index];

    Path p;
    p.pos_0 = cueball.pos_from_path(time);
    p.vel_0 = vel;

    invalids cancelled = cueball.append_path(time, p);
    set_index_max_time(cueball_index, time);

    for(tuple<double, Subject*>& can : cancelled){
        set_index_max_time(get_index(get<1>(can)), get<0>(can));
    }
}

void SearchTable::set_subject_time(Subject* s, double t){
    for(int i=0; i<NUM_BALLS; i++){
        if(balls[i] == s){
            set_index_max_time(i, t);
            return;
        }
    }

    for(int i=0; i<NUM_NON_BALLS; i++){
        if(non_balls[i] == s){
            set_index_max_time(i + NUM_BALLS, t);
        }
    }
}


void SearchTable::set_index_time(int index, double t){
    if(index < NUM_BALLS){
        for(int s=0; s<NUM_NON_BALLS; s++)
            time_safe[index][s] = t;
    }

    for(int b=0; b<NUM_BALLS; b++)
        time_safe[b][index] = t;
}

void SearchTable::set_index_max_time(int index, double t){
    if(index < NUM_BALLS){
        for(int s=0; s<NUM_NON_BALLS; s++)
            time_safe[index][s] = min(t, time_safe[index][s]);
    }

    for(int b=0; b<NUM_BALLS; b++)
        time_safe[b][index] = min(t, time_safe[b][index]);
}

int SearchTable::get_index(Subject* s){
    for(int i=0; i<NUM_BALLS; i++){
        if(balls[i] == s)
            return i;
    }
    for(int i=0; i<NUM_NON_BALLS; i++){
        if(non_balls[i] == s)
            return i + NUM_BALLS;
    }
}


double SearchTable::global_time_safe() const{
    double result = DBL_MAX;

    for(int r=0; r<NUM_BALLS; r++){
        for(int c=r+1; c<NUM_BALLS + NUM_NON_BALLS; c++){
            result = min(result, time_safe[r][c]);
        }
    }
    return result;
}

void SearchTable::check_until(double until){
    bool flag = true;
    unsigned int passes = 0;
    while(flag){
        passes++;

        flag = false;
        // iterate over balls
        for(int b=0; b<NUM_BALLS; b++){

            // Find target
            double next_coll = DBL_MAX;
            int coll_index = -1;
            for(int s=b+1; s<NUM_BALLS + NUM_NON_BALLS; s++){
                if(time_safe[b][s] >= until)
                    continue;
                flag = true;
                Subject* sub = (s < NUM_BALLS) ? balls[s] : non_balls[s-NUM_BALLS];

                double coll = sub->next_collision(balls[b], time_safe[b][s]);
                // if coll != -1, then time_safe WILL be overwritten later
                if(coll == -1 || coll == DBL_MAX)
                    time_safe[b][s] = DBL_MAX;
                else if(coll < next_coll){
                    next_coll = coll;
                    coll_index = s;
                }
            }

            // Perform collision (if needed)|
            if(coll_index != -1){
                // Any future collisions laid out for involved objects are dropped
                invalids cancelled;
                if(coll_index < NUM_BALLS)
                    cancelled = apply_collision(*balls[b], *balls[coll_index], next_coll);
                else
                    cancelled = apply_collision(*balls[b], non_balls[coll_index - NUM_BALLS], next_coll);

                set_index_time(b, next_coll);
                set_index_time(coll_index, next_coll);

                for(tuple<double, Subject*>& can : cancelled){
                    set_index_max_time(get_index(get<1>(can)), get<0>(can));
                }
            }
        }
    }
}
