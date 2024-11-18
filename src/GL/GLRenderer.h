#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "GL_DefaultDfs.h"
#include "Shader.h"
#include <QObject>
#include <QWidget>
#include <vector>
#include <string>
#include <atomic>
#include <map>

namespace SDL_UI_MODULE
{
    class EditorPage;
}

namespace SDL_GL_MODULE {

    class GLRenderer : public QObject{
        friend class GLSessionManager;
        Q_OBJECT

    public:
        GLRenderer(const std::string& title,PixelType type = PixelType::SDL_GL_RGB,QObject* receiver = nullptr);
        ~GLRenderer();

        void setReceiver(QObject* receiver);
        void setTitle(const char* title);
        void clearShaders();
        void addShader(const std::shared_ptr<SDL_SHADER_INFO> &shader_info);
        void sortShader();
        std::vector< std::shared_ptr<SDL_SHADER_INFO>>& getAttachedShaders() { return m_attachedShaders; }
        const int getFPS() { return m_fps; }
        void __exit();
        void __start();
        void __pause();
        void __restart();
        void __update();
        void __record();
        void __endrecord();
        static void __reset(const QSize& newSize);

    private:
        void initShader(std::shared_ptr< SDL_SHADER_INFO> &shader_info);
        void renderScene(float time, int frame);
        void renderQuad(int context_width,int context_height);
        void loadTextures();
        void loadShaders();
        void updateGLContext();
        struct Deleter {
            void operator()(GLFWwindow* w)
            {
                if (w != nullptr)
                {
                    glfwTerminate();
                    glfwDestroyWindow(w);
                }
            }
        };

    public slots:
        void contextDraw();

    public:
        int getWidth() const;
        int getHeight() const;

        static std::atomic<int> SCR_WIDTH;
        static std::atomic<int> SCR_HEIGHT;
        static int resolution[2];
        void inline setStatus(uchar bit_mask,bool open_bit);
        bool inline getFlag(SDL_RENDER_FLAG_BIT_POS pos);

    private:
        void* m_windowbuf;
        std::unique_ptr <GLFWwindow,Deleter> m_window;

        QObject* m_receiver;
        PixelType m_PixType;
        std::vector< std::shared_ptr<SDL_SHADER_INFO>> m_attachedShaders;
        Shader* m_mainShader = nullptr;
        std::string m_title;

        int m_windowBufSize;
        int m_activeShaderCount = 0;
        int m_fps = 0;

        float m_anchor_time;
		//use uint_8 flags to express 8 bools
        //0-3 reserved 4.record| 5. update |6 pause | 7 exit 
        std::atomic<uchar> state_bit = 0b00000000;
    };
}

#endif
