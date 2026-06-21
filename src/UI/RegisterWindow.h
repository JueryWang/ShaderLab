#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QEvent>
#include "ClickableLabel.h"
#include <qt_windows.h>

namespace SDL_UI_MODULE
{
	enum WindowState
	{
		SignUp,
		LogIn
	};

	class RegisterWindow : public QWidget
	{
		Q_OBJECT
	public:
		RegisterWindow(QWidget* parent = nullptr);
		~RegisterWindow();

		void SwitchToLogIn();

	};

	class SignUpPanel : public QWidget
	{
		Q_OBJECT
	public:
		SignUpPanel(RegisterWindow* parent = nullptr);
		 ~SignUpPanel();

		 void SetAcrylicMode();
		 void paintEvent(QPaintEvent* ev);

		 bool eventFilter(QObject* obj, QEvent* event)
		 {
			 return QWidget::eventFilter(obj, event);
		 }

	public slots:
		void SignUpByFaceBook();
		void SignUpByGoogle();

	private:
		QLabel* m_signUpLabel = nullptr;
		QLabel* m_alReadyMemberhint = nullptr;
		QLineEdit* m_emailInput = nullptr;
		QLineEdit* m_logIn = nullptr;
		QLineEdit* m_passWrdInput = nullptr;

		QPushButton* m_signUpButton = nullptr;

		QLabel* m_signUpModeHint = nullptr;
		ClickableLabel* m_faceBookSignUp = nullptr;
		ClickableLabel* m_googleSignUp = nullptr;

		RegisterWindow* m_parentWindow = nullptr;
	};
}