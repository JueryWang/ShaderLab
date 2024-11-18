#include "ProjectProcessor.h"
#include "UI/AssetWindow.h"
#include "UI/CodeEditor.h"
#include "GL/GLRenderer.h"
#include "Utility/GlobalConfig.h"
#include <string>
#include <iostream>

namespace SDL_UTILS
{
	std::string tagPage = "page";
	std::string tagTexture = "texture";
	int PAGE_INDEX_OFFSET = static_cast<int>(tagPage[0])*1 + static_cast<int>(tagPage[1])*10 + static_cast<int>(tagPage[2])*100 + static_cast<int>(tagPage[3]) * 1000;
	int TEXTURE_INDEX_OFFSET = static_cast<int>(tagTexture[0])*1 + static_cast<int>(tagTexture[1])*10 + static_cast<int>(tagTexture[2])*100 + static_cast<int>(tagTexture[3])*1000 + static_cast<int>(tagTexture[4]) * 10000;

	ProjectProcessor::ProjectProcessor()
	{

	}

	ProjectProcessor::~ProjectProcessor()
	{
		if (m_fileHandler)
		{
			fclose(m_fileHandler);
		}
	}

	void ProjectProcessor::setRenderer(SDL_GL_MODULE::GLRenderer* glRenderer)
	{
		m_glRenderer = glRenderer;
	}

	bool ProjectProcessor::save(const char* filePath)
	{
		m_status = WritingProject;
		m_filePath = filePath;
		if (m_fileHandler != NULL)
		{
			fclose(m_fileHandler);
		}
		m_fileHandler = fopen(filePath, "wb");
		if (m_fileHandler == NULL)
		{
			m_status = Idle;
			std::cerr << " Error filePath: " << std::endl;
			return false;
		}
		writePage();
		writePageContents();
		writeTextures();
		writeHeader();
		return true;
	}

	bool ProjectProcessor::load(const char* filePath)
	{
		if (m_fileHandler != NULL)
		{
			fclose(m_fileHandler);
		}
		m_status = ReadingProject;
		m_filePath = filePath;
		m_fileHandler = fopen(filePath,"rb");
		if (m_fileHandler == NULL)
		{
			m_status = Idle;
			std::cerr << " Error filePath:" <<filePath<< std::endl;
			return false;
		}

		fseek(m_fileHandler, -sizeof(SDL_HEADER), SEEK_END);
		SDL_HEADER header;
		fread(&header, sizeof(SDL_HEADER), 1, m_fileHandler);

		int num_texture = 0;
		
		fseek(m_fileHandler, 0, SEEK_SET);
		for (int i = 0; i < header.num_chunks; i++)
		{
			SDL_SHADER_PAGE pageChunk;
			fread(&pageChunk, sizeof(SDL_SHADER_PAGE), 1, m_fileHandler);
			for (int i = 0; i < 4; i++)
			{
				if (pageChunk.texture[i] > 0)
				{
					num_texture++;
				}

			}
			m_readPages.push_back(pageChunk);
		}


		for (int i = 0; i < m_readPages.size(); i++)
		{
			char c;
			while ((c = fgetc(m_fileHandler)) != '#' && c != EOF)
			{
				continue;
			}
			fseek(m_fileHandler, -1, SEEK_CUR);

			int contentSize = m_readPages[i].data_size;
			std::string pageContent;
			pageContent.resize(contentSize);
			unsigned int pageHandle;
			fscanf(m_fileHandler, "#Page:%d", &pageHandle);
			fscanf(m_fileHandler, "#content:");
			for (int i = 0; i < contentSize; i++)
			{
				pageContent[i] = fgetc(m_fileHandler);
			}
			m_pageContentMp[pageHandle] = pageContent;
		}

		for (int i = 0; i < num_texture; i++)
		{

		}

		return false;
	}

	void ProjectProcessor::writeHeader()
	{
		SDL_HEADER header;
		strcpy(header.magic_name, "SDL");
		header.size = sizeof(header);
		strcpy(header.author, "juery");
		header.num_chunks = num_chunk;
		header.flags = 0;
		fwrite(&header, sizeof(SDL_HEADER), 1, m_fileHandler);
	}

