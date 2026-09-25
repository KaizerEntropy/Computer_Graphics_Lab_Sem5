# Assignment 3: Ellipse Drawing Algorithms

**Course:** Computer Graphics Lab
**Semester:** B.C.S.E. Third Year

## Objective
Implement an ellipse drawing algorithm to draw an ellipse with given radii (Rx, Ry) in the raster grid using:
1. Polar Coordinates Algorithm
2. Bresenham's Midpoint Ellipse Algorithm

Additionally, check and compare the execution time of both algorithms in nanoseconds (ns).

## Methodology

### 1. Polar Coordinates Ellipse Algorithm
The polar coordinates algorithm computes the boundary points of an ellipse using its parametric equations. We iterate the angle θ (theta) from 0 to π/2 to find the points in the first quadrant. 
- **Equations used:** 
  `x = Rx * cos(θ)`
  `y = Ry * sin(θ)`
- **Symmetry:** The generated point `(x, y)` in the first quadrant is reflected to `(-x, y)`, `(x, -y)`, and `(-x, -y)` utilizing 4-way symmetry.
- **Computational Cost:** This method involves expensive floating-point arithmetic and trigonometric function calls (`cos`, `sin`) for every pixel, making it relatively slow.

### 2. Bresenham's Midpoint Ellipse Algorithm
Bresenham's Midpoint Algorithm is an optimized, integer-based approach. The first quadrant of the ellipse is divided into two distinct regions based on the slope of the curve.
- **Region 1:** Where the slope magnitude is less than 1 (dx/dy < 1). The decision parameter `p1` determines whether to decrement `y` or keep it the same when `x` increments.
- **Region 2:** Where the slope magnitude is greater than or equal to 1. The decision parameter `p2` dictates whether to increment `x` when `y` decrements.
- **Efficiency:** The algorithm operates predominantly using addition, subtraction, and multiplication of integers (and bit-shifting, implicitly or explicitly), making it highly optimized for raster devices.
- **Symmetry:** Like the polar approach, it only computes points for the first quadrant and uses 4-way symmetry for the rest.

## Performance Analysis
The application integrates `QElapsedTimer` to measure the precise execution time of each algorithm in nanoseconds (ns).

- **Polar Algorithm:** Consistently exhibits a higher execution time because of the repeated use of floating-point computations and trigonometric operations.
- **Midpoint Algorithm:** Consistently executes significantly faster than the Polar method, confirming its efficiency due to its integer-only arithmetic operations.

## Conclusion
Both algorithms successfully and accurately render an ellipse with arbitrary semi-major and semi-minor axes on the raster grid. However, for practical computer graphics rendering, Bresenham's Midpoint Algorithm is vastly superior in performance and efficiency as it avoids floating-point mathematics entirely.
