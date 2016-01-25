#include "stdafx.h"
#include "SetRemoveTresholds.h"
#include "Corel.h"

#include "DebugMess.h"


namespace SetRemoveTresholdsSpace
{
	template<class O, class P>struct __set__;
	template<int N, class P>struct __set__<EParam<N>, P>
	{
		typedef EParam<N> O;
		void operator()(O *o, P *p)
		{
			o->value = p[N - 1];
		}
	};
}

SetRemoveTresholds::SetRemoveTresholds(std::list<Data> &l, int(&d)[8])
	: tresholds(l)
	, lines(d)
{}

void SetRemoveTresholds::All()
{
#if 1
	corel.RemoveAllElement();
	corel.InsertListData(tresholds);
    
	int currentID = Singleton<CurrentParametersTable>::Instance().items.get<CurrentID>().value;
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		 ParametersTable param;
	     int id = Select<ParametersTable>(base).ID(currentID).Execute(param);
		 dprint("id__ %d", id);
		 ThresholdsTable &thresholdsTable = Singleton<ThresholdsTable>::Instance();
		 TL::foreach<ThresholdsTable::items_list, SetRemoveTresholdsSpace::__set__>()(&thresholdsTable.items, (int *)lines);
		
		 int treshId = Select<ThresholdsTable>(base).eq_all<ThresholdsTable::items_list>(&thresholdsTable.items).Execute();
		 if(0 == treshId)
		 {
			 Insert_Into<ThresholdsTable>(thresholdsTable, base).Execute();
			 treshId = Select<ThresholdsTable>(base).eq_all<ThresholdsTable::items_list>(&thresholdsTable.items).Execute();
		 }	
		 Update<ParametersTable>(base).set<ID<ThresholdsTable> >(treshId).Where().ID(id).Execute();
	}
#endif
}