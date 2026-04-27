#include "Model.h"
#include <iostream>

Model::Model(const std::vector<float>& vertex, const std::vector<float>& UV, const std::vector<float>& normal)
{
	//calculo el numero de vertex. (x,y,z)
	this->numVertex = vertex.size() / 3;

	//generem el VAO y el VBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1,&this->VBO);

	//definim el VAO com a actiu
	glBindVertexArray(this->VAO);

	//define el VBO de la posicio i li pasa les dades.
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), vertex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FALSE, GL_FALSE, 3 * sizeof(float), (void*)0);

	//activem el atribut 0
	glEnableVertexAttribArray(0);
	

	//desactivem VAO i VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void Model::Render() const
{
	//vinculo el VAO 
	glBindVertexArray(this->VAO);

	//incido 
	glDrawArrays(GL_TRIANGLES, 0, this->numVertex);


	glBindVertexArray(0);

}
