// Event.cpp


#include "event.h"


Event::Event(int eType, int mods, int x, int y):
	type(eType),
	modifiers(mods),
	xpos(x),
	ypos(y),
	foundTarget(false),
	alwaysDragSelected(false)
{}

Event::Event(const Event& other):
	type(other.type),
	xpos(other.xpos),
	ypos(other.ypos),
	realxpos(other.realxpos),
	realypos(other.realypos),
	modifiers(other.modifiers),
	foundTarget(other.foundTarget),
	alwaysDragSelected(other.alwaysDragSelected)
{}

ostream& operator<<(ostream& os, const Event& event)
{
	os << "Event of type " << event.type;
	
	if (event.xpos != NA && event.ypos != NA)
	{
		os << ", (" << event.xpos << ", " << event.ypos << ").";
		os << " Real co-ordinates: (" << event.realxpos << ", " << event.realypos << ").";
	}
	else
	{
		os << ".";
	}
	
		return os;
}


EventHandler::EventHandler(ObjectDisplay* screen, const vector<ObjectManager*>& objects):
	display(screen)
{
	clients.reserve(objects.size());
	
	for (vector<ObjectManager*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		clients.push_back(*iter);
	}
}

void EventHandler::RegisterClient(ObjectManager* client)
{
	clients.push_back(client);
}

void EventHandler::RegisterMenu(ButtonMenu* menu)
{
	menus.push_back(menu);
}

void EventHandler::ProcessQueue()
{
	while (!oldEventQueue.empty()) oldEventQueue.pop();  // Clear the old event queue
	
	Event currentEvent;
	
	while (!eventQueue.empty())
	{
		currentEvent = eventQueue.front();  // Get the current event
		
		// Pass the event to menus
		for (vector<ButtonMenu*>::const_iterator iter = menus.begin(); iter != menus.end(); ++iter)
		{
			(*iter)->HandleEvent(currentEvent);
		}
		
		// Pass the event to object managers
		for (vector<ObjectManager*>::const_iterator iter = clients.begin(); iter != clients.end(); ++iter)
		{
			(*iter)->HandleEvent(currentEvent);
		}
		
		// Pass the event to the object display
		display->HandleEvent(currentEvent);
		
		oldEventQueue.push(currentEvent);
		
		eventQueue.pop();
	}
}

void EventHandler::EmptyQueue()
{
	while (!eventQueue.empty())
	{
		eventQueue.pop();
	}
}

void EventHandler::AddEvent(Event& event)
{
	Vector2d offset = display->GetOffset();
	float zoomFactor = display->GetZoom();
	
	if (event.xpos != NA && event.ypos != NA)
	{
		Vector2d realpos = display->ScreenToReal(Vector2d((float)event.xpos, (float)event.ypos));
		event.realxpos = realpos.x; event.realypos = realpos.y;
	}
	
	eventQueue.push(event);
}
