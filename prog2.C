///////////////////////////////////////////////////////////////////////////////
////Authors: Colton Fuhrmann, Kevin Hilt
////Date: December 8, 2014
////Course: CSC372
////Instructor: Dr. Corwin
////
////Description: 
//		This program uses computational geometry to find the area of a simple
//	  polygon using Pick's theorem. The input is given as a series of (x,y)
//	  points between -100 to 100 in counter-clockwise order. The program
//	  returns the area of the polygon, and the interior and boundary points.
//
//		The algorithm is to draw a line that is outside the shape to every 
//    point from shape_min_x & shape_min_y to shape_max_x & shape_max_y then
//    determine how many times the line intersects any boundary lines. An
//		odd number of intersections means the point is an interior point, and
//		even means the point is outside the polygon. If the line intersects a
//		vertex, we increase max_x++ and check that that same point again.
//
//		All computational geometry functions are Dr.Corwin's functions and
//		can be found towards the end of the file.
//
//		Usage: prog2 <name>
//		NOTE: input file needs to exist as name.in, results will be printed to
//					name.out
// 
//                    
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
#include <cstring>

using namespace std;

double VERTICAL_LINE = -100000;
struct point
  {
  double x;
  double y;

    bool equals( point x_y )
    {
      if( x_y.x == x && x_y.y == y )
        return true;
    
      return false;
    }

		void print()
    {
      cout << "x: " << x << " y: " << y << endl;
    }

    double slope( point a_point)
    {  
     // check for vertical line
     if( x == a_point.x )
       return VERTICAL_LINE;
     
     // check for horizontal line
     else if( y == a_point.y )
       return 0;
   
     return ( y - a_point.y ) / ( x - a_point.x );
    }
  };

