#include "searchTable.h"

#include <float.h>
#include <iostream>

#include "subject.h"
#include "ball.h"
#include "collision.h"

using namespace std;

void SearchTable::populate(const std::vector<Subject*>& subjects){
    int b_index = 0;
    int s_index = 0;

    cout << "Number of subjects: " << subjects.size() << endl;

    for(int i=0; i<subjects.size(); i++){
        Ball* b = dynamic_cast<Ball*>(subjects[i]);
        if(b && b->is_movable)
            balls[b_index++] = b;
        else
            non_balls[s_index++] = subjects[i];
    }

    for(int b=0; b<11; b++){
        for(int s=0; s<35; s++){
            time_safe[b][s] = 0;
        }
    }
}

void SearchTable::set_index_time(int index, double t){
    if(index < 11){
        for(int s=0; s<35; s++)
            time_safe[index][s] = t;
    }

    for(int b=0; b<11; b++)
        time_safe[b][index] = t;
}

void SearchTable::set_index_max_time(int index, double t){
    if(index < 11){
        for(int s=0; s<35; s++)
            time_safe[index][s] = min(t, time_safe[index][s]);
    }

    for(int b=0; b<11; b++)
        time_safe[b][index] = min(t, time_safe[b][index]);
}

int SearchTable::get_index(Subject* s){
    for(int i=0; i<11; i++){
        if(balls[i] == s)
            return i;
    }
    for(int i=0; i<35; i++){
        if(non_balls[i] == s)
            return i + 11;
    }
}


double SearchTable::global_time_safe() const{
    double result = DBL_MAX;

    for(int r=0; r<11; r++){
        for(int c=r+1; c<35; c++){
            result = min(result, time_safe[r][c]);
        }
    }
    return result;
}

void SearchTable::check_until(double until){
    bool flag = true;
    while(flag){
        flag = false;
        cout << "Performing SearchTable pass\n";
        // iterate over balls
        for(int b=0; b<11; b++){

            // Find target
            double next_coll = DBL_MAX;
            int coll_index = -1;
            for(int s=b+1; s<35; s++){
                cout << "Checking " << b << ", " << s << endl;
                if(time_safe[b][s] >= until)
                    continue;
                flag = true;
                Subject* sub = (s < 11) ? balls[s] : non_balls[s-11];

                double coll = sub->next_collision(balls[b], time_safe[b][s]);
                // if coll != -1, then time_safe WILL be overwritten later
                if(coll == -1)
                    time_safe[b][s] = DBL_MAX;
                else if(coll < next_coll){
                    next_coll = coll;
                    coll_index = s;
                }
            }

            // Perform collision (if needed)
            if(coll_index != -1){
                // Any future collisions laid out for involved objects are dropped
                invalids cancelled;
                if(coll_index < 11)
                    cancelled = apply_collision(*balls[b], *balls[coll_index], next_coll);
                else
                    cancelled = apply_collision(*balls[b], non_balls[coll_index - 11], next_coll);

                set_index_time(b, next_coll);
                set_index_time(coll_index, next_coll);

                for(tuple<double, Subject*>& can : cancelled){
                    set_index_max_time(get_index(get<1>(can)), get<0>(can));
                }
            }
        }
    }
}
