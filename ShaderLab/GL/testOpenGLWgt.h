#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>

class XVideoWiget : public QOpenGLWidget,protected QOpenGLFunctions
{
	Q_OBJECT
public:
	XVideoWiget(QWidget* parent);
	~XVideoWiget();
protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int width,int height);
private:
	QGLShaderProgram program;
};