void Usage();
double min(double a, double b);
double max(double a, double b);
double cross(point a, point b);
bool on(point a, point b, point c);
double direction(point a, point b, point c);
bool turn(point a, point b, point c);
bool clockwise(point p[]);
bool intersect(point p1, point p2, point p3, point p4);
double area(point p[], int n);
int main( int argc, char **argv)
{  
  //Since grid is from -100 to 100, maximum number of points in any list will be 200^2

	const int array_size = 40000;

	point i_points[array_size];
	int i_points_length = 0;
	point b_points[array_size];
	int b_points_length = 0;
	point vertices[array_size];
	int vertices_length = 0;

	int max_x = -101;
	int max_y = -101;
	int min_x = 101;
	int min_y = 101;

	int i, current_x, current_y;
	int vertex;
  double area;

  if( argc != 2 )
  Usage();

  // get input and output name from command line and append .in & .out
  string input_name(argv[1]);
  string output_name(argv[1]);
  input_name.append(".in");
  output_name.append(".out");

  ifstream fin;
  fin.open(input_name.c_str());
  if( !fin.is_open())
  {
    cout << "Input file failed to open, exiting." << endl;
    exit(0);
  }

  i = 0;
  while( fin >> vertices[i].x )
  {
    fin >> vertices[i].y;
    i++;
  }
  // copy first x & y point into last spot in vertices array so when looping
  // through, we can connect the polygon without special casing
  vertices[i].x = vertices[0].x;
  vertices[i].y = vertices[0].y;
  vertices_length = i + 1;
  

  //Get the maximum and minimum y values in the vertex list
  for(i = 0; i < vertices_length; i++)
  {
    current_x = vertices[i].x;
    current_y = vertices[i].y;
   
    if(current_x > max_x)
    {
        max_x = current_x;
    }
   
    if(current_x < min_x)
    {
        min_x = current_x;
    }
   
    if(current_y > max_y)
    {
        max_y = current_y;
    }
   
    if(current_y < min_y)
    {
        min_y = current_y;
    }
  }

//since we'll mostly be comparing to/using max_x + 1 and max_y + 1, add 1 to each
max_x += 1;
max_y += 1;

bool is_vertex = 0;
bool is_on_line = 0;
bool is_intersect = 0;
//Check all points in the box from (min_x, min_y) to (max_x, max_y)
//to determine if they are i_points, b_points, or outside
for(current_x = min_x; current_x < max_x; current_x++)
{
    for(current_y = min_y; current_y < max_y; current_y++)
    {
        is_vertex = 0;
        is_on_line = 0;
        is_intersect = 0;
        point current_point;
        current_point.x = current_x;
        current_point.y = current_y;

       //if vertex, put in border array
       for( vertex = 0; vertex < vertices_length && !is_vertex; vertex++ )
       {
         if( current_point.equals( vertices[vertex] ))
         { 
           b_points[b_points_length] = current_point;
           b_points_length++;
           is_vertex = true; 
         }
       }
 
       //else, pick check what vertices are on the border
       if( is_vertex == false )
       {
					for( vertex = 0; vertex < vertices_length - 1 && !is_on_line; vertex++ )
          {
						if( current_point.slope(vertices[vertex]) == current_point.slope(vertices[vertex+1]))
            {
              is_on_line = on( vertices[vertex], vertices[vertex+1], current_point );
              if( is_on_line == true )
              {
							  b_points[b_points_length] = current_point;
                b_points_length++;
              }
            }
          }
       }

       // set max point that will be used to test if point is inside or outside polygon
       point max_point;
       max_point.x = max_x;
       max_point.y = max_y;
       int intersection_count = 0;
       bool vertex_intersect = 0;
       if( is_on_line == false && is_vertex == false )
       {
         //check how many times current point intersects with polygon
         for( vertex = 0; vertex < vertices_length -1; vertex++ )
         {  
            vertex_intersect = false;
 				    is_intersect = intersect(max_point, current_point, vertices[vertex], vertices[vertex+1]);
 
            // the line from max_point to current_point intersected with 
            // vertices[vertex] & vertices[vertex +1]
            if( is_intersect == true )
            {
                //increment intersection count, even total means point is outside
                intersection_count++;

               //check if intersection point was a vertex, if so, increment
               //max_point.x, set vertex to 0 to reset loop and try again
               int j;
							 for( j = 0; j < vertices_length - 1 && !vertex_intersect; j++ )
               {
                  if( current_point.slope(vertices[j]) == current_point.slope(max_point))
                  {
										// if current vertex isn't on line from max_point to current_point
				            // keep is_intersect set as true and check the other vertices
				            if (on(max_point, current_point, vertices[j]))
				            {
				              vertex_intersect = true;
				            }
                  }
               }
               
               // intersect() returned true, but line intersected with a vertex
               // so set is_intersect to false, increment max x and try again
               // by setting vertex = 0
               if( vertex_intersect == true )
               {
                 max_point.x ++;
                 vertex = -1;
                 intersection_count = 0;
                 is_intersect = false;
               }

             }    
           }//end checking how many times current point intersects w/ polygon

          if( intersection_count > 0 )
          {
		          //cout << "count : " << intersection_count << endl;
		      	// if the intersection count is odd, then current_point is inside of polygon
		     		if( intersection_count % 2 == 1 )
		     		{
		     		  i_points[i_points_length] = current_point;
		     		  i_points_length++;
		    		}
          }
        }
     }
 }
    area = i_points_length + (b_points_length/2) - 1;
    ofstream fout;
    fout.open(output_name.c_str());
    fout << "Reading input from file " << input_name << "\n";
    fout << "Area = " << area << "\n";
    fout << "I(P) = " << i_points_length << "\n";
    fout << "B(P) = " << b_points_length << "\n";
    fout << "Interior points\n";
 		for( i = 0; i < i_points_length; i++)
		{
		 fout << "     " << i_points[i].x << " ";
		 fout << i_points[i].y << "\n";
		}
    
    fout << "Boundary points\n";
		for( i = 0; i < b_points_length; i++)
		{
		 fout << "     " << b_points[i].x << " ";
		 fout << b_points[i].y << "\n";
		}

  fout.close();
  fin.close();

  return 0;
}


