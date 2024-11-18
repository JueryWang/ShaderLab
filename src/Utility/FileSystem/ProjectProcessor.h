#pragma once
#include "GL/GL_DefaultDfs.h"
#include "UI/EditorPage.h"
#include <stdlib.h>
#include <stdio.h>
#include <map>

namespace SDL_GL_MODULE
{
	class GLRenderer;
}
namespace SDL_UTILS
{
	enum SDL_ASSET_TYPE
	{
		IMAGE,
		AUDIO,
		MODEL,
		SCENE
	};

	enum SDL_IMAGE_FORMAT
	{
		JPG,
		PNG
	};

	enum SDL_AUDIO_FORMAT
	{
		MP3,
		WAV,
		PCM
	};

	enum SDL_DATA_ENCODING
	{
		SDL_DATA_ENCODING_RAW
	};

	enum SDL_CHUNK_TYPE
	{
		SDL_CHUNK_PAGE_DATA = 0,
		SDL_CHUNK_IMAGE_DATA,
		SDL_CHUNK_AUDIO_DATA,
		SDL_CHUNK_MODEL_DATA,
		SDL_CHUNK_SCENE_DATA

	};
	typedef struct SDL_HEADER_t
	{
		union
		{
			unsigned int magic;
			char magic_name[4];			//SDL - version number
		};

		unsigned int size;				//struct size
		char author[32];
		unsigned int num_chunks;
		unsigned int flags;
	} SDL_HEADER;


	typedef struct SDL_CHUNK_HEADER_t
	{
		union
		{
			unsigned int chunk_type;
			char	 chunk_name[4];
		};
		unsigned int size;
	}SDL_CHUNK_HEADER;

	typedef struct SDL_SHADER_PAGE_t
	{
		SDL_CHUNK_HEADER	header;
		unsigned int shadertype;	    //use opengl type
		unsigned int encoding;
		unsigned int handle;
		unsigned int texture[4] = { 0,0,0,0 };
		unsigned int reference[4] = { 0,0,0,0 };
		unsigned int data_offset;		//struct SDL_SHADER_PPAGE size
		unsigned int data_size;			//data content size
		bool frame_buffer;
		char page_name[64];
	} SDL_SHADER_PAGE;

	typedef struct SDL_ASSET_IMAGE_t
	{
		SDL_CHUNK_HEADER header;
		SDL_IMAGE_FORMAT format;
		int image_index;
		char path[64];
		unsigned int data_offset;
	}SDL_ASSET_IMAGE;

	typedef struct SDL_ASSET_AUDIO_t
	{
		SDL_CHUNK_HEADER header;
		SDL_AUDIO_FORMAT format;
		char path[64];
		unsigned int data_offset;
	} SDL_ASSET_AUDIO;

	enum ProcessingStatus
	{
		Idle,
		ReadingProject,
		WritingProject
	};

	class ProjectProcessor
	{
	public:
		ProjectProcessor();
		~ProjectProcessor();
		void setRenderer(SDL_GL_MODULE::GLRenderer* glRenderer);
		bool save(const char* filePath);
		bool load(const char* filePath);

	private:
		void writeHeader();
		void writePage();
		void writePageContents();
		void writeTextures();

	private:
		FILE* m_fileHandler = NULL;
		const char* m_filePath = nullptr;

		std::map<GLuint, unsigned int> m_textureTable;
		std::map<SDL_UI_MODULE::EditorPage*, unsigned int> m_pageTable;

		unsigned long chunksum_cursorPos = 0;
		int num_chunk = 0;
		int chunk_index_page = 0;
		int chunk_index_texture = 0;
		SDL_GL_MODULE::GLRenderer* m_glRenderer = nullptr;
		ProcessingStatus m_status = Idle;

		std::vector< SDL_SHADER_PAGE> m_readPages;
		std::map<unsigned int, std::string> m_pageContentMp;
		std::map<unsigned int, std::string> m_texPathMp;
	};

}