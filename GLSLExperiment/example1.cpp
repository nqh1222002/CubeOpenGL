﻿// Display a rotating cube with lighting
//
// Light and material properties are sent to the shader as uniform
// variables. Vertex positions and normals are sent after each
// rotation.
#include "Angel.h"
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;
const int NumPoints = 36; //(6 faces)(2 triangles/face)
point4 points[NumPoints];
vec3 normals[NumPoints];
color4 colors[NumPoints];
// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
						point4(-0.5, -0.5, 0.5, 1.0),
						point4(-0.5, 0.5, 0.5, 1.0),
						point4(0.5, 0.5, 0.5, 1.0),
						point4(0.5, -0.5, 0.5, 1.0),
						point4(-0.5, -0.5, -0.5, 1.0),
						point4(-0.5, 0.5, -0.5, 1.0),
						point4(0.5, 0.5, -0.5, 1.0),
						point4(0.5, -0.5, -0.5, 1.0)
};
// Array of rotation angles (in degrees) for each coordinate axis

GLfloat Theta[3] = { 0,0,0 };
GLfloat eyeDistance = 3;
GLfloat cameraMove[3] = { 0,0,eyeDistance };
// Model-view and projection matrices uniform location
GLuint modelViewLoc, projectionLoc;
mat4 model_view;
GLfloat value[] = { 0,0,0,0 };
//----------------------------------------------------------------------
// quad generates two triangles for each face and assigns colors
// to the vertices
int Index = 0;
void quad(int a, int b, int c, int d)
{
	// Initialize temporary vectors along the quad’s edge to
	// compute its face normal
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));
	normals[Index] = normal; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; points[Index] = vertices[d]; Index++;
}
//----------------------------------------------------------------------
 //generate 12 triangles: 36 vertices and 36 colors
void colorcube(void)
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);	
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
//----------------------------------------------------------------------
// OpenGL initialization
void init(void)
{
	colorcube();
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),sizeof(normals), normals);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
	// Initialize shader lighting parameters
	point4 light_position(0, 0, 1, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);
	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_specular(1.0, 0.8, 0.0, 1.0);
	float material_shininess = 100;
	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "ambientObject"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "diffuseObject"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "specularObject"),
		1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);
	// Retrieve transformation uniform variable locations
	modelViewLoc = glGetUniformLocation(program, "ModelView");
	projectionLoc = glGetUniformLocation(program, "Projection");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
