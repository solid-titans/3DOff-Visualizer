#include "glwidget.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>

#include <cmath>


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent) {

    vertices  = NULL;
    normals   = NULL;
    texCoords = NULL;
    tangents  = NULL;
    indices   = NULL;

    vboVertices  = NULL;
    vboNormals   = NULL;
    vboTexCoords = NULL;
    vboTangents  = NULL;
    vboIndices   = NULL;

    shaderProgram  = NULL;
    vertexShader   = NULL;
    fragmentShader = NULL;

    currentShader = 0;
}

GLWidget::~GLWidget() {
    destroyVBOs();
    destroyShaders();
}

// Cleaning the shaders pipeline
void GLWidget::destroyShaders() {
    // Cleaning the vertexShader object
    delete vertexShader;
    vertexShader = NULL;

    // Cleaning the fragmentShader object
    delete fragmentShader;
    fragmentShader = NULL;

    // if shaderProgram != NULL clean it too
    if (shaderProgram) {
        shaderProgram->release();
        delete shaderProgram;
        shaderProgram = NULL;
    }
}

// Freeing the memory of all VBOs
void GLWidget::destroyVBOs() {
    if(vboVertices) {
        vboVertices->release();
        delete vboVertices;
        vboVertices = NULL;
    }

    if(vboNormals) {
        vboNormals->release();
        delete vboNormals;
        vboNormals = NULL;
    }

    if(vboTexCoords) {
        vboTexCoords->release();
        delete vboTexCoords;
        vboTexCoords = NULL;
    }

    if(vboTangents) {
        vboTangents->release();
        delete vboTangents;
        vboTangents = NULL;
    }

    if(vboIndices) {
        vboIndices->release();
        delete vboIndices;
        vboIndices = NULL;
    }
}

void GLWidget::initializeGL() {

    QString texturesDir = ":/textures/brick/";

    glEnable(GL_DEPTH_TEST);

    QImage texColor = QImage(texturesDir + "bricksDiffuse.png");
    QImage texNormal = QImage(texturesDir + "bricksNormal.png");
    glActiveTexture(GL_TEXTURE);
    texID[0] = bindTexture(texColor);
    glActiveTexture(GL_TEXTURE);
    texID[1] = bindTexture(texNormal);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));

    timer.start(0);
}

void GLWidget::resigeGL(int width, int height) {
    glViewport(0,0,width,height);

    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0,
              static_cast<qreal>(width) /
              static_cast<qreal>(height), 0.1, 20.0);

    trackBall.resizeViewport(width,height);

    updateGL();
}

void GLWidget::paintGL () {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!vboVertices)
        return;

    modelViewMatrix.setToIdentity();
    modelViewMatrix.lookAt(camera.eye,camera.at,camera.up);
    modelViewMatrix.translate(0, 0, zoom);
    modelViewMatrix.rotate(trackBall.getRotation());

    shaderProgram->bind() ;

    shaderProgram->setUniformValue("modelViewMatrix",
    modelViewMatrix);

    shaderProgram->setUniformValue("projectionMatrix",
    projectionMatrix) ;

    shaderProgram->setUniformValue("normalMatrix",
    modelViewMatrix.normalMatrix());

    QVector4D ambientProduct = light.ambient * material.
    ambient;

    QVector4D diffuseProduct = light.diffuse * material.
    diffuse;

    QVector4D specularProduct = light.specular * material.
    specular;

    shaderProgram->setUniformValue("lightPosition", light.
    position);

    shaderProgram->setUniformValue("ambientProduct",
    ambientProduct) ;

    shaderProgram->setUniformValue("diffuseProduct",
    diffuseProduct);

    shaderProgram->setUniformValue("specularProduct",
    specularProduct) ;

    shaderProgram->setUniformValue ("shininess", static_cast<GLfloat>(material.shininess));

    shaderProgram->setUniformValue("texColorMap", 0);
    shaderProgram->setUniformValue("texNormalMap", 1);
    glActiveTexture(GL_TEXTURE0) ;
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texID[1]);

    vboVertices ->bind();
    shaderProgram->enableAttributeArray("vPosition");
    shaderProgram->setAttributeBuffer("vPosition", GL_FLOAT,
    0, 4, 0);

    vboNormals->bind();
    shaderProgram->enableAttributeArray("vNormal");
    shaderProgram->setAttributeBuffer("vNormal", GL_FLOAT,
    0, 3, 0);

    vboTexCoords ->bind() ;
    shaderProgram->enableAttributeArray("vTexCoord");
    shaderProgram->setAttributeBuffer("vTexCoord", GL_FLOAT,
    0,2,0);

    vboTangents ->bind() ;
    shaderProgram->enableAttributeArray("vTangent");
    shaderProgram->setAttributeBuffer ("vTangent", GL_FLOAT, 0, 4, 0);
    vboIndices ->bind() ;

    glDrawElements(GL_TRIANGLES, numFaces * 3,GL_UNSIGNED_INT, 0);

    vboIndices->release();
    vboTangents->release();
    vboNormals->release();
    vboVertices->release();
    shaderProgram->release();

}

