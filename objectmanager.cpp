// objectmanager.cpp


#include "objectmanager.h"


ObjectManager::ObjectManager(const vector<Object*>& newObjects):
	state(OBJECT_VIEW)
{
	for (vector<Object*>::const_iterator iter = newObjects.begin(); iter != newObjects.end(); ++iter)
	{
		objects.push_back(*iter);
	}
}

ObjectManager::~ObjectManager()
{
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		delete(*iter);
	}
}

void ObjectManager::GoEdit()
{
	state = EDITING;
	
	for (vector<Object*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter)->IsSelected()) (*iter)->SetState(EDITING);
	}
}

void ObjectManager::GoObjectView()
{
	state = OBJECT_VIEW;
	
	for (vector<Object*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->SetState(OBJECT_VIEW);
	}
}

int ObjectManager::GetNumSelected() const
{
	int count = 0;
	
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter)->IsSelected()) ++count;
	}
	
		return count;
}

void ObjectManager::SetPixelSize(float newSize) const
{
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->SetPixelSize(newSize);
	}
}

void ObjectManager::Draw() const
{
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->Draw();
	}
}

void ObjectManager::HandleEvent(Event& event)
{
	int type = event.type;
	
	if (event.HasFoundTarget()) return;
	
	if (state == OBJECT_VIEW)
	{		
		// Tab - go into edit mode
		if (event.type == EVENT_KEYBOARD_K_TAB)
		{
			// Only go into edit mode if at least one object is selected
			if (GetNumSelected() > 0)
			{
				state = EDITING;
				
				for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->IsSelected())
					{
						(*iter)->HandleEvent(event);
					}
				}
			}
		}
		
		// Arrow-key events - used for re-ordering objects (shifting them around the vector alters their drawing order)
		else if (event.type == EVENT_KEYBOARD_K_UP || event.type == EVENT_KEYBOARD_K_DOWN)
		{
			for (vector<Object*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
			{
				if ((*iter)->IsSelected())
				{
					if (event.type == EVENT_KEYBOARD_K_DOWN)  // Move backwards
					{
						if (iter != objects.begin())
						{
							Object* temp = *iter;
							objects.erase(iter);
							objects.insert((iter - 1), temp);
						}
					}
					else if (event.type == EVENT_KEYBOARD_K_UP)  // Bring forwards
					{
						if ((iter + 1) != objects.end())
						{
							Object* temp = *iter;
							objects.erase(iter);
							objects.insert((iter + 1), temp);
							iter++;
						}
					}
				}
			}
		}
		
		// Left-click - used for absolutely bloody everything
		else if (event.type == EVENT_MOUSE_DOWN_LEFT)
		{
			for (vector<Object*>::const_iterator iter = (objects.end() - 1); iter >= objects.begin(); --iter)
			{
				if (objects.empty()) break;  // This loop is potentially dangerous, so this line protects the program against crashes
				
				(*iter)->HandleEvent(event);
			}
			
			// If the click hit a selected object, pass over the objects again to make sure all other selected objects begin dragging
			if (event.alwaysDragSelected)
			{
				event.ForgetTarget();  // Pretend the event hasn't hit anything yet
				
				// Revert all objects to as they were before handling the current event
				for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
				{
					(*iter)->RevertState();
				}
				
				// Handle the event, this time with event.alwaysDragSelected set to true (so all selected objects will be dragged)
				for (vector<Object*>::const_iterator iter = (objects.end() - 1); iter >= objects.begin(); --iter)
				{
					if (objects.empty()) break;  // This loop is potentially dangerous, so this line protects the program against crashes
					
					(*iter)->HandleEvent(event);
				}
			}
		}
		
		// Delete key - used for deleting (in a massive departure from tradition)
		else if (event.type == EVENT_KEYBOARD_K_DEL)
		{
			// Create a vector of iterators pointing to objects to be deleted
			vector<vector<Object*>::iterator> iters;
			
			// If an object is selected, append the vvalue of its iterator to the vector
			for (vector<Object*>::iterator iter = objects.begin(); iter != objects.end(); ++iter)
			{
				if ((*iter)->IsSelected())
				{
					iters.push_back(iter);
				}
			}
			
			// Delete objects pointed to by iterators and remove them from the objects vector
			for (vector<vector<Object*>::iterator>::const_iterator iterIter = iters.begin(); iterIter != iters.end(); ++iterIter)
			{
				// Get the object iterator at this position in the vector of iterators
				vector<Object*>::iterator objectIter = (*iterIter);
				
				// Delete the object (on the free store), then remove its pointer from the objects vector
				delete (*objectIter);
				objects.erase(objectIter);
				
				// Decrement all stored object iterators (as the object vector is now shorter)
				for (vector<vector<Object*>::iterator>::iterator iterIter = iters.begin(); iterIter != iters.end(); ++iterIter)
				{
					--(*iterIter);
				}
			}
		}
		
		// CTRL-A - select all
		else if (event.type == EVENT_KEYBOARD_K_A && event.modifiers == GLUT_ACTIVE_CTRL)
		{
			SelectAll();
		}
		
		else
		{
			for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
			{
				(*iter)->HandleEvent(event);
			}
		}
	}
	
	else if (state == EDITING)
	{
		// Tab - go back to object view
		if (event.type == EVENT_KEYBOARD_K_TAB)
		{
			state = OBJECT_VIEW;
			
			for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
			{
				if ((*iter)->GetState() == EDITING)
				{
					(*iter)->HandleEvent(event);
				}
			}
		}
		
		// All other events
		else
		{
			for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
			{
				if ((*iter)->GetState() == EDITING)
				{
					(*iter)->HandleEvent(event);
					
					// If it's a mousedown and has hit the object, tell the event it has found its target
					if (event.type == EVENT_MOUSE_DOWN_LEFT && (*iter)->MouseHitObject(event.realxpos, event.realypos))
					{
						event.TargetFound();
					}
				}
			}
		}
	}
}

bool ObjectManager::MouseHitObject(float x, float y) const
{
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		if ((*iter)->MouseHitObject(x, y))
		{
			return true;
		}
	}
	
	return false;
}

void ObjectManager::SelectAll()
{
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->Select();
	}
}

void ObjectManager::DeselectAll()
{
	for (vector<Object*>::const_iterator iter = objects.begin(); iter != objects.end(); ++iter)
	{
		(*iter)->Deselect();
	}
}
