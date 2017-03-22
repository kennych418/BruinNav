#include "provided.h"
#include "MyMap.h"
#include <queue>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	MapLoader m_map;
	AttractionMapper m_attractions;
	SegmentMapper m_segments;
	string calculateAngle(double degree) const;
	string sider(int degree) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	m_map.load(mapFile);
	m_attractions.init(m_map);
	m_segments.init(m_map);
	return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	MyMap<GeoCoord, GeoCoord> route;
	MyMap<GeoCoord,string> visited;
	queue<GeoCoord> theQueue;
	bool foundIt = false;
	bool canContinue = true;
	bool firstRun = true;
	string* checkingVisited;

	//get starting location's geocoord
	GeoCoord startGeoCoord;
	GeoCoord endGeoCoord;

	if (m_attractions.getGeoCoord(start, startGeoCoord) == false)	//check for bad source/destination while loading start and end GeoCoords
		return NAV_BAD_SOURCE;
	if (m_attractions.getGeoCoord(end, endGeoCoord) == false)
		return NAV_BAD_DESTINATION;

	//push start onto THE queue and visited vector
	theQueue.push(startGeoCoord);
	visited.associate(startGeoCoord, "");

	while (theQueue.empty() != true) {
		GeoCoord currentGeoCoord = theQueue.front();
		theQueue.pop();

		//check if it is the end
		if (currentGeoCoord == endGeoCoord) {
			foundIt = true;
			break;
		}

		//begin pushing all connected streetseg GeoCoords onto the queue
		vector<StreetSegment> connectedStreets = m_segments.getSegments(currentGeoCoord);
		for (int i = 0; i < connectedStreets.size(); i++) {	
			if (firstRun == true) {
				theQueue.push(connectedStreets[i].segment.start);
				visited.associate(connectedStreets[i].segment.start,"iamnotanullptr");
				route.associate(connectedStreets[i].segment.start, currentGeoCoord);
				firstRun = false;
			}
			else if (currentGeoCoord == connectedStreets[i].segment.end) {
				
				checkingVisited = visited.find(connectedStreets[i].segment.start);	//check if it is in the visited map
				if (checkingVisited != nullptr)
					canContinue = false;
		
				if (canContinue == true) {
					theQueue.push(connectedStreets[i].segment.start);
					visited.associate(connectedStreets[i].segment.start,"iamnotanullptr");
					route.associate(connectedStreets[i].segment.start, currentGeoCoord);
				}
				else
					canContinue = true;
			}
			else if (currentGeoCoord == connectedStreets[i].segment.start) {

				checkingVisited = visited.find(connectedStreets[i].segment.end);	//check if it is in the visited map
				if (checkingVisited != nullptr)
					canContinue = false;

				if (canContinue == true) {
					theQueue.push(connectedStreets[i].segment.end);
					visited.associate(connectedStreets[i].segment.end, "iamnotanullptr");
					route.associate(connectedStreets[i].segment.end, currentGeoCoord);
				}
				else
					canContinue = true;
			}
			if (canContinue == true && connectedStreets[i].attractions.size() > 0) {
				for (int A = 0; A < connectedStreets[i].attractions.size(); A++) {

					checkingVisited = visited.find(connectedStreets[i].attractions[A].geocoordinates);	//check if it is in the visited map
					if (checkingVisited != nullptr)
						canContinue = false;

					if (canContinue == true) {
						theQueue.push(connectedStreets[i].attractions[A].geocoordinates);
						visited.associate(connectedStreets[i].attractions[A].geocoordinates, "iamnotanullptr");
						route.associate(connectedStreets[i].attractions[A].geocoordinates, currentGeoCoord);
					}
					else
						canContinue = true;
				}
			}
		}
	}

	//if the bool was never changed, then there were no routes found
	if (foundIt == false)
		return NAV_NO_ROUTE;
	
	//set up variables to begin returning route
	vector<NavSegment> Directions;
	GeoCoord current = endGeoCoord;
	GeoCoord previous = endGeoCoord;
	string previousStreetName;
	string newStreetName;
	GeoCoord* parent = &current;
	bool firsttime2 = true;

	while (*parent != startGeoCoord) {

		GeoSegment baseGeoSeg(current, *parent);	//used to keep track of turn angles
		current = *parent;						//You moved this up


		parent = route.find(current);
		vector<StreetSegment> availableOptions = m_segments.getSegments(current);

		if (firsttime2 == true) {	//During the first run, both street names are "" and ""
			newStreetName = availableOptions[0].streetName;
			previousStreetName = newStreetName;
			firsttime2 = false;
		}

		GeoSegment parentGeoSeg(current, *parent);	//used to keep track of turn angles

		for (int i = 0; i < availableOptions.size(); i++) {
			if ((availableOptions[i].segment.start == current && availableOptions[i].segment.end == *parent) || (availableOptions[i].segment.start == *parent && availableOptions[i].segment.end == current))
				newStreetName = availableOptions[i].streetName;
		}



		if (previousStreetName == newStreetName) {
			string position = calculateAngle(angleOfLine(parentGeoSeg));
			Directions.push_back(NavSegment(position, newStreetName, distanceEarthMiles(current, *parent), GeoSegment(current, *parent)));
		}
		else {
			string turndirection = sider(angleBetween2Lines(baseGeoSeg, parentGeoSeg));
			//string position = calculateAngle(angleOfLine(baseGeoSeg));
			//Directions.push_back(NavSegment(position, previousStreetName, distanceEarthKM(current, *parent), GeoSegment(current, *parent)));
			Directions.push_back(NavSegment(turndirection, previousStreetName));
			previousStreetName = newStreetName;
		}
	}

	//directions = Directions;

	vector<NavSegment> newstart; //give directions an empty vector
	directions = newstart;
	for (int i = Directions.size() - 1; i >= 0; i--) {	//we dont want the first or last nav segments (map from the attraction to the same street
		directions.push_back(Directions[i]);
	}
	return NAV_SUCCESS;  // This compiles, but may not be correct*/
}

string NavigatorImpl::calculateAngle(double degree) const {
	if (degree > 337.5 || degree <= 22.5) {
		return "west";
	}
	else if (degree > 22.5 && degree <= 67.5) {		//0 = south //45 = southwest //90 = west //135 = northwest //180 = north //225 = northeast //270 = east //315 = southeast
		return "southwest";
	}
	else if (degree > 67.5 && degree <= 112.5) {
		return "south";
	}
	else if (degree > 112.5 && degree <= 157.5) {
		return "southeast";
	}
	else if (degree > 157.5 && degree <= 202.5) {
		return "east";
	}
	else if (degree > 202.5 && degree <= 247.5) {
		return "northeast";
	}
	else if (degree > 247.5 && degree <= 292.5) {
		return "north";
	}
	else if (degree > 292.5 && degree <= 337.5) {
		return "northwest";
	}
	return "";
}

string NavigatorImpl::sider(int degree) const{
	string returnside = "";
	if (degree >= 0 && degree <= 180)
		returnside = "right";
	else
		returnside = "left";
	return returnside;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
