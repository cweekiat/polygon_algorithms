#define _USE_MATH_DEFINES
// #include "matplotlibcpp.h" // uncomment if using matplotlib
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>
#include <sys/time.h>
#include "geom.h"
using namespace std;
// namespace plt = matplotlibcpp; // uncomment if using matplotlib

/**** Global variables ****/
point2D home; // The return home point
point2D pos; // Lat-Lon of position
vector<segment2D>        polygonSegments; // The array of segments that form the polygon.
vector<segment2D>        visibleAreaSegments; // The array of segments that form the visible area of the polygon.
vector<double>           visibleAreaSegmentSlopes; // The slope of the visible area segments.
vector<point2D>          visibleAreaVertices; // The vertices of the polygon that are visible from the visibility point.
vector<point2D>          extendedLineIntersections; // The intersection points of the extended lines with the egdes of the polygon.
vector<point2D>          polygonVertices;
vector<point2D>          order;
const int WINDOWSIZE    = 1000;
const int NUM_SEGMENTS  = 800; // Number of segments that form the circle.
const double OFFSET     = 1; // Number of pixels the point moves.

enum Direction {UP, DOWN, LEFT, RIGHT};

/**** Forward declarations of functions ****/

/* Helper computation functions */
int intersectPolygon(segment2D current);
void computeSlopes(void);
void clearVectors(void);
point2D closestEdgePoint(segment2D line, vector<int> edgeIndices);

/* Main computation functions */
int inside(vector<point2D> pgon, point2D point);
void computeVisibleArea(vector<point2D> pgon);
void computeVisibleVertices(vector<point2D> pgon);
void extendVisibleVertices(void);
void refineVisibleVerticesExtensions(segment2D extendedLine, int index);
void constructPolygon(vector<point2D> pgon);

/**** Computation Functions ****/
// Convert vertices to line segments
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
// Return 1 if current edge intersects with any other edge of the polygon;
// 0 otherwise.
int intersectPolygon(segment2D current) {
    if (polygonSegments.size() > 0) {
        // The last segment is the current segment.
        for (int i = 0; i < polygonSegments.size() - 1; i++) {
            if (intersect(current, polygonSegments[i])) {
                return 1;
            }
        }
    }
    return  0;
}

// Return 1 if point is inside the polygon.
int inside(vector<point2D> pgon, point2D point)
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


// Compute the initial visible vertices from the visibility point.
void computeVisibleVertices(vector<point2D> pgon) {
    for (int i = 0; i < pgon.size(); i++) {
        // Create line segments from visibility point to other vertices.
        segment2D line = {home, pgon[i]};
        
        // Check if this segment intersects any polygon edge.
        int hasIntersected = 0;
        for (int j = 0; j < polygonSegments.size(); j++) {
            if (intersect_proper(line, polygonSegments[j])) {
                hasIntersected = 1;
            }
        }
        // If no intersection, add vertex and segment to the vectors to be drawn.
        if (!hasIntersected) {
            visibleAreaSegments.push_back(line);
            visibleAreaVertices.push_back(pgon[i]);
        }
    }
}

// Compute and store the slopes of the visible area segments.
void computeSlopes(vector<segment2D> segments) {
    for (int i = 0; i < segments.size(); i++) {
        double m = slope(segments[i]);
        visibleAreaSegmentSlopes.push_back(m);
    }
}

// Extend visiblility lines of certain vertices to the edge of the polygon.
void extendVisibleVertices(vector<point2D> visibleAreaVertices) {
    for (int i = 0; i < visibleAreaVertices.size(); i++) {
        point2D p = visibleAreaVertices[i];
        
        // Endpoint of the extension line.
        point2D offScreenEndPoint;
        double xDiff = p.x - home.x;
       
        if (xDiff > 0) {
            // Draw extension segment to the right of visibility point.
            offScreenEndPoint.x = home.x + WINDOWSIZE;
            offScreenEndPoint.y = WINDOWSIZE * visibleAreaSegmentSlopes[i] + home.y;
        } else if (xDiff < 0){
            // Draw extension segment to the left of visibility point.
            offScreenEndPoint.x = home.x - WINDOWSIZE;
            offScreenEndPoint.y = -1 * WINDOWSIZE * visibleAreaSegmentSlopes[i] + home.y;
        } else {
            // Vertical line.
            offScreenEndPoint.x = home.x;
            if (p.y - home.y > 0) {
                // Draw extension segment above visibility point.
                offScreenEndPoint.y = WINDOWSIZE;
            } else {
                // Draw extension segment below visibility point.
                offScreenEndPoint.y = 0;
            }
        }
        segment2D extendedLine = {home, offScreenEndPoint};
        // Cut off the extension so that line remains inside polygon.
        
        order.push_back(visibleAreaVertices[i]);
        refineVisibleVerticesExtensions(extendedLine, i);
    }
}

