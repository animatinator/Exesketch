// Bezier.cpp


#include "bezier.h"


BezierPoint::BezierPoint(Object* theParent, int direction, const Vector2d& startPos):
	parent(theParent),
	pos(startPos),
	type(0),
	selected(false),
	dragging(false),
	dragIndex(1)
{
	// Position the control points in accordance with the direction the curve is currently going in
	Vector2d handleSize(10.0f, 0.0f);
	
	if (direction == HANDLEDIR_LEFT)  // Curve is going to the left
	{
		pointL = pos + handleSize;
		pointR = pos - handleSize;
	}
	else  // Curve is going to the right
	{
		pointL = pos - handleSize;
		pointR = pos + handleSize;
	}
}

BezierPoint::BezierPoint(const BezierPoint& other):
	parent(other.parent),
	pos(other.pos),
	pointL(other.pointL),
	pointR(other.pointR),
	type(other.type),
	selected(other.selected),
	dragging(other.dragging),
	dragIndex(other.dragIndex)
{}

BezierPoint::BezierPoint(Object* theParent, const BezierPoint& other):
	parent(theParent),
	pos(other.pos),
	pointL(other.pointL),
	pointR(other.pointR),
	type(other.type),
	selected(other.selected),
	dragging(other.dragging),
	dragIndex(other.dragIndex)
{}

void BezierPoint::ToggleType()
{
	type = (type + 1) % 2;  // Toggles the type between 0 (ALIGNED) and 1 (FREE)
}

bool BezierPoint::MouseHitPoint(const Vector2d& mousepos) const
{
	float pixelSize = parent->GetPixelSize();
	Vector2d rootpos = parent->GetPos();
	
	return ((Vector2d::Distance(mousepos, rootpos + pos) / pixelSize < HANDLE_SIZE)
		|| (parent->GetState() == EDITING
		&& ((Vector2d::Distance(mousepos, rootpos + pointL) / pixelSize < HANDLE_SIZE)
		|| (Vector2d::Distance(mousepos, rootpos + pointR) / pixelSize < HANDLE_SIZE))));
}

int BezierPoint::GetHitPoint(const Vector2d& mousepos) const
{
	Vector2d rootpos = parent->GetPos();
	float pixelSize = parent->GetPixelSize();
	
	if (Vector2d::Distance(mousepos, rootpos + pointL) / pixelSize < HANDLE_SIZE) return 0;
	else if (Vector2d::Distance(mousepos, rootpos + pos) / pixelSize < HANDLE_SIZE) return 1;
	else if (Vector2d::Distance(mousepos, rootpos + pointR) / pixelSize < HANDLE_SIZE) return 2;
}

void BezierPoint::StartDrag(int newDragIndex)
{
	dragIndex = newDragIndex;
	
	oldPos = pos;
	oldPointL = pointL;
	oldPointR = pointR;
}

void BezierPoint::Drag(const Vector2d& mouseShift)
{
	switch(dragIndex)
	{
		case LEFT:
			pointL = oldPointL + mouseShift;
			if (type == ALIGNED) UpdateDependentPoint(RIGHT);
				break;
		
		case CENTRE:
			pos = oldPos + mouseShift;
			pointL = oldPointL + mouseShift;
			pointR = oldPointR + mouseShift;
				break;
		
		case RIGHT:
			pointR = oldPointR + mouseShift;
			if (type == ALIGNED) UpdateDependentPoint(LEFT);
				break;
		
		default:
			break;
	}
}

void BezierPoint::UpdateDependentPoint(int depPointIndex)
{
	Vector2d* depPoint = NULL;
	Vector2d leadPoint;
	
	// Work out which point is the dependent point
	if (depPointIndex == 0)
	{
		depPoint = &pointL;
		leadPoint = pointR;
	}
	else
	{
		depPoint = &pointR;
		leadPoint = pointL;
	}
	
	// Get the vector from pos to the leading point, and subtract it from the pos to get the
	// dependent point's position
	(*depPoint) = pos - (leadPoint - pos);
}

