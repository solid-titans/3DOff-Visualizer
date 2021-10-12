#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget
{
public:
    GLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

public slots:
    void toggleBackgroundColor(int toBlack);
};

#endif // GLWIDGET_H
