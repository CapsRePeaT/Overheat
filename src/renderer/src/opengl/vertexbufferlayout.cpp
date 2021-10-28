#include "vertexbufferlayout.h"

#include <opengllog.h>

unsigned int VertexBufferLayout::VertexBufferElement::size() const {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
	switch (type) {
		case gl::GL_FLOAT:          return sizeof(gl::GLfloat);
		case gl::GL_UNSIGNED_INT:   return sizeof(gl::GLuint);
		case gl::GL_UNSIGNED_BYTE:  return sizeof(gl::GLbyte);
	}
	OGL_ASSERT(false);
	return 0;
#pragma clang diagnostic pop
}
