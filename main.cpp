#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

#include <iostream>
#include <string>
#include "MyMap.h"
#include "support.h"
#include "provided.h"

#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

int main()
{
	/*cout << "About to test MyMap" << endl;
	{
		MyMap<int, string> mm;
		mm.associate(20, "Ethel");
		mm.associate(10, "Fred");
		const string* p = mm.find(10);
		assert(p != nullptr  &&  *p == "Fred");
		assert(mm.find(30) == nullptr);
		assert(mm.size() == 2);
	}
	cout << "MyMap PASSED" << endl;

	cout << "About to test MapLoader" << endl;
	{
		MapLoader ml;
		assert(ml.load("testmap.txt"));
		size_t numSegments = ml.getNumSegments();
		assert(numSegments == 7);
		bool foundAttraction = false;
		for (size_t i = 0; i < numSegments; i++)
		{
			StreetSegment seg;
			assert(ml.getSegment(i, seg));
			if (seg.streetName == "Picadilly")
			{
				assert(seg.attractions.size() == 1);
				assert(seg.attractions[0].name == "Eros Statue");
				foundAttraction = true;
				break;
			}
		}
		assert(foundAttraction);
	}
	cout << "MapLoader PASSED" << endl;

	cout << "About to test AttractionMapper" << endl;
	{
		MapLoader ml;
		assert(ml.load("testmap.txt"));
		AttractionMapper am;
		am.init(ml);
		GeoCoord gc;
		assert(am.getGeoCoord("Hamleys Toy Store", gc));
		assert(gc.latitudeText == "51.512812");
		assert(gc.longitudeText == "-0.140114");
	}
	cout << "AttractionMapper PASSED" << endl;

	cout << "About to test SegmentMapper" << endl;
	{
		MapLoader ml;
		assert(ml.load("testmap.txt"));
		SegmentMapper sm;
		sm.init(ml);
		GeoCoord gc("51.510087", "-0.134563");
		vector<StreetSegment> vss = sm.getSegments(gc);
		assert(vss.size() == 4);
		string names[4];
		for (size_t i = 0; i < 4; i++)
			names[i] = vss[i].streetName;
		sort(names, names + 4);
		const string expected[4] = {
			"Coventry Street", "Picadilly", "Regent Street", "Shaftesbury Avenue"
		};
		assert(equal(names, names + 4, expected));
	}
	cout << "SegmentMapper PASSED" << endl;

	cout << "About to test Navigator" << endl;
	{
		Navigator nav;
		assert(nav.loadMapData("testmap.txt"));
		vector<NavSegment> directions;
		assert(nav.navigate("Eros Statue", "Hamleys Toy Store", directions) == NAV_SUCCESS);
		assert(directions.size() == 6);
		struct ExpectedItem
		{
			NavSegment::NavCommand command;
			string direction;
			double distance;
			string streetName;
		};
		GeoCoord base("51.510087", "-0.134563");
		GeoCoord par("51.509894", "-0.134482");
		GeoSegment daddy(base, par);
		cout << angleOfLine(daddy);
		const ExpectedItem expected[6] = {
			{ NavSegment::PROCEED, "northwest", 0.0138, "Picadilly" },
			{ NavSegment::TURN, "left", 0, "" },
			{ NavSegment::PROCEED, "west", 0.0119, "Regent Street" },
			{ NavSegment::PROCEED, "west", 0.0845, "Regent Street" },
			{ NavSegment::PROCEED, "west", 0.0696, "Regent Street" },
			{ NavSegment::PROCEED, "northwest", 0.1871, "Regent Street" },
		};
		for (size_t i = 0; i < 6; i++)
		{
			const NavSegment& ns = directions[i];
			const ExpectedItem& exp = expected[i];
			assert(ns.m_command == exp.command);
			assert(ns.m_direction == exp.direction);
			if (ns.m_command == NavSegment::PROCEED)
			{
				assert(abs(ns.m_distance - exp.distance) < 0.00051);
				assert(ns.m_streetName == exp.streetName);
			}
		}
	}
	cout << "Navigator PASSED" << endl;*/
	
		Navigator segmappery;
		segmappery.loadMapData("mapdata.txt");
	
		vector<NavSegment> temp;
		NavResult readme = NAV_NO_ROUTE;
		readme = segmappery.navigate("Brentwood Country Mart", "Twentieth Century Fox Film Corporation", temp);
	
		if (readme == NAV_SUCCESS)
			cout << "You did it!" << endl;
		else if (readme == NAV_NO_ROUTE)
			cout << "Fuck this project" << endl;
		else if (readme == NAV_BAD_SOURCE)
			cout << "Bad Source" << endl;
		else
			cout << "Bad Destination" << endl;
}


//#include <iostream>
//#include <string>
//#include "MyMap.h"
//#include "support.h"
//#include "provided.h"
//
//using namespace std;
//
//
//int main() {
//	Navigator segmappery;
//	segmappery.loadMapData("mapdata.txt");
//
//	vector<NavSegment> temp;
//	NavResult readme = NAV_NO_ROUTE;
//	readme = segmappery.navigate("Brentwood Country Mart", "Thalians Mental Health Center", temp);
//
//	if (readme == NAV_SUCCESS)
//		cout << "You did it!" << endl;
//	else if (readme == NAV_NO_ROUTE)
//		cout << "Fuck this project" << endl;
//	else if (readme == NAV_BAD_SOURCE)
//		cout << "Bad Source" << endl;
//	else
//		cout << "Bad Destination" << endl;
//
//	//AttractionMapper attractions;
//	//GeoCoord holder;
//
//	//attractions.getGeoCoord("BrentwOOd Country Mart", holder);
//	/*MyMap<string, int> mappery;
//	cout << mappery.size();
//	mappery.associate("ora", 103);
//	mappery.associate("dora", 103);
//	mappery.associate("muda", 103);
//	mappery.associate("muda", 135397);
//	cout << mappery.size();
//	mappery.clear();
//	return 0;*/
//}