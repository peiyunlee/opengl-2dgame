#include "Planet.h"

Planet::Planet(float radius, int colorType, int pointsNum) {

	_pointsNum = pointsNum;

	_points = new vec4[_pointsNum];
	_colors = new vec4[_pointsNum];

	vec4 color;

	if (colorType == 1) color = vec4(0.4f, 0.56f, 0.8f,1.0f); //淡藍
	else if(colorType == 2)  color = vec4(0.7f, 0.4f, 0.8f, 1.0f);  //淡綠
	else if (colorType == 3) color = vec4(0.4f, 0.7f, 0.68f, 1.0f); //淡紫
	else color = vec4(0.84f, 0.85f, 0.61f, 1.0f); //淡藍紫

	for (int i = 0; i < _pointsNum; i++) {
		_points[i].x = radius * cosf(M_PI*2.0f*i / _pointsNum); //2拍乘以該點分之總點
		_points[i].y = radius * sinf(M_PI*2.0f*i / _pointsNum);
		_points[i].w = 1.0f;
		_colors[i] = color;
	}

	CreateBufferObject();
	_bUpdateProj = false;
}

Planet::~Planet() {
	delete[] _points;
	delete[] _colors;
}

void Planet::CreateBufferObject() {

	glGenVertexArrays(1, &_uiVao);
	glBindVertexArray(_uiVao);

	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*_points)*_pointsNum + sizeof(*_colors)*_pointsNum, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(*_points)*_pointsNum, *_points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(*_points)*_pointsNum, sizeof(*_colors)*_pointsNum, *_colors);
}
void Planet::SetShader(mat4 &mxView, mat4 &mxProjection, GLuint uiShaderHandle) {

	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader("vsPlanet2.glsl", "fsVtxColor.glsl");
	else _uiProgram = uiShaderHandle;

	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(*_points)*_pointsNum));

	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	_mxView = mxView;
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	_mxProjection = mxProjection;
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float g_Tottime = 0.0;
GLfloat tx = 0.0f;
GLfloat ty = 0.0f;

void Planet::AutoTranslate(float dt) {
	mat4 mxTra;

	g_Tottime += dt;
	if (g_Tottime >= 10.0)g_Tottime -= 10.0f;

	ty = g_Tottime*-1.0f;
	mxTra = Translate(tx, ty,0.0f);

	SetTRSMatrix(mxTra);
}

void Planet::SetTRSMatrix(mat4 &mat)
{
	_mxTRS = mat;
	_bUpdateMV = true;
}

void Planet::Draw() {
	glUseProgram(_uiProgram);
	glBindVertexArray(_uiVao);

	//更新
	if (_bUpdateMV) {
		_mxMVFinal = _mxView*_mxTRS;
		_bUpdateMV = false;
	}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	glDrawArrays(GL_TRIANGLE_FAN, 0, _pointsNum);
}