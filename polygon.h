// polygon.h


#ifndef POLYGON_H
#define POLYGON_H

#include <vector>

#include "object.h"
#include "vector2d.h"

class Event;


class Poly : public Object
{
	public:
		Poly();
		Poly(const Vector2d& startPos);
		Poly(const Poly& other);
		
		void InitShape();
		
		virtual void Draw();
		virtual void HandleEvent(Event& event);
		virtual bool MouseHitObject(float x, float y) const;
		
		int MouseHitHandle(float x, float y) const;
		
		virtual Vector2d GetMin() const;
		virtual Vector2d GetMax() const;
		
		virtual Object* CreateCopy();
	
	protected:
		vector<Vector2d> points;
		int selectIndex;
		bool dragPoint;
		
		bool fill;
};

#endif
