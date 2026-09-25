# Assignment 4: Polygon Fill Algorithms

**Course**: Computer Graphics Lab  
**Semester**: B.C.S.E. Third Year  

## Objective
1. Implement seed-fill algorithms:
   - Boundary fill
   - Flood fill
2. Implement the scanline fill algorithm:
   - Draw a closed polygon and fill it.

## Implementation Details

The project is built using C++ and the Qt Framework. The UI provides a grid, drawing tools, and controls for the different fill algorithms.

### 1. Seed-fill Algorithms

Seed-fill algorithms start from a given point (the seed) and recursively or iteratively fill contiguous regions of a specified color. To prevent stack overflow issues on large regions, both algorithms are implemented using a stack-based iterative approach rather than recursion.

#### Boundary Fill
- **Concept**: Fills an enclosed area defined by a specific boundary color. It starts from the seed point and expands outwards. If it encounters a pixel that is neither the boundary color nor the fill color, it colors the pixel and pushes its 4-connected neighbors onto the stack.
- **Usage**: Click `Set Vertex` a few times to draw boundary lines (default boundary is Red). Select a fill color from the dropdown, pick a point inside the boundary by clicking on the grid, and press `Boundary Fill`.

#### Flood Fill
- **Concept**: Fills an area of a specific target color (the old color). It starts from the seed point, reads its color, and then expands. It replaces any connected pixels of the old color with the new fill color.
- **Usage**: Draw some shapes, select a fill color, click inside an area of a uniform color on the grid, and press `Flood Fill`.

### 2. Scanline Fill Algorithm

- **Concept**: Fills a polygon by finding intersections between scanlines (horizontal lines) and the edges of the polygon.
- **Implementation**:
  - The user defines a polygon by plotting points using `Set Vertex`.
  - The user closes the polygon using `Close Polygon`.
  - Upon clicking `Scanline Fill`, the algorithm extracts all edges (ignoring horizontal ones).
  - It maintains an Edge Table (ET) indexed by the minimum Y-coordinate of each edge.
  - As it sweeps from the global minimum Y to maximum Y, it maintains an Active Edge Table (AET). The AET is updated by adding new edges, removing edges that end at the current Y, and updating X intersections.
  - The intersections are sorted by X coordinate, and pixels are filled in pairs (from `x_start` to `x_end`).
- **Usage**: Click `Set Vertex` to add polygon corners, click `Close Polygon` to connect the last to the first, pick a fill color, and hit `Scanline Fill`.

## Qt Specifics
- The grid is drawn on a `QPixmap`.
- For fast pixel manipulation during seed-fill, `QPixmap::toImage()` is used to read pixel colors directly from memory (`img.pixelColor()`), and a `QPainter` operates on the `QImage` to perform the filling.
- Drawing operations (DDA line drawing for boundaries and scanline fills) use `QPainter` with standard coordinate mapping.
- All drawing operations are measured and their execution time is displayed in nanoseconds.