// Compute the point of intersection of the edge closest to visibility point.
point2D closestEdgePoint(segment2D line, vector<int> edgeIndices) {
    double closestDistance = INFINITY;
    point2D closestIntersection;
    for (int i = 0; i < edgeIndices.size(); i++) {
       
        // Compute the intersection points and compare distance.
        point2D intersection = computeIntersection(line, polygonSegments[edgeIndices[i]]);
        double d = distance_(home, intersection);
        
        if (d < closestDistance) {
            closestDistance = d;
            closestIntersection = intersection;
        }
    }
    return closestIntersection;
}

// Helper function to extend lines from visible verticies.
void refineVisibleVerticesExtensions(segment2D extendedLine, int index) {
    // Check the number of proper intersections between the long extended line and the polygon.
    int numProperIntersections = 0;
    vector<int> indicesOfIntersectingEdges;
    
    for (int j = 0; j < polygonSegments.size(); j++) {
        if (intersect_proper(extendedLine, polygonSegments[j])) {
            numProperIntersections++;
            indicesOfIntersectingEdges.push_back(j);
        } else if (collinear(polygonSegments[j].start,polygonSegments[j].end,extendedLine.start) && 
                    collinear(polygonSegments[j].start,polygonSegments[j].end,extendedLine.end) ){
            cout << "collinear" << endl;
        }  //TODO: COLLINEAR VISIBILITY EDGES
    }
    // Extend this segment to the edge if an odd number of proper intersections was found.
    if (numProperIntersections % 2 == 1) {
        point2D edgePoint = closestEdgePoint(extendedLine, indicesOfIntersectingEdges);
        if (order.size() > 0){
            for (int i = 0; i< polygonVertices.size(); i++){
                if ( same(polygonVertices[i], visibleAreaVertices[index]) &&
                    same(polygonVertices[i-1], visibleAreaVertices[index-1])){
                    order.push_back(edgePoint);
                }
            }
        // order.push_back(visibleAreaVertices[index]);
        order.insert(order.end()-1,edgePoint);
        }
    }
}

// Call various functions to compute the visible area of the polygon.
void computeVisibleArea(vector<point2D> pgon) {
    computeVisibleVertices(pgon);
    computeSlopes(visibleAreaSegments);
    extendVisibleVertices(visibleAreaVertices);
}

