# Pick-s-Theorem-Solution
Analysis of Algorithms program#2, worked on as team of 2


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
