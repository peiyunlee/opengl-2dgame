#include "Transform.h"

Transform::Transform(mat4 &mxView, mat4 &mxProjection, int pointNum, vec4 *points, vec4 *colors) {
	_pointNum = pointNum;
	_points = points;
	_colors = colors;
	CreateBufferObject();
	SetShader(mxView, mxProjection);
}

void Transform::CreateBufferObject() {

	glGenVertexArrays(1, &_uiVao);
	glBindVertexArray(_uiVao);
	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*_points)*_pointNum + sizeof(*_colors)*_pointNum, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(*_points)*_pointNum, *_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(*_points)*_pointNum, sizeof(*_colors)*_pointNum, *_colors);
}

void Transform::SetShader(mat4 &mxView, mat4 &mxProjection, GLuint uiShaderHandle) {

	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader("vsVtxColor.glsl", "fsVtxColor.glsl");
	else _uiProgram = uiShaderHandle;

	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(*_colors)*_pointNum));

	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	_mxView = mxView;
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	_mxProjection = mxProjection;
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Transform::SetTRSMatrix(mat4 &mat)
{
	//�x�}����
	_mxTRS = mat;
	_bUpdateMV = true;
}

void Transform::Draw() {
	glUseProgram(_uiProgram);
	glBindVertexArray(_uiVao);

	if (_bUpdateMV) {
		_mxMVFinal = _mxView*_mxTRS;
		_bUpdateMV = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

}

void Transform::Reset() {
	mat4 init;
	_mxTRS = init;
	_mxMVFinal = init;
	_mxOri = init;
}

void Transform::SetColorA(float a,int startIndex, int endIndex) {
	for (int i = startIndex; i <= endIndex; i++)
	{
		_colors[i].w = a;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(*_points)*_pointNum + sizeof(*_colors)*startIndex, sizeof(*_colors)*(endIndex-startIndex)+1, _colors[startIndex]);
}