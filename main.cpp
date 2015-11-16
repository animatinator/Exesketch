// main.cpp


#include <iostream>
#include <cstdlib>

#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#include <GL/freeglut.h>

#include "main.h"

using namespace std;


int width = 800;
int height = 600;

ObjectDisplay* objectDisplay;
EventHandler* events;
ObjectManager* objects;
ButtonMenu* objectMenu;
ButtonMenu* modeMenu;


void InitRendering()
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.93333333333333335, 0.93333333333333335, 0.92549019607843142, 1.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	
	// Set up antialiasing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glHint(GL_POLYGON_SMOOTH, GL_DONT_CARE);
	
	// Enable antialiasing for lines and polygons
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	
	// Enable texturing
	glEnable(GL_TEXTURE_2D);
}

void WindowResize(int w, int h)
{
	if (w == 0 || h == 0) return;  // Avoids strange problems caused by minimising the window
	
	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	
	width = w;
	height = h;
	
	// Reposition the mode menu
	modeMenu->SetPos(Vector2d(0.0f, height - modeMenu->GetDimensions().y));
}

void HandleMouseButton(int button, int state, int x, int y)
{
	int modifiers = glutGetModifiers();
	
	int eventType = NA;
	
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON) eventType = EVENT_MOUSE_DOWN_LEFT;
		else if (button == GLUT_MIDDLE_BUTTON) eventType = EVENT_MOUSE_DOWN_MIDDLE;
		else if (button == GLUT_RIGHT_BUTTON) eventType = EVENT_MOUSE_DOWN_RIGHT;
	}
	else if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON) eventType = EVENT_MOUSE_UP_LEFT;
		else if (button == GLUT_MIDDLE_BUTTON) eventType = EVENT_MOUSE_UP_MIDDLE;
		else if (button == GLUT_RIGHT_BUTTON) eventType = EVENT_MOUSE_UP_RIGHT;
	}
	
	Event newEvent(eventType, modifiers, x, y);
	events->AddEvent(newEvent);
}

void HandleMouseMotion(int x, int y)
{
	Event newEvent(EVENT_MOUSE_MOTION, glutGetModifiers(), x, y);
	events->AddEvent(newEvent);
}

void HandlePassiveMouseMotion(int x, int y)
{
	Event newEvent(EVENT_MOUSE_MOTION, glutGetModifiers(), x, y);
	events->AddEvent(newEvent);
}

void HandleKeyPress(unsigned char key, int x, int y)
{
	int modifiers = glutGetModifiers();
	
	Event newEvent;
	
	switch(key)
	{
		// Escape key
		case 27:
			exit(0);
				break;
		
		// CTRL-A (for some reason, this event is registered as a smiley-face symbol - ASCII 1)
		case 1:
			newEvent = Event(EVENT_KEYBOARD_K_A, GLUT_ACTIVE_CTRL);
			events->AddEvent(newEvent);
				break;
		
		// Delete
		case 127:
		{
			newEvent = Event(EVENT_KEYBOARD_K_DEL, modifiers);
			events->AddEvent(newEvent);
				break;
		}
		
		// Tab
		case '\t':
		{
			newEvent = Event(EVENT_KEYBOARD_K_TAB, modifiers);
			events->AddEvent(newEvent);
				break;
		}
		
		// All other events
		default:
		{
			int eventType = NO_EVENT;
			
			// 7 onwards = key events
			if (key > 64 && key < 91)  // Uppercase
			{
				eventType = (key - 65) + EVENT_KEYBOARD_K_A;
			}
			else if (key > 96 && key < 123)  // Lowercase
			{
				eventType = (key - 97) + EVENT_KEYBOARD_K_A;
			}
			
			if (eventType != NO_EVENT)
			{
				newEvent = Event(eventType, modifiers);
				events->AddEvent(newEvent);
			}
			
				break;
		}
	}
}