void BezierPoint::Draw()
{
	Vector2d rootpos = parent->GetPos();
	
	if (parent->GetState() == EDITING)
	{
		// Draw lines connecting the points
		glColor3f(0.7f, 0.7f, 0.7f);	
		
		glBegin(GL_LINES);
			glVertex2f(rootpos.x + pointL.x, rootpos.y + pointL.y);
			glVertex2f(rootpos.x + pos.x, rootpos.y + pos.y);
			
			glVertex2f(rootpos.x + pos.x, rootpos.y + pos.y);
			glVertex2f(rootpos.x + pointR.x, rootpos.y + pointR.y);
		glEnd();
		
		// Draw the end points (darkened when this point is selected)
		if (selected) glColor3f(0.2f, 0.2f, 0.2f);
		else glColor3f(0.5f, 0.5f, 0.5f);
		Object::DrawHandle(rootpos + pointL);
		Object::DrawHandle(rootpos + pointR);
		
		// Draw the central point
		glColor3f(0.0f, 0.0f, 0.0f);
		if (type == ALIGNED) Object::DrawHandle(rootpos + pos, true);
		else if (type == FREE)
		{
			float size = Object::GetPixelSize() * HANDLE_SIZE;
			Vector2d handlePos = rootpos + pos;
			Vector2d curPoint;
			
			glBegin(GL_QUADS);
				curPoint = (handlePos + Vector2d(0.0f, -size));
				glVertex2f(curPoint.x, curPoint.y);
				curPoint = (handlePos + Vector2d(size, 0.0f));
				glVertex2f(curPoint.x, curPoint.y);
				curPoint = (handlePos + Vector2d(0.0f, size));
				glVertex2f(curPoint.x, curPoint.y);
				curPoint = (handlePos + Vector2d(-size, 0.0f));
				glVertex2f(curPoint.x, curPoint.y);
			glEnd();
		}
	}
}


BezierSpline::BezierSpline(const Vector2d& startPos):
	Object(startPos, OBJ_BEZIER),
	closed(false),
	fill(false),
	selectIndex(0),
	draggingPoint(false),
	resolution(100)
{
	points.push_back(BezierPoint(this, RIGHT, Vector2d(0.0f, 0.0f)));
	points.push_back(BezierPoint(this, RIGHT, Vector2d(50.0f, 20.0f)));
	
	points.at(selectIndex).Select();
}

BezierSpline::BezierSpline(const BezierSpline& other):
	Object(other.pos, OBJ_BEZIER),
	oldPoints(other.oldPoints),
	closed(other.closed),
	fill(other.fill),
	selectIndex(other.selectIndex),
	draggingPoint(other.draggingPoint),
	resolution(other.resolution)
{
	for (vector<BezierPoint>::const_iterator iter = other.points.begin(); iter != other.points.end(); ++iter)
	{
		points.push_back(BezierPoint(this, (*iter)));
	}
}

void BezierSpline::Draw()
{
	DrawBezierSpline();
	
	if (selected && state != EDITING)
	{
		Object::DrawBoundingBox();
	}
	
	for (vector<BezierPoint>::iterator iter = points.begin(); iter != points.end(); ++iter)
	{
		iter->Draw();
	}
}

