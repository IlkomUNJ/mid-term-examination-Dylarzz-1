#include "drawingcanvas.h"
#include <fstream>
#include <iostream>


using namespace std;

bool DrawingCanvas::matchesPattern(bool local_window[3][3],
                                   const std::vector<std::vector<std::vector<bool>>>& idealPatterns)
{
    for (const auto& pattern : idealPatterns) {
        bool match = true;

        for (int x = 0; x < 3 && match; ++x) {
            for (int y = 0; y < 3; ++y) {
                if (local_window[x][y] != pattern[x][y]) {
                    match = false;
                    break;
                }
            }
        }

        if (match)
            return true;
    }

    return false;
}


DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    // Trigger a repaint to clear the canvas
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
    */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();

    cout << "image width " << image.width() << endl;
    cout << "image height " << image.height() << endl;

    //To not crash we set initial size of the matrix
    vector<CustomMatrix> windows;
    windows.reserve(image.width()*image.height());

    // Dump Window
    ofstream dumpFile("window_dump.txt");

    // Define ideal patterns untuk segment detection
    // Pattern untuk line segments (horizontal, vertical, diagonal)
    std::vector<std::vector<std::vector<bool>>> idealPatterns;

    // Horizontal line patterns
    idealPatterns.push_back({
        {false, false, false},
        {true, true, true},
        {false, false, false}
    });

    idealPatterns.push_back({
        {false, false, false},
        {false, true, true},
        {false, false, false}
    });

    idealPatterns.push_back({
        {false, false, false},
        {true, true, false},
        {false, false, false}
    });

    // Vertical line patterns
    idealPatterns.push_back({
        {false, true, false},
        {false, true, false},
        {false, true, false}
    });

    idealPatterns.push_back({
        {false, true, false},
        {false, true, false},
        {false, false, false}
    });

    idealPatterns.push_back({
        {false, false, false},
        {false, true, false},
        {false, true, false}
    });

    // Diagonal line patterns (top-left to bottom-right)
    idealPatterns.push_back({
        {true, false, false},
        {false, true, false},
        {false, false, true}
    });

    idealPatterns.push_back({
        {true, false, false},
        {false, true, false},
        {false, false, false}
    });

    idealPatterns.push_back({
        {false, false, false},
        {false, true, false},
        {false, false, true}
    });

    // Diagonal line patterns (top-right to bottom-left)
    idealPatterns.push_back({
        {false, false, true},
        {false, true, false},
        {true, false, false}
    });

    idealPatterns.push_back({
        {false, false, true},
        {false, true, false},
        {false, false, false}
    });

    idealPatterns.push_back({
        {false, false, false},
        {false, true, false},
        {true, false, false}
    });

    // Clear previous candidates
    segmentCandidates.clear();

    // Get the pixel value as an ARGB integer (QRgb is a typedef for unsigned int)
    for(int i = 1; i < image.width()-1;i++){
        for(int j = 1; j < image.height()-1;j++){
            bool local_window[3][3] = {false};
            bool isEmpty = true;

            for(int m=-1;m<=1;m++){
                for(int n=-1;n<=1;n++){
                    QRgb rgbValue = image.pixel(i+m, j+n);
                    local_window[m+1][n+1] = (rgbValue != 0xffffffff);
                    if(local_window[m+1][n+1]) isEmpty = false;
                }
            }

            if(!isEmpty){
                CustomMatrix mat(local_window);
                windows.push_back(mat);

                dumpFile <<"Position (" <<i <<","<< j <<"):"<<endl;
                for(int x=0; x<3; x++){
                    for(int y=0; y<3; y++){
                        dumpFile << (local_window[x][y]?"1":"0")<<" ";
                    }
                    dumpFile << endl;
                }
                dumpFile << "---" << endl;

                if (matchesPattern(local_window, idealPatterns)) {
                    segmentCandidates.append(QPoint(i, j));
                }
            }
        }
    }

    dumpFile.close();
    cout << "Total non-empty windows: " << windows.size() << endl;
    cout << "Segment candidates found: " << segmentCandidates.size() << endl;

    // Tambahkan flag & repaint
    isSegmentDetectionClicked = true;
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(4); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.
        painter.setPen(pen);

        painter.setRenderHint(QPainter::Antialiasing, false);

        for(int i=0;i<m_points.size()-1;i+=2){
            //cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i+1]);
        }

        painter.setRenderHint(QPainter::Antialiasing, true);

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }

    if(isSegmentDetectionClicked){
        pen.setColor(Qt::magenta);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        // Loop untuk deteksi pattern
        for(const QPoint& detectedPos : std::as_const(segmentCandidates)){
            // Draw small purple rectangle
            painter.drawRect(detectedPos.x()-2, detectedPos.y()-2, 4, 4);
        }
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}
