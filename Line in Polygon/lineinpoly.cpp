#define _USE_MATH_DEFINES
#include "matplotlibcpp.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>
#include <sys/time.h>
#include "geom.h"
using namespace std;
using namespace std;
namespace plt = matplotlibcpp;

// The array of segments that form the polygon.
vector<segment2D>  polygonSegments;
// The vertices of the polygon.
vector<point2D> polygonVertices;

void constructPolygon(vector<point2D> pgon){
    segment2D edge;
    // Connect the vertices of the polygon to form edges and store in polygonSegments
    for (int i = 0; i < pgon.size(); i++){
        if (i == 0){
            segment2D edge = {pgon[pgon.size()-1], pgon[i]};
            polygonSegments.push_back(edge);
        }else{
            segment2D edge = {pgon[i-1], pgon[i]};
            polygonSegments.push_back(edge);
        }
    }
}

int insidePolygon(vector<point2D> pgon, point2D point)
{
    int inside_flag;
    point2D vtx0, vtx1;
    vtx0 = pgon[pgon.size()-1];                                                         // starting vertex
    vtx1 = pgon[0];                                                                     // next vertex
    inside_flag = 0;                                                                    // Assume the point is outside
    for (int j = 0; j < pgon.size(); j++) {
        if (between(vtx1, vtx0, point)){                                                // if X and Y values are between 2 vertices and collinear, point is on the edge.
            inside_flag = 0;
            return(inside_flag);
        }
        else{                                                                               
            if ((vtx0.y >= point.y) != (vtx1.y >= point.y)){                            // If both vertices are on the opposite side, do cross product of (point and vertex1) and (vertex1 and vertex2) 
                if ((signed_area2D(vtx1, vtx0, point)<0) == (vtx1.y >=point.y)) {       // Cross product finds which side of the edge the point is at. 
                    inside_flag = !inside_flag;                                         // If point is on the leftside of the edge and first vertex is below the point's Y value, it will cross.
                    }
            }
        }
        vtx0 = vtx1;                                                                    // Retaining state of crossing (Inside/Outside) and iterate through all vertices
        vtx1 = pgon[j+1];
    }
    return(inside_flag);
}

int main() {

    // POINT TO TEST
    // GENERATE RANDOM DECIMALS
    int n = 100; // Number of random points
    srand( (unsigned)time( NULL ) );
    vector<double> pos_x, pos_y, home_x, home_y ;
    for (int i = 0; i < n; i++) 
    {
        pos_x.push_back((double) rand()/(RAND_MAX/1));
        pos_y.push_back((double) rand()/(RAND_MAX/1));
        home_x.push_back((double) rand()/(RAND_MAX/1));
        home_y.push_back((double) rand()/(RAND_MAX/1));
    }

    // POLYGON TO TEST
    // PLOT POLYGON
    vector<double> vertex_x = {0.0771889400921659,
0.0633640552995392,
0.922811059907834,
0.284562211981567,
0.862903225806452,
0.270737327188940,
0.876728110599078,
0.902073732718894};
    vector<double> vertex_y = {0.879008746355685,
0.0364431486880465,
0.0422740524781340,
0.190962099125364,
0.295918367346939,
0.409620991253644,
0.569970845481050,
0.879008746355685};
                            
    plt::fill(vertex_x, vertex_y,{});
    vector<point2D> polygonVertices;
    for (int i = 0; i < vertex_x.size(); i++){
        point2D vertex = {vertex_x[i], vertex_y[i]};
        polygonVertices.push_back(vertex);
    }

    // start timer.
    struct timeval start, end;
    gettimeofday(&start, NULL);
    ios_base::sync_with_stdio(false);

    // Construct line and Polygon
    constructPolygon(polygonVertices);

    for (int j=0;j<n;j++){
        int count = 0;
        point2D position = {pos_x[j],pos_y[j]}, home = {home_x[j], home_y[j]};
        segment2D path = {position, home};
        if (insidePolygon(polygonVertices, position) && insidePolygon(polygonVertices, home)){
            for (int i=0;i<polygonSegments.size();i++){
                if (intersect(polygonSegments[i], path)){
                    count++;
                }
            }
            if (count == 0){
                plt::plot({pos_x[j], home_x[j]}, {pos_y[j], home_y[j]}, "g");
            }
            else{
                plt::plot({pos_x[j], home_x[j]}, {pos_y[j], home_y[j]}, "r");
            }    
        }
    }

    // stop timer.
    gettimeofday(&end, NULL);
    double time_taken;
  
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - 
                              start.tv_usec)) * 1e-6;
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(6);
    cout << " sec" << endl;
        plt::show();
    return 0;
}