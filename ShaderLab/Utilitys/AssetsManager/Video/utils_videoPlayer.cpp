#include "utils_videoPlayer.h"
#include "GL/glmodule_backstatge.h"
#include "Utilitys/uitilityDfs.h"
#include "Utilitys/utils_backendThread.h"
#include <QDebug>
XA_VIDEO_PLAYER* XA_VIDEO_PLAYER::_instance;
int gif_error;

XA_VIDEO_PLAYER::XA_VIDEO_PLAYER()
{
	XA_FFMPEG_HELPER::getHelper()->disableLog();//close log debug output
}

XA_VIDEO_PLAYER::~XA_VIDEO_PLAYER()
{
	if (DGifCloseFile(_gif_file, &gif_error) == GIF_ERROR)
	{
		const char* err_code = GifErrorString(gif_error);
	}
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

bool XA_VIDEO_PLAYER::acquireGifImageSeq(const char* path, QObject* receiver)
{
	_gif_file = DGifOpenFileName(path, &gif_error);
	if (_gif_file == NULL) {
		const char* err_code = GifErrorString(gif_error);
		return false;

	}
	gif_error = DGifSlurp(_gif_file);
	gif_info.width = _gif_file->SWidth;
	gif_info.height = _gif_file->SHeight;
	gif_info.colors = (1 << _gif_file->SColorResolution);

	std::pair<QObject*, XA_UTILS_TASK> gifdecodeTask;
	gifdecodeTask.first = this;
	gifdecodeTask.second.type = XA_UTIL_DECODEGIF;
	gifdecodeTask.second.param.decodeGif_param.info = &gif_info;
	gifdecodeTask.second.param.decodeGif_param.gifFile = _gif_file;
	gifdecodeTask.second.param.decodeGif_param.receiver = receiver;
	XA_UTILS_BACKEND::getUtilBackend()->addTask(gifdecodeTask);
}

void XA_VIDEO_PLAYER::recordDone()
{
	XA_FFMPEG_HELPER::getHelper()->AVRecordDone();
}

