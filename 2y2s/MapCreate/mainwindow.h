#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLFramebufferObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>

#include <QVector>
#include <QMatrix4x4>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>

#include <QTimer>
#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <cube.h>

class Cube;
class QOpenGLFramebufferObject;

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

    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *ke);

    void initShaders();
    void initCube(QVector< float> &aVector);
    void setDataToVector(const QStringList &aStringList,
                         QVector< float> &aVector);
    const QString allFileToString(QFile &aFile);
    void rewrite(const QStringList &aStringList);

private:
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_projectionLightMatrix;
    QMatrix4x4 m_LightMatrix;
    QMatrix4x4 m_shadowLightMatrix;

    float m_ligthRotateX;
    float m_ligthRotateY;

    QOpenGLShaderProgram m_shaderProgramm;
    QOpenGLShaderProgram m_programDepth;

    QVector2D m_mousePosition;
    QQuaternion m_rotation;

    QVector<Cube *> m_objects;

    QOpenGLFramebufferObject * m_depthBuffer;
    quint32 m_fbHeigth;
    quint32 m_fbWidth;

    int counter = 0;
};

#endif // MAINWINDOW_H