void GLWidget::animate() {
    updateGL();
}

// Change the background color (White <-> Black)
void GLWidget::toggleBackgroundColor(bool toBlack) {
    if(toBlack)
        glClearColor(0,0,0,1);
    else
        glClearColor(1,1,1,1);

    updateGL();
}

void GLWidget::showFileOpenDialog() {
    // Defining the file format to open
    QByteArray fileFormat = "off";

    // Opening the file dialog from the user's home directory
    QString fileName;
    fileName = QFileDialog :: getOpenFileName ( this ,
    " Open File " ,
    QDir :: homePath () ,
    QString ( " %1 Files (*.%2) " )
    . arg ( QString ( fileFormat . toUpper () ) )
    . arg ( QString ( fileFormat )));

    // Verifying if it's a valid file
    if (!fileName.isEmpty()) {
        readOFFFile(fileName);

        genNormals();           // Generating the normals for the meshes
        genTexCoordsCylinder(); // Generating cylindrical texture coordinates for the mesh vertices
        genTangents();          // Estimating per-vertex tangent vectors required by Normal Mapping
        /*
        createVBOs();
        createShaders();
        */
        updateGL();
    }
}

void GLWidget::readOFFFile(const QString &filename) {

    std::ifstream stream;
    stream.open(filename.toUtf8(), std::ifstream::in);

    // Checking if the file is already opened
    if(!stream.is_open()) {
        qWarning("Cannot open file.");
        return;
    }

    std::string line;

    stream >> line;
    stream >> numVertices >> numFaces >> line;

    delete[] vertices;
    vertices = new QVector4D[numVertices];

    delete[] indices;
    indices = new unsigned int[numFaces * 3];

    // If the object has vertex
    // build the object inside the vertices array
    if(numVertices > 0) {
        // Verifying if the positions decribed inside the .OFF file
        // is valid
        double minLim = std::numeric_limits<double>::min();
        double maxLim = std::numeric_limits<double>::max();
        QVector4D max(minLim, minLim, minLim, 1.0);
        QVector4D min(maxLim, maxLim, maxLim, 1.0);

        for(unsigned int i =0 ; i < numVertices; i++) {

            double x,y,z;
            stream >> x >> y >> z;

            max.setX(qMax((double)max.x(),x));
            max.setY(qMax((double)max.y(),y));
            max.setZ(qMax((double)max.z(),z));
            min.setX(qMin((double)min.x(),x));
            min.setY(qMin((double)min.y(),y));
            min.setZ(qMin((double)min.z(),z));

            vertices[i] = QVector4D(x,y,z,1.0);
        }

        QVector4D midpoint = (min + max) * 0.5;
        double invdiag = 1 / (max - min).length();

        for(unsigned int i = 0; i < numVertices; i++) {
            vertices[i] = (vertices[i] - midpoint)*invdiag;
            vertices[i].setW(1);
        }
    }

    for(unsigned int i = 0; i < numFaces ; i++) {
        unsigned int a,b,c;
        stream >> line >> a >> b >> c;

        indices[i*3]   = a;
        indices[i*3+1] = b;
        indices[i*3+2] = c;
    }

    stream.close();
}

