#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include <memory>
#include "ExpandableBuffer.h"

using std::shared_ptr;

class OpenGLBatch
{

public:

	class VertexBufferObject
	{
	public:
		VertexBufferObject(): _vboId(0)
		{
		}

		~VertexBufferObject()
		{
			glDeleteBuffers(1, &_vboId);
		}

		void generateBufferObject()
		{
			glGenBuffers(1, &_vboId); // Ask OpenGL to generate 1 buffer and store id in _vboId
		}

		static void unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vboId);
		}

		template <class V>
		void bufferData(ExpandableBuffer<V>& buffer)
		{
			glBufferData(GL_ARRAY_BUFFER, buffer.sizeInBytes(), &(buffer.rawData().get()[0]), GL_STATIC_DRAW);
		}

	public:
		GLuint _vboId;
	};

	class VertexArrayObject
	{
	public:
		VertexArrayObject() : _vaoId(0)
		{
		};

		void generateArrayObject()
		{
			glGenVertexArrays(1, &_vaoId); // Ask OpenGL to generate 1 buffer and store id in _vboId
		}

		static void unbind()
		{
			glBindVertexArray(0);
		}

		void bind()
		{
			glBindVertexArray(_vaoId);
		}
	private:
		GLuint _vaoId;
	};
	
	OpenGLBatch() : _sizeInBytes(0), _numOfVertices(0)
	{
		// Automatically create and assign vbo and vao
	}

	~OpenGLBatch() {}

	void generate()
	{
		_vao.generateArrayObject();
		_vbo.generateBufferObject();
	}

	void bind()
	{
		// Assumption is using interleaved data so only 1 buffer is used
		_vao.bind();
		_vbo.bind();
	}

	static void unbind()
	{
		VertexBufferObject::unbind();
		VertexArrayObject::unbind();
	}

	template <class V>
	void bufferData(ExpandableBuffer<V>& buffer)
	{
		_sizeInBytes = buffer.sizeInBytes();
		_numOfVertices = buffer.size();
		_vbo.bufferData(buffer);
	}

	const int sizeInBytes() const { return _sizeInBytes; };
	const int numOfVertices() const { return _numOfVertices; };

private:
	VertexBufferObject _vbo;
	VertexArrayObject _vao;
	int _numOfVertices;
	int _sizeInBytes;
};

