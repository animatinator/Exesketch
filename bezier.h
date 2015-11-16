// Bezier.h


#ifndef BEZIER_H
#define BEZIER_H

#include <vector>

#include "object.h"
#include "vector2d.h"

class Event;


enum POINT_TYPE {ALIGNED = 0, FREE};
enum SUBPOINTS {LEFT = 0, CENTRE, RIGHT};
enum HANDLEDIRECTION {HANDLEDIR_LEFT = 0, HANDLEDIR_RIGHT};


class BezierPoint
{
	public:
		BezierPoint(Object* theParent = NULL, int direction = RIGHT, const Vector2d& startPos = Vector2d(0.0f, 0.0f));
		BezierPoint(const BezierPoint& other);
		BezierPoint(Object* theParent, const BezierPoint& other);  // Copy constructor for different parent
		
		Vector2d GetPos() const {return pos;}
		Vector2d GetOldPos() const {return oldPos;}
		void SetPos(const Vector2d& newPos) {pos = newPos;}
		
		Vector2d GetL() const {return pointL;}
		Vector2d GetOldL() const {return oldPointL;}
		Vector2d GetR() const {return pointR;}
		Vector2d GetOldR() const {return oldPointR;}
		void SetL(const Vector2d& newPos) {pointL = (newPos - pos);}
		void SetR(const Vector2d& newPos) {pointR = (newPos - pos);}
		
		void ToggleType();
		void SetType(int newType) {type = newType;}
		int GetType() const {return type;}
		
		bool IsSelected() const {return selected;}
		void Select() {selected = true;}
		void Deselect() {selected = false;}
		
		bool MouseHitPoint(const Vector2d& mousePos) const;
		int GetHitPoint(const Vector2d& mousePos) const;
		
		void SetDragIndex(int newIndex) {dragIndex = newIndex;}
		int GetDragIndex() {return dragIndex;}
		void StartDrag(int newDragIndex);
		
		void Drag(const Vector2d& mouseShift);
		void UpdateDependentPoint(int depPointIndex);
		
		void Draw();
	
	private:
		Vector2d pos, oldPos;
		Vector2d pointL, oldPointL;
		Vector2d pointR, oldPointR;
		
		int type;
		
		bool selected;
		bool dragging;
		int dragIndex;
		
		Object* parent;
};


class BezierSpline : public Object
{
	public:
		BezierSpline(const Vector2d& startPos = Vector2d(0.0f, 0.0f));
		BezierSpline(const BezierSpline& other);
		
		virtual void Draw();
		virtual void HandleEvent(Event& event);
		virtual bool MouseHitObject(float x, float y) const;
		
		void AddPoint(Vector2d newPoint);
		void InsertPoint(int index, Vector2d newPoint);
		void DeletePoint(int index) {points.erase(vector<BezierPoint>::iterator(points.begin() + index));}
		
		int GetHitPoint(const Vector2d& mousepos) const;
		void SetDragPoint(int dragPoint) {draggingPoint = dragPoint;}
		
		void SelectAll();
		void DeselectAll();
		
		static Vector2d GetPoint(const Vector2d& pos, const BezierPoint& point1, const BezierPoint& point2, float t = 0.0f);
		void DrawBezierCurve(const BezierPoint& point1, const BezierPoint& point2);
		void DrawBezierSpline();
		
		virtual Vector2d GetMin() const;
		virtual Vector2d GetMax() const;
		
		virtual Object* CreateCopy();
	
	private:
		vector<BezierPoint> points, oldPoints;
		bool closed, fill;
		int selectIndex;
		bool draggingPoint;
		int resolution;
		float minx, maxx, miny, maxy;
};


#endif