// Generating the normals for the meshes
void GLWidget::genNormals() {
    delete[] normals;
    normals = new QVector3D[numVertices];

    for (unsigned int i = 0; i < numFaces; i++) {
        // Storing the indices for the faces
        unsigned int i1 = indices[i*3  ];
        unsigned int i2 = indices[i*3+1];
        unsigned int i3 = indices[i*3+2];

        QVector3D v1 = vertices[i1].toVector3D();
        QVector3D v2 = vertices[i2].toVector3D();
        QVector3D v3 = vertices[i3].toVector3D();

        // Calculating the normals for all the faces
        QVector3D faceNormal = QVector3D::crossProduct(v2 - v1, v3 - v1);

        normals[i1] += faceNormal;
        normals[i2] += faceNormal;
        normals[i3] += faceNormal;

    }

    // Normalizing all normals
    for(unsigned int i = 0; i < numVertices; i++)
        normals[i].normalize();
}

// Generating cylindrical texture coordinates for the mesh vertices
void GLWidget::genTexCoordsCylinder() {
    delete[] texCoords;
    texCoords = new QVector2D[numVertices];

    // Storing the limits that a position could has
    double minLim = std::numeric_limits<double>::min();
    double maxLim = std::numeric_limits<double>::max();
    QVector2D max(minLim, minLim);
    QVector2D min(maxLim, maxLim);

    for(unsigned int i = 0; i < numVertices; i++) {
        QVector2D pos = vertices[i].toVector2D();

        max.setX(qMax(max.x(), pos.x()));
        max.setY(qMax(max.y(), pos.y()));
        min.setX(qMin(min.x(), pos.x()));
        min.setY(qMin(min.y(), pos.y()));

    }

    QVector2D size = max - min;
    for (unsigned int i = 0; i < numVertices; i++) {
        double x = 2.0 * (vertices[i].x() - min.x()) / size.x() - 1.0;
        texCoords[i] = QVector2D(acos(x) / M_PI, vertices[i].y() - min.y()) / size.y();

    }
}

// Estimating per-vertex tangent vectors required by Normal Mapping
void GLWidget::genTangents() {
    delete[] tangents;

    tangents = new QVector4D[numVertices];
    QVector3D *bitangents = new QVector3D[numVertices];

    for(unsigned int i = 0; i < numFaces; i++) {
        unsigned int i1 = indices[i*3  ];
        unsigned int i2 = indices[i*3+1];
        unsigned int i3 = indices[i*3+2];

        QVector3D E = vertices[i1].toVector3D();
        QVector3D F = vertices[i2].toVector3D();
        QVector3D G = vertices[i3].toVector3D();

        QVector2D stE = texCoords[i1];
        QVector2D stF = texCoords[i2];
        QVector2D stG = texCoords[i3];

        QVector3D P = F - E;
        QVector3D Q = G - E;

        QVector2D st1 = stF - stE;
        QVector2D st2 = stG - stE;

        QMatrix2x2 M;
        M(0,0) = st2.y();
        M(0,1) = -st1.y();
        M(1,0) = -st2.x();
        M(1,1) = st1.x();

        M *= (1.0 / (st1.x() * st2.y() - st2.x()*st1.y()));

        QVector4D T = QVector4D(M(0,0)*P.x()+M(0,1)*Q.x(),
                                M(0,0)*P.y()+M(0,1)*Q.y(),
                                M(0,0)*P.z()+M(0,1)*Q.z(),
                                0.0);

        QVector3D B = QVector3D(M(1,0)*P.x()+M(1,1)*Q.x(),
                                M(1,0)*P.y()+M(1,1)*Q.y(),
                                M(1,0)*P.z()+M(1,1)*Q.z());

        tangents[i1] += T;
        tangents[i2] += T;
        tangents[i3] += T;

        bitangents[i1] += B;
        bitangents[i2] += B;
        bitangents[i3] += B;
    }

    for(unsigned int i = 0; i < numVertices; i++) {
        const QVector3D& n = normals[i];
        const QVector4D& t = tangents[i];

        tangents[i] = (t - n * QVector3D::dotProduct(n,t.toVector3D())).normalized();

        QVector3D b = QVector3D::crossProduct(n,t.toVector3D());

        double hand = QVector3D::dotProduct(b,bitangents[i]);

        tangents[i].setW((hand < 0.0) ? -1.0 : 1.0);
    }

    delete[] bitangents;
}

