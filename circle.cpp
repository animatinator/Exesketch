// Circle.cpp


#include "circle.h"

Circle::Circle():
	Object(OBJ_CIRCLE),
	radius(10.0f),
	pointAngle(0.0f),
	resize(false),
	fill(true)
{}

Circle::Circle(const Vector2d& startPos):
	Object(startPos, OBJ_CIRCLE),
	radius(10.0f),
	pointAngle(0.0f),
	resize(false),
	fill(true)
{}

Circle::Circle(const Circle& other):
	Object(other),
	radius(other.radius),
	pointAngle(other.pointAngle),
	resize(false),
	fill(other.fill)
{}


void Circle::Draw()
{
	if (state == EDITING) glColor3f(0.5f, 1.0f, 0.5f);
	else if (selected) glColor3f(0.0f, 0.5f, 1.0f);
	else glColor3f(0.0f, 0.0f, 0.0f);
	
	int numPoints = (int)((1.2f * radius) / pixelSize);
	if (numPoints < 6) numPoints = 6;
	float curAngle;
	
	float curX, curY;
	
	if (fill)
	{
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(pos.x, pos.y);
	}
	else glBegin(GL_LINE_LOOP);
	
	for (int curPoint = 0; curPoint <= numPoints; ++curPoint)
	{
		curAngle = TWOPI * ((float)curPoint / (float)numPoints);
		
		curX = radius * sin(curAngle);
		curY = radius * cos(curAngle);
		
		glVertex2f(pos.x + curX, pos.y + curY);
	}
	
	glEnd();
	
	if (resize)
	{
		glColor3f(0.5f, 0.5f, 1.0f);
		
		glBegin(GL_LINE_LOOP);
		
		for (int curPoint = 0; curPoint <= numPoints; ++curPoint)
		{
			curAngle = TWOPI * ((float)curPoint / (float)numPoints);
			
			curX = oldRadius * sin(curAngle);
			curY = oldRadius * cos(curAngle);
			
			glVertex2f(pos.x + curX, pos.y + curY);
		}
		
		glEnd();
	}
	
	if (selected && state != EDITING)
	{
		Object::DrawBoundingBox();
	}
	
	if (state == EDITING)
	{		
		glColor3f(0.2f, 0.2f, 0.2f);
		
		Vector2d pointPos = CalculatePointPos();
		Object::DrawHandle(Vector2d(pointPos.x, pointPos.y));
	}
}

void Circle::HandleEvent(Event& event)
{
	if (event.type == EVENT_KEYBOARD_K_F && selected) fill = !fill;
	
	else if (state == EDITING)
	{
		if (event.type == EVENT_MOUSE_DOWN_LEFT)
		{
			if (MouseHitHandle(event.realxpos, event.realypos))
			{
				resize = true;
				oldRadius = radius;
			}
		}
		
		else if (event.type == EVENT_MOUSE_UP_LEFT)
		{
			resize = false;
		}
		
		else if (event.type == EVENT_MOUSE_MOTION)
		{
			if (resize)
			{
				pointAngle = GetMouseAngle(event.realxpos, event.realypos);
				radius = Vector2d::Distance(pos, Vector2d(event.realxpos, event.realypos));
			}
		}
		
		else if (event.type == EVENT_KEYBOARD_K_TAB)
		{
			state = OBJECT_VIEW;
		}
	}
	
	else Object::HandleEvent(event);
}

bool Circle::MouseHitObject(float x, float y) const
{
	return ((Vector2d::Distance(Vector2d(x, y), pos) <= radius) || MouseHitHandle(x, y));
}

Vector2d Circle::CalculatePointPos() const
{
	float pointx, pointy;
	
	pointx = pos.x + (radius * sin(pointAngle));
	pointy = pos.y - (radius * cos(pointAngle));
	
		return Vector2d(pointx, pointy);
}

bool Circle::MouseHitHandle(float x, float y) const
{
	return ((Vector2d::Distance(CalculatePointPos(), Vector2d(x, y)) / pixelSize) <= HANDLE_SIZE);
}

float Circle::GetMouseAngle(float x, float y)
{
	// Get the distance to the mouse
	float dist = Vector2d::Magnitude(Vector2d(x - pos.x, y - pos.y));
	float angle;
	
	if (asin((x - pos.x) / dist) < 0)  // If the inverse sin is negative (angle > 180)
	{
		angle = PI + acos((y - pos.y) / dist);
	}
	else  // If the inverse sin is positive (angle <= 180; 180 is included in this for no particular reason)
	{
		angle = PI - acos((y - pos.y) / dist);
	}
	
		return angle;
}

Vector2d Circle::GetMin() const
{
	return (pos - Vector2d(radius, radius));
}

Vector2d Circle::GetMax() const
{
	return (pos + Vector2d(radius, radius));
}

Object* Circle::CreateCopy()
{
	Object* newObject = new Circle(*this);
	
		return newObject;
}
