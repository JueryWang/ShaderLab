#include "GLSessionManager.h"
#include "GL_DefaultDfs.h"
#include "GLRenderer.h"
#include "Utility/Utils_DefaultDfs.h"
#include "UI/GLWidget.h"
#include "UI/CodeEditor.h"
#include "UI/EditorPage.h"
#include "Utility/Parser/ShaderParser.h"
#include "Utility/AssetsManager/Video/VideoManager.h"
#include <algorithm>
#include <Qsci/qsciscintilla.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace  SDL_UTILS;

namespace SDL_GL_MODULE {

    GLSessionManager* GLSessionManager::s_instance = nullptr;

    GLSessionManager *SDL_GL_MODULE::GLSessionManager::getInstance() {
        if(s_instance == nullptr)
        {
            s_instance = new GLSessionManager();
        }
        return s_instance;
    }

    void GLSessionManager::deleteTexture(int idx) {
        auto texIter = m_textures.begin();

        for( ;texIter != m_textures.end();texIter++)
        {
            if(((*texIter)->index == idx) && (*texIter)->address != nullptr)
            {
                (*texIter)->index = 0;
                delete (*texIter)->address;
            }
            (*texIter)->status = TEXTURE_ST_DEPRECATED;
        }

        if(texIter == m_textures.end())
        {
            //Do some Log here
        }
    }

    bool GLSessionManager::findTexture(const SDL_GL_TEXTURE_INFO *target) {
        int targetID = target->textureID;
        return std::find_if(m_textures.begin(),m_textures.end(),[targetID](SDL_GL_TEXTURE_INFO* another)->bool{return targetID == another->textureID;})!=m_textures.end();
    }

    void GLSessionManager::run() {

        while(true)
        {
            while(!m_task_queue.empty())
            {
                std::pair<GLRenderer*,SDL_GL_TASK> crt_task = m_task_queue.front();
                switch (crt_task.second.type) {
                    case GL_DRAW_FRONT:
                    {
                        handleDrawFront(crt_task);
                        break;
                    }
                    case GL_LOAD_TEXTURE:
                    {
                        handleLoadTexture(crt_task);
                        break;
                    }
                    case GL_COMPILE_SHADER:
                    {
                        handleCompileShader(crt_task);
                        break;
                    }
                    case GL_RECORD:
                    {
                        handleAVRecord(crt_task);
                        break;
                    }
                    default:
                        break;
                }
                m_task_queue.pop();
            }
            QThread::msleep(10);
        }
    }

    void GLSessionManager::handleDrawFront(const std::pair<GLRenderer*,SDL_GL_TASK>& crt_task)
    {
        SDL_UI_MODULE::GLWidget* glWidget = (SDL_UI_MODULE::GLWidget*)(crt_task.second.param.drawFrontTask.target_reciver);
        emit glWidget->beginGLDraw();
        crt_task.first->setReceiver(glWidget);
        crt_task.first->__start();
    }