void HandleSpecialKeyPress(int key, int x, int y)
{
	int modifiers = glutGetModifiers();
	int eventType = NO_EVENT;
	
	switch(key)
	{
		// Arrow keys
		case GLUT_KEY_UP:
			eventType = EVENT_KEYBOARD_K_UP;
			break;
		
		case GLUT_KEY_DOWN:
			eventType = EVENT_KEYBOARD_K_DOWN;
			break;
		
		case GLUT_KEY_LEFT:
			eventType = EVENT_KEYBOARD_K_LEFT;
			break;
		
		case GLUT_KEY_RIGHT:
			eventType = EVENT_KEYBOARD_K_RIGHT;
			break;
		
		// Miscellaneous keys
		case GLUT_KEY_PAGE_UP:
			eventType = EVENT_KEYBOARD_K_PGUP;
			break;
		
		case GLUT_KEY_PAGE_DOWN:
			eventType = EVENT_KEYBOARD_K_PGDN;
			break;
		
		case GLUT_KEY_HOME:
			eventType = EVENT_KEYBOARD_K_HOME;
			break;
		
		case GLUT_KEY_END:
			eventType = EVENT_KEYBOARD_K_END;
			break;
		
		case GLUT_KEY_INSERT:
			eventType = EVENT_KEYBOARD_K_INSERT;
			break;
		
		// F-keys
		default:
			if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12)
			{
				eventType = (key - GLUT_KEY_F1) + EVENT_KEYBOARD_K_F1;
			}
			
				break;
	}
	
	if (eventType != NO_EVENT)
	{
		Event newEvent(eventType, modifiers);
		events->AddEvent(newEvent);
	}
}

GLuint LoadTexture(Image *image)
{
	GLuint id;
	glGenTextures(1, &id);
	
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
		return id;
}

vector<GLuint> LoadObjectImages()
{
	GLuint id;
	vector<GLuint> images;
	
	Image* curImage;
	curImage = loadBMP("Rect.bmp");
	images.push_back(LoadTexture(curImage));
	curImage = loadBMP("Polygon.bmp");
	images.push_back(LoadTexture(curImage));
	curImage = loadBMP("Circle.bmp");
	images.push_back(LoadTexture(curImage));
	curImage = loadBMP("Bezier.bmp");
	images.push_back(LoadTexture(curImage));
	
	delete curImage;
	
		return images;
}

vector<GLuint> LoadModeImages()
{
	GLuint id;
	vector<GLuint> images;
	
	Image* curImage;
	curImage = loadBMP("Object_view.bmp");
	images.push_back(LoadTexture(curImage));
	curImage = loadBMP("Editing.bmp");
	images.push_back(LoadTexture(curImage));
	
	delete curImage;
	
		return images;
}

void CreateObject(float x, float y)
{
	objects->DeselectAll();
	Object* newObject = NULL;
	
	switch(objects->GetAddObject())
	{
		case OBJ_RECT:
			newObject = new Rect(Vector2d(x, y));
				break;
		
		case OBJ_POLYGON:
			newObject = new Poly(Vector2d(x, y));
				break;
		
		case OBJ_CIRCLE:
			newObject = new Circle(Vector2d(x, y));
				break;
		
		case OBJ_BEZIER:
			newObject = new BezierSpline(Vector2d(x, y));
				break;
		
		default:
			break;
	}
	
	if (newObject)
	{
		newObject->Select();
		objects->AddObject(newObject);
	}
}

void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	objectDisplay->DrawObjects();
	
	glutSwapBuffers();
}

