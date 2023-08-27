#include "testOpenGLWgt.h"
#include <QDebug>
//自动加双引号
#define GET_STR(x) #x

const char* vString = GET_STR(
	attribute vec4 vertexIn;
	attribute vec2 textureIn;
	varying vec2 vertexOut;
	void main(void)
	{
		gl_position = vertexIn;
		vertexOut = vertexIn;
	}
);

const char* tString = GET_STR(
	varying vec2 vertexOut;
	uniform sampler2D tex_y;
	uniform sampler2D tex_u;
	uniform sampler2D tex_v;
	void main()
	{
		vec3 yuv;
		vec3 rgb;
		yuv.x = texture2D(tex_y, vertexOut);
		yuv.y = texture2D(tex_u, vertexOut);
		yuv.z = texture2D(tex_v, vertexOut);

		rgb = mat3(1, 1, 1,
			0, -0.39465, 2.03211,
			1.13983, -0.58060, 0) * yuv;
		gl_FragColor = vec4(rgb, 1);
	}
);

XVideoWiget::XVideoWiget(QWidget* parent)
{

}

XVideoWiget::~XVideoWiget()
{

}

void XVideoWiget::paintGL()
{

}

void XVideoWiget::initializeGL()
{
	initializeOpenGLFunctions();

	program.addShaderFromSourceCode(QGLShader::Vertex, vString);
	program.addShaderFromSourceCode(QGLShader::Fragment, tString);
}

void XVideoWiget::resizeGL(int width, int height)
{

}

