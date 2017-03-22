#include "provided.h"
#include "support.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	int m_size;
	vector<StreetSegment> m_streetsegholder;
	void hardcoder(string wholecoord, string& lat1, string& long1, string& lat2, string& long2);
};

MapLoaderImpl::MapLoaderImpl()
{
	m_size = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
}

void MapLoaderImpl::hardcoder(string wholecoords, string& lat1, string& long1, string& lat2, string& long2) {
	int counter = 0;
	//lat1
	for (; counter < wholecoords.length(); counter++) {
		if (wholecoords[counter] != ',')
			lat1 += wholecoords[counter];
		else {
			counter++;
			break;
		}
	}
	//long1
	
	if(wholecoords[counter] == ' ')
		counter++;	//in case starts with a space

	for (; counter < wholecoords.length(); counter++) {
		if (wholecoords[counter] != ' ')
			long1 += wholecoords[counter];
		else {
			counter++;
			break;
		}
	}
	//lat2
	for (; counter < wholecoords.length(); counter++) {
		if (wholecoords[counter] != ',')
			lat2 += wholecoords[counter];
		else {
			counter++;
			break;
		}
	}

	if (wholecoords[counter] == ' ')
		counter++;	//in case starts with a space

	//long2
	for (; counter < wholecoords.length(); counter++)
		long2 += wholecoords[counter];
}
bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	
	if (!infile)
		return false;

	string name;
	while (getline(infile, name)) {
		
		///////////// Name ////////////////

		StreetSegment currentStreet;
		currentStreet.streetName = name;
		
		///////////// Coord ////////////////

		string wholecoords;
		getline(infile, wholecoords);
		string lat1 = "";
		string long1 = "";
		string lat2 = "";
		string long2 = "";
		
		hardcoder(wholecoords, lat1, long1, lat2, long2);

		//cout << lat1 << " " << long1 << " " << lat2 << " " << long2 << endl;
		currentStreet.segment = GeoSegment(GeoCoord(lat1, long1), GeoCoord(lat2, long2));
		
		////////////////// Attraction /////////////////////////
		
		string attractionstring = "";
		getline(infile, attractionstring);
		int attractionnum = stoi(attractionstring);

		for (int i = 0; i < attractionnum; i++) {
			Attraction currentAttraction;
			int counter = 0;
			
			string attractioninfo="";
			string attractionname="";
			string attractionlat="";
			string attractionlong = "";
			getline(infile, attractioninfo);

			for (; counter < attractioninfo.length(); counter++) {	//Name
				if (attractioninfo[counter] != '|')
					attractionname += attractioninfo[counter];
				else {
					counter++;
					break;
				}
			}

			for (; counter < attractioninfo.length(); counter++) {	//lat
				if (attractioninfo[counter] != ',')
					attractionlat += attractioninfo[counter];
				else {
					counter++;
					break;
				}
			}

			if(attractioninfo[counter] == ' ')	//in case of space
				counter++;

			for (; counter < attractioninfo.length(); counter++)	//long
				attractionlong += attractioninfo[counter];

			currentAttraction.name = attractionname;
			currentAttraction.geocoordinates = GeoCoord(attractionlat, attractionlong);

			currentStreet.attractions.push_back(currentAttraction);
		}
		m_streetsegholder.push_back(currentStreet);
		m_size++;
	}

	/*for (int i = 0; i < m_streetsegholder.size(); i++)	Debugging
		cout << m_streetsegholder[i].streetName << endl;*/

	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_size;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if(segNum < 0 || segNum > getNumSegments() - 1)
		return false;  // This compiles, but may not be correct
	seg = m_streetsegholder[segNum];
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