void Usage()
{
  cout << "Incorrect number of arguments." << endl;
  cout << "Usage: supply name for input and output files" << endl;
  cout << "prog2 <input & output filename>" << endl;
  exit(0);
}




// The code under this line is all from Dr. Corwin's website

////////////////////////////////////////////////////////////////////
//                                                                //
//            Functions for Computational Geometry assignments    //
//                                                                //
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

double min(double a, double b)
  {
  if (a < b)
    return a;
  return b;
  }

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

double max(double a, double b)
  {
  if (a > b)
    return a;
  return b;
  }

////////////////////////////////////////////////////////////////////
//          Cross product of vectors a and b                      //
////////////////////////////////////////////////////////////////////

double cross(point a, point b)
  {
  return a.x * b.y - a.y * b.x;
  }

////////////////////////////////////////////////////////////////////
//      Is c on the line segment from a to b?                     //
//      Assumes c is on the line from a to b                      //
////////////////////////////////////////////////////////////////////

bool on(point a, point b, point c)
  {
  if (((min(a.x, b.x) <= c.x) && (c.x <= max(a.x, b.x))) &&
      ((min(a.y, b.y) <= c.y) && (c.y <= max(a.y, b.y))))
    return true;
     
  return false;
  }

////////////////////////////////////////////////////////////////////
//       Compute cross product of vector from a to b and          //
//       vector from b to c                                       //
//       Set to zero if close to zero                             //
////////////////////////////////////////////////////////////////////

double direction(point a, point b, point c)
  {
  point ab;
  point bc;
  double result;

  ab.x = b.x - a.x;
  ab.y = b.y - a.y;
  bc.x = c.x - b.x;
  bc.y = c.y - b.y;
  result =  cross(ab, bc);
  if (fabs(result) < 1.0e-6)
    result = 0.0;
  return result;
  }

////////////////////////////////////////////////////////////////////
//    Is the turn from a to b to c clockwise?                     //
////////////////////////////////////////////////////////////////////

bool turn(point a, point b, point c)
  {
  return (direction(a, b, c) > 0);
  }

////////////////////////////////////////////////////////////////////
//    Is the turn from p[0] to p[1] to p[2] clockwise?            //
////////////////////////////////////////////////////////////////////

bool clockwise(point p[])
  {
  return turn(p[0], p[1], p[2]);
  }

////////////////////////////////////////////////////////////////////
//     From text, page 937 (second edition)                       //
//     Does line segment from p1 to p2 intersect                  //
//     line segment from p3 to p4?                                //
////////////////////////////////////////////////////////////////////

bool intersect(point p1, point p2, point p3, point p4)
  {
  double d1;
  double d2;
  double d3;
  double d4;

  d1 = direction(p3, p4, p1);
  d2 = direction(p3, p4, p2);
  d3 = direction(p1, p2, p3);
  d4 = direction(p1, p2, p4);

  if ((((d1 > 0) && (d2 < 0)) || ((d1 < 0) && (d2 > 0))) &&
      (((d3 > 0) && (d4 < 0)) || ((d3 < 0) && (d4 > 0))))
    return true;

  if ((d1 == 0) && on(p3, p4, p1))
    return true;

  if ((d2 == 0) && on(p3, p4, p2))
    return true;

  if ((d3 == 0) && on(p1, p2, p3))
    return true;

  if ((d4 == 0) && on(p1, p2, p4))
    return true;

  return false;
  }

////////////////////////////////////////////////////////////////////
//          Area of polygon by adding/subtracting trapezoids      //
////////////////////////////////////////////////////////////////////

double area(point p[], int n)
  {
  int i;
  int j;
  double result;

  result = 0;
  for (i = 0; i < n; i++)
    {
    j = (i + 1) % n;
    result += p[i].x * p[j].y;
    result -= p[i].y * p[j].x;
    }
  return fabs(result / 2);
  }

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



