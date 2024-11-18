#ifndef UI_DEFAULT_DFS_H
#define UI_DEFAULT_DFS_H

#include <QString>
#include <QColor>
#include <QScreen>
#include <QGuiApplication>
#include <QMap>

namespace SDL_UI_MODULE {
#ifdef Q_OS_WIN
#define _STRING_WRAPPER(str) QStringLiteral(str)
#else
#define _STRING_WRAPPER(str) str
#endif
#define USER_TEMPORARY_SHADER_PATH "Resources/user/temporary"
#define USER_TEMPORARY_AUDIO_PATH "Resources/user/temporary"

#define GL_DEFAULT_RECORD_VIDEO_PATH "Resources/user/video"
#define ICOPATH(x) "Resources/icon/"#x
#define AVTRPATH(x) "Resources/user/avatar/"#x
#define FONTPATH(x) "Resources/font/"#x
#define EDITOR_LANG_CONFIG_PATH(x) "Resources/editor/intellisense/"#x
#define SHADOW_WIDTH 10

    extern enum SDL_CONTEXT_STATUS
    {
        //last two bit flag - login save
        Archive_updated = 0x00000001,
        Logged=0x00000002,
    } sdl_context_flag;

    enum MsgBoxIcon {
        NoIcon = 0,
        Information = 1,
        Warning = 2,
        Critical = 3,
        Question = 4
    };

	extern QMap<QString, QString> global_font_mp;
    extern int screen_resolution_x;
	extern int screen_resolution_y;

#define UI_COMPONENT_WIDTH(ratio) screen_resolution_x * ratio
#define UI_COMPONENT_HEIGHT(ratio) screen_resolution_y * ratio

    extern float about_menu_width_refactor;                     //135
    extern float file_menu_width_refactor;                      //135
    extern float look_menu_width_refactor;                      //135
    extern float window_menu_width_refactor;                    //135
    extern float profile_menu_width_refactor;                   //100
    extern float menubar_height_refactor;                       //38
    extern float msgbox_width_refactor;                         //470
    extern float msgbox_height_refactor;                        //120
    extern float msgbox_icon_width_refactor;                    //65
    extern float msgbox_icon_height_refactor;                   //65
    extern float tablabel_tip_width_factor;                     //50
    extern float editorTab_refactor_width_factor;               //440
    extern float editorTab_refactor_height_factor;              //80
    extern float window_asset_width_factor;                     //200
    extern float window_asset_height_factor;                    //100
    extern float variable_board_height_factor;                 //20
    extern float variable_board_commitBtn_width_factor;         //21
    extern float window_infoPanel_width_factor;                 //30
    extern float editor_compile_bar_height;

    extern  int ovWindow_minimum_width;
    extern  int ovWindow_minimum_height;

    extern QColor msgbox_maindisp_label_color;
    extern QColor msgbox_attached_label_color;

    extern int sdl_default_font_size;

    extern QString EDITOR_PAGE_STYLE;
    extern QString EDITOR_TOOLBUTTON_STYLE;

    extern QString WINDOW_FONT_COLOR_STYLE;

    extern QString WINDOWINFO_BTN_STYLE_1;

    extern QString WINDOWINFO_BTN_STYLE_2;

    extern QString WINDOW_ASEETBAR_BTN_STYLE;

    extern QString PREFERENCE_WINDOW_STYLE;

    extern QString PREFERENCE_COLUMBTN_STYLE_FIRST;

    extern QString PREFERENCE_COLUMBTN_STYLE_LAST;

    extern QString PREFERENCE_COLUMBTN_STYLE_MEDIUM;

    extern QString TABLABLE_EDITOR_STYLE;

    extern QString TYPES_LISTVIEW_STYLE;

    extern QString TYPES_MENU_STYLE;
}
#endif // UI_DEFAULTDFS_H
