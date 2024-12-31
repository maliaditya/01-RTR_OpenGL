#pragma once
#include <OGL/precomp.h>

// Forward declaration of Mesh
class Mesh;


class ImguiManager {
public:
	ImguiManager(HWND hwnd);
	~ImguiManager();

	void beginFrame();
	//void renderUI(Mesh& mesh); // pass mesh object to this function
	void endFrame();
	//void renderGUI(Mesh& mesh);
	void destroy();
	void imguiCustomColorTheme();
};