void BezierSpline::HandleEvent(Event& event)
{
	if (state == EDITING)
	{
		if (event.type == EVENT_MOUSE_DOWN_LEFT)
		{
			DeselectAll();
			
			int hitPoint = GetHitPoint(Vector2d(event.realxpos, event.realypos));
			selectIndex = hitPoint;
			
			if (hitPoint != NA)
			{
				int hitSubPoint = points.at(hitPoint).GetHitPoint(Vector2d(event.realxpos, event.realypos));
				
				points.at(hitPoint).Select();
				points.at(hitPoint).StartDrag(hitSubPoint);
				
				storedMouse = Vector2d(event.realxpos, event.realypos);
				draggingPoint = true;
			}
		}
		
		else if (event.type == EVENT_MOUSE_UP_LEFT)
		{
			draggingPoint = false;
		}
		
		else if (event.type == EVENT_MOUSE_MOTION)
		{
			// Get the mouse position
			Vector2d mousePos(event.realxpos, event.realypos);
			
			// If the user is dragging a point
			if (draggingPoint)
			{
				// Get the point and the amount by which the mouse has moved since the beginning of the drag
				BezierPoint& curPoint = points.at(selectIndex);
				Vector2d mouseShift = mousePos - storedMouse;
				// Drag the point by that amount (the movement is added to its position before dragging)
				curPoint.Drag(mouseShift);
			}
		}

		else if (event.type == EVENT_MOUSE_DOWN_RIGHT)
		{
			// If a point is selected, add the new point after it
			if (selectIndex != NA)
			{
				InsertPoint(selectIndex, Vector2d(event.realxpos, event.realypos));
			}
			// Otherwise append the new point to the end of the spline
			else AddPoint(Vector2d(event.realxpos, event.realypos));
		}
		
		else if (event.type == EVENT_KEYBOARD_K_DEL)
		{
			// If a point is selected and there are at least two points
			if ((selectIndex != NA) && (points.size() >= 2))
			{
				// Delete the selected point and decrement the select index where appropriate
				points.erase(points.begin() + selectIndex);
				
				if (selectIndex > 0) --selectIndex;
			}
		}
		
		else if (event.type == EVENT_KEYBOARD_K_H)
		{
			points.at(selectIndex).ToggleType();
		}
		
		else if (event.type == EVENT_KEYBOARD_K_TAB)
		{
			state = OBJECT_VIEW;
		}
	}
	
	else Object::HandleEvent(event);
	
	if (event.type == EVENT_KEYBOARD_K_C && selected)
	{
		closed = !closed;
		if (!closed) fill = false;
	}
	
	else if (event.type == EVENT_KEYBOARD_K_F && selected)
	{
		if (!fill)
		{
			if (closed) fill = true;
		}
		else fill = false;
	}
}

bool BezierSpline::MouseHitObject(float x, float y) const
{
	Vector2d mouse(x, y);
	bool hit = false;
	
	for (vector<BezierPoint>::const_iterator iter = points.begin(); iter != points.end(); ++iter)
	{
		if (iter->MouseHitPoint(mouse)) hit = true;
	}
	
	if (state == OBJECT_VIEW)
	{
		float t = 0.0f;
		BezierPoint curPoint;
		BezierPoint nextPoint;
		Vector2d bezierPoint;
		
		for (vector<BezierPoint>::const_iterator iter = points.begin(); iter!= points.end(); ++iter)
		{
			curPoint = (*iter);
			
			if ((iter + 1) != points.end())
			{
				nextPoint = (*(iter + 1));
			}
			else if (closed)
			{
				nextPoint = points.at(0);
			}
			
			for (t = 0.0f; t <= 1.0f; t += 0.01)
			{
				bezierPoint = GetPoint(pos, curPoint, nextPoint, t);
				
				if ((Vector2d::Distance(mouse, bezierPoint) / pixelSize) < HANDLE_SIZE)
				{
					hit = true;
					break;
				}
			}
			
			if (hit) break;
		}
	}
	
		return hit;
}

void BezierSpline::AddPoint(Vector2d newPoint)
{
	// Get the position in object space
	newPoint -= pos;
	
	// Work out which direction the curve is going in
	int direction;
	
	if ((newPoint.x) > points.at(points.size() - 1).GetPos().x)
	{
		direction = HANDLEDIR_RIGHT;
	}
	else direction = HANDLEDIR_LEFT;
	
	points.push_back(BezierPoint(this, direction, newPoint));
	selectIndex = points.size() - 1;
}

void BezierSpline::InsertPoint(int index, Vector2d newPoint)
{
	// Get the position in object space
	newPoint -= pos;
	
	// Work out which direction the curve is going in
	int direction;
	
	if ((newPoint.x) > points.at(index).GetPos().x)
	{
		direction = HANDLEDIR_RIGHT;
	}
	else direction = HANDLEDIR_LEFT;
	
	vector<BezierPoint>::iterator insertPoint = (points.begin() + (++index));
	points.insert(insertPoint, BezierPoint(this, direction, newPoint));
	selectIndex += 1;
}

int BezierSpline::GetHitPoint(const Vector2d& mousepos) const
{
	int index = 0;
	bool hit = false;
	
	for (vector<BezierPoint>::const_iterator iter = points.begin(); iter != points.end(); ++iter, ++index)
	{
		if (iter->MouseHitPoint(mousepos))
		{
			hit = true;
				break;
		}
	}
	
	if (!hit) index = NA;
	
		return index;
}

