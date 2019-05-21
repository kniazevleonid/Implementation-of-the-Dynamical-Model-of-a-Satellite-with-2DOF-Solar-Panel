#include "cube.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

Cube::Cube() :
        m_indexBuffer(QOpenGLBuffer::IndexBuffer),
        m_texture(0)
{

}

Cube::Cube(const QVector <VertexData> &vetData, const QVector <GLuint> &indexes, const QImage &texture) :
        m_indexBuffer(QOpenGLBuffer::IndexBuffer),
        m_texture(0)
{
    init (vetData, indexes, texture);
}

Cube::~Cube()
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (m_texture != 0)
        if (m_texture->isCreated())
            m_texture->destroy();

}

void Cube::init(const QVector<VertexData> &vetData, const QVector<GLuint> &indexes, const QImage &texture)
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (m_texture != 0)
    {
        if (m_texture->isCreated())
        {
            delete m_texture;
            m_texture = 0;
        }
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vetData.constData(), vetData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);

    m_modelMatrix.setToIdentity();
}

void Cube::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (!m_vertexBuffer.isCreated())
        return;

    if (!m_indexBuffer.isCreated())
        return;

    m_texture->bind(0);
    program->setUniformValue("u_texture", 0);
    program->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    int offset = 0;

    int location = program->attributeLocation("a_position");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    location = program->attributeLocation("a_textcoord0");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    location = program->attributeLocation("a_normal");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();

}

void Cube::translate(const QVector3D &trvec)
{
    m_modelMatrix.translate(trvec);
}

