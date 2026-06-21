#include "RegisterWindow.h"
#include "UI/UI_DefaultDfs.h"
#include "UI/OverallWindow.h"
#include <QFile>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QFrame>

namespace SDL_UI_MODULE
{
	typedef enum _WINDOWCOMPOSITIONATTRIB
	{
		CA_UNDEFINED = 0,
		WCA_NCRENDERING_ENABLED = 1,
		WCA_NCRENDERING_POLICY = 2,
		WCA_TRANSITIONS_FORCEDISABLED = 3,
		WCA_ALLOW_NCPAINT = 4,
		WCA_CAPTION_BUTTON_BOUNDS = 5,
		WCA_NONCLIENT_RTL_LAYOUT = 6,
		WCA_FORCE_ICONIC_REPRESENTATION = 7,
		WCA_EXTENDED_FRAME_BOUNDS = 8,
		WCA_HAS_ICONIC_BITMAP = 9,
		WCA_THEME_ATTRIBUTES = 10,
		WCA_NCRENDERING_EXILED = 11,
		WCA_NCADORNMENTINFO = 12,
		WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
		WCA_VIDEO_OVERLAY_ACTIVE = 14,
		WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
		WCA_DISALLOW_PEEK = 16,
		WCA_CLOAK = 17,
		WCA_CLOAKED = 18,
		WCA_ACCENT_POLICY = 19,//
		WCA_FREEZE_REPRESENTATION = 20,
		WCA_EVER_UNCLOAKED = 21,
		WCA_VISUAL_OWNER = 22,
		WCA_LAST = 23
	} WINDOWCOMPOSITIONATTRIB;

	typedef struct _WINDOWCOMPOSITIONATTRIBDATA
	{
		WINDOWCOMPOSITIONATTRIB Attrib;
		PVOID pvData;
		SIZE_T cbData;
	} WINDOWCOMPOSITIONATTRIBDATA;

	typedef enum _ACCENT_STATE
	{
		ACCENT_DISABLED = 0,
		ACCENT_ENABLE_GRADIENT = 1,
		ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
		ACCENT_ENABLE_BLURBEHIND = 3,
		ACCENT_INVALID_STATE = 4
	} ACCENT_STATE;

	typedef struct _ACCENT_POLICY
	{
		ACCENT_STATE AccentState;
		DWORD AccentFlags;
		DWORD GradientColor;
		DWORD AnimationId;
	} ACCENT_POLICY;

	WINUSERAPI
		BOOL
		WINAPI
		GetWindowCompositionAttribute(
			_In_ HWND hWnd,
			_Inout_ WINDOWCOMPOSITIONATTRIBDATA* pAttrData);

