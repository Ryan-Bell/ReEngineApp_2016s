#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - rjb1928"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AppClass::InitVariables(void)
{
	m_m4Sun = IDENTITY_M4;
	m_m4Earth = IDENTITY_M4;
	m_m4Moon = IDENTITY_M4;

	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
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

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	static float nEarthOrbits = 0;
	static float nEarthRevolutions = 0;
	static float nMoonOrbits = 0;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	nEarthRevolutions = fRunTime / m_fDay;
	nEarthOrbits = nEarthRevolutions / 365.0f;
	nMoonOrbits = nEarthRevolutions / 28.0f;

	//This matrices will just orient the objects to the camera
	//matrix4 rotateX = glm::rotate(IDENTITY_M4, 90.0f, vector3(1.0f, 0.0f, 0.0f));
	//matrix4 rotateY = glm::rotate(IDENTITY_M4, 90.0f, vector3(0.0f, 1.0f, 0.0f));

	//This matrices will hold the relative transformation of the Moon and the Earth
	matrix4 distanceEarth = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);

	//Earth's orbit around the Sun
	matrix4 orbitEarth = glm::rotate(IDENTITY_M4, nEarthOrbits * 360, vector3(0.0f, 1.0f, 0.0f));
	//Earth's rotation on its own axis
	matrix4 rotationEarth = glm::rotate(IDENTITY_M4, nEarthRevolutions * 360, vector3(0.0f, 1.0f, 0.0f));

	//Moon's orbit around the Earth
	matrix4 orbitMoon = glm::rotate(IDENTITY_M4, nMoonOrbits * 360, vector3(0.0f, -1.0f, 0.0f));
	//Moon's rotation around its own axis
	matrix4 rotationMoon = glm::rotate(IDENTITY_M4, nMoonOrbits, vector3(0.0f, 1.0f, 0.0f));

	//I will calculate the Earth position in space relative to the Sun (which is in global space)
	matrix4 earthsSpace = orbitEarth * distanceEarth;//the translation depends on the orientation of the space (Earths orbit)
	m_m4Earth = earthsSpace * rotationEarth;//the rotation of the Earth will depend on the new space


	//I will calculate the moon's position in space relative to the Earth (earthsSpace)
	matrix4 moonsSpace = orbitMoon * distanceMoon;//the moon's translation depends on its orientation (Moons orbit)
	m_m4Moon = earthsSpace * moonsSpace;//Then we place the moons space in therms of the earth space




	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "Sun");
	m_pMeshMngr->SetModelMatrix(m_m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m_m4Moon, "Moon");


	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");


	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);


}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}