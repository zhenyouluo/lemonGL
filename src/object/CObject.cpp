/*
	Copyright 2012 Thomas Le Guerroué-Drévillon
	This file is part of lemonGL.

	lemonGL is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	lemonGL is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with lemonGL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CObject.h"

CObject::CObject():
m_parent(NULL),
m_mat(CMaterialMngr::get("default")),
m_translucent(false)
{
	m_modelMatrix.setToIdentity();
}

CObject::CObject(const CObject& p_o):
m_parent(p_o.m_parent),
m_mat(p_o.m_mat)
{

}

void CObject::draw(const CMVP* p_camera, QList<CLight*>* p_lights, QVector2D p_biasVP){
	float mvp[16];
	float mv[16];
	float mvi[9];
	const QMatrix4x4 camView = p_camera->viewMatrix();
	const QMatrix4x4 camProj = p_camera->projMatrix();
	const QMatrix4x4 modelView = camView * modelMatrix();
	const QMatrix4x4 modelViewProj = camProj * modelView;
	const CLight* light;

	p_camera->setViewPort(p_biasVP);

    this->convertMatrix(modelView, mv);
    this->convertMatrix(modelView.normalMatrix(), mvi);
    this->convertMatrix(modelViewProj, mvp);

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, CVA::vertices() );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, CVA::textures() );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, CVA::normals() );

	CShaderInterface::sendUniformMatrix4fv("mvp", 1, GL_TRUE, mvp);
	CShaderInterface::sendUniformMatrix4fv("mv", 1, GL_TRUE, mv);
	CShaderInterface::sendUniformMatrix3fv("mvi", 1, GL_TRUE, mvi);

	order(modelViewProj);

	CVA::enable();

	if(m_mat != NULL)
		m_mat->sendToProgram();

	if(p_lights == NULL)
		drawSub();
	else{
		for(int a = 0; a < p_lights->size(); a++){
			light = p_lights->at(a);
			if(p_lights->at(a)->enabled()){
				light->sendToProgram(p_camera, m_modelMatrix);

				drawSub();
			}
		}
	}

	CVA::disable();
}

void CObject::setParent(const CObject* p_obj){
	m_parent = p_obj;
}

void CObject::setMaterial(const CMaterial* p_mat){
	m_mat = p_mat;
}

void CObject::pushMatrix(){
	m_matrixStack.append(m_modelMatrix);
}

void CObject::popMatrix(){
	m_modelMatrix = m_matrixStack.takeLast();
}

void CObject::loadIdentity(){
	m_modelMatrix.setToIdentity();
}

void CObject::translate(GLfloat x, GLfloat y, GLfloat z)
{
	m_modelMatrix.translate(x, y, z);
}

void CObject::rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	m_modelMatrix.rotate(angle, x, y, z);
}

void CObject::scale(GLfloat x, GLfloat y, GLfloat z)
{
	m_modelMatrix.scale(x, y, z);
}

const QMatrix4x4 CObject::modelMatrix() const {
	if(m_parent != NULL)
		return m_parent->modelMatrix() * m_modelMatrix;
	return m_modelMatrix;
}

void CObject::convertMatrix(const QMatrix4x4 p_in, float* p_out){
	for(unsigned int a = 0; a < 4; a++)
		for(unsigned int b = 0; b < 4; b++)
			p_out[a*4 + b] = p_in(a, b);
}

void CObject::convertMatrix(const QMatrix3x3 p_in, float* p_out){
	for(unsigned int a = 0; a < 3; a++)
		for(unsigned int b = 0; b < 3; b++)
			p_out[a*3 + b] = p_in(a, b);
}