	void ProjectProcessor::writePage()
	{
		SDL_UI_MODULE::CodeEditor* codeEditor = SDL_UI_MODULE::CodeEditor::getEditor();

		m_glRenderer->sortShader();
		std::vector<std::shared_ptr<SDL_GL_MODULE::SDL_SHADER_INFO>> sortedShaders = m_glRenderer->getAttachedShaders();

		for (auto& shader : sortedShaders)
		{
			SDL_UI_MODULE::EditorPage* attachedPage = codeEditor->findEditorPageByShader(shader);
			SDL_CHUNK_HEADER chunkHeader;
			SDL_SHADER_PAGE pageChunk;

			strcpy(chunkHeader.chunk_name, "page");
			chunkHeader.size = sizeof(SDL_CHUNK_HEADER);

			pageChunk.header = chunkHeader;
			pageChunk.shadertype = shader.get()->type;
			pageChunk.encoding = SDL_DATA_ENCODING_RAW;
			pageChunk.handle = PAGE_INDEX_OFFSET + chunk_index_page;
			chunk_index_page++;
			m_pageTable[attachedPage] = pageChunk.handle;
			for (int i = 0; i < 4; i++)
			{
				if (shader.get()->textures[i])
				{
					pageChunk.texture[i] = TEXTURE_INDEX_OFFSET + chunk_index_texture;
					chunk_index_page++;
					m_textureTable[shader.get()->textures[i]->textureID] = pageChunk.texture[i];
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (shader.get()->reference[i].lock() != nullptr)
				{
					SDL_UI_MODULE::EditorPage* refPage = codeEditor->findEditorPageByShader(shader.get()->reference[i].lock());
					pageChunk.reference[i] = m_pageTable[refPage];
				}
			}
			pageChunk.data_offset = sizeof(SDL_SHADER_PAGE);
			pageChunk.data_size = attachedPage->text().length();
			pageChunk.frame_buffer = shader.get()->fbo == -1 ? 0 : 1;
			strcpy(pageChunk.page_name, attachedPage->title.toLocal8Bit());
			fwrite(&pageChunk, sizeof(SDL_SHADER_PAGE), 1, m_fileHandler);
			num_chunk++;
		}
	}

	void ProjectProcessor::writePageContents()
	{
		m_glRenderer->sortShader();
		std::vector<std::shared_ptr<SDL_GL_MODULE::SDL_SHADER_INFO>> sortedShaders = m_glRenderer->getAttachedShaders();

		for (auto& shader : sortedShaders)
		{
			SDL_UI_MODULE::EditorPage* page = SDL_UI_MODULE::CodeEditor::s_codeEditor->findEditorPageByShader(shader);
			fprintf(m_fileHandler,"#Page:%d",m_pageTable[page]);
			fputs("#content:", m_fileHandler);
			fputs(page->text().toLatin1(), m_fileHandler);
			fputs("\n", m_fileHandler);
		}
	}

	void ProjectProcessor::writeTextures()
	{
		SDL_UI_MODULE::CodeEditor* codeEditor = SDL_UI_MODULE::CodeEditor::getEditor();

		std::vector< SDL_UI_MODULE::EditorPage*> pages = codeEditor->getPages();
		for (auto& page : pages)
		{
			for (int i = 0; i < 4; i++)
			{
				if (page->GetShader().get()->textures[i])
				{
					fprintf(m_fileHandler, "#Texture:%d", m_textureTable[page->GetShader().get()->textures[i]->textureID]);
					fwrite(&page->GetShader().get()->textures[i]->width, sizeof(int), 1,m_fileHandler);
					fwrite(&page->GetShader().get()->textures[i]->height, sizeof(int), 1, m_fileHandler);
					fwrite(&page->GetShader().get()->textures[i]->channel, sizeof(int), 1, m_fileHandler);
					fputs("#path:", m_fileHandler);
					fputs(page->GetShader().get()->textures[i]->path, m_fileHandler);
					fputs("\n", m_fileHandler);
				}
			}
		}
	}
}
