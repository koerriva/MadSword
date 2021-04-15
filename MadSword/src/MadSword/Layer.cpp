#include "mspch.h"
#include "Layer.h"

namespace MadSword {
	Layer::~Layer()
	{
	}
	void Layer::OnEvent(Event& e)
	{
		MS_CORE_TRACE("Layer:{0}",e);
	}
}