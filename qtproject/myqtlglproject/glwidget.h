#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLBuffer>
#include <QGLShader>

#include<iostream>
#include<fstream>
#include<limits>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    virtual ~GLWidget();

protected:
    void initializeGL();
    void resigeGL(int width, int height);
    void paintGL();

signals:

public slots:
    void toggleBackgroundColor(bool toBlack);
    void showFileOpenDialog();

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


};

#endif // GLWIDGET_H
