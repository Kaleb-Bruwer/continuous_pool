#include "collision_test_helpers.h"

void assert_path_eq(const Path& lhs, const Path& rhs){
    double tolerance = 0.00001;

    if(lhs.pocketed || rhs.pocketed){
        ASSERT_TRUE(lhs.pocketed == rhs.pocketed) << "one of two paths pocketed";
        // positional info irrelevant if pocketed

        double diffd = abs(lhs.time_start - rhs.time_start);
        ASSERT_TRUE(diffd < tolerance) << "time_start mismatch";

        diffd = abs(lhs.time_end - rhs.time_end);
        ASSERT_TRUE(diffd < tolerance) << "time_end mismatch";
        return;
    }

    vec2d diff;
    diff = abs(lhs.pos_0 - rhs.pos_0);
    ASSERT_TRUE(diff[0] < tolerance) << "pos_0 mismatch";
    ASSERT_TRUE(diff[1] < tolerance) << "pos_0 mismatch";

    diff = abs(lhs.vel_0 - rhs.vel_0);
    ASSERT_TRUE(diff[0] < tolerance) << "vel_0 mismatch";
    ASSERT_TRUE(diff[1] < tolerance) << "vel_0 mismatch";

    diff = abs(lhs.accel - rhs.accel);
    ASSERT_TRUE(diff[0] < tolerance) << "accel mismatch";
    ASSERT_TRUE(diff[1] < tolerance) << "accel mismatch";

    double diffd = abs(lhs.time_start - rhs.time_start);
    ASSERT_TRUE(diffd < tolerance) << "time_start mismatch";

    diffd = abs(lhs.time_end - rhs.time_end);
    ASSERT_TRUE(diffd < tolerance) << "time_end mismatch";
}

void assert_ball_path(const Ball& b, const Ball& expected){
    ASSERT_TRUE(b.path.size() == expected.path.size())
            << "Path size mismatch: " << b.path.size()
            << " instead of " << expected.path.size();

    for(int i=0; i<b.path.size(); i++){
        assert_path_eq(b.path[i], expected.path[i]);
    }
}