//----------------------------------------------------------------------
void ban4chan() {
	mat4 matban = Scale(1.2, 0.02, 0.6);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * matban);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	mat4 chan1 = Translate(-.57, -.41, .27) * Scale(.06, .8, .06);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * chan1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	mat4 chan2 = Translate(-.57, -.41, -.27) * Scale(.06, .8, .06);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * chan2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	mat4 chan3 = Translate(.57, -.41, -.27) * Scale(.06, .8, .06);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * chan3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	mat4 chan4 = Translate(.57, -.41, .27) * Scale(.06, .8, .06);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * chan4);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void nganKeo() {
	//tu ngan keo
	mat4 tuNganKeo = Translate(-.55, -.06, 0) * Scale(.02, .1, .48);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tuNganKeo);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//ngăn kéo
	mat4 nganKeo = Translate(-.27, -.1, 0) * Translate(0, 0, value[0]) * Scale(.54, .02, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * nganKeo);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//cua ngan keo
	mat4 cuaNganKeo = Translate(-.27, -.075, .28) * Translate(0, 0, value[0]) * Scale(.54, .15, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaNganKeo);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hopTu() {
	//tu ban sau
	mat4 tuBanSau = Translate(0, -.36, -.29) * Scale(1.08, .7, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tuBanSau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//hop tu phai
	mat4 hopTuPhai = Translate(.55, -.36, 0) * Scale(.02, .7, .48);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuPhai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//hop tu trai
	mat4 hopTuTrai = Translate(0, -.36, 0) * Scale(.02, .7, .56);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuTrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//hop tu day
	mat4 hopTuDay = Translate(.27, -.7, 0) * Scale(.55, .02, .56);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuDay);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//cua tu
	mat4 cuaTu = Translate(.53, -.36, .29) * RotateY(value[1]) * Translate(-.27, 0, 0) * Scale(.55, .7, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaTu);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void keSach() {
	//xương cạnh trái kệ sách
	mat4 xuongDoc1 = Translate(-.59,.51,-.20) * Scale(.02, 1, .2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//xương cạnh phải
	mat4 xuongDoc2 = Translate(.59, .51, -.20) * Scale(.02, 1, .2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//tấm chắn sau kệ
	//xương ngang trên cùng
	mat4 xuongNgang1 = Translate(0, .91, -.20) * Scale(1.16,.02,.2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongNgang1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//xương ngang thứ 2
	mat4 xuongNgang2 = Translate(0, .66, -.20) * Scale(1.16, .02, .2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongNgang2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//xương ngang thứ 3
	mat4 xuongNgang3 = Translate(0, .41, -.2) * Scale(1.16, .02, .2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongNgang3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//tấm nối xương 1 - 2 
	mat4 tamNoi1_2 = Translate(0, .785, -.2) * Scale(.02,.23,.2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNoi1_2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//tủ mini
	//hộp tủ sau
	mat4 hopTuSau = Translate(0, .535, -.29) * Scale(.31, .23, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuSau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//hộp tủ trái
	mat4 hopTuTrai = Translate(-.15, .535, -.2) * Scale(.01, .23, .19);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuTrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//hộp tủ phai
	mat4 hopTuPhai = Translate(.15, .535, -.2) * Scale(.01, .23, .19);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuPhai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//cua tu
	mat4 cuaTu = Translate(.155, .535, -.1)*RotateY(value[1])*Translate(-.165,0,0) * Scale(.31, .23, .01);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaTu);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void table() {
	ban4chan();
	nganKeo();
	hopTu();
	keSach();
}

void keTu() {
	//phần kệ dưới
	//mặt trên kệ
	mat4 matTren = Scale(2, .02, .6);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * matTren);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// hộp tủ sau
	mat4 hopTuSau = Translate(0,-.24,-.29) * Scale(1.96,.46,.02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuSau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//hộp tủ trái
	mat4 hopTuTrai = Translate(-.99,-.24,0) * Scale(.02,.46,.6);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuTrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//hộp tủ phải
	mat4 hopTuPhai = Translate(.99, -.24, 0) * Scale(.02, .46, .6);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * hopTuPhai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//mặt đáy kệ
	mat4 matDay = Translate(0,-.47,0) * Scale(2,.02,.6);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * matDay);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tấm giữa ngang
	mat4 tamGiuaNgang = Translate(0, -.24, 0) * Scale(2, .02, .6);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamGiuaNgang);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tấm dọc 1
	mat4 tamDoc1 = Translate(-.40,-.12,.02) * Scale(.02,.22,.58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamDoc1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tấm dọc 2
	mat4 tamDoc2 = Translate(.40, -.12, .02) * Scale(.02, .22, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamDoc2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tấm dọc 3
	mat4 tamDoc3 = Translate(-.5, -.36, .02) * Scale(.02, .21, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamDoc3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tấm dọc 4
	mat4 tamDoc4 = Translate(0, -.36, .02) * Scale(.02, .21, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamDoc4);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tấm dọc 5
	mat4 tamDoc5 = Translate(.5, -.36, .02) * Scale(.02, .21, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamDoc5);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	
}
void ngan_keo() {
	//ngan keo 
	mat4 nganKeo1 = Translate(-.75, -.45, 0) * Translate(0, 0, value[0]) * Scale(.47, .02, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * nganKeo1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 nganKeo2 = Translate(-.25, -.45, 0) * Translate(0, 0, value[0]) * Scale(.47, .02, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * nganKeo2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 nganKeo3 = Translate(.25, -.45, 0) * Translate(0, 0, value[0]) * Scale(.47, .02, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * nganKeo3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 nganKeo4 = Translate(.75, -.45, 0) * Translate(0, 0, value[0]) * Scale(.47, .02, .58);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * nganKeo4);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//cua keo
	mat4 cuaKeo1 = Translate(-.75, -.36, .29) * Translate(0, 0, value[0]) * Scale(.49, .25, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaKeo1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 cuaKeo2 = Translate(-.25, -.36, .29) * Translate(0, 0, value[0]) * Scale(.49, .25, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaKeo2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 cuaKeo3 = Translate(.25, -.36, .29) * Translate(0, 0, value[0]) * Scale(.49, .25, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaKeo3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	mat4 cuaKeo4 = Translate(.75, -.36, .29) * Translate(0, 0, value[0]) * Scale(.49, .25, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * cuaKeo4);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuKinh() {
	//xuong doc 1
	mat4 xuongDoc1 = Translate(-.99,.61,-.15) * Scale(.02, 1.2, .3);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//xuong doc 2
	mat4 xuongDoc2 = Translate(-.63, .61, -.15) * Scale(.02, 1.2, .3);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//xuong doc 3
	mat4 xuongDoc3 = Translate(.63, .61, -.15) * Scale(.02, 1.2, .3);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//xuong doc 4
	mat4 xuongDoc4 = Translate(.99, .61, -.15) * Scale(.02, 1.2, .3);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc4);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//xuong doc sau trai
	mat4 xuongDoc5 = Translate(-.81,.61,-.29) * Scale(.34, 1.2, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc5);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//xuong doc sau phai
	mat4 xuongDoc6 = Translate(.81, .61, -.29) * Scale(.34, 1.2, .02);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * xuongDoc6);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	
	//tam ngang 1
	mat4 tamNgang1 = Translate(-.81,1.22,-.15) * Scale(.38, .02, .3);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang1);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//tam ngang 2
	mat4 tamNgang2 = Translate(.81, 1.22, -.15) * Scale(.38, .02, .3);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang2);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang 3
	mat4 tamNgang3 = Translate(-.81, .92, -.15) * Scale(.34, .01, .28);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang3);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang 4
	mat4 tamNgang4 = Translate(.81, .92, -.15) * Scale(.34, .01, .28);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang4);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang 5
	mat4 tamNgang5 = Translate(-.81, .62, -.15) * Scale(.34, .01, .28);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang5);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang 6
	mat4 tamNgang6 = Translate(.81, .62, -.15) * Scale(.34, .01, .28);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang6);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang 7
	mat4 tamNgang7 = Translate(-.81, .32, -.15) * Scale(.34, .01, .28);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang7);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang 8
	mat4 tamNgang8 = Translate(.81, .32, -.15) * Scale(.34, .01, .28);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgang8);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//tam ngang giua
	mat4 tamNgangGiua = Translate(0,1.1,-.15) * Scale(1.28, .02, .2);
	glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, model_view * tamNgangGiua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void keTV() {
	keTu();
	ngan_keo();
	tuKinh();
}
void cameraController() {
	const vec3 cameraPos(cameraMove[0], cameraMove[1], cameraMove[2]);
	model_view = Translate(-cameraPos) *
		RotateX(Theta[0]) *
		RotateY(Theta[1]) *
		RotateZ(Theta[2]);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// camera controller
	cameraController();

	//draw model
	//table();
	keTV();

	glutSwapBuffers();
}
//----------------------------------------------------------------------


void keyboard(unsigned char key, int x, int y)
{
	
	switch (key) {
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'p':
		Theta[0] += 5;
		if (Theta[0] > 360) Theta[0] -= 360;
		glutPostRedisplay();
		break;
	case 'P':
		Theta[0] -= 5;
		if (Theta[0] > 360) Theta[0] -= 360;
		glutPostRedisplay();
		break;
	case 'y':
		Theta[1] -= 5;
		if (Theta[1] > 360) Theta[1] -= 360;
		glutPostRedisplay();
		break;
	case 'Y':
		Theta[1] += 5;
		if (Theta[1] > 360) Theta[1] -= 360;
		glutPostRedisplay();
		break;
	case 'r':
		Theta[2] -= 5;
		if (Theta[2] > 360) Theta[2] -= 360;
		glutPostRedisplay();
		break;
	case 'R':
		Theta[2] += 5;
		if (Theta[2] > 360) Theta[2] -= 360;
		glutPostRedisplay();
		break;
	case 'u':
		cameraMove[1] += 0.2;
		glutPostRedisplay();
		break;
	case 'U':
		cameraMove[1] -= 0.2;
		glutPostRedisplay();
		break;
	case 'd': cameraMove[0] += 0.2;
		if (cameraMove[0] > 2) cameraMove[0] = 2;
		glutPostRedisplay();
		break;
	case 'a': cameraMove[0] -= 0.2;
		if (cameraMove[0] < -2) cameraMove[0] = -2;
		glutPostRedisplay();
		break;
	case 'w': cameraMove[2] -= 0.2;
		if (cameraMove[2] > eyeDistance +1) cameraMove[2] = eyeDistance + 1;
		glutPostRedisplay();
		break;
	case 's': cameraMove[2] += 0.2;
		if (cameraMove[2] < -(eyeDistance+1)) cameraMove[2] = -(eyeDistance + 1);
		glutPostRedisplay();
		break;
	case 'k':
		value[0] += .05;
		if (value[0] > .54) value[0] = .54;
		glutPostRedisplay();
		break;
	case 'K':
		value[0] -= .05;
		if (value[0] < 0) value[0] = 0;
		glutPostRedisplay();
		break;
	case 'c':
		value[1] += 5;
		if (value[1] > 90) value[1] = 90;
		glutPostRedisplay();
		break;
	case 'C':
		value[1] -= 5;
		if (value[1] < 0) value[1] = 0;
		glutPostRedisplay();
		break;
	}
}
//----------------------------------------------------------------------
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	GLfloat aspect = GLfloat(width) / height;
	mat4 projection = Perspective(45, aspect, 0.5, 3.0);
	glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);
}
//----------------------------------------------------------------------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(650, 650);
	glutInitWindowPosition(350, 100);
	glutCreateWindow("Draw an object");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	
	glutMainLoop();

	
	return 0;
}