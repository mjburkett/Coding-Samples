/**
 * \file Road.h
 *
 * \author Chris Cardimen
 *
 * This class managers a river
 *
 */
#pragma once
#include<vector>
#include<string>
#include "Terrain.h"
#include "Car.h"
using namespace std;

/// <summary>
/// This class manages a river
/// </summary>
class CRoad :
    public CTerrain
{
private:
    vector<shared_ptr<CCar>> mCars; ///< vector of all cars on the road
public:

    CRoad(CGame* game);

    /**
     * Accept function for the road class
     * \param visitor Visitor to be accepted
     */
    virtual void Accept(CItemVisitor* visitor) { visitor->VisitRoad(this); }

    vector<shared_ptr<CCar>> GetCars();

    void AddCar(shared_ptr<CCar> car);

    virtual void XmlLoad(const std::shared_ptr<xmlnode::CXmlNode>& node);
};

