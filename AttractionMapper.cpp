#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_mymap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment holder;
	for (unsigned int i = 0; i < ml.getNumSegments(); i++) {
		ml.getSegment(i, holder);
		if (holder.attractions.size() > 0) {
			for (size_t a = 0; a < holder.attractions.size(); a++) {
				string loweringname = holder.attractions[a].name;
				for (int r = 0; r < loweringname.size(); r++)
					loweringname[r] = tolower(loweringname[r]);
				m_mymap.associate(loweringname, holder.attractions[a].geocoordinates);
			}
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int i = 0; i < attraction.size(); i++)
		attraction[i] = tolower(attraction[i]);
	const GeoCoord* temp = m_mymap.find(attraction);
	
	if (temp == nullptr)
		return false;

	gc = *temp;
	return true;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