	typedef BOOL(WINAPI* pfnGetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

	WINUSERAPI
		BOOL
		WINAPI
		SetWindowCompositionAttribute(
			_In_ HWND hWnd,
			_Inout_ WINDOWCOMPOSITIONATTRIBDATA* pAttrData);

	typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

	RegisterWindow::RegisterWindow(QWidget* parent)
	{

	}

	RegisterWindow::~RegisterWindow()
	{

	}

	void RegisterWindow::SwitchToLogIn()
	{
	}

	SignUpPanel::SignUpPanel(RegisterWindow* parent) : m_parentWindow(parent)
 	{
		setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
		setAttribute(Qt::WA_TranslucentBackground);
		this->setFixedSize(400, 300);

//#ifdef Q_OS_WIN
//		HWND hWnd = HWND(this->winId());
//		HMODULE hUser = GetModuleHandle(L"user32.dll");
//		if (hUser)
//		{
//			pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
//			if (setWindowCompositionAttribute)
//			{
//				ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND,0,0,0 };
//				WINDOWCOMPOSITIONATTRIBDATA data;
//				data.Attrib = WCA_ACCENT_POLICY;
//				data.pvData = &accent;
//				data.cbData = sizeof(accent);
//				setWindowCompositionAttribute(hWnd, &data);
//			}
//		}
//#endif
		//load qss theme
		QString qssPath = "./Resources/qss";
		QFile file(qssPath + "/userInterface.qss");
		file.open(QIODevice::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);

		//this->setFixedSize(800, 600);

		QVBoxLayout* vlay = new QVBoxLayout(this);
		m_signUpLabel = new QLabel();
		m_signUpLabel->setObjectName("SignUp");
		m_signUpLabel->setText("Sign Up");
		m_signUpLabel->setAlignment(Qt::AlignCenter);
		
		m_alReadyMemberhint = new QLabel(this);
		m_alReadyMemberhint->setObjectName("LogHint");
		m_alReadyMemberhint->setText("Already a member? <a href=\"log_in\" style=\"color: blue; text-decoration: none;\">Log In</a>");
		m_alReadyMemberhint->setTextFormat(Qt::RichText);
		m_alReadyMemberhint->setOpenExternalLinks(false);

		QObject::connect(m_signUpLabel, &QLabel::linkActivated, m_parentWindow, [&](const QString& link)
			{
				if (link == "login") {
					m_parentWindow->SwitchToLogIn();
				}
			});

		m_emailInput = new QLineEdit();
		m_emailInput->setPlaceholderText("Email");
		m_passWrdInput = new QLineEdit();
		m_passWrdInput->setPlaceholderText("PassWord");
		m_signUpButton = new QPushButton();
		m_signUpButton->setObjectName("SinUpButton");
		m_signUpButton->setText("Sign Up");
		m_signUpModeHint = new QLabel();
		m_signUpModeHint->setStyleSheet("color: white;padding: 0 10px;");
		m_signUpModeHint->setText("or sign up with");

		m_faceBookSignUp = new ClickableLabel();
		QWidget* container = new QWidget();
		QHBoxLayout* hlay = new QHBoxLayout(container);
		hlay->setContentsMargins(0, 0, 0, 0);

		QFrame* leftLine = new QFrame(container);
		leftLine->setStyleSheet("color: white;");
		leftLine->setFrameShape(QFrame::HLine);
		leftLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


		QFrame* rightLine = new QFrame(container);
		rightLine->setStyleSheet("color: white;");
		rightLine->setFrameShape(QFrame::HLine);
		rightLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		QHBoxLayout* hlay2 = new QHBoxLayout();
		hlay2->addWidget(leftLine);
		hlay2->addWidget(m_signUpModeHint);
		hlay2->addWidget(rightLine);

		QHBoxLayout* hlay3 = new QHBoxLayout();
		m_faceBookSignUp = new ClickableLabel(ICOPATH(facebook.svg),48);
		connect(m_faceBookSignUp, &ClickableLabel::clicked, this, &SignUpPanel::SignUpByFaceBook);
		m_googleSignUp = new ClickableLabel(ICOPATH(google.svg), 48);
		connect(m_googleSignUp, &ClickableLabel::clicked, this, &SignUpPanel::SignUpByGoogle);
		
		hlay3->addStretch();
		hlay3->addWidget(m_faceBookSignUp);
		hlay3->addSpacerItem(new QSpacerItem(48, 48, QSizePolicy::Expanding, QSizePolicy::Minimum));
		hlay3->addWidget(m_googleSignUp);
		hlay3->addStretch();

		vlay->addWidget(m_signUpLabel,Qt::AlignCenter);
		vlay->addWidget(m_alReadyMemberhint, Qt::AlignCenter);
		vlay->addWidget(m_emailInput, Qt::AlignCenter);
		vlay->addWidget(m_passWrdInput, Qt::AlignCenter);
		vlay->addWidget(m_signUpButton, Qt::AlignCenter);
		vlay->addLayout(hlay2);
		vlay->addLayout(hlay3);

		this->setLayout(vlay);
		this->installEventFilter(this);
	}

	SignUpPanel::~SignUpPanel()
	{
	}

	void SignUpPanel::SetAcrylicMode()
	{

	}

	void SDL_UI_MODULE::SignUpPanel::paintEvent(QPaintEvent* ev)
	{

	}
	void SignUpPanel::SignUpByGoogle()
	{
	}

	void SignUpPanel::SignUpByFaceBook()
	{
	}
}
