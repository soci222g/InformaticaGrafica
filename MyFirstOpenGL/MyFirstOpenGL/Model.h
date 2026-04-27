#ifndef MODEL_H
#define MODEL_H


#include <vector>
#include <GL/glew.h>


class Model
{
public:
	Model(const std::vector<float>& vertex, const std::vector<float>& UV, const std::vector<float>& normal);
	void Render() const;

private:
	GLuint VAO, VBO;
	unsigned int numVertex;
};

#endif