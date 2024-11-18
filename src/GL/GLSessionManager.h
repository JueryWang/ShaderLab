#ifndef GL_SESSIONMANAGER_H
#define GL_SESSIONMANAGER_H

#include "GL_DefaultDfs.h"
#include "SessionManager.h"
#include <utility>
#include <queue>
#include <array>
#include <QList>
#include <QThread>


namespace SDL_GL_MODULE {
    class GLRenderer;

    class GLSessionManager : public SessionManager<GLRenderer,SDL_GL_TASK>{
        friend class GLRenderer;
        Q_OBJECT
    public:
        static GLSessionManager *getInstance();
        void deleteTexture(int idx);
        bool findTexture(const SDL_GL_TEXTURE_INFO *target);

    signals:
        void shaderParseDone();
        void videoRecordDone();

    protected:
        void run() override;

    private:
        GLSessionManager();
        ~GLSessionManager();
        void handleDrawFront(const std::pair<GLRenderer*,SDL_GL_TASK>& crt_task);
        void handleLoadTexture(const std::pair<GLRenderer*,SDL_GL_TASK>& crt_task);
        void handleCompileShader(const std::pair<GLRenderer*, SDL_GL_TASK> &crt_task);
        void handleAVRecord(const std::pair<GLRenderer*,SDL_GL_TASK>& crt_task);

    public:
        static GLSessionManager* s_instance;
        std::vector<SDL_GL_TEXTURE_INFO*> m_textures;

    };

} // SDL_GL_MODULE

#endif
