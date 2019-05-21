#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

struct VertexData {
    VertexData()
    {
    }

    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {
    }

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class Cube
{
public:
    Cube();
    Cube(const QVector <VertexData> &vetData, const QVector <GLuint> &indexes, const QImage &texture);
    ~Cube();

    void init (const QVector <VertexData> &vetData, const QVector <GLuint> &indexes, const QImage &texture);
    void draw (QOpenGLShaderProgram * program, QOpenGLFunctions * functions);
    void translate (const QVector3D &trvec);
private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_texture;
};

#endif // CUBE_H
