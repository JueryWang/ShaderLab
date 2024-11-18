#include "GL/GLRenderer.h"
#include "UI/UI_DefaultDfs.h"
#include "UI/GLWidget.h"
#include "Utility/AssetsManager/Video/VideoManager.h"
#include "GLSessionManager.h"
#include "EvSendFrame.h"
#include <QTimer>
#include <QApplication>
#include <QThread>
#include <memory>

using namespace SDL_GL_MODULE;

namespace SDL_GL_MODULE {

    std::atomic<int> GLRenderer::SCR_WIDTH = 0;
    std::atomic<int> GLRenderer::SCR_HEIGHT = 0;
    int GLRenderer::resolution[2];
    std::timed_mutex glLocker;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    int frame = 0;
    unsigned int quadVAO;
    unsigned int quadVBO;
    static uint8_t* sdl_opengl_runtime_buffer = nullptr;
    static int bit_per_pixel;

    inline void flip(uint8_t* buf,int context_width,int context_height)
    {
        static uint8_t* sdl_opengl_runtime_buffer = (uint8_t*) malloc(GLRenderer::SCR_WIDTH * GLRenderer::SCR_HEIGHT *  bit_per_pixel);

        int totalLength = context_width * context_height * bit_per_pixel;
        int oneLineLength = context_width * bit_per_pixel;
        memcpy(sdl_opengl_runtime_buffer,buf,totalLength);
        memset(buf,0,sizeof(uchar) * totalLength);
        for(int i =0;i<context_height;i++)
        {
            memcpy(buf + oneLineLength * i,sdl_opengl_runtime_buffer + totalLength - oneLineLength * (i+1),oneLineLength);
        }
    }

