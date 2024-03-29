# polygon_algorithms

This repository contains polygon algorithms of the following:

1. Point in Polygon
2. Line in Polygon
3. Visibility Polygon

## Point in Polygon 

**Requirements:**

[Matplotlib](https://github.com/lava/matplotlib-cpp) for C++ (recommended)

**Files to be saved in the same folder:**

* ptinpoly.cpp (main script)
* geom.h (header file for helper functions)
* geom.cpp (cpp file for helper functions)
* matplotlibcpp.h (for plotting)

**How to compile:**

Run in command line (tested in linux)

`g++ ptinpoly.cpp geom.cpp -o ptinpoly -I/usr/include/python3.8 -lpython3.8`  (if using matplotlib)

or 

`g++ ptinpoly.cpp geom.cpp -o ptinpoly`

**How to run additional test cases:** (Suggested)

1. Using MATLAB, use [drawpolygon()](https://www.mathworks.com/help/images/ref/drawpolygon.html) function to draw any desired polygon 
2. Retrieve coordinates of the vertices from MATLAB workspace.
3. Copy and Paste into ptinpoly.cpp (need to add commas after each coordinate)
4. Re-compile

**How it works**

The point-in-polygon code utilises a Ray-Casting Algorithm to check if a given point is inside a simple convex/concave polygon. [Computational Geometry in C 2nd edition by Joseph O’Rourke, page 239.](https://github.com/sarcilav/analisis-numerico/blob/master/doc/Computational%20Geometry%20In%20C%202nd%20ed.%20-%20J.%20O%27Rourke%20(1997)%20WW.pdf)

Reference for helper functions: [Zackery Leman & Ivy Xing](https://github.com/ivyxing/PolygonVisibility/tree/master/PolygonVisibility)

**Step 1:** Iterate through every vertex of polygon. 

**Step 2:** Check the following:

1. The position is between the current vertex and the next, and 
2. Position is collinear to both vertices. 
If the position is between 2 vertices and collinear, it is on the edge of the polygon, thus return 'outside'. 

**Step 3:** Check if the y coordinates of both current and next vertex are on the same side of the horizontal ray (from the position) or lying on the ray. 

This will generate 4 different conditions:
1. Both on opposite sides (inside)
2. 1 above, 1 on the ray (inside)
3. 1 below, 1 on the ray (outside)
4. Both on the ray (outside)
    
**Step 4:** If we get conditions 1 and 2 from step 3, check if the position is on the left or right side of the vertices. 

As the horizontal ray is in the direction of the positive x-axis, the position must be on the left of the 2 vertices in order to intersect. [Cross product](https://www.geeksforgeeks.org/direction-point-line-segment/) is used to determine where the point is with respect to the two vertices. 

**Step 5:** For every intersection in step 4, the inside_flag is switched. A better way to understand is: Odd number of intersection = inside, Even number of intersection = outside.

## Line in Polygon 

**Requirements:**

[Matplotlib](https://github.com/lava/matplotlib-cpp) for C++ (recommended)

**Files to be saved in the same folder:**
* lineinpoly.cpp (main script)
* geom.h (header file for helper functions)
* geom.cpp (cpp file for helper functions)
* matplotlibcpp.h (for plotting)

**How to compile:**

Run in command line (tested in linux)

`g++ lineinpoly.cpp geom.cpp -o lineinpoly -I/usr/include/python3.8 -lpython3.8` (if using matplotlib)

or 

`g++ lineinpoly.cpp geom.cpp -o lineinpoly`

**How to run additional test cases:** (Suggested)

1. Using MATLAB, use [drawpolygon()](https://www.mathworks.com/help/images/ref/drawpolygon.html) function to draw any desired polygon 
2. Retrieve coordinates of the vertices from MATLAB workspace.
3. Copy and Paste into lineinpoly.cpp (need to add commas after each coordinate)
4. Re-compile

**How it works**

The line-in-polygon utilises a naive algorithm to check if the line (formed by two points) is not fully contained within a concave polygon. The algorithm iteratively checks for intersection between the line segment and every edge of the polygon. 

Reference for helper functions: [Zackery Leman & Ivy Xing](https://github.com/ivyxing/PolygonVisibility/tree/master/PolygonVisibility)

**Step 1:** Check if two given points are inside the polygon (using the point-in-polygon algorithm above). 

**Step 2:** Construct line segment from two given points.

**Step 3:** Iterate through every line segment and check for intersection. 

**Step 4:** If intersection exists, line is not fully contained within the concave polygon. 

## Visibility Polygon

**Requirements:**

[Matplotlib](https://github.com/lava/matplotlib-cpp) for C++ (recommended)

**Files to be saved in the same folder:**
* visibility.cpp (main script)
* geom.h (header file for helper functions)
* geom.cpp (cpp file for helper functions)
* matplotlibcpp.h (for plotting)

**How to compile:**

Run in command line (tested in linux)

`g++ visibility.cpp geom.cpp -o visibility -I/usr/include/python3.8 -lpython3.8` (if using matplotlib)

or 

`g++ visibility.cpp geom.cpp -o visibility`

**How to run additional test cases:** (Suggested)

1. Using MATLAB, use [drawpolygon()](https://www.mathworks.com/help/images/ref/drawpolygon.html) function to draw any desired polygon 
2. Retrieve coordinates of the vertices from MATLAB workspace.
3. Copy and Paste into lineinpoly.cpp (need to add commas after each coordinate)
4. Re-compile

**How it works**

The visibility polygon code belongs to [Zackery Leman & Ivy Xing](https://github.com/ivyxing/PolygonVisibility/tree/master/PolygonVisibility). The algorithm utilises a naive ray casting to every vertex method, which has a time complexity of O(n^{2}) as described in [Wikipedia](https://en.wikipedia.org/wiki/Visibility_polygon). The original algorithm can display the visibility polygon, however, the algorithm is unable to return the vertices of the visibility polygon. My work mainly retrieves the visibility vertices and sort them in an counter-clockwise order, along with other minor modifications.

To compute the visibility area, the algorithm performs five main steps:

**Step 1:** Iterate through every vertices of the original. If the vertex is visible from the point q, it is added to the vector of visible vertices.

**Step 2:** Compute the gradient of the line connecting each visible vertex to point z. 

**Step 3:** Using the gradient, construct a ray from each visible vertex and count the number of intersection of the original polygon. 

**Step 4:** If there is odd number of intersection, compute the coordinates of the closest point of intersection from point z. (new vertex of the visibility polygon)  

**Step 5:** Using the original concave polygon as reference, sort the visibility vertices. 




