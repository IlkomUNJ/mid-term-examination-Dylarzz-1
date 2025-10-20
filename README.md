[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XxdT5pUo)
Repository for final lesson before mid term of computer graphic course

## Objective
1. Analyze and finding correct window size in identifying a segment.
2. Report all fitting of appropriate window pattern for identifying a segment.
3. Develop automated algorithm based on the previous findings to detect all potential candidates
of a segment

# No.1
In this part, a 3x3 window is used to analyze the line patterns drawn on the canvas.
This size is chosen because it is small enough to focus on a pixel’s local area, but still large enough to capture the direction of a line (horizontal, vertical, or diagonal).

Each non-empty pixel is taken along with its eight surrounding pixels and stored in a file called window_dump.txt.
From this file, we can observe the 3x3 patterns that appear along the drawn lines.

Example of a detected window pattern for a vertical line segment:
0 1 0
0 1 0
0 1 0

and for the horizontal
0 0 0
1 1 1
0 0 0

Through this process, the 3×3 window is confirmed to be an appropriate and effective size for detecting simple line segments, as it captures both the center pixel and its immediate neighbors in all directions.

# No.2
From the analysis of the window_dump.txt file, several 3x3 ideal patterns were found to represent different line orientations.
These ideal patterns will later be used as references for detecting line segments automatically.

Line Type
Horizontal
{   {0,0,0},
    {1,1,1},
    {0,0,0} }

Vertical
{   {0,1,0},
    {0,1,0},
    {0,1,0} }

Diagonal right down
{   {1,0,0},
    {0,1,0},
    {0,0,1} }

Diagonal left down
{   {0,0,1},
    {0,1,0},
    {1,0,0} }

These four patterns represent the main possible line directions that can appear in the image.
They will be used in the next step to match the patterns detected on the canvas during the automatic detection process.

# Link AI
https://gemini.google.com/share/41fbe59382b9