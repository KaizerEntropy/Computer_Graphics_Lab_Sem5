# Raster Line Drawing — DDA vs Bresenham (Qt 6 Widgets)

Implements Q2: a line-drawing tool that plots the segment between two
user-clicked endpoints on a raster grid using **(a) DDA** and
**(b) Bresenham's algorithm**, reporting execution time (ms) for each,
and working correctly for endpoints in **all four quadrants**.

## How it works

- `GridWidget` is the canvas. Its origin (0,0) sits at the widget's centre,
  so clicked points map to positive/negative grid coordinates depending on
  quadrant. Each grid coordinate `(X, Y)` is rendered as exactly **one
  raster pixel** — a filled `gridOffset × gridOffset` screen block — via
  `GridWidget::colorPoint()`.
- **Axes are fully pixelated red.** `GridWidget::drawGridAndAxes()` walks
  every single raster pixel along X=0 and Y=0 (the whole visible row and
  column, not just a thin line through the middle) and colors each one red
  via the same `colorPoint()` used for every other point on the grid — so
  the axis looks and behaves exactly like the rest of the raster content,
  distinct from DDA (pink) and Bresenham (green).
- **Point selection is a locked pair, not an ever-growing list.** Clicking
  the grid calls `MainWindow::on_gridPointClicked`. The first click starts a
  pair; the second click locks it in as `start`/`end` and marks
  `pairReady = true`. A **third** click discards the old pair immediately
  and starts a fresh one. This fixes a earlier bug where line-drawing kept
  indexing `points[n-2]/points[n-1]` on a vector that never got reset —
  after a few clicks that could read a stale or out-of-bounds point, making
  new lines appear to start from `(0,0)`. Now `start`/`end` are the single
  source of truth for "the current pair," and every draw button reads from
  them directly.
- **1) Draw DDA** calls `generate_dda_linePts(start, end)` and renders the
  result in **pink**.
- **2) Draw Bresenham** calls `generate_bresenham_linePts(start, end)` and
  renders the result in **green**.
- **3) Draw Both** runs DDA then Bresenham back-to-back on the *same*
  locked pair, drawing pink then green, and shows both execution times
  side-by-side in a comparison label (`DDA: x ms | Bresenham: y ms`).
- All three draw buttons can be pressed **repeatedly on the same pair** —
  points are only ever cleared when you click a *new* point, so you can
  freely compare 1) vs 2) vs 3) without re-selecting endpoints.
- All three generator paths reuse the nested `MainWindow::myTimer` class
  (mirrors the original `myTimer` design: starts a `QElapsedTimer` in its
  constructor, and `measureTime()` reads `nsecsElapsed()`, converts to ms,
  writes "By <Algorithm> drawing algorithm" + the ms value into the UI, and
  also stashes it in `lastElapsedMs` so "Draw Both" can report both times).
- **Clear Grid** resets everything (points, pair, comparison label) and
  redraws the empty grid + pixelated axes.

## Algorithm notes

- **DDA** (`generate_dda_linePts`): floating-point incremental algorithm.
  `steps = max(|dx|, |dy|)`, then `xinc = dx/steps`, `yinc = dy/steps`;
  each step accumulates and rounds to the nearest integer grid cell. Works
  in all 4 quadrants automatically because `xinc`/`yinc` carry the sign of
  `dx`/`dy`.
- **Bresenham** (`generate_bresenham_linePts`): integer-only algorithm,
  generalized beyond the first octant using `stepX = sign(dx)`,
  `stepY = sign(dy)` and `dx, dy = |dx|, |dy|`. Splits into the "shallow"
  case (`dx >= dy`, step along X, decide Y) and the "steep" case
  (`dx < dy`, step along Y, decide X), each using the standard
  decision-variable update (`2*dy - dx`, etc.) — no floating point, no
  trig, no division.

## Build & run

Requires Qt 6 (Widgets) and a C++17 compiler.

```bash
qmake6 LineDrawingRaster.pro   # or: qmake LineDrawingRaster.pro (Qt5: QT5 works too, minor tweaks)
make
./LineDrawingRaster
```

Or with CMake-less qmake on Windows/macOS: open `LineDrawingRaster.pro` in
Qt Creator and hit Run.

## Verified

This project was compiled and linked cleanly against Qt 6.4.2 with zero
warnings, and smoke-tested (launches and runs its event loop without
crashing). It was additionally checked with automated test harnesses that
simulate real mouse clicks via `QTest::mouseClick`:

- DDA and Bresenham were cross-checked across 8 cases — quadrants I–IV, a
  45° diagonal, and pure horizontal/vertical lines — confirming both always
  produce the exact same start point, end point, and pixel count for the
  same input.
- The full pixelated axis was checked pixel-by-pixel across the visible
  grid range and confirmed solid red on every cell of X=0 and Y=0.
- The exact bug scenario reported earlier (click 2 points → draw → click a
  3rd point → draw again) was reproduced and confirmed fixed: the second
  draw now correctly requires a fresh 2nd click and locks onto the new
  pair, with no stale/garbage point causing a spurious line from `(0,0)`.

## Files

- `mainwindow.h` / `mainwindow.cpp` — `GridWidget` (raster canvas) and
  `MainWindow` (UI, algorithms, timer, event handling)
- `main.cpp` — entry point
- `LineDrawingRaster.pro` — qmake project file