    void GLSessionManager::handleLoadTexture(const std::pair<GLRenderer*, SDL_GL_TASK> &crt_task) {
        LoadTextureTask_param temp = crt_task.second.param.loadTextureTask;

        int width,height,nrComponents;
        unsigned char* data = stbi_load(temp.texture_path,&width,&height,&nrComponents,0);
        SDL_SHADER_INFO* attachedShader = (SDL_SHADER_INFO*)temp.attachedShader;

        if(data)
        {
            GLenum format;
            if(nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            SDL_GL_TEXTURE_INFO *tex_info = new SDL_GL_TEXTURE_INFO();
            tex_info->index = temp.index;
            tex_info->address = data;
            tex_info->width = width;
            tex_info->height = height;
            tex_info->format = format;
            tex_info->status = TEXTURE_ST_UNLOAD;
            strcpy(tex_info->path, temp.texture_path);

            if (attachedShader)
            {
                attachedShader->textures[temp.index] = tex_info;
            }
            m_textures.push_back(tex_info);
        }

        crt_task.first->__update();
    }

    void GLSessionManager::handleCompileShader(const std::pair<GLRenderer*, SDL_GL_TASK> &crt_task) {
        char vs_path[128];
        char fs_path[128];

        switch (crt_task.second.param.shaderCompileTask.rule) {
            case ParserRule::ShaderToy:
            {
                ShaderParser* parser = ShaderParser::getParser();
                assert(parser != nullptr);
                SDL_UI_MODULE::CodeEditor* codeEditor = SDL_UI_MODULE::CodeEditor::getEditor();
                parser->setContextParserRule(ParserRule::ShaderToy);

                crt_task.first->clearShaders();

                if(codeEditor->comboShader())
                {
                    parser->setComboParse(true);
                    crt_task.first->setTitle("Image");
                    std::vector<SDL_UI_MODULE::EditorPage*> pages = codeEditor->getPages();

                    for(SDL_UI_MODULE::EditorPage* page : pages)
                    {
                        auto shader = page->GetShader();
                        shader->inited = false;

                        if(!codeEditor->m_scriptTypelist.contains(page->title))
                        {
                            QByteArray s = page->title.toLatin1();
                            strcpy(shader->path_vertex,USER_TEMPORARY_SHADER_PATH);
                            strcat(shader->path_vertex,"/");
                            strcat(shader->path_vertex,s.data());
                            strcat(shader->path_vertex,"_combo.vert");

                            strcpy(shader->name, s.data());

                            strcpy(shader->path_fragment,USER_TEMPORARY_SHADER_PATH);
                            strcat(shader->path_fragment,"/");
                            strcat(shader->path_fragment,s.data());
                            strcat(shader->path_fragment,"_combo.frag");

                            parser->setCurrentFileName(QString::fromLatin1(s.data()) + "_combo.vert",ShaderType::VERTEX);
                            parser->parse("",ShaderType::VERTEX);

                            parser->setCurrentFileName(QString::fromLatin1(s.data()) + "_combo.frag",ShaderType::FRAGMENT);
                            parser->parse(codeEditor->findPageContentByName(page->title),ShaderType::FRAGMENT);
                        }
                        crt_task.first->addShader(shader);
                    }
                    emit shaderParseDone();

                    crt_task.first->__exit();
                    SDL_UI_MODULE::GLWidget* glWidget = (SDL_UI_MODULE::GLWidget*)crt_task.first->m_receiver;
                    glWidget->__reshow();
                    crt_task.first->__restart();
                }
                else
                {
                    auto shader = codeEditor->currentPage()->GetShader();

                    parser->setComboParse(false);
                    QsciScintilla* page = (QsciScintilla*)codeEditor->currentWidget();
                    QFileInfo fileInfo(codeEditor->m_currentFile.fileName());
                    QString fileName = fileInfo.baseName();
                    QByteArray s = fileName.toLatin1();
                    crt_task.first->setTitle(s.data());

					strcpy(shader->path_vertex, USER_TEMPORARY_SHADER_PATH);
					strcat(shader->path_vertex, "/");
					strcat(shader->path_vertex, s.data());
					strcat(shader->path_vertex, ".vert");

                    strcpy(shader->name, s.data());

					strcpy(shader->path_fragment, USER_TEMPORARY_SHADER_PATH);
					strcat(shader->path_fragment, "/");
					strcat(shader->path_fragment, s.data());
					strcat(shader->path_fragment, ".frag");

					parser->setCurrentFileName(QString::fromLatin1(s.data()) + ".vert", ShaderType::VERTEX);
					parser->parse("", ShaderType::VERTEX);

					parser->setCurrentFileName(QString::fromLatin1(s.data()) + ".frag", ShaderType::FRAGMENT);
					parser->parse(page->text(), ShaderType::FRAGMENT);

                    emit shaderParseDone();

                    crt_task.first->__exit();
                
                    crt_task.first->clearShaders();
                    shader->inited = false;
                    crt_task.first->addShader(shader);
                    SDL_UI_MODULE::GLWidget* glWidget = (SDL_UI_MODULE::GLWidget*)crt_task.first->m_receiver;
                    glWidget->__reshow();
                    crt_task.first->__start();
                }

                break;
            }
        }
    }

    void GLSessionManager::handleAVRecord(const std::pair<GLRenderer*, SDL_GL_TASK> &crt_task) {
        VideoManager::getVideoManager()->writeRecord(crt_task.second.param.recordTask.frameAddr);
    }

    GLSessionManager::GLSessionManager() {

    }

    GLSessionManager::~GLSessionManager() {

    }


} // SDL_GL_MODULE