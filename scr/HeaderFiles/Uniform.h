#pragma once
#include <gl/glew.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Shader.h"
#include <string>

// wrapper class to upload uniforms to the GPU, it has several offsprings that relate to each of the uniforms used in the shaders.
class Shader;


	class Uniform {
	protected:
		GLuint location;
		GLuint size;
		GLenum type;

		std::string name;
		Shader*owner;



	public:
		Uniform(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) {
			this->location = location;
			this->size = size;
			this->name = name;
			this->type = type;
			this->owner = owner;
		}
		Shader* getOwner() {
			return this->owner;
		}

		std::string getName() {
			return name;
		}

		GLenum getType() {
			return type;
		}

		GLuint getSize() {
			return size;
		}

		GLuint getLocation() {
			return location;
		}

		inline bool operator == (const Uniform &u) const
		{
			return (bool)(name.compare(u.name));
		}

		class UniformHash {
		public:
			size_t operator()(const Uniform& u) const { return (u.location * 2654435761U) ^ u.owner->GetProgram(); }
		};


	};

	class uFloat : public Uniform {
	public:
		//uFloat(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(float parameter) {
			assert(this->owner->BeingUsed() );
			glUniform1f(this->getLocation(), parameter);
		}

	};
	class u2Float : public Uniform {
	public:
		//u2Float(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(glm::vec2 parameter) {
			assert(this->owner->BeingUsed());
			glUniform2f(this->getLocation(), parameter.x, parameter.y);

		}
		void Set(float x, float y) {
			assert(this->owner->BeingUsed());
			glUniform2f(this->getLocation(), x, y);

		}

	};
	class u3Float : public Uniform {
	public:
		//u3Float(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(glm::vec3 parameter) {
			assert(this->owner->BeingUsed());
			glUniform3f(this->getLocation(), parameter.x, parameter.y, parameter.z);

		}
		void Set(float x, float y, float z) {
			assert(this->owner->BeingUsed());
			glUniform3f(this->getLocation(), x, y, z);

		}

	};

	class u4Float : public Uniform {
	public:
		//u4Float(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(glm::vec4 parameter) {
			assert(this->owner->BeingUsed());
			glUniform4f(this->getLocation(), parameter.x, parameter.y, parameter.z, parameter.w);

		}
		void Set(float x, float y, float z, float w) {
			assert(this->owner->BeingUsed());
			glUniform4f(this->getLocation(), x, y, z, w);

		}

	};
	class uMatrix4Float : public Uniform {
	public:
		//u4Float(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(glm::mat4 matrix, int traspose) {
			assert(traspose < 2 && traspose>=0);
			assert(this->owner->BeingUsed());
			glUniformMatrix4fv(this->getLocation(),1, traspose, &(matrix[0][0]));


		}

	};

	class uInt : public Uniform {
	public:
		//uInt(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(int parameter) {
			assert(this->owner->BeingUsed() && "Program not active!!");
			glUniform1i(this->getLocation(), parameter);

		}

	};
	class u2Int : public Uniform {
	public:
		//u2Int(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(glm::vec2 parameter) {
			assert(this->owner->BeingUsed());
			glUniform2i(this->getLocation(), (int)parameter.x, (int)parameter.y);

		}
		void Set(int x, int y) {
			assert(this->owner->BeingUsed());
			glUniform2i(this->getLocation(), x, y);

		}

	};
	class u3Int : public Uniform {
	public:
		//u3Int(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(glm::vec3 parameter) {
			assert(this->owner->BeingUsed());
			glUniform3i(this->getLocation(), (int)parameter.x, (int)parameter.y, (int)parameter.z);

		}
		void Set(int x, int y, int z) {
			assert(this->owner->BeingUsed());
			glUniform3i(this->getLocation(), x, y, z);

		}

	};

	class u1Floatv : public Uniform {
	public:
		//u3Int(Shader* owner, GLuint location, GLuint size, char* name, GLenum type) : Uniform(owner, location, size, name, type) {}
		void Set(int size,GLfloat* value) {
			assert(this->owner->BeingUsed());
			glUniform1fv(this->getLocation(), size,value);

		}

	};