/* ****************************** */
int main(int argc, char** argv) {
    double x = atof(argv[1]), y = atof(argv[2]);
    pos = {x,y};       // Current position
    home = {0.4, 0.5}; // return home location
    
    // PLOT POLYGON
        vector<double> vertex_x = {0.220046082949309,
0.162442396313364,
0.429723502304147,
0.634792626728111,
0.480414746543779,
0.890552995391705,
0.874423963133641};
    vector<double> vertex_y = {0.779883381924198,
0.409620991253644,
0.424198250728863,
0.724489795918367,
0.167638483965015,
0.258017492711370,
0.879008746355685};
    // plt::fill(vertex_x, vertex_y,{}); // uncomment if using matplotlib

    // START TIMER
    struct timeval start, end;
    gettimeofday(&start, NULL);
    ios_base::sync_with_stdio(false);

    // RUN ALGORITHM
    for (int i = 0; i < vertex_x.size(); i++)           // Converts into {x, y} - point2D type
    {
    point2D vertex = {vertex_x[i], vertex_y[i]};
    polygonVertices.push_back(vertex);
    }

    if (inside(polygonVertices, home)){
        constructPolygon(polygonVertices);              // Construct Edges
        computeVisibleArea(polygonVertices);            // Compute Visibility Area 
        vector<double> vis_x, vis_y;

        for (int j = 0; j < order.size(); j++){         // Convert to format readable by Matplotlib
            vis_x.push_back(order[j].x);
            vis_y.push_back(order[j].y);
            cout << "{" << order[j].x << ", " << order[j].y << "}, " << endl;
        }


        // // uncomment if using matplotlib
        // plt::fill(vis_x, vis_y, {});                    // Plot Visibility Polygon 
        // plt::plot({home.x, }, {home.y}, "ko");          // Plot Home Location
        // plt::plot({vis_x}, {vis_y}, "bo-");              // Plot Visibility Polygon Vertices

        if (inside(order, pos)){                        // Check if current location is inside the visibility area.
            // plt::plot({pos.x}, {pos.y}, "go"); // uncomment if using matplotlib
        }else{
            cout << "Current Position is outside" << endl;
            // plt::plot({pos.x}, {pos.y}, "ro"); // uncomment if using matplotlib
        }      

    }else{
        cout << "Home location is not valid." << endl;
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

    // plt::show(); // uncomment if using matplotlib
    return 0;
}


// POLYGON TO TRY:
// vector<double> vertex_x = {0.519585253456221,
//                             0.217741935483871,
//                             0.450460829493088,
//                             0.171658986175115,
//                             0.293778801843318,
//                             0.314516129032258,
//                             0.402073732718894,
//                             0.367511520737327,
//                             0.549539170506913,
//                             0.496543778801843,
//                             0.650921658986175,
//                             0.738479262672811,
//                             0.865207373271889,
//                             0.842165898617512,
//                             0.759216589861751,
//                             0.821428571428571,
//                             0.932027649769585,
//                             0.908986175115207,
//                             0.966589861751152,
//                             0.941244239631336,
//                             0.858294930875576,
//                             0.872119815668203,
//                             0.743087557603687,
//                             0.641705069124424,
//                             0.563364055299539,
//                             0.660138248847926,
//                             0.770737327188940,
//                             0.683179723502304,
//                             0.800691244239631,
//                             0.876728110599078,
//                             0.782258064516129,
//                             0.620967741935484,
//                             0.586405529953917,
//                             0.519585253456221,
//                             0.600230414746544,
//                             0.526497695852535,
//                             0.491935483870968,
//                             0.339861751152074,
//                             0.381336405529954,
//                             0.213133640552995,
//                             0.309907834101383,
//                             0.397465437788018,
//                             0.307603686635945,
//                             0.0633640552995392,
//                             0.0841013824884793,
//                             0.178571428571429};
//     vector<double> vertex_y = {0.637026239067055,
//                             0.534985422740525,
//                             0.430029154518950,
//                             0.0976676384839650,
//                             0.0947521865889213,
//                             0.158892128279883,
//                             0.167638483965015,
//                             0.0714285714285714,
//                             0.0918367346938775,
//                             0.225947521865889,
//                             0.246355685131195,
//                             0.0510204081632653,
//                             0.100583090379009,
//                             0.258017492711370,
//                             0.196793002915452,
//                             0.368804664723032,
//                             0.342565597667638,
//                             0.214285714285714,
//                             0.281341107871720,
//                             0.497084548104956,
//                             0.453352769679300,
//                             0.587463556851312,
//                             0.500000000000000,
//                             0.322157434402332,
//                             0.365889212827988,
//                             0.497084548104956,
//                             0.669096209912536,
//                             0.741982507288630,
//                             0.803206997084548,
//                             0.846938775510204,
//                             0.881924198250729,
//                             0.876093294460641,
//                             0.922740524781341,
//                             0.846938775510204,
//                             0.797376093294461,
//                             0.736151603498542,
//                             0.776967930029155,
//                             0.811953352769679,
//                             0.881924198250729,
//                             0.890670553935860,
//                             0.759475218658892,
//                             0.756559766763848,
//                             0.648688046647230,
//                             0.715743440233236,
//                             0.581632653061225,
//                             0.602040816326531};