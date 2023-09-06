#include "utils_videoPlayer.h"
#include "GL/glmodule_backstatge.h"
#include "Utilitys/uitilityDfs.h"
#include <QDebug>
XA_VIDEO_PLAYER* XA_VIDEO_PLAYER::_instance;

XA_VIDEO_PLAYER::XA_VIDEO_PLAYER()
{
	XA_FFMPEG_HELPER::getHelper()->disableLog();//close log debug output
}

XA_VIDEO_PLAYER::~XA_VIDEO_PLAYER()
{

}


XA_VIDEO_PLAYER* XA_VIDEO_PLAYER::get_player()
{
	if (!_instance)
	{
		_instance = new XA_VIDEO_PLAYER();
		connect(XA_GLMODULE_BACKSTG::getBackStage(), &XA_GLMODULE_BACKSTG::videoRecordDone, _instance, &XA_VIDEO_PLAYER::recordDone);
	}
	return _instance;
}

void XA_VIDEO_PLAYER::setRecordParam(const char* name, int width, int height)
{
	std::string s = GL_DEFAULT_RECORD_VIDEO_PATH;
	s += "/";
	s += name;
	s += ".mov";
	XA_FFMPEG_HELPER::getHelper()->setAVRecordParam(s, width, height);
}

void XA_VIDEO_PLAYER::writeRecord(uchar* frameraw)
{
	XA_FFMPEG_HELPER::getHelper()->pushFrame(frameraw);
	delete frameraw;
}

void XA_VIDEO_PLAYER::recordDone()
{
	XA_FFMPEG_HELPER::getHelper()->AVRecordDone();
}