    GLRenderer::GLRenderer(const std::string& title,PixelType type,QObject* receiver) : m_title(title),m_PixType(type)
    {
        m_receiver = receiver;
        if(GLRenderer::SCR_WIDTH == 0 || GLRenderer::SCR_HEIGHT == 0)
        {
            return;
        }
        switch (type) {
            case SDL_GL_RGB :
                m_windowBufSize = GLRenderer::SCR_WIDTH * GLRenderer::SCR_HEIGHT * 3 * sizeof(uchar);
                m_windowbuf = malloc(m_windowBufSize);
                bit_per_pixel = 3;
                break;
            case SDL_GL_RGBA:
                m_windowBufSize = GLRenderer::SCR_WIDTH * GLRenderer::SCR_HEIGHT * 4 * sizeof(uchar);
                m_windowbuf = malloc(m_windowBufSize);
                bit_per_pixel = 4;
                break;
        }

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_SAMPLES,4);
        glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
    }

    GLRenderer::~GLRenderer() {
        free(m_windowbuf);
        delete m_mainShader;
    }

    void GLRenderer::setReceiver(QObject *receiver) {
        m_receiver = receiver;
    }

    void GLRenderer::setTitle(const char *title) {
        m_title = title;
    }


	void GLRenderer::clearShaders()
	{
		for (auto& shader : m_attachedShaders)
		{
			shader.reset();
		}

		m_attachedShaders.clear();
	}



	void GLRenderer::initShader(std::shared_ptr<SDL_SHADER_INFO> &shader_info) {
		//Ring reference detect
		shader_info->queryed = true;

        if(!shader_info->inited)
        {
            for(auto & ref_shader : shader_info->reference)
            {
				auto temp = ref_shader.lock();
				if (shader_info->queryed)
					goto initOperaton;

				if (temp != nullptr && !ref_shader.lock()->inited)
				{
					initShader(temp);
				}
            }

		initOperaton:
			if (shader_info->type == SCRIPT_IMAGE)
			{
				//last render mainImage
				shader_info->order = m_attachedShaders.size();
			}
			else
			{
				setShaderOrder(shader_info,shader_info->order);
			}
            shader_info->inited = true;
            shader_info->program = new Shader(shader_info->path_vertex,shader_info->path_fragment);
            for (int i = 0;i<4 ; i++)
            {
                if(shader_info->textures[i])
                {
                    std::pair<GLRenderer*,SDL_GL_TASK> loadtextureTsk;
                    loadtextureTsk.first = this;
                    loadtextureTsk.second.type = GL_LOAD_TEXTURE;
                    loadtextureTsk.second.param.loadTextureTask.index = i;
                    GLSessionManager::getInstance()->addTask(loadtextureTsk);
                }
            }
        }
    }

	void GLRenderer::addShader(const std::shared_ptr<SDL_SHADER_INFO>& shader_info) {

        m_attachedShaders.push_back(shader_info);

        setStatus(UPDATE,true);
    }

	void GLRenderer::sortShader()
	{
		std::sort(m_attachedShaders.begin(), m_attachedShaders.end(), [&](std::shared_ptr<SDL_SHADER_INFO> shader1, std::shared_ptr<SDL_SHADER_INFO> shader2) {
			return shader1->order < shader2->order;
			});
	}

    int GLRenderer::getWidth() const {
        return SCR_WIDTH.load(std::memory_order_consume);
    }

    int GLRenderer::getHeight() const {
        return SCR_HEIGHT.load(std::memory_order_consume);
    }

    void inline GLRenderer::setStatus(uchar bit_mask, bool open_bit) {
        if(open_bit)
        {
            uchar temp = state_bit.load(std::memory_order_consume) | bit_mask;
            state_bit.store(temp,std::memory_order_release);
        }
        else
        {
            uchar temp = state_bit.load(std::memory_order_consume);
            temp = temp & (bit_mask ^ temp);
            state_bit.store(temp,std::memory_order_release);
        }
    }

    bool inline GLRenderer::getFlag(SDL_RENDER_FLAG_BIT_POS pos) {
        return state_bit.load(std::memory_order_consume) & (1<<pos);
    }

    void GLRenderer::__exit() {
        setStatus(EXIT,true);
    }

    void GLRenderer::__start() {
        setStatus(PAUSE | EXIT,false);
        glfwSetTime(m_anchor_time);
    }

    void GLRenderer::__pause() {
        setStatus(PAUSE,true);
        m_anchor_time = glfwGetTime();
    }

    void GLRenderer::__restart() {
        glfwSetTime(0.0f);
        setStatus(PAUSE | EXIT,false);
        frame = 0;
    }

    void GLRenderer::__update() {
        setStatus(UPDATE,true);
    }

    void GLRenderer::__record() {
		SDL_UTILS::VideoManager::getVideoManager()->setRecordParam(m_title.c_str(), SCR_WIDTH, SCR_HEIGHT);
        setStatus(RECORD,true);
    }

    void GLRenderer::__endrecord() {
        setStatus(RECORD,false);
		emit SDL_UTILS::VideoManager::getVideoManager()->recordDone();
    }

    void GLRenderer::__reset(const QSize &newSize) {
        SDL_GL_MODULE::GLRenderer::SCR_WIDTH.store(newSize.width(),std::memory_order_release);
        SDL_GL_MODULE::GLRenderer::SCR_HEIGHT.store(newSize.height(),std::memory_order_release);
        SDL_GL_MODULE::GLRenderer::resolution[0] = newSize.width();
        SDL_GL_MODULE::GLRenderer::resolution[1] = newSize.height();
    }

    void GLRenderer::renderQuad(int context_width, int context_height) {
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, (float)context_height,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					1.0f,  1.0f, 0.0f, (float)context_width,(float)context_height,
					1.0f, -1.0f, 0.0f, (float)context_width, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
    }

	void GLRenderer::renderScene(float time,int frame)
	{
		for (auto& shader : m_attachedShaders)
		{
			shader->program->use();
			shader->program->setFloat("runtime_data.iTime", time);
			shader->program->setInt("runtime_data.iFrame", frame);
			glUniform2iv(glGetUniformLocation(m_mainShader->ID, "iResolution"), 1, &resolution[0]);

			switch (shader->type)
			{
				case SCRIPT_NOTYPE:
				case SCRIPT_IMAGE:
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					break;
				case SCRIPT_BUFFERA:
				case SCRIPT_BUFFERB:
				case SCRIPT_BUFFERC:
				case SCRIPT_BUFFERD:
					glBindFramebuffer(GL_FRAMEBUFFER, shader->fbo);
					break;
			}

			for (int i = 0; i < 4; i++)
			{
				if (shader->reference[i].lock() != nullptr)	//FrameBufferTexture
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, shader->reference[i].lock()->framebufferTex);
				}
				if (shader->textures[i])		//NormalTexture
				{
					glActiveTexture(GL_TEXTURE0 + i);
				}
			}
			renderQuad(GLRenderer::SCR_WIDTH, GLRenderer::SCR_HEIGHT);
		}
	}

    void GLRenderer::loadTextures() {
		std::vector<int> removeIdxs;
		int pos = 0;
		for (SDL_GL_TEXTURE_INFO* texture_info : GLSessionManager::getInstance()->m_textures)
		{
			if (texture_info->status == TEXTURE_ST_UNLOAD)
			{
				glActiveTexture(GL_TEXTURE0 + texture_info->index);
				unsigned int textureID;
				glGenTextures(1, &textureID);
				texture_info->textureID = textureID;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, texture_info->format, texture_info->width,
					texture_info->height, 0, texture_info->format, GL_UNSIGNED_BYTE, texture_info->address);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				texture_info->status = TEXTURE_ST_LOADED;
			}
			if (texture_info->status == TEXTURE_ST_DEPRECATED)
			{
				glDeleteTextures(1, &texture_info->textureID);
				removeIdxs.push_back(pos);
			}
			pos++;
		}

		//Remove DEPRECATED textures
		auto begin = GLSessionManager::getInstance()->m_textures.begin();
		for (int remove_idx : removeIdxs)
		{
            GLSessionManager::getInstance()->m_textures.erase(begin + remove_idx);
		}
    }

    void GLRenderer::loadShaders() {
		std::for_each(m_attachedShaders.begin(), m_attachedShaders.end(), [&](std::shared_ptr<SDL_SHADER_INFO>& shader) { shader->queryed = false; });

		for (auto it = m_attachedShaders.begin(); it != m_attachedShaders.end();)
		{
			std::shared_ptr<SDL_SHADER_INFO> shader = *it;
			if (shader->should_clear)
			{
				shader->clearContext();
				auto it = std::find(m_attachedShaders.begin(), m_attachedShaders.end(), shader);
				shader.reset();
				if (it != m_attachedShaders.end())
				{
					std::swap(*it, *(m_attachedShaders.end() - 1));
				}
				m_attachedShaders.erase(m_attachedShaders.end() - 1);
			}
			else
			{
				initShader(shader);
				it++;
			}
		}

		sortShader();

		for (auto& shader : m_attachedShaders)
		{
			if (shader->type & (SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERA | SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERB | SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERC | SDL_GL_SCRIPT_TYPE::SCRIPT_BUFFERD))
			{
				glGenFramebuffers(1, &shader->fbo);
				glBindFramebuffer(GL_FRAMEBUFFER, shader->fbo);
				glGenTextures(1, &shader->framebufferTex);
				glBindTexture(GL_TEXTURE_2D, shader->framebufferTex);
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, SCR_WIDTH, SCR_HEIGHT);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shader->framebufferTex, 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);
			}
		}

	}

	void GLRenderer::updateGLContext() {
		loadTextures();
		loadShaders();
	}

	void GLRenderer::contextDraw() {
		static char windowTitle[32];
		static int windowCount = 0;
		bool firstRecord = false;
		memcpy(windowTitle, m_title.c_str(), m_title.size() * sizeof(char));

		static GLFWwindow* window_inst = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
		m_window.reset(window_inst);
		if (window_inst == nullptr)
		{
			//Do some log Record
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window.release());
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//Do some log Record
			return;
		}

		if (m_mainShader != nullptr)
		{
			glDeleteProgram(m_mainShader->ID);
			delete m_mainShader;
		}


		auto it = std::find_if(m_attachedShaders.begin(), m_attachedShaders.end(), [](std::shared_ptr<SDL_SHADER_INFO> shader) { return shader->type == SCRIPT_IMAGE; });
		if (it == m_attachedShaders.end())
			return;

		m_mainShader = new Shader((*it)->path_vertex, (*it)->path_fragment);
		m_mainShader->use();

		loadTextures();
		//glEnable(GL_SAMPLE_SHADING);
		//glMInSampleShading(0.7)

		while (!getFlag(BIT_EXIT_POS))
		{
			if (getFlag(BIT_PAUSE_POS))
			{
				QThread::msleep(10);
				continue;
			}
			if (getFlag(BIT_UPDATE_POS))
			{
				updateGLContext();
				setStatus(UPDATE, false);
			}

			float time = glfwGetTime();
			deltaTime = time - lastTime;
			lastTime = time;
			m_fps = 1. / (deltaTime);

			int context_width = SCR_WIDTH.load(std::memory_order_consume);
			int context_height = SCR_HEIGHT.load(std::memory_order_consume);

			renderScene(time, frame);
			frame++;

			switch (m_PixType) {
			case SDL_GL_RGB:
				glReadPixels(0, 0, context_width, context_height, GL_RGB, GL_UNSIGNED_BYTE, m_windowbuf);
				break;
			case SDL_GL_RGBA:
				glReadPixels(0, 0, context_width, context_height, GL_RGBA, GL_UNSIGNED_BYTE, m_windowbuf);
				break;
			default:
				break;
			}

			flip((uint8_t*)(m_windowbuf), context_width, context_height);

			memcpy(SDL_UI_MODULE::GLWidget::sdl_glwgt_pctBuffing, m_windowbuf, m_windowBufSize);

			auto event = std::make_unique<EvSendFrame>();
			QApplication::postEvent(m_receiver, event.release(), Qt::HighEventPriority);

			if (getFlag(BIT_RECORD_POS))
			{
				std::pair<GLRenderer*, SDL_GL_TASK> recordTask;
				recordTask.first = this;
				recordTask.second.type = GL_RECORD;
				int frame_size = SCR_WIDTH.load(std::memory_order_consume) * SCR_HEIGHT.load(std::memory_order_consume) * 3 * sizeof(uchar);
				uint8_t* newFrame = (uint8_t*)malloc(frame_size);
				switch (m_PixType)
				{
				case SDL_GL_RGB:
					memcpy(newFrame, m_windowbuf, frame_size);
					recordTask.second.param.recordTask.frameAddr = newFrame;
					break;
				case SDL_GL_RGBA:
					//ignore alpha bit
					//RGBA2RBG(&newFrame, &((uint8_t*)m_windowbuf));
					recordTask.second.param.recordTask.frameAddr = newFrame;
					break;
				}
				GLSessionManager::getInstance()->addTask(recordTask);
			}

			glfwSwapBuffers(window_inst);
			glfwSwapInterval(1);
			glfwPollEvents();
		}
    }
}