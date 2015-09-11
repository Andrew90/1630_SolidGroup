#include "stdafx.h"
#include "GlobalHandlers.h"
//#include "TresholdPanelViewer.h"


template<class O, class P>struct UpdateReferencePointsGrid_Do
{
	void operator()(O *, P *)
	{
		//GlobalHandlers<O>()();
		O()();
	}
};
void UpdateReferencePointsGrid::operator()()
{
	TL::foreach<items_list, UpdateReferencePointsGrid_Do>()((TL::Factory<items_list> *)0, (int *)0);
}

