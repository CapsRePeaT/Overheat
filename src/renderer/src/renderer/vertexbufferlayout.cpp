#include "vertexbufferlayout.h"

#include <cassert>

namespace renderer {

unsigned int VertexBufferLayout::VertexBufferElement::size() const {
	switch (type) {
		case GL_FLOAT:
			return sizeof(float);
		case GL_UNSIGNED_INT:
			return sizeof(uint32_t);
		case GL_BYTE:
			return sizeof(int8_t);
		case GL_DOUBLE:
			return sizeof(double);
	}
	return 0;
}

}  // namespace renderer
