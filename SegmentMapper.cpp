#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> m_mymap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment currentStreetSeg;
	GeoCoord currentGeoCoord;
	vector<StreetSegment>* findreturn;

	for (int i = 0; i < ml.getNumSegments(); i++) {
		ml.getSegment(i, currentStreetSeg);
		
		//////////////////////// Start Coord ////////////////////////
		findreturn = m_mymap.find(currentStreetSeg.segment.start);
		if (findreturn == nullptr) {
			vector<StreetSegment> newvector;
			newvector.push_back(currentStreetSeg);
			m_mymap.associate(currentStreetSeg.segment.start, newvector);
		}
		else 
			findreturn->push_back(currentStreetSeg);
		//////////////////////// End Coord ////////////////////////
		findreturn = m_mymap.find(currentStreetSeg.segment.end);
		if (findreturn == nullptr) {
			vector<StreetSegment> newvector;
			newvector.push_back(currentStreetSeg);
			m_mymap.associate(currentStreetSeg.segment.end, newvector);
		}
		else
			findreturn->push_back(currentStreetSeg);
		////////////////////// Attractions ////////////////////////
		if (currentStreetSeg.attractions.size() > 0) {
			for (int a = 0; a < currentStreetSeg.attractions.size(); a++) {
				findreturn = m_mymap.find(currentStreetSeg.attractions[a].geocoordinates);
				if (findreturn == nullptr) {
					vector<StreetSegment> newvector;
					newvector.push_back(currentStreetSeg);
					m_mymap.associate(currentStreetSeg.attractions[a].geocoordinates, newvector);
				}
				else
					findreturn->push_back(currentStreetSeg);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	const vector<StreetSegment>* checker = m_mymap.find(gc);
	if (checker != nullptr)
		return *checker;
	vector<StreetSegment> emptyshit;
	return emptyshit;  // This compiles, but may not be correct
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
