#define _USE_MATH_DEFINES
// #include "matplotlibcpp.h" //uncomment if using matplotlib
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <bits/stdc++.h>
#include <sys/time.h>
#include "geom.h"
using namespace std;
// namespace plt = matplotlibcpp; //uncomment if using matplotlib

vector<point2D> polygonVertices;

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
    // GENERATE RANDOM DECIMALS
    int n = 1000; // Number of random points
    srand( (unsigned)time( NULL ) );
    vector<double> x ,y ;
    for (int i = 0; i < n; i++) 
    {
        x.push_back((double) rand()/(RAND_MAX/1));
        y.push_back((double) rand()/(RAND_MAX/1));
    }


    // PLOT POLYGON
    vector<double> vertex_x = {0.139400921658986,
                            0.413594470046083,
                            0.455069124423963,
                            0.540322580645161,
                            0.733870967741936,
                            0.644009216589862,
                            0.802995391705069,
                            0.591013824884793,
                            0.823732718894009,
                            0.551843317972350,
                            0.468894009216590,
                            0.289170506912442,
                            0.406682027649770,
                            0.180875576036866,
                            0.344470046082949,
                            0.0656682027649770};
    vector<double> vertex_y = {0.593294460641399,
                            0.607871720116618,
                            0.963556851311953,
                            0.654518950437318,
                            0.739067055393586,
                            0.517492711370262,
                            0.532069970845481,
                            0.260932944606414,
                            0.272594752186589,
                            0.0481049562682216,
                            0.301749271137026,
                            0.0830903790087463,
                            0.406705539358601,
                            0.234693877551020,
                            0.482507288629738,
                            0.3804664723032071};
    // plt::fill(vertex_x, vertex_y,{}); //uncomment if using matplotlib

    vector<point2D> polygonVertices;
    for (int i = 0; i < vertex_x.size(); i++){
        point2D vertex = {vertex_x[i], vertex_y[i]};
        polygonVertices.push_back(vertex);
    }


    // START TIMER
    struct timeval start, end;
    gettimeofday(&start, NULL);
    ios_base::sync_with_stdio(false);

    // RUN ALGORITHM
    for (int i = 0; i < x.size(); i++){
        vector<double> a = {x[i]};
        vector<double> b = {y[i]};
        point2D testpoint = {x[i], y[i]}; 
        if (insidePolygon(polygonVertices, testpoint)){
            cout << "inside" << endl; 
            // plt::plot(a,b, "go"); //uncomment if using matplotlib
        }else{
            cout << "outside" << endl;
            // plt::plot(a,b, "ro"); //uncomment if using matplotlib
        }
    } 

    // STOP TIMER
    gettimeofday(&end, NULL);
    double time_taken;
  
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - 
                              start.tv_usec)) * 1e-6;
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(6);
    cout << " sec" << endl;

    // plt::show(); //uncomment if using matplotlib
    return 0;
}

// TIME TAKEN for 10000 points:
// To print inside/outside: ~0.027308 sec
// To plot on MPL: ~45.023767 sec

// vector<double> vertex_x = {0.125576036866359,
//                         0.144009216589862,
//                         0.286866359447005,
//                         0.256912442396313,
//                         0.482718894009217,
//                         0.468894009216590,
//                         0.644009216589862,
//                         0.802995391705069,
//                         0.448156682027650,
//                         0.819124423963134,
//                         0.869815668202765,
//                         0.678571428571429,
//                         0.906682027649770,
//                         0.897465437788018,
//                         0.915898617511521,
//                         0.837557603686636,
//                         0.611751152073733,
//                         0.480414746543779,
//                         0.436635944700461,
//                         0.353686635944701,
//                         0.2269585253456224};
// vector<double> vertex_y = {0.817784256559767,
//                         0.362973760932945,
//                         0.730320699708455,
//                         0.190962099125364,
//                         0.575801749271137,
//                         0.158892128279883,
//                         0.406705539358601,
//                         0.654518950437318,
//                         0.689504373177843,
//                         0.811953352769679,
//                         0.371720116618076,
//                         0.120991253644315,
//                         0.255102040816327,
//                         0.543731778425656,
//                         0.774052478134111,
//                         0.908163265306123,
//                         0.896501457725948,
//                         0.820699708454810,
//                         0.867346938775510,
//                         0.855685131195335,
//                         0.881924198250729};

// vector<double> vertex_x = {0.144009216589862,
//                         0.0288018433179724,
//                         0.351382488479263,
//                         0.250000000000000,
//                         0.632488479262673,
//                         0.531105990783410,
//                         0.800691244239631,
//                         0.879032258064516,
//                         0.683179723502304,
//                         0.908986175115207,
//                         0.561059907834101,
//                         0.627880184331797,
//                         0.434331797235023,
//                         0.392857142857143,
//                         0.316820276497696,
//                         0.224654377880184};
// vector<double> vertex_y = {0.803206997084548,
//                         0.243440233236152,
//                         0.660349854227405,
//                         0.141399416909621,
//                         0.666180758017493,
//                         0.0889212827988337,
//                         0.476676384839650,
//                         0.706997084548105,
//                         0.739067055393586,
//                         0.817784256559767,
//                         0.849854227405248,
//                         0.957725947521866,
//                         0.826530612244898,
//                         0.937317784256560,
//                         0.817784256559767,
//                         0.922740524781341};