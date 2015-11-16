// objectdisplay.cpp


#include "objectdisplay.h"


ObjectDisplay::ObjectDisplay():
	zoomFactor(1.0f),
	offset(0.0f, 0.0f),
	storedMouse(0.0f, 0.0f),
	dragging(false),
	zooming(false)
{}

ObjectDisplay::ObjectDisplay(const vector<ObjectManager*>& startClients):
	clients(startClients),
	zoomFactor(1.0f),
	offset(0.0f, 0.0f),
	storedMouse(0.0f, 0.0f),
	dragging(false),
	zooming(false)
{}

ObjectDisplay::ObjectDisplay(const ObjectDisplay& other):
	clients(other.clients),
	menus(other.menus),
	zoomFactor(other.zoomFactor),
	offset(other.offset),
	storedMouse(other.storedMouse),
	dragging(other.dragging),
	zooming(other.zooming)
{}

void ObjectDisplay::DrawObjects() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375f, 0.375f, 0.0f);
	
	glScalef(zoomFactor, zoomFactor, 0.0f);
	glTranslatef((-offset.x), (-offset.y), 0.0f);
	
	// Draw the axes
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-100.0f, 0.0f);
		glVertex2f(100.0f, 0.0f);
	glEnd();
	
	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 100.0f);
		glVertex2f(0.0f, -100.0f);
	glEnd();
	
	// Redraw objects
	for (vector<ObjectManager*>::const_iterator iter = clients.begin(); iter != clients.end(); ++iter)
	{
		(*iter)->Draw();
	}
	
	// Set the matrix to represent physical pixels
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.375f, 0.375f, 0.0f);
	
	// Redraw menus
	for (vector<ButtonMenu*>::const_iterator iter = menus.begin(); iter != menus.end(); ++iter)
	{
		(*iter)->Draw();
	}
	
	glPopMatrix();
}

Vector2d ObjectDisplay::ScreenToReal(Vector2d coords)
{
	float realxpos, realypos;
	realxpos = ((float)coords.x / zoomFactor); realypos = ((float)coords.y / zoomFactor);
	realxpos += offset.x; realypos += offset.y;
	
		return Vector2d(realxpos, realypos);
}

void ObjectDisplay::HandleEvent(const Event& event)
{	
	if (event.type == EVENT_MOUSE_DOWN_MIDDLE && !event.HasFoundTarget())
	{
		if (!dragging)
		{
			storedMouse = Vector2d(event.xpos, event.ypos);
			realStoredMouse = ScreenToReal(storedMouse);
			zooming = true;
			oldZoomFactor = zoomFactor;
			oldOffset = offset;
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		}
	}
	
	else if (event.type == EVENT_MOUSE_UP_MIDDLE)
	{
		zooming = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	
	else if (event.type == EVENT_MOUSE_DOWN_LEFT && !event.HasFoundTarget())
	{
		if (!zooming)
		{
			storedMouse = Vector2d(event.xpos, event.ypos);
			dragging = true;
			oldOffset = offset;
			glutSetCursor(GLUT_CURSOR_CYCLE);
		}
	}
	
	else if (event.type == EVENT_MOUSE_UP_LEFT)
	{
		dragging = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	
	else if (event.type == EVENT_MOUSE_MOTION)
	{
		currentMouse.x = event.xpos;
		currentMouse.y = event.ypos;
		
		if (dragging)
		{
			offset = oldOffset + ((storedMouse - currentMouse) / zoomFactor);  // Distance to move varies inversely with the square of the zoom factor
		}
	}
}

void ObjectDisplay::Update()
{
	if (zooming)
	{
		zoomFactor += ((storedMouse.y - currentMouse.y) / 1000.0f);
		
		if (zoomFactor < 0.1f)  // Don't let the user zoom out too far
		{
			zoomFactor = 0.1f;
		}
		
		// Alter the offset so that the scaling occurs about the original mouse position
		offset = realStoredMouse - (storedMouse / zoomFactor);
	}
	
	for (vector<ObjectManager*>::const_iterator iter = clients.begin(); iter != clients.end(); ++iter)
	{
		(*iter)->SetPixelSize(ScreenToReal(Vector2d(1.0f, 1.0f)).x - ScreenToReal(Vector2d(0.0f, 0.0f)).x);
	}
}
