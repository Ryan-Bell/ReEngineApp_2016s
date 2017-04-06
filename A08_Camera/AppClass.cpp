#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Camera"); // Window Name
	//Setting the color to black
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AppClass::InitVariables(void)
{
	camera = (Camera*)malloc(sizeof(Camera));
	camera->SetPosition(vector3(10.f, 0.f, 0.f));
	camera->SetUp(vector3(0, 1, 0));
	camera->SetTarget(vector3(0.f, 0.f, 0.f));

	//Generate the Cone
	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 1.0f, 10, RERED);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 1.0f, 10, REGREEN);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the cone
	m_pCone->Render(camera->GetProjection(false), camera->GetView(), IDENTITY_M4);

	//Render the cylinder
	m_pCylinder->Render(camera->GetProjection(false), camera->GetView(), glm::translate(IDENTITY_M4, REAXISZ * -3.0f));

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}