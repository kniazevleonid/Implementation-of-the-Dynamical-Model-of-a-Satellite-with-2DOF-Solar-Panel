#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent)
{
    m_projectionLightMatrix.setToIdentity();
    m_projectionLightMatrix.ortho(-40, 40, -40, 40, -40, 40);

    m_ligthRotateX = 30;
    m_ligthRotateY = 40;

    m_shadowLightMatrix.setToIdentity();
    m_shadowLightMatrix.rotate(m_ligthRotateX, 1.0, 0.0, 0.0);
    m_shadowLightMatrix.rotate(m_ligthRotateY, 0.0, 1.0, 0.0);

    m_LightMatrix.setToIdentity();
    m_LightMatrix.rotate(-m_ligthRotateY, 0.0, 1.0, 0.0);
    m_LightMatrix.rotate(-m_ligthRotateX, 1.0, 0.0, 0.0);


    m_fbHeigth = 16384;
    m_fbWidth = 16384;

}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    QVector<float> vector;
    QVector<float> vector1;
    QFile file("output.txt");


    setDataToVector(allFileToString(file).split("\n"), vector);
    file.close();

    vector1.push_back(1);
    vector1.push_back(0);
    vector1.push_back(0);
    vector1.push_back(0);
    vector1.push_back(0);
    vector1.push_back(0);
    vector1.push_back(0);


    vector1.push_back(-0.5);
    vector1.push_back(0.25);
    vector1.push_back(0);

    vector1.push_back(-0.5);
    vector1.push_back(0.25);
    vector1.push_back(1.0);

    vector1.push_back(0.5);
    vector1.push_back(0.25);
    vector1.push_back(0);

    vector1.push_back(0.5);
    vector1.push_back(0.25);
    vector1.push_back(1.0);

    vector1.push_back(-0.5);
    vector1.push_back(-0.75);
    vector1.push_back(0);

    vector1.push_back(-0.5);
    vector1.push_back(-0.75);
    vector1.push_back(1.0);

    vector1.push_back(0.5);
    vector1.push_back(-0.75);
    vector1.push_back(0);

    vector1.push_back(0.5);
    vector1.push_back(-0.75);
    vector1.push_back(1.0);

    initCube(vector);


    initCube(vector1);

    m_depthBuffer = new QOpenGLFramebufferObject(m_fbWidth, m_fbHeigth, QOpenGLFramebufferObject::Depth);

    return;
}

