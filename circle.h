// Circle.h


#ifndef CIRCLE_H
#define CIRCLE_H

#define PI 3.141592653
#define TWOPI 6.283185307

#include "math.h"

#include "object.h"
#include "vector2d.h"

using namespace std;

class Event;


class Circle : public Object
{
	public:
		Circle();
		Circle(const Vector2d& startPos);
		Circle(const Circle& other);
		
		virtual void Draw();
		virtual void HandleEvent(Event& event);
		virtual bool MouseHitObject(float x, float y) const;
		
		Vector2d CalculatePointPos() const;
		
		bool MouseHitHandle(float x, float y) const;
		float GetMouseAngle(float x, float y);
		
		virtual Vector2d GetMin() const;
		virtual Vector2d GetMax() const;
		
		virtual Object* CreateCopy();
	
	protected:
		float radius, oldRadius;
		float pointAngle;
		
		bool resize;
		bool fill;
};

#endif
