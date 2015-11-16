//polygon.cpp


#include "polygon.h"


Poly::Poly():
	Object(OBJ_POLYGON),
	selectIndex(0),
	dragPoint(false),
	fill(true)
{
	InitShape();
}

Poly::Poly(const Vector2d& startPos):
	Object(startPos, OBJ_POLYGON),
	selectIndex(0),
	dragPoint(false),
	fill(true)
{
	InitShape();
}

Poly::Poly(const Poly& other):
	Object(other),
	points(other.points),
	selectIndex(other.selectIndex),
	dragPoint(other.dragPoint),
	fill(other.fill)
{}

void Poly::InitShape()
{
	points.push_back(Vector2d(0.0f, 0.0f));
	points.push_back(Vector2d(25.0f, 0.0f));
	points.push_back(Vector2d(0.0f, 25.0f));
}

void Poly::Draw()
{
	if (state == EDITING) glColor3f(0.5f, 1.0f, 0.5f);
	else if (selected) glColor3f(0.0f, 0.5f, 1.0f);
	else glColor3f(0.0f, 0.0f, 0.0f);
	
	if (fill) glBegin(GL_POLYGON);
	else glBegin(GL_LINE_LOOP);
		Vector2d curPoint;
		
		for (vector<Vector2d>::const_iterator iter = points.begin(); iter != points.end(); ++iter)
		{
			curPoint = (*iter);
			glVertex2f(pos.x + curPoint.x, pos.y + curPoint.y);
		}
	glEnd();
	
	if (selected && state != EDITING)
	{
		Object::DrawBoundingBox();
	}
	
	if (state == EDITING)
	{
		vector<Vector2d>::const_iterator iter; int curHandle;
		
		for (iter = points.begin(), curHandle = 0; iter != points.end(); ++iter, ++curHandle)
		{
			curPoint = (*iter);
			
			if (curHandle == selectIndex) glColor3f(0.2f, 0.2f, 0.2f);
			else glColor3f(0.5f, 0.5f, 0.5f);
			
			DrawHandle(pos + curPoint);
		}
	}
}

void Poly::HandleEvent(Event& event)
{
	if (event.type == EVENT_KEYBOARD_K_F && selected) fill = !fill;
	
	else if (state == EDITING)
	{
		if (event.type == EVENT_MOUSE_DOWN_LEFT)
		{
			selectIndex = MouseHitHandle(event.realxpos, event.realypos);
			
			if (selectIndex != NA)
			{
				dragPoint = true;
				storedMouse = Vector2d(event.realxpos, event.realypos);
			}
		}
		
		else if (event.type == EVENT_MOUSE_UP_LEFT)
		{
			dragPoint = false;
		}
		
		else if (event.type == EVENT_MOUSE_DOWN_RIGHT)
		{
			if (selectIndex != NA)
			{
				vector<Vector2d>::iterator insertPoint = points.begin() + (++selectIndex);
				points.insert(insertPoint, Vector2d(event.realxpos - pos.x, event.realypos - pos.y));
			}
			else points.push_back(Vector2d(event.realxpos - pos.x, event.realypos - pos.y));
		}
		
		else if (event.type == EVENT_MOUSE_MOTION)
		{
			if (dragPoint)
			{
				points.at(selectIndex) = Vector2d(event.realxpos - pos.x, event.realypos - pos.y);
			}
		}
		
		else if (event.type == EVENT_KEYBOARD_K_DEL)
		{
			if (points.size() > 3)
			{
				points.erase(points.begin() + selectIndex);
				
				if (selectIndex > 0) --selectIndex;
			}
		}
		
		else if (event.type == EVENT_KEYBOARD_K_TAB)
		{
			state = OBJECT_VIEW;
		}
	}
	
	else Object::HandleEvent(event);
}

bool Poly::MouseHitObject(float x, float y) const
{
	// If the mouse has hit a control point (in editing mode)
	if ((state == EDITING) && (MouseHitHandle(x, y) != NA)) return true;
	
	// Translate the mouse cursor into local space
	x -= pos.x; y -= pos.y;
	
	// Begin by defining a horizontal line stretching (almost) indefinitely left from the mouse cursor
	float minx = x - 99999.0f; float maxx = x;
	
	int crossCount = 0;
	Vector2d cur, next;
	
	Vector2d startPoint, endPoint;
	float lineFactor;
	float linex;
	
	for (vector<Vector2d>::const_iterator iter = points.begin(); iter != points.end(); ++iter)
	{
		// Get the current and next point
		cur = (*iter);
		if ((iter + 1) == points.end()) next = (*points.begin());
		else next = (*(iter + 1));
		
		// If the line is likely to actually cross the horizontal line
		if ((cur.y <= y && next.y >= y) || (next.y <= y && cur.y >= y))
		{
			// Take the point with the lowest y-value as the line start point			
			if (cur.y <= next.y)
			{
				startPoint = cur;
				endPoint = next;
			}
			else
			{
				startPoint = next;
				endPoint = cur;
			}
			
			// Calculate the factor along the line the mouse is based on its y-position
			// (e.g. 0.5 = halfway along)
			lineFactor = (y - startPoint.y) / (endPoint.y - startPoint.y);
			// Calculate the x-position the line will have at that y-value based on the lineFactor
			linex = startPoint.x + (lineFactor * (endPoint.x - startPoint.x));
			
			// If the x-value of the line is within the range, increment the number of line crossings
			if (linex >= minx && linex <= maxx)
			{
				++crossCount;
			}
		}
	}
	
	// If the line crosses the polygon an odd number of times
	if (crossCount % 2 == 1) return true;
	else return false;
}

int Poly::MouseHitHandle(float x, float y) const
{
	// Translate the point into local space
	x -= pos.x; y -= pos.y;
	
	int handle = NA;
	
	vector<Vector2d>::const_iterator iter; int curHandle;
	Vector2d curPoint;
	
	for (iter = points.begin(), curHandle = 0; iter != points.end(); ++iter, ++curHandle)
	{
		curPoint = (*iter);
		
		if ((Vector2d::Distance(Vector2d(x, y), curPoint) / pixelSize) < HANDLE_SIZE)
		{
			handle = curHandle;
		}
	}
	
		return handle;
}

Vector2d Poly::GetMin() const
{
	float minx = 99999; float miny = 99999;
	
	Vector2d curPoint;
	
	for (vector<Vector2d>::const_iterator iter = points.begin(); iter != points.end(); ++iter)
	{
		curPoint = (*iter);
		if (curPoint.x < minx) minx = curPoint.x;
		if (curPoint.y < miny) miny = curPoint.y;
	}
	
		return (pos + Vector2d(minx, miny));
}

Vector2d Poly::GetMax() const
{
	float maxx = -99999; float maxy = -99999;
	
	Vector2d curPoint;
	
	for (vector<Vector2d>::const_iterator iter = points.begin(); iter != points.end(); ++iter)
	{
		curPoint = (*iter);
		if (curPoint.x > maxx) maxx = curPoint.x;
		if (curPoint.y > maxy) maxy = curPoint.y;
	}
	
		return (pos + Vector2d(maxx, maxy));
}

Object* Poly::CreateCopy()
{
	Object* newObject = new Poly(*this);
	
		return newObject;
}
