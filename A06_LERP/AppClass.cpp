#include "AppClass.h"
#include <algorithm>

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	vector3 vecs[11] = {
		vector3(-4.0f, -2.0f, 5.0f),
		vector3(1.0f, -2.0f, 5.0f),
		vector3(-3.0f, -1.0f, 3.0f),
		vector3(2.0f, -1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f, -3.0f),
		vector3(4.0f, 1.0f, -3.0f),
		vector3(0.0f, 2.0f, -5.0f),
		vector3(5.0f, 2.0f, -5.0f),
		vector3(1.0f, 3.0f, -5.0f)
	};
	
	for (int i = 0; i < 11; i++) {
		m_pMeshMngr->AddSphereToRenderList(glm::scale(glm::translate(vecs[i]), vector3(0.3f,0.3f,0.3f)));
	}
	static DWORD timerSinceBoot = GetTickCount();
	float fTimer = (GetTickCount() - timerSinceBoot) / 1000.0f;
	int currentPoint = (int)(fTimer / fDuration) % 11;
	
	float percent = std::fmod((fTimer / fDuration), 1.0f);
	int nextPoint = currentPoint == 10 ? 0 : currentPoint + 1;
	m_pMeshMngr->SetModelMatrix(glm::translate((1-percent) * vecs[currentPoint] + percent * vecs[nextPoint]), "WallEye");
	
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
	m_pMeshMngr->Print("\nTime:");
	m_pMeshMngr->Print(std::to_string(fTimer), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}