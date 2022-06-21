#include "collision.h"

#include <float.h>
#include <complex>

#include "ball.h"
#include "quartic.h"
#include "matrix.h"

using namespace std;

#define sq(a) pow(a,2)

// Path and radius
double _next_collision(const Path p1, double r1, const Path p2, double r2){
    // Pick latest time_start between paths
    // get  pos & vel at that time
    double start = max(p1.time_start, p2.time_start);
    double end1 = (p1.time_end == -1) ? DBL_MAX : p1.time_end;
    double end2 = (p2.time_end == -1) ? DBL_MAX : p2.time_end;

    double end = min(end1, end2);

    vec2d pos = p1.pos(start) - p2.pos(start);
    vec2d speed = p1.vel(start) - p2.vel(start);
    vec2d accel = p1.accel - p2.accel;

    double min_dist = r1 + r2;

    // No relative movement -> either continuous contact or no collision
    //      1st case would break whole algorithm
    //      2nd case just a computational waste
    if(speed.is_zero() && accel.is_zero())
        return -1;

    // Keep in mind that this doesn't have to be repeated every tick

    double a = 0.5 * (sq(accel[0]) + sq(accel[1]));
    double b = accel[0] * speed[0] + accel[1] * speed[1];
    double c = accel[0] * pos[0] + accel[1] * pos[1]
            + sq(speed[0]) + sq(speed[1]);
    double d = 2 * pos[0] * speed[0] + 2 * pos[1] * speed[1];
    double e = sq(pos[0]) + sq(pos[1]) - min_dist*min_dist;

    // solve_quartic solves quartic equations of the form x^4 + a*x^3 + b*x^2 + c*x + d
    // so every parameter must be divided by a to scale 1st term to 1
    DComplex* solutions = solve_quartic(b/a, c/a, d/a, e/a);

    double next_coll = DBL_MAX;

    for(int i=0; i<4; i++){
        if(solutions[i].imag() != 0)
            continue;
        if(solutions[i].real() > 0 && solutions[i].real() < next_coll)
            next_coll = solutions[i].real();
    }
    delete [] solutions;

    if(next_coll == DBL_MAX || next_coll + start > end)
        next_coll = -1;
    else
        next_coll += start;

    return next_coll;
}

double _next_collision(const Path path, double r, Line l){
    vec2d pos = path.pos_0;
    vec2d speed = path.vel_0;
    vec2d accel = path.accel;

    if(speed.is_zero() && accel.is_zero())
        return -1;

    pos -= l.p1;
    l.p2 -= l.p1;
    l.p1 = vec2d{0,0};

    RotMatrix2d rot(l.p2);

    pos = pos * rot;
    speed = speed * rot;
    accel = accel * rot;

    // At this point, y component is used to determine time until collision
    // if y<0, +r
    // if y>=0, -r

    if(pos[1] < 0){
        pos[1] += r;
        if(pos[1] >= 0)
            return 0;
    }
    else{
        pos[1] -= r;
        if(pos[1] <= 0)
            return 0;
    }

    double t = quadratic_next(0.5*accel[1], speed[1], pos[1]);
    if(t == -1 || t + path.time_start > path.time_end)
        return -1;

    double x = pos[0] + speed[0] * t + 0.5 * t*t * accel[0];
    if(x < 0 || x > l.p2.magnitude())
        return -1;

    return t + path.time_start;
}

double quadratic_next(double a, double b, double c){
    double result;

    double sqrtTerm = b*b + 4*a*c;
    if(sqrtTerm <= 0)
        return -1;

    sqrtTerm = sqrt(sqrtTerm);


    result = (-b - sqrtTerm)/(2*a);
    if(result > 0)
        return result;

    result = (-b + sqrtTerm)/(2*a);
    if(result > 0)
        return result;

    return -1;
}

invalids apply_collision(Ball& b1, Subject* s, double time){
    Ball* b2 = dynamic_cast<Ball*>(s);
    if(b2)
        return apply_collision(b1, *b2, time);

    Rect* r = dynamic_cast<Rect*>(s);
    if(r)
        return apply_collision(b1, r->line, time);
}


invalids apply_collision(Ball& b1, Ball& b2, double time){
    double combined_r = b1.posData.radius + b2.posData.radius;

    // Get next path section's info
    vec2d b1_pos = b1.pos_from_path(time);
    vec2d b1_vel = b1.vel_from_path(time);

    vec2d b2_pos = b2.pos_from_path(time);
    vec2d b2_vel = b2.vel_from_path(time);
    // vec2d b1_accel = b1.accel_from_path(time);

    collision2Ds(b1.mass, b2.mass, combined_r,
        b1_pos[0], b1_pos[1], b2_pos[0], b2_pos[1],
        b1_vel[0], b1_vel[1], b2_vel[0], b2_vel[1]);

    // Create next path section
    invalids cancelled = b1.append_path(time, b1_pos, b1_vel, &b2);
    invalids temp = b2.append_path(time, b2_pos, b2_vel, &b1);

    cancelled.insert(cancelled.end(), temp.begin(), temp.end());
    return cancelled;
}

invalids apply_collision(Ball& b, const Line l, double time){
    vec2d pos = b.pos_from_path(time);
    vec2d vel = b.vel_from_path(time);

    // flip velocity around line
    vec2d rotation = l.p2 - l.p1;
    RotMatrix2d rot(rotation);
    vel = vel * rot;
    vel[1] = -vel[1];

    rotation[1] = -rotation[1];
    rot.set_from_vec(rotation);
    vel = vel * rot;

    return b.append_path(time, pos, vel, 0);
}


// https://www.plasmaphysics.org.uk/programs/coll2d_cpp.htm
void collision2Ds(double m1, double m2, double R,
                  double x1, double y1, double x2, double y2,
                  double& vx1, double& vy1, double& vx2, double& vy2)
{
    double  m21,dvx2,a,x21,y21,vx21,vy21,fy21,sign,vx_cm,vy_cm;

    m21=m2/m1;
    x21=x2-x1;
    y21=y2-y1;
    vx21=vx2-vx1;
    vy21=vy2-vy1;

    vx_cm = (m1*vx1+m2*vx2)/(m1+m2) ;
    vy_cm = (m1*vy1+m2*vy2)/(m1+m2) ;


    //     *** return old velocities if balls are not approaching ***
    if ( (vx21*x21 + vy21*y21) >= 0) return;


    //     *** I have inserted the following statements to avoid a zero divide;
    //         (for single precision calculations,
    //          1.0E-12 should be replaced by a larger value). **************

    fy21=1.0E-12*std::fabs(y21);
    if ( std::fabs(x21)<fy21 ) {
        if (x21<0) { sign=-1; }
        else { sign=1;}
        x21=fy21*sign;
    }

    //     ***  update velocities ***
    a=y21/x21;
    dvx2= -2*(vx21 +a*vy21)/((1+a*a)*(1+m21)) ;
    vx2=vx2+dvx2;
    vy2=vy2+a*dvx2;
    vx1=vx1-m21*dvx2;
    vy1=vy1-a*m21*dvx2;

    //     ***  velocity correction for inelastic collisions ***
    vx1=(vx1-vx_cm)*R + vx_cm;
    vy1=(vy1-vy_cm)*R + vy_cm;
    vx2=(vx2-vx_cm)*R + vx_cm;
    vy2=(vy2-vy_cm)*R + vy_cm;

    return;
}