void BezierSpline::SelectAll()
{
	for (vector<BezierPoint>::iterator iter = points.begin(); iter != points.end(); ++iter) iter->Select();
}

void BezierSpline::DeselectAll()
{
	for (vector<BezierPoint>::iterator iter = points.begin(); iter != points.end(); ++iter) iter->Deselect();
}

Vector2d BezierSpline::GetPoint(const Vector2d& pos, const BezierPoint& point1, const BezierPoint& point2, float t)
{
	// Return the point on the curve between point1 and point2 for a specific value of t
	// This uses the cubic Bézier formula:
	// B(t) = P0.(1 - t)^3 + 3.P1.(1-t)^2.t + 3.P2.(1-t).t^2 + P3.t^3
	Vector2d point;
	
	Vector2d p0 = point1.GetPos() + pos;
	Vector2d p1 = point1.GetR() + pos;
	Vector2d p2 = point2.GetL() + pos;
	Vector2d p3 = point2.GetPos() + pos;
	
	float onetaket = 1.0f-t;
	float onetaketsquared = onetaket * onetaket;
	float onetaketcubed = onetaketsquared * onetaket;
	float tsquared = t * t;
	float tcubed = tsquared * t;
	
	point = (p0 * onetaketcubed)  // P0.(1 - t)^3
			+ (p1 * 3.0f * onetaketsquared * t)  // 3.P1.(1-t)^2.t
			+ (p2 * 3.0f * onetaket * tsquared)  // 3.P2.(1-t).t^2
			+ (p3 * tcubed);  // P3.t^3
	
	return point;
}

void BezierSpline::DrawBezierCurve(const BezierPoint& point1, const BezierPoint& point2)
{
	// Draw a curve between two BezierPoint objects
	vector<Vector2d> interPoints;  // Intermediate points
	
	// t, and how much to increment it by for each intermediate point
	float t = 0.0f;
	float increment = 1.0f / (float)resolution;
	
	// Get the intermediate points
	for (; t <= 1.0f; t += increment)
	{
		interPoints.push_back(BezierSpline::GetPoint(pos, point1, point2, t));
	}
	
	glBegin(GL_LINE_STRIP);
		for (vector<Vector2d>::const_iterator iter = interPoints.begin(); iter != interPoints.end(); ++iter)
		{
			// Add this point to the bounding box
			if (iter->x < minx) minx = iter->x;
			if (iter->x > maxx) maxx = iter->x;
			if (iter->y < miny) miny = iter->y;
			if (iter->y > maxy) maxy = iter->y;
			
			// Draw it
			glVertex2f(iter->x, iter->y);
		}
	glEnd();
}

void BezierSpline::DrawBezierSpline()
{
	// Draw the full Bézier spline
	
	// Reset the bounding box values
	minx = miny = (float)NA;
	maxx = maxy = -(float)NA;
	
	// Set the colour
	if (state == EDITING) glColor3f(0.5f, 1.0f, 0.5f);
	else if (selected) glColor3f(0.0f, 0.5f, 1.0f);
	else glColor3f(0.0f, 0.0f, 0.0f);
	
	// Iterate through the points
	int curIndex = 0;
	for (vector<BezierPoint>::const_iterator iter = points.begin(); iter != points.end(); ++iter, ++curIndex)
	{
		BezierPoint curPoint, nextPoint;
		
		// If this is the last point, decide whether or not to draw a curve between this point and the first point
		if (curIndex == points.size() - 1)
		{
			if (closed)  // Only draw the next curve if the spline is closed
			{
				nextPoint = points.at(0);
			}
			else break;
		}
		// Otherwise, draw the curve between this point and the next point
		else
		{
			nextPoint = *(iter+1);
		}
		
		curPoint = (*iter);
		
		DrawBezierCurve(curPoint, nextPoint);
	}
}

Vector2d BezierSpline::GetMin() const
{
	return Vector2d(minx, miny);
}

Vector2d BezierSpline::GetMax() const
{
	return Vector2d(maxx, maxy);
}

Object* BezierSpline::CreateCopy()
{
	Object* newObject = new BezierSpline(*this);
	
		return newObject;
}