// Creating the shaders and linking them to the shaderProgram
void GLWidget::createShaders() {
    destroyShaders();

    QString directory = ":/shaders/glsl/";

    QString vertexShaderFile[] = {
        directory + "vgouraud.glsl",
        directory + "vphong.glsl",
        directory + "vtexture.glsl",
        directory + "vnormal.glsl",
    };

    QString fragmentShaderFile[] = {
         directory + "fgouraud.glsl",
         directory + "fphong.glsl",
         directory + "ftexture.glsl",
         directory + "fnormal.glsl",
    };

    vertexShader = new QGLShader(QGLShader::Vertex);
    if(!vertexShader->compileSourceFile(vertexShaderFile[currentShader]))
        qWarning() << vertexShader->log();

    fragmentShader = new QGLShader(QGLShader::Fragment);
    if(!fragmentShader->compileSourceFile(fragmentShaderFile[currentShader]))
        qWarning() << fragmentShader->log();

    shaderProgram = new QGLShaderProgram;
    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);

    if(!shaderProgram->link())
        qWarning() << shaderProgram->log() << Qt::endl;

}

/* Creating all Vertex Buffer Objects
 *
 * vboVertices: vertex positions of type QVector4D.
 * vboNormals: vertex normals of type QVector3D.
 * vboTexCoords: texture coordinates of type QVector2D.
 * vboTangents: tangent vectors of type QVector4D.
 *
*/
void GLWidget::createVBOs() {
    destroyVBOs();

    vboVertices = new QGLBuffer(QGLBuffer::VertexBuffer);
    vboVertices->create();
    vboVertices->bind();
    vboVertices->setUsagePattern(QGLBuffer::StaticDraw);
    vboVertices->allocate(vertices, numVertices * sizeof(QVector4D));

    delete[] vertices;
    vertices = NULL;

    vboNormals = new QGLBuffer(QGLBuffer::VertexBuffer);
    vboNormals->create();
    vboNormals->bind();
    vboNormals->setUsagePattern(QGLBuffer::StaticDraw);
    vboNormals->allocate(normals, numVertices * sizeof(QVector3D));

    delete[] normals;
    normals = NULL;

    vboTexCoords = new QGLBuffer(QGLBuffer::VertexBuffer);
    vboTexCoords->create();
    vboTexCoords->bind();
    vboTexCoords->setUsagePattern(QGLBuffer::StaticDraw);
    vboTexCoords->allocate(texCoords, numVertices * sizeof(QVector2D));

    delete[] tangents;
    tangents = NULL;

    vboIndices = new QGLBuffer(QGLBuffer::IndexBuffer);
    vboIndices->create();
    vboIndices->bind();
    vboIndices->setUsagePattern(QGLBuffer::StaticDraw);
    vboIndices->allocate(indices, numFaces * 3 * sizeof(unsigned int));

    delete[] indices;
    indices = NULL;
}

// Saving a image from the actual frame buffer on disk
void GLWidget::takeScreenshot() {
    QImage screenshot = grabFrameBuffer();

    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "Save File As", QDir::homePath(), QString("PNG Files (*.png)"));

    // If filename exists write the image to disk
    if(fileName.length()) {
        if(!fileName.contains(".png"))
            fileName += ".png";
        if(screenshot.save(fileName, "PNG")) {
            QMessageBox::information(this, "Screenshot", "Screenshot taken!", QMessageBox::Ok);
        }
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
        case Qt::Key_0:
            currentShader = 0;
            createShaders();
            updateGL();
            break;
        case Qt::Key_1:
            currentShader = 0;
            createShaders();
            updateGL();
            break;
        case Qt::Key_2:
            currentShader = 0;
            createShaders();
            updateGL();
            break;
        case Qt::Key_3:
            currentShader = 0;
            createShaders();
            updateGL();
            break;
        case Qt::Key_Escape:
            qApp->exit();
            break;
        }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    trackBall.mouseMove(event->posF());
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() & Qt::LeftButton)
        trackBall.mousePress(event->posF());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton)
        trackBall.mouseRelease(event->posF());
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    zoom += 0.001 * event->angleDelta().y();
}














