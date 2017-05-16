#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName) {
	super::InitWindow("AStar");
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void) {
	sideLength = 15;
	data = (Node*)malloc(sizeof(Node) * sideLength * sideLength * sideLength);
	Generate();
	
}
void AppClass::Generate() {
	open = PriorityQueue();
	current = nullptr;

	for (int x = 0; x < sideLength; x++) {
		for (int y = 0; y < sideLength; y++) {
			for (int z = 0; z < sideLength; z++) {
				data[at(x, y, z)] = { nullptr, 0.f, 0.f,{ x, y, z }, REWHITE, true, false };
			}
		}
	}

	int NUM_OBSTACLES = 1000;
	for (int i = 0; i < NUM_OBSTACLES; i++) {
		int rand = randIndex();
		if (!data[rand].walkable) i--;
		data[rand].color = REGRAY;
		data[rand].walkable = false;
	}

	Point rand = randPoint();
	start = &(data[at(rand.x, rand.y, rand.z)] = { nullptr, 0.f, 0.f,rand, REGREEN, true, false });
	rand = randPoint();
	goal = &(data[at(rand.x, rand.y, rand.z)] = { nullptr, 0.f, 0.f,rand, RERED, true, false });

	open.Enqueue(start);
	start->FScore = start->GScore + Distance(start->loc, goal->loc);
}

void AppClass::Step()
{
	if (current == goal)
	{
		complete = true;
		//TracePath();                                                    //call TracePath() to highlight the calculated path
		return;
	}
	current = open.Dequeque();
	current->closed = true;
	
	if(current != start && current != goal)
		current->color = REYELLOW;
	Node** nes = GetNeighbors(current);
	Node* negh;
	for (int i = 0; i < 26; i++){
		negh = nes[i];
		if (!negh) continue;
		if (negh->closed)
			continue;
		if (!negh->walkable)
			continue;
		int tenativeGscore = current->GScore + Distance(current->loc, nes[i]->loc);
		
		if (std::find(open.queue.begin(), open.queue.end(), nes[i]) == open.queue.end() || tenativeGscore < nes[i]->GScore) {
			negh->Parent = current;
			negh->GScore = tenativeGscore;
			negh->FScore = nes[i]->GScore + Distance(nes[i]->loc, goal->loc);
			if (std::find(open.queue.begin(), open.queue.end(), nes[i]) == open.queue.end()) {
				nes[i]->color = REBLUE;
				open.Enqueue(nes[i]);
			}
		}

	}
	free(nes);
}

Node** AppClass::GetNeighbors(Node* subject)
{
	Node** neighbors = (Node**)malloc(sizeof(Node*) * 26);
	Node** start = neighbors;
	int i = subject->loc.x;                                                     //the given coords of the node
	int j = subject->loc.y;                                                     //...
	int k = subject->loc.z;
	
	for (int x = std::max(0, i - 1); x <= std::min(i + 1, sideLength); x++)          //looping in the horizontal direction for one node on either side
		for (int y = std::max(0, j - 1); y <= std::min(j + 1, sideLength); y++)     //looping in the vert direction for one node above and below
			for (int z = std::max(0, k - 1); z <= std::min(k + 1, sideLength); z++)
				if (x < sideLength && y < sideLength && z < sideLength)                                    //checking the larger bounds
					if (x != i || y != j || z != k){                                               //ommitting the subject node itself
							*neighbors = &data[at(x, y, z)];                              //adding the remaining to the list
							neighbors++;
						}
	return start;                                                               //returning the list of neighbors
}

void AppClass::Update(void) {
	m_pSystem->UpdateTime();
	m_pMeshMngr->Update();
	CameraRotation();
	
	static float fTimer = 0.0f;
	static int nClock = m_pSystem->GenClock();
	static float lastStep = fTimer;
	float fDeltaTime = static_cast<float>(m_pSystem->LapClock(nClock));
	fTimer += fDeltaTime;

	if (!play) {
		lastStep = fTimer;
		return;
	}
	float steptime = 0.2f;
	if (fTimer - lastStep > steptime) {
		Step();
		lastStep = fTimer;
	}
	
}

void AppClass::Display(void) {
	ClearScreen();
	
	for (int x = 0; x < sideLength; x++) {
		for (int y = 0; y < sideLength; y++) {
			for (int z = 0; z < sideLength; z++) {
				Node* n = &data[at(x,y,z)];
				m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(x, y, z)) * glm::scale(vector3(.2,.2,.2)), n->color);
				if (start == n || goal == n) {
					m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(x, y, z)) * glm::scale(vector3(.4, .4, .4)), n->color);
					m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(x, y, z)), n->color, WIRE);
				}
					
			}
		}
	}
	
	m_pMeshMngr->Render();
	m_pMeshMngr->ClearRenderList();
	m_pGLSystem->GLSwapBuffers();
}

void AppClass::Release(void) {
	free(data);
	super::Release();
}
        
void PriorityQueue::Enqueue(Node* data) {
    queue.push_back(data);                                
    index = queue.size() - 1;                             
    parent = (index - 1) / 2;                             
    while (true)                                          
    {
        if (queue[parent]->FScore > queue[index]->FScore) 
        {
            Node* temp = queue[parent];                   
            queue[parent] = queue[index];                 
            queue[index] = temp;
            index = (index - 1) / 2;                      
            parent = (index - 1) / 2;                     
            if (index <= 0)                               				   
                break;
        }
        else
            break;
    }
}

Node* PriorityQueue::Dequeque()
{
    index = 0;
    Node* root = queue[0];
    Node* returned = root;
    Node* temp;
    queue[0] = queue[ (int)queue.size() - 1];
    queue.erase(queue.end() - 1);
    bool noChild = false;
    while (true)
    {
        noChild = false;
        childLeft = 2 * index + 1;
        childRight = 2 * index + 2;
		
        if (childLeft > (int)queue.size() - 1)
            return returned;
        if (childRight > (int)queue.size() - 1)
            noChild = true;
        if (queue[childLeft]->FScore < queue[index]->FScore)
        {
            if (!noChild && queue[childRight]->FScore < queue[index]->FScore)
            {
                if (!noChild && queue[childLeft]->FScore < queue[childRight]->FScore)
                {
                    temp = queue[index];
                    queue[index] = queue[childLeft];
                    queue[childLeft] = temp;
                    index = childLeft;
                }
                else
                {
                    temp = queue[index];
                    queue[index] = queue[childRight];
                    queue[childRight] = temp;
                    index = childRight;
                }
            }
            else
            {
                temp = queue[index];
                queue[index] = queue[childLeft];
                queue[childLeft] = temp;
                index = childLeft;
            }
        }
        else if (!noChild && queue[childRight]->FScore < queue[index]->FScore)
        {
            temp = queue[index];
            queue[index] = queue[childRight];
            queue[childRight] = temp;
            index = childRight;
        }
        else
            return returned;

    }
    return returned;

}

Node* PriorityQueue::Peek() {
    return queue[0];
}

bool PriorityQueue::IsEmpty() {
    if (queue.size() == 0)
        return true;
    return false;
}
