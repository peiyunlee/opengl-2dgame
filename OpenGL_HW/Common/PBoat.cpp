#include "PBoat.h"

PBoat::PBoat() {

	//pBRSide
	_points[0] = vec4(0.150f, -0.12f, 0.0, 1.0f);
	_points[1] = vec4(0.230f, -0.12f, 0.0, 1.0f);
	_points[2] = vec4(0.150f, 0.02f, 0.0, 1.0f);

	//pBLSide
	_points[3] = vec4(-0.150f, -0.12f, 0.0, 1.0f);
	_points[4] = vec4(-0.230f, -0.12f, 0.0, 1.0f);
	_points[5] = vec4(-0.150f, 0.02f, 0.0, 1.0f);

	//pBBody
	_points[6] = vec4(-0.150f, -0.16f, 0.0, 1.0f);
	_points[7] = vec4(-0.150f, 0.16f, 0.0, 1.0f);
	_points[8] = vec4(0.150f, -0.16f, 0.0, 1.0f);
	_points[9] = vec4(0.150f, 0.16f, 0.0, 1.0f);

	//cBRSide
	_colors[0] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	_colors[1] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	_colors[2] = vec4(0.78f, 0.208f, 0.22f, 1.0f);

	//cBLSide
	_colors[3] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	_colors[4] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	_colors[5] = vec4(0.78f, 0.208f, 0.22f, 1.0f);

	//cBBody
	_colors[6] = vec4(0.94f, 0.94f, 0.94f, 1.0f);
	_colors[7] = vec4(0.94f, 0.94f, 0.94f, 1.0f);
	_colors[8] = vec4(0.94f, 0.94f, 0.94f, 1.0f);
	_colors[9] = vec4(0.94f, 0.94f, 0.94f, 1.0f);
	//pBCenter
	for (int i = 10; i < 30; i++) {
		_points[i].x = 0.07f * cosf(M_PI*2.0f*(float)(i- 10) / BCPOINT_NUM); //2拍乘以該點分之總點
		_points[i].y = 0.07f * sinf(M_PI*2.0f*(float)(i- 10) / BCPOINT_NUM)-0.02f;
		_points[i].w = 1.0f;
		_colors[i] = vec4(0.0667f, 0.1333f, 0.2392f, 1.0f);
	}

	//pbTop
	for (int i = 30; i < 41; i++) {
		_points[i].x = 0.15f * cosf(M_PI*2.0f*(float)(i) / BCPOINT_NUM); //2拍乘以該點分之總點
		_points[i].y = -0.15f * sinf(M_PI*2.0f*(float)(i) / BCPOINT_NUM) +0.13f;
		_points[i].w = 1.0f;
		_colors[i] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	}

	//pBFire Yellow
	for (int i = 50; i > 40; i--) {
		_points[i].x = 0.016f*(i-51)+0.01f;
		_points[i].y = 10.0f*(_points[i].x)*(_points[i].x) + 0.025f*(_points[i].x) - 0.37f;
		_points[i].w = 1.0f;
		_colors[i] = vec4(0.87f, 0.58f, 0.22f, 1.0f);
	}

	_points[51].x = 0.0f;
	_points[51].y = -0.37f;
	_points[51].w = 1.0f;
	_colors[51] = vec4(0.87f, 0.58f, 0.22f, 1.0f);

	for (int i = 52; i < 62; i++) {
		_points[i].x = 0.016f*(i - 51)- 0.01f;;
		_points[i].y = 10.0f*(_points[i].x)*(_points[i].x)-0.025f*(_points[i].x) - 0.37f;
		_points[i].w = 1.0f;
		_colors[i] = vec4(0.87f, 0.58f, 0.22f, 1.0f);
	}

	//pBFire red
	for (int i = 71; i > 61; i--) {
		_points[i].x = 0.008f*(i - 72) ;
		_points[i].y = 20.0f*(_points[i].x)*(_points[i].x) + 0.05f*(_points[i].x) - 0.275f;
		_points[i].w = 1.0f;
		_colors[i] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	}

	_points[72].x = 0.0f;
	_points[72].y = -0.275f;
	_points[72].w = 1.0f;
	_colors[72] = vec4(0.78f, 0.208f, 0.22f, 1.0f);

	for (int i = 73; i < 83; i++) {
		_points[i].x = 0.008f*(i - 72) ;
		_points[i].y = 20.0f*(_points[i].x)*(_points[i].x) - 0.05f*(_points[i].x) - 0.275f;
		_points[i].w = 1.0f;
		_colors[i] = vec4(0.78f, 0.208f, 0.22f, 1.0f);
	}

	CreateBufferObject();
}

void PBoat::CreateBufferObject() {

	glGenVertexArrays(1, &_uiVao);
	glBindVertexArray(_uiVao);

	glGenBuffers(1, &_uiBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_points) + sizeof(_colors), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_points), _points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(_points), sizeof(_colors), _colors);
}

void PBoat::SetShader(mat4 &mxView, mat4 &mxProjection, GLuint uiShaderHandle) {

	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader("vsVtxColor.glsl", "fsVtxColor.glsl");
	else _uiProgram = uiShaderHandle;

	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(_uiProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(_points)));

	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	_mxView = mxView;
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	_mxProjection = mxProjection;
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PBoat::Draw() {
	glUseProgram(_uiProgram);
	glBindVertexArray(_uiVao);

	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxMVFinal);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, BSPOINT_NUM);
	glDrawArrays(GL_TRIANGLE_STRIP, BSPOINT_NUM, BSPOINT_NUM);
	glDrawArrays(GL_TRIANGLE_STRIP, BSPOINT_NUM*2, BBPOINT_NUM);
	glDrawArrays(GL_TRIANGLE_FAN, BSPOINT_NUM * 2+BBPOINT_NUM, BCPOINT_NUM);
	glDrawArrays(GL_TRIANGLE_FAN, BSPOINT_NUM * 2 + BBPOINT_NUM+ BCPOINT_NUM, BTPOINT_NUM);
	glDrawArrays(GL_TRIANGLE_FAN, BSPOINT_NUM * 2 + BBPOINT_NUM + BCPOINT_NUM+ BTPOINT_NUM, BPPOINT_NUM);
	glDrawArrays(GL_TRIANGLE_FAN, BSPOINT_NUM * 2 + BBPOINT_NUM + BCPOINT_NUM + BTPOINT_NUM+ BPPOINT_NUM, BPPOINT_NUM);
}