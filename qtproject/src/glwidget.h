#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_GLEXT_PROTOTYPES
#include <QGLWidget>
#include <QWidget>
#include<QtOpenGL>
#include <GL/glu.h>
#include "camera.h"
#include "light.h"
#include "material.h"
#include "trackball.h"
#include <map>

class GLWidget : public QGLWidget
{

    const std::map<std::string,int> available_shaders = {
        {"Gouraud",0},
        {"Phong",1},
        {"Texture",2},
        {"Normal",3}
    };

    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    virtual ~GLWidget();

signals:
    void statusBarMessage(QString ns);

public slots:
    void chooseBackgroundColor();
    void showFileOpenDialog ();
    void takeScreenshot();
    void animate();
    void changeShader(const QString shaderName);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void wheelEvent ( QWheelEvent * event );
    void keyPressEvent ( QKeyEvent * event );

private:
    void readOFFFile ( const QString & fileName );
    void genNormals ();
    void genTexCoordsCylinder ();
    void genTangents ();
    void createVBOs ();
    void destroyVBOs ();
    void createShaders ();
    void destroyShaders ();

    QPointF pixelPosToViewPos(const QPointF &p);

    unsigned int numVertices;
    unsigned int numFaces;
    QVector4D *vertices;
    QVector3D *normals;
    QVector2D *texCoords;
    QVector4D *tangents;
    unsigned int *indices;

    QGLBuffer *vboVertices;
    QGLBuffer *vboNormals;
    QGLBuffer *vboTexCoords;
    QGLBuffer *vboTangents;
    QGLBuffer *vboIndices;

    QGLShader *vertexShader;
    QGLShader *fragmentShader;
    QGLShaderProgram *shaderProgram;
    unsigned int currentShader;

    int texID[2];

    QMatrix4x4 modelViewMatrix;
    QMatrix4x4 projectionMatrix;

    Camera camera;
    Light light;
    Material material;

    TrackBall trackBall;

    double zoom;

    QTimer timer;

    QOpenGLFunctions texture;
};



#endif // GLWIDGET_H
