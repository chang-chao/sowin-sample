// This example demonstrates how to set up and SoWin renderarea within
// a "stand-alone" application window.

#include <stdlib.h> // exit()
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinRenderArea.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
//#include <Inventor/nodes/SoGradientBackground.h>
#include "SoFCBackgroundGradient.h"

#include <stdio.h>
int readFile(float xyz[][3], int* size);

// Dummy message handler for 2nd window
LRESULT CALLBACK
WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void convertTextToBinary() {
	float** xyz1 = (float**)malloc(10000000 * 3 * sizeof(float));
	int pointSize = 0;
	readFile((float(*)[3])xyz1, &pointSize);


	FILE *f = fopen("C:\\chang\\sph\\solver-output\\OT-SOLID00020.bin", "wb");
	int written = fwrite(xyz1, sizeof(float), pointSize*3, f);
	if (written == 0) {
		printf("Error during writing to file !");
	}
	fclose(f);


}

int readFile(float xyz[][3],int* size) {
FILE * fp;
size_t len = 0;
char line[256];
fp = fopen("C:\\chang\\sph\\solver-output\\OT-SOLID00020.DAT", "r");
if (fp == NULL)
exit(EXIT_FAILURE);
int size_t = 0;
char remains[256];
while ( fgets(line, sizeof(line), fp) !=NULL) {
	sscanf(line, "%f    %f    %f %s", &xyz[size_t][0],&xyz[size_t][1],&xyz[size_t][2], remains);
	size_t++;
}

fclose(fp);
*size = size_t;
return 0;
}

int readFileFromBinary(float xyz[][3], int* size) {
	FILE * fp;
	fp = fopen("C:\\chang\\sph\\solver-output\\OT-SOLID00020.bin", "rb");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	fseek(fp, 0L, SEEK_END);
	long lSize = ftell(fp);
	rewind(fp);


	size_t read=fread(xyz, lSize, 1, fp);

	fclose(fp);
	*size = lSize/sizeof(float)/3;
	return 0;
}

int
main_sample(int argc, char ** argv)
{

	float** xyz1 = (float**)malloc(10000000*3*sizeof(float));
	int pointSize = 0;
	readFileFromBinary((float(*)[3])xyz1,&pointSize);

	WNDCLASSEX wcl;
	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hInstance = NULL;
	wcl.lpszClassName = "AppWindow";
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = 0;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	//wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcl.hbrBackground= CreateSolidBrush(RGB(255, 255, 255));

	if (!RegisterClassEx(&wcl)) { exit(1); }
	HWND appwin = CreateWindow("AppWindow",
		"AppWindow", // title
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		HWND_DESKTOP,
		NULL,
		(HINSTANCE)NULL,
		NULL);


	SoWin::init(appwin);

	//HWND appwin = SoWin::init("test");

	// Create a scene cointaining a sphere
	SoFCBackgroundGradient::initClass();

	SoFCBackgroundGradient *pcBackGround = new SoFCBackgroundGradient;
	float r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
	unsigned long col1 = 3940932863UL;
	unsigned long col2 = 859006463UL; // default color (dark blue)
	unsigned long col3 = 2880160255UL; // default color (blue/grey)
	unsigned long col4 = 1869583359UL; // default color (blue/grey)

	r1 = ((col1 >> 24) & 0xff) / 255.0; g1 = ((col1 >> 16) & 0xff) / 255.0; b1 = ((col1 >> 8) & 0xff) / 255.0;
	r2 = ((col2 >> 24) & 0xff) / 255.0; g2 = ((col2 >> 16) & 0xff) / 255.0; b2 = ((col2 >> 8) & 0xff) / 255.0;
	r3 = ((col3 >> 24) & 0xff) / 255.0; g3 = ((col3 >> 16) & 0xff) / 255.0; b3 = ((col3 >> 8) & 0xff) / 255.0;
	r4 = ((col4 >> 24) & 0xff) / 255.0; g4 = ((col4 >> 16) & 0xff) / 255.0; b4 = ((col4 >> 8) & 0xff) / 255.0;

	pcBackGround->setColorGradient(SbColor(r2, g2, b2), SbColor(r3, g3, b3), SbColor(r4, g4, b4));


	SoSeparator * root = new SoSeparator;
	root->ref(); // increment the root's reference counter
	SoPerspectiveCamera * camera = new SoPerspectiveCamera;
	SoDirectionalLight * light = new SoDirectionalLight;
	SoMaterial * material = new SoMaterial;
	material->diffuseColor.setValue(1, 0, 0);
	SoSphere * sphere = new SoSphere;
	SoCoordinate3* coordinate = new SoCoordinate3();
	//float xyz[][3] = { { -1, 1, 0 },{ -1, -1, 0 },{ 1, -1, 0 },{ 1, 1, 0 },{ 0, 2, -1 },{ -2, 0, -1 },{ 0, -2, -1 },{ 2, 0, -1 } };
	coordinate->point.setValues(0, pointSize, (float(*)[3])xyz1);
	SoPointSet* poinsSet = new SoPointSet();

	SoDrawStyle* drawStyle = new SoDrawStyle();
	drawStyle->pointSize = 1;

	root->addChild(pcBackGround);

	root->addChild(camera); // add camera node to the scene graph
	root->addChild(light); // add directional light to the scene
	root->addChild(new SoTrackballManip);
	root->addChild(material); // add material (with default settings)
	//root->addChild(sphere); // add sphere node to the scene graph

	root->addChild(drawStyle);
	root->addChild(coordinate);
	root->addChild(poinsSet);
							// Create a renderingarea which will be used to display the
							// scene graph in the window.
	SoWinExaminerViewer* renderarea = new SoWinExaminerViewer(appwin);
//	SoWinRenderArea * renderarea = new SoWinRenderArea(appwin);

	// Make the camera able to see the whole scene
	//camera->viewAll(root, renderarea->getViewportRegion());

	// Display the scene in our renderarea and change the title
	renderarea->setSceneGraph(root);
	renderarea->setTitle("SPH Demo");
	renderarea->show();

	SoWin::show(appwin);  //display the main window
	SoWin::mainLoop();  //main Coin event loop
	delete renderarea;  //free up the resources occupied by the renderarea

	root->unref();  //decrement the root's reference counter
	return 0;
}
