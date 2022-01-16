/**
 * \file Road.cpp
 *
 * \author Chris Cardimen
 *
 */
#include "pch.h"
#include "Road.h"
#include "Car.h"

/**
 * Constructor for road
 * \param game Game the road is a part of
 */
CRoad::CRoad(CGame* game) : CTerrain(game)
{
    SetIsDeadly(false);
}

/**
 * Get the cars on the road
 * \returns Cars on the road
 */
vector<shared_ptr<CCar>> CRoad::GetCars()
{
    return mCars;
}

/**
 * Add a car to the cars vector
 * \param car Car to be added
 */
void CRoad::AddCar(shared_ptr<CCar> car)
{
    mCars.push_back(car);
}

/**
 * Load the road data from an xml file
 * \param node Node to load data from
 */
void CRoad::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode>& node) 
{
    SetCoords(GetX(), node->GetAttributeDoubleValue(L"y", 0));
    SetWidth(node->GetAttributeDoubleValue(L"width", 0));
    SetSpeed(node->GetAttributeDoubleValue(L"speed", 0));
}