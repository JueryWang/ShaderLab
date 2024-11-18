#ifndef GL_DEFAULT_DFS_H
#define GL_DEFAULT_DFS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <QObject>
#include "Shader.h"
#include "DefaultDfs.h"
#include "Utility/Parser/ShaderParser_Dfs.h"

namespace SDL_UI_MODULE
{
    class VarShowBoard;
}

namespace SDL_GL_MODULE {

#define MAX_GL_RENDERER 3

#define GL_INNER_UI_COMPONENT_DEFAULT_WIDTH_RATIO 0.5
#define GL_INNER_UI_COMPONENT_DEFAULT_HEIGHT_RATIO 0.8

#define GL_INNER_UI_COMPONENT_MINIMUM_WIDTH_RATIO 0.2
#define GL_INNER_UI_COMPONENT_MINIMUM_HEIGHT_RATIO 0.2

#define GL_INNER_UI_COMPONENT_MAXIMUM_WIDTH_RATIO 0.8
#define GL_INNER_UI_COMPONENT_MAXIMUM_HEIGHT_RATIO 0.8

    enum PixelType {
        SDL_GL_RGB,
        SDL_GL_RGBA
    };

    enum SDL_RENDERER_STATE {
        ACTIVE_DRAW_FRONT = 0,
        ACTIVE_DRAW_BACK,
        INACTIVE
    };

    enum SDL_GL_TASK_TYPE
    {
        GL_DRAW_FRONT = 0,
        GL_COMPILE_SHADER,
        GL_LOAD_TEXTURE,
        GL_RECORD
    };

    enum SDL_GL_SCRIPT_TYPE {
        SCRIPT_NOTYPE =     0b00000001,
        SCRIPT_COMMON =     0b00000010,
        SCRIPT_IMAGE  =     0b00000100,
        SCRIPT_BUFFERA =    0b00001000,
        SCRIPT_BUFFERB =    0b00010000,
        SCRIPT_BUFFERC =    0b00100000,
        SCRIPT_BUFFERD =    0b01000000,
        SCRIPT_CUBEMAP =    0b10000000
    };

    struct DrawFrontTask_param
    {
        QObject* target_reciver;
    };

    struct LoadTextureTask_param
    {
        int index;
        char texture_path[64];
        void* attachedShader;
    };

    struct ShaderCompileTask_param
    {
        SDL_UTILS::ParserRule rule;
        SDL_UI_MODULE::VarShowBoard* varboard;
    };

    struct RecordTask_param
    {
        uint8_t* frameAddr;
    };

    union GL_Task_param
    {
        DrawFrontTask_param drawFrontTask;
        LoadTextureTask_param loadTextureTask;
        ShaderCompileTask_param shaderCompileTask;
        RecordTask_param recordTask;
    };

    struct SDL_GL_TASK
    {
        SDL_GL_TASK_TYPE type;
        GL_Task_param param;
    };

    enum TEXTURE_STATUS
    {
        TEXTURE_ST_UNLOAD = 0,
        TEXTURE_ST_LOADED,
        TEXTURE_ST_DEPRECATED
    };

    struct SDL_GL_TEXTURE_INFO
    {
        int index = -1;
        int width;
        int height;
        int channel = 3;
        GLenum format;
        GLuint textureID = 0;
        unsigned char* address = nullptr;
        char path[64];
        TEXTURE_STATUS status = TEXTURE_ST_DEPRECATED;
    };

    enum SDL_RENDERER_FLAG_BIT
    {
        RECORD = 0b00001000,
        UPDATE = 0b00000100,
        PAUSE  = 0b00000010,
        EXIT   = 0b00000001,
    };

    enum SDL_RENDER_FLAG_BIT_POS
    {
        BIT_RECORD_POS = 3,
        BIT_UPDATE_POS = 2,
        BIT_PAUSE_POS = 1,
        BIT_EXIT_POS = 0
    };

    enum SDL_SHADER_TYPE
    {
        SDL_GL_MAIN_SHADER,
        SDL_GL_BUFFER_BUFFER,
        SDL_GL_COMPUTE_SHADER
    };

    struct SDL_SHADER_INFO
    {

    public:
        SDL_SHADER_INFO() :program(nullptr), inited(false), order(0), path_vertex(), path_fragment(), fbo(0), framebufferTex(0), reference(), type(SCRIPT_IMAGE) {}
        ~SDL_SHADER_INFO()
        {

        }
        void clearContext()
		{
            if (program)
            {
                delete program;
            }
			if (fbo > 0)
			{
				glDeleteFramebuffers(1, &fbo);
			}
            if (framebufferTex)
            {
                glDeleteTextures(1, &framebufferTex);
            }
            for (int i = 0; i < 4; i++)
            {
                if (textures[i]->textureID > 0)
                {
                    glDeleteTextures(1, &(textures[i]->textureID));
                }
                delete textures[i];
            }
		}

    public:
        Shader *program = nullptr;
        bool inited = false;
        bool should_clear = false;
        //As the number rises,the priority of it decrease,which means 0 should be first executed
        int order = 0;
        bool queryed = false;
        char name[32];
        char path_vertex[128];
        char path_fragment[128];
        GLuint fbo = -1;
        GLuint framebufferTex = -1;
        std::array<std::weak_ptr<SDL_SHADER_INFO>, 4> reference;
        SDL_GL_SCRIPT_TYPE type;
        std::array<SDL_GL_TEXTURE_INFO*, 4> textures = { nullptr,nullptr,nullptr,nullptr };
    };

    void inline setShaderOrder(std::shared_ptr<SDL_SHADER_INFO> shader_info,int &result)
    {
        if(!shader_info->reference.empty())
        {
            for(auto &ref: shader_info->reference)
            {
                std::shared_ptr<SDL_SHADER_INFO> shaderRef = ref.lock();

                if (ref.lock())
                {
                    if(shaderRef && shaderRef->order + 1 > result)
                    {
                        result = shaderRef->order+1;
                    }
                }
            }
        }
    }
}
#endif
