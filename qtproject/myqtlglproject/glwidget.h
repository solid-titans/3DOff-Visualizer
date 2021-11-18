#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QGLShader>
#include <QTimer>

#include <iostream>
#include <fstream>
#include <limits>

#include "camera.h"
#include "light.h"
#include "material.h"
#include "trackball.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    virtual ~GLWidget();
signals:
    void statusBarMessage(QString ns);

public slots:
    void toggleBackgroundColor(bool toBlack);
    void showFileOpenDialog();
    void animate();
    void takeScreenshot();

protected:
    void initializeGL();
    void resigeGL(int width, int height);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void readOFFFile(const QString &filename);
    void genNormals();
    void genTexCoordsCylinder();
    void genTangents();
    void createVBOs();
    void destroyVBOs();
    void createShaders();
    void destroyShaders();

    unsigned int numVertices;
    unsigned int numFaces;
    unsigned int *indices;
    unsigned int currentShader;

    QVector4D *vertices;
    QVector4D *tangents;

    QVector3D *normals;

    QVector2D *texCoords;

    QGLBuffer *vboVertices;
    QGLBuffer *vboNormals;
    QGLBuffer *vboTexCoords;
    QGLBuffer *vboTangents;
    QGLBuffer *vboIndices;

    QGLShader *vertexShader;
    QGLShader *fragmentShader;

    QGLShaderProgram *shaderProgram;

    int texID[2];

    QMatrix4x4 modelViewMatrix;
    QMatrix4x4 projectionMatrix;

    Camera camera;
    Light light;
    Material material;

    TrackBall trackBall;

    double zoom;

    QTimer timer;

};

#endif // GLWIDGET_H