void Update(int value)
{
	events->ProcessQueue();
	objectDisplay->Update();
	
	objects->SetAddObject(objectMenu->GetSelection());
	
	switch(modeMenu->GetSelection())
	{
		case OBJECT_VIEW:
			objects->GoObjectView();
			break;
		
		case EDITING:
			objects->GoEdit();
			break;
		
		default:
			break;
	}
	
	switch(objects->GetState())
	{
		case OBJECT_VIEW:
			glutSetWindowTitle("ExeSketch - Object View mode");
				break;
		
		case EDITING:
			glutSetWindowTitle("ExeSketch - Editing mode");
				break;
	}
	
	queue<Event> currentEvents = events->GetEvents();
	
	while (!currentEvents.empty())
	{
		Event current = currentEvents.front();
		
		// Right-click (add object)
		if (current.type == EVENT_MOUSE_DOWN_RIGHT && objects->GetState() == OBJECT_VIEW && !current.HasFoundTarget())
		{
			CreateObject(current.realxpos, current.realypos);
		}
		
		// Shift-D (duplicate)
		else if (current.type == EVENT_KEYBOARD_K_D && current.modifiers == GLUT_ACTIVE_SHIFT)
		{
			vector<Object*> curObjects = objects->GetObjects();  // Currently existing objects
			vector<Object*> newObjects;  // Objects to add
			Object* newObject = NULL;  // The current object
			
			for (vector<Object*>::const_iterator iter = curObjects.begin(); iter != curObjects.end(); ++iter)
			{
				if ((*iter)->IsSelected())
				{
					// Create a copy, offset it, select it and add it to the list of new objects
					newObject = (*iter)->CreateCopy();
					newObject->SetPos(newObject->GetPos() + Vector2d(10.0f, 10.0f));
					newObject->Select();
					newObjects.push_back(newObject);
					
					(*iter)->Deselect();  // Deselect the duplicated object
				}
			}
			
			// For each object in the vector of new objects, add it to the ObjectManager's vector of objects
			for (vector<Object*>::const_iterator iter = newObjects.begin(); iter != newObjects.end(); ++iter)
			{
				objects->AddObject((*iter));
			}
		}
		
		// Tab (mode switch)
		else if (current.type == EVENT_KEYBOARD_K_TAB)
		{
			modeMenu->SetSelection((modeMenu->GetSelection() + 1) % 2);
		}
		
		currentEvents.pop();
	}
	
	glutPostRedisplay();
	glutTimerFunc((int)(1000.0 / 30.0), Update, 0);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ExeSketch");
	
	InitRendering();
	
	vector<GLuint> objectImages = LoadObjectImages();
	vector<GLuint> modeImages = LoadModeImages();
	objectMenu = new ButtonMenu(objectImages, Vector2d(0.0f, 0.0f), HORIZONTAL, Vector2d(64.0f, 64.0f), Vector2d(5.0f, 5.0f), 5.0f);
	modeMenu = new ButtonMenu(modeImages, Vector2d(0.0f, 0.0f), VERTICAL, Vector2d(32.0f, 32.0f), Vector2d(1.0f, 1.0f), 1.0f);
	modeMenu->SetPos(Vector2d(0.0f, height - modeMenu->GetDimensions().y));
	
	objectDisplay = new ObjectDisplay();
	objectDisplay->AddMenu(objectMenu);
	objectDisplay->AddMenu(modeMenu);
	
	events = new EventHandler(objectDisplay);
	events->RegisterMenu(objectMenu);
	events->RegisterMenu(modeMenu);
	
	objects = new ObjectManager();
	objects->SetAddObject(OBJ_RECT);
	objectDisplay->AddClient(objects);
	events->RegisterClient(objects);
	
	objectDisplay->SetOffset(Vector2d(-400.0f, -300.0f));
	
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(WindowResize);
	glutMouseFunc(HandleMouseButton);
	glutMotionFunc(HandleMouseMotion);
	glutPassiveMotionFunc(HandlePassiveMouseMotion);
	glutKeyboardFunc(HandleKeyPress);
	glutSpecialFunc(HandleSpecialKeyPress);
	
	glutTimerFunc(25, Update, 0);
	
	glutMainLoop();
	
		return 0;
}
