//
//  water_line.h
//  firstSketch
//
//  Created by 민동현 on 26/09/2019.
//

#ifndef water_line_h
#define water_line_h

#include "ofMain.h"

#define EPSILON 0.01


struct LineSegment {
    int x1, y1;
    int x2, y2;
    float slope;
    float x_coef;
    float y_coef;
    float constant;
};

struct Dot {
    int x1, y1;
};
class WaterLine{
    
public:
    WaterLine(int num_of_line);
    ~WaterLine();
    

    void reset();
    void update();
    void draw();
    void calculate_path(LineSegment *lineseg, int num_of_line);
    
    Dot* path = NULL;
    
    float uniqueColor_r, uniqueColor_g, uniqueColor_b;
    float scale;
    int hexcolor;
    
    int path_idx;
    int draw_complete;
    int calc_complete;
    ofPoint start_dot;
    float dot_radius;
    
};

#endif /* water_line_h */