void MainWindow::resizeGL(int w, int h)
{
    float aspect = w / qreal(h ? h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(90, aspect, 0.1f, 10.0f);
    return;
}

void MainWindow::paintGL()
{
    //paint to frame buffer
    m_depthBuffer->bind();

    glViewport(0, 0, m_fbWidth, m_fbHeigth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programDepth.bind();//
    m_programDepth.setUniformValue("u_projectionLightMatrix", m_projectionLightMatrix);
    m_programDepth.setUniformValue("u_shadowLightMatrix", m_shadowLightMatrix);


    for (int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->draw(&m_programDepth, context()->functions());
    }


    m_programDepth.release();

    m_depthBuffer->release();

    GLuint texture = m_depthBuffer->texture();


    context()->functions()->glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, texture);

    //paint on screen
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0.0f, 0.0f, -0.0);
    modelViewMatrix.rotate(m_rotation);

    QVector<float> vector;
    QFile file("output.txt");

    setDataToVector(allFileToString(file).split("\n"), vector);

    if (vector[0]==2)
        close();

    m_shaderProgramm.bind();
    m_shaderProgramm.setUniformValue("u_shadowMap", GL_TEXTURE1-GL_TEXTURE0);
    m_shaderProgramm.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_shaderProgramm.setUniformValue("u_viewMatrix", modelViewMatrix);
    m_shaderProgramm.setUniformValue("u_lightDirection", QVector4D (-vector[1], -vector[2], -vector[3], 0.0));
    m_shaderProgramm.setUniformValue("u_projectionLightMatrix", m_projectionLightMatrix);
    m_shaderProgramm.setUniformValue("u_shadowLightMatrix", m_shadowLightMatrix);
    m_shaderProgramm.setUniformValue("u_LightMatrix", m_LightMatrix);
    m_shaderProgramm.setUniformValue("u_lightPower", 5.0f);

    for (int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->draw(&m_shaderProgramm, context()->functions());
    }
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "In close event\n";

    QMessageBox::StandardButton ret;
    ret = QMessageBox::question( this,  QApplication::applicationName(), tr("Do you want to close this programm?"),
                                 QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
    if (ret == QMessageBox::No)
        event->ignore();
    else
        close();

    return;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
   // while (1)
  //  {
        QFile file("output.txt");

        rewrite(allFileToString(file).split("\n"));
        file.close();

        QFile file1 ("output.txt");
        file1.remove();
        file1.close();

        QFile::rename ( "data.txt", "output.txt" );

        QVector<float> V;
        QFile file2("output.txt");

        setDataToVector(allFileToString(file2).split("\n"), V);
        file2.close();

        QVector<VertexData> vertexes;

        // Top side
        vertexes.append(VertexData(QVector3D(V[7], V[8], V[9]), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V[10], V[11], V[12]), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V[13], V[14], V[15]), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V[16], V[17], V[18]), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));


        // Down side
        vertexes.append(VertexData(QVector3D(V[25], V[26], V[27]), QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V[28], V[29], V[30]), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V[19], V[20], V[21]), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V[22], V[23], V[24]), QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));

        // Left side
        vertexes.append(VertexData(QVector3D(V.at(7), V.at(8), V.at(9)), QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V.at(19), V.at(20), V.at(21)), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V.at(10), V.at(11), V.at(12)), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V.at(22), V.at(23), V.at(24)), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

        // Right side
        vertexes.append(VertexData(QVector3D(V.at(16), V.at(17), V.at(18)), QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V.at(28), V.at(29), V.at(30)), QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V.at(13), V.at(14), V.at(15)), QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(V.at(25), V.at(26), V.at(27)), QVector2D(1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

        // Front side
        vertexes.append(VertexData(QVector3D(V.at(10), V.at(11), V.at(12)), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
        vertexes.append(VertexData(QVector3D(V.at(22), V.at(23), V.at(24)), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
        vertexes.append(VertexData(QVector3D(V.at(16), V.at(17), V.at(18)), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
        vertexes.append(VertexData(QVector3D(V.at(28), V.at(29), V.at(30)), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

        // Back side
        vertexes.append(VertexData(QVector3D(V.at(13), V.at(14), V.at(15)), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
        vertexes.append(VertexData(QVector3D(V.at(25), V.at(26), V.at(27)), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
        vertexes.append(VertexData(QVector3D(V.at(7), V.at(8), V.at(9)), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
        vertexes.append(VertexData(QVector3D(V.at(19), V.at(20), V.at(21)), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));

        QVector<GLuint> indexes;
        for (int i = 0; i < 24; i += 4){
            indexes.append(i + 0);
            indexes.append(i + 1);
            indexes.append(i + 2);
            indexes.append(i + 2);
            indexes.append(i + 1);
            indexes.append(i + 3);
        }

        m_objects.append(new Cube(vertexes, indexes, QImage("://panel.jpg")));
        paintGL();

        this->repaint();
   // }
    return;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0f;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();

}

void MainWindow::mousePressEvent(QMouseEvent *ke)
{
    /*
    switch (ke->key())
    {
    QFile file("output.txt");

    rewrite(allFileToString(file).split("\n"));
    QFile("output.txt").remove();
    QFile::rename ( "data.txt", "output.txt" );
    }
    */
}


void MainWindow::initShaders()
{
    if (!m_shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Vertex, "://vshader.vsh"))
        close();

    if (!m_shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Fragment, "://fshader.fsh"))
        close();

    if (!m_shaderProgramm.link())
        close();

    if (!m_shaderProgramm.bind())
        close();

    if (!m_programDepth.addShaderFromSourceFile(QOpenGLShader::Vertex, "://depth.vsh"))
        close();

    if (!m_programDepth.addShaderFromSourceFile(QOpenGLShader::Fragment, "://depth.fsh"))
        close();

    if (!m_programDepth.link())
        close();

}

void MainWindow::initCube(QVector< float> &V)
{
    QVector<VertexData> vertexes;
    std::cout<<V[0];

    if (V[0] == 0)
    {
        // Top side
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));


        // Down side
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));

        // Left side
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

        // Right side
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

        // Front side
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

        // Back side
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
        vertexes.append(VertexData(QVector3D(0, 0, 0), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    }

    if(V[0]==2)
        close();

    // Top side
    vertexes.append(VertexData(QVector3D(V[7], V[8], V[9]), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V[10], V[11], V[12]), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V[13], V[14], V[15]), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V[16], V[17], V[18]), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));


    // Down side
    vertexes.append(VertexData(QVector3D(V[25], V[26], V[27]), QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V[28], V[29], V[30]), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V[19], V[20], V[21]), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V[22], V[23], V[24]), QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));

    // Left side
    vertexes.append(VertexData(QVector3D(V.at(7), V.at(8), V.at(9)), QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V.at(19), V.at(20), V.at(21)), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V.at(10), V.at(11), V.at(12)), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V.at(22), V.at(23), V.at(24)), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

    // Right side
    vertexes.append(VertexData(QVector3D(V.at(16), V.at(17), V.at(18)), QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V.at(28), V.at(29), V.at(30)), QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V.at(13), V.at(14), V.at(15)), QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(V.at(25), V.at(26), V.at(27)), QVector2D(1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

    // Front side
    vertexes.append(VertexData(QVector3D(V.at(10), V.at(11), V.at(12)), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(V.at(22), V.at(23), V.at(24)), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(V.at(16), V.at(17), V.at(18)), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(V.at(28), V.at(29), V.at(30)), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    // Back side
    vertexes.append(VertexData(QVector3D(V.at(13), V.at(14), V.at(15)), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(V.at(25), V.at(26), V.at(27)), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(V.at(7), V.at(8), V.at(9)), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(V.at(19), V.at(20), V.at(21)), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));


    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4){
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_objects.append(new Cube(vertexes, indexes, QImage("://panel.jpg")));
}



const QString MainWindow:: allFileToString(QFile &aFile)
{
    if (!aFile.open(QFile::ReadOnly | QFile::Text)) {
        std::cout << "Error opening file!" << std::endl;
        return NULL;
    }
    QTextStream in(&aFile);
    return in.readAll();
}

void MainWindow::setDataToVector(const QStringList &aStringList,
                                 QVector< float> &aVector)
{
    size_t x = aStringList.size() - 1;
    size_t y = aStringList.at(0).count(' ');
    for (size_t i = 0; i < 1; ++i) {
        for (size_t j = 0; j < y; ++j) {
            aVector.push_back(aStringList.at(i).split(' ').at(j).toFloat());
        }
    }
}



void MainWindow::rewrite(const QStringList &aStringList)
{
    size_t x = aStringList.size() - 1; // Count of line, 8


    QString filename="data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        for (size_t i = 1; i < x; ++i)
        {
            QTextStream stream( &file );
            stream << aStringList.at(i)<<"\r\n";

        }
    }
}









