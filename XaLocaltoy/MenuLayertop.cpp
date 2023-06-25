#include "MenuLayertop.h"

MenuLayertop::MenuLayertop(QMenuBar* parent /*= NULL*/)
{

	QMenu *m_file = new XA_UIModule_FileMenu("File");
	m_file->setObjectName("FILE_MENU");
	m_file->setProperty("class", "blackMenu");
	m_file->setWindowFlags(m_file->windowFlags() | Qt::FramelessWindowHint);
	m_file->setAttribute(Qt::WA_TranslucentBackground);
	menu_list.append(m_file);


	this->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
	this->addMenu(m_file);
}
