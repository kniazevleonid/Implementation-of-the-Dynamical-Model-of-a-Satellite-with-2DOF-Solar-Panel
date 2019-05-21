#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <stdio.h>
//#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <boost/numeric/mtl/mtl.hpp>

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

class MainWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void initShaders();
    void initCube(float width, float higth);

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_shaderProgramm;
    QOpenGLTexture *m_texture;

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    QVector2D m_mousePosition;
    QQuaternion m_rotation;

    int counter = 0;
};

#endif // MAINWINDOW_H
