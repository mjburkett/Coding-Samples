/**
 * \file Level.cpp
 *
 * \author Jason Israilov
 * 
 * This manages the entire levels and what happens in them in the game.
 */


#include "pch.h"
#include "Level.h"
#include "Tile.h"
#include "Rectangle.h"
#include "XmlNode.h"
#include "Hero.h"
#include "Cargo.h"
#include "LivingCargo.h"
#include "Car.h"
#include "Road.h"
#include "River.h"
#include "Space.h"
#include "Boat.h"
#include "SpaceShip.h"

using namespace xmlnode;
using namespace std;

/// loading the decor name from Xml
const wstring decorName = L"decor"; 
/// loading the car name from Xml
const wstring carName = L"car"; 
/// loading the boat name from Xml
const wstring boatName = L"boat"; 
/// loading the image name from Xml
const wstring spaceShipName = L"space-ship";
/// loading the image name from Xml
const wstring imageName = L"image"; 


/**
 * Level constructor
 * \param game The game that level is in
 * \param filename The filename for the level
 */
CLevel::CLevel(CGame* game, const wstring& filename)
{
    mGame = game;
    XmlLoad(filename);
}

/**
 * Loads the attributes for a level load.
 * \param filename The filename to load
 */
void CLevel::XmlLoad(const wstring& filename)
{

    
    try
    {
        // Open the document to read
        shared_ptr<CXmlNode> root = CXmlNode::OpenDocument(filename);

        // Once we know it is open, clear the existing data
        Clear();

        for (auto node : root->GetChildren()) {
            if (node->GetName() == L"types") {
                //Creation of new Tiles to pass to CLandScape
                vector<shared_ptr<CTile>> tileVec;

                // Instead of have member variables of the vector of nodes,
                // create vectors of tiles, cars, and boats and use those in
                // factory functions to create several

                //Loops through the types, gathering data for Tiles, Cars, and Boats
                for (auto val : node->GetChildren())
                {
                    auto type = val->GetName();

                    if (type == decorName)
                    {
                        mDecorTypes.push_back(val);
                    }
                    else if (type == carName)
                    {
                        mCarTypes.push_back(val);
                    }
                    else if (type == boatName)
                    {
                        mBoatTypes.push_back(val);
                    }
                    else if (type == spaceShipName)
                    {
                        mSpaceShipTypes.push_back(val);
                    }
                }
            }
            else if (node->GetName() == L"background") {
                for (auto val : node->GetChildren()) {
                    if (val->GetName() == L"decor") {
                        XmlTile(val);
                    }
                    else if (val->GetName() == L"rect") {
                        XmlRect(val);
                    }
                }
            }
            else if (node->GetName() == L"hero") {
                XmlHero(node);
            }
            else if (node->GetName() == L"cargo") {
                XmlCargo(node);
            }
            else if (node->GetName() == L"river") {
                XmlRiver(node);
            }
            else if (node->GetName() == L"road") {
                XmlRoad(node);
            }
            else if (node->GetName() == L"space")
            {
                XmlSpace(node);
            }
        }
    }
    catch (CXmlNode::Exception ex)
    {
        AfxMessageBox(ex.Message().c_str());
    }
}

/**
 * Load the attributes for a tile/decor item node. The node is compared with the id of the
 * given types. If it matches, the type's attributes are applied to the object that is created. Lastly,
 * the created item is added to CGame.
 * \param node Node the tile is being loaded from.
 */
void CLevel::XmlTile(const std::shared_ptr<xmlnode::CXmlNode>& node) {
    //These hold how many times to repeat making the item. If no repeat value is present, default to one time.
    int yRepeatNum = node->GetAttributeIntValue(L"repeat-y", 1);
    int xRepeatNum = node->GetAttributeIntValue(L"repeat-x", 1);

    // Instead of the below loop, use a landscape factory function (unknown name) to create a vector
    // of all tiles necessary for level
    for (int y = 0; y < yRepeatNum; y++) {
        for (int x = 0; x < xRepeatNum; x++) {
            //Create a tile
            shared_ptr<CTile> tile = make_shared<CTile>(mGame);
            //Load its attributes
            tile->XmlLoad(node, x, y);
            //Loop through the type nodes from earlier to find match
            for (auto typeNode : mDecorTypes) {
                //When a match is found, set the image of the item to the image attribute value of the match.
                if (typeNode->GetAttributeValue(L"id", L"none") == node->GetAttributeValue(L"id", L"na")) {
                    tile->SetImage(typeNode->GetAttributeValue(L"image", L""));
                }
            }
            //Add the item to the level's list
            mLevelItems.push_back(tile);
        }
    }
}

/**
 * Load the attributes for a rect item node. The created item is added to CGame.
 * \param node Node the rectangle is being loaded from.
 */
void CLevel::XmlRect(const std::shared_ptr<xmlnode::CXmlNode>& node) {
    //These hold how many times to repeat making the item. If no repeat value is present, default to one time.
    int yRepeatNum = node->GetAttributeIntValue(L"repeat-y", 1);
    int xRepeatNum = node->GetAttributeIntValue(L"repeat-x", 1);

    for (int y = 0; y < yRepeatNum; y++) {
        for (int x = 0; x < xRepeatNum; x++) {
            //Create a tile
            shared_ptr<CRectangle> rect = make_shared<CRectangle>(mGame);
            //Load its attributes
            rect->XmlLoad(node, x, y);
            //Add the item to the level's list
            mLevelItems.push_back(rect);
        }
    }
}


/**
 * Load the attributes for a hero item node. The created item is added to CGame.
 * \param node Node the hero is being loaded from.
 */
void CLevel::XmlHero(const std::shared_ptr<xmlnode::CXmlNode>& node) {
    //Create a new item in the game
    shared_ptr<CHero> hero = make_shared<CHero>(mGame);
    //Have the item handle retreiving its attributes from the node
    hero->XmlLoad(node);
    hero->SetIndex(mHero.size());
    //Add the hero to the Hero pointer
    mHero.push_back(hero);
    
}


/**
 * Load the attributes for a cargo item node. The created item is added to CGame.
 * \param node Noad the cargo is bring loaded from.
 */
void CLevel::XmlCargo(const std::shared_ptr<xmlnode::CXmlNode>& node) {
    //Create a new item in the game
    if (node->GetAttributeValue(L"eats", L"") == L"")
    {
        shared_ptr<CCargo> cargo = make_shared<CCargo>(mGame);
        cargo->XmlLoad(node);
        mLevelItems.push_back(cargo);
    }
    else
    {
        shared_ptr<CLivingCargo> cargo = make_shared<CLivingCargo>(mGame);
        cargo->XmlLoad(node);
        //Add the item to the level's list
        mLevelItems.push_back(cargo);
    }
}

 /**
  * Load the attributes for a river item node. The created item is added to CGame.
  * \param node Node the river is being loaded from.
  **/
void CLevel::XmlRiver(const std::shared_ptr<xmlnode::CXmlNode>& node) {
    //Create a new item in the game
    shared_ptr<CRiver> river = make_shared<CRiver>(mGame);
    //Have the item handle retreiving its attributes from the node
    river->XmlLoad(node);
    //Add the item to the level's list
    mLevelItems.push_back(river);

    //Create the boats.
    for (auto val : node->GetChildren()) 
    {
        if (val->GetName() == L"boat") {
            XmlBoat(val, node, river);
        }
    }
}


 /**
  * Load the attributes for a boat item node. The node is compared with the id of the
  * given types. If it matches, the type's attributes are applied to the object that is created. Lastly,
  * the created item is added to CGame.
  * \param boatNode Node the boat is being loaded from.
  * \param riverNode Node for the river this boat is in
  * \param parent Pointer to this boat's river
  **/
void CLevel::XmlBoat(const std::shared_ptr<xmlnode::CXmlNode>& boatNode, const std::shared_ptr<xmlnode::CXmlNode>& riverNode, 
    std::shared_ptr<CRiver> parent) {
    //Create a new item in the game
    shared_ptr<CBoat> boat = make_shared<CBoat>(mGame);

    //Set type-related values
    //Loop through the type nodes from earlier to find match
    for (auto typeNode : mBoatTypes) 
    {
        //When a match is found, set the image of the item to the image attribute value of the match.
        if (typeNode->GetAttributeValue(L"id", L"none") == boatNode->GetAttributeValue(L"id", L"na")) 
        {
            boat->SetImage(typeNode->GetAttributeValue(L"image", L""));
        }
    }

    //Set river-related values
    boat->SetCoords(boatNode->GetAttributeIntValue(L"x", 0) + 0.5, riverNode->GetAttributeDoubleValue(L"y", 0) + 0.5);

    //Sets the boats looping width
    boat->SetBoundingDistance(riverNode->GetAttributeIntValue(L"width", 18));

    //Sets the boat's speed
    boat->SetSpeed(riverNode->GetAttributeDoubleValue(L"speed", 1));

    //Add the item to the level's list
    mLevelItems.push_back(boat);

    //Add the boat to the river
    parent->AddBoat(boat);
}

/**
 * Load the attributes for a space item node. The created item is added to CGame.
 * \param node Node the space is being loaded from
 **/
void CLevel::XmlSpace(const std::shared_ptr<xmlnode::CXmlNode>& node)
{
    //Create a new item in the game
    shared_ptr<CSpace> space = make_shared<CSpace> (mGame);
    //Have the item handle retreiving its attributes from the node
    space->XmlLoad(node);
    //Add the item to the level's list
    mLevelItems.push_back(space);

    //Create the boats.
    for (auto val : node->GetChildren()) 
    {
        XmlSpaceShip(val, node, space);
    }
}

/**
  * Load the attributes for a space ship item node. The created item is added to CGame.
  * \param spaceShipNode Node the space ship is being loaded from.
  * \param spaceNode Node of the space terrain that the ship is being added to
  * \param parent Pointer to the space terrain that the ship is being added to
  **/
void CLevel::XmlSpaceShip(const std::shared_ptr<xmlnode::CXmlNode>& spaceShipNode, const std::shared_ptr<xmlnode::CXmlNode>& spaceNode, 
    std::shared_ptr<CSpace> parent) {
    //Create a new item in the game
    shared_ptr<CSpaceShip> spaceShip = make_shared<CSpaceShip>(mGame);

    //Loop through the type nodes from earlier to find match
    for (auto typeNode : mSpaceShipTypes) {
        //When a match is found, set the image of the item to the image attribute value of the match.
        if (typeNode->GetAttributeValue(L"id", L"none") == spaceShipNode->GetAttributeValue(L"id", L"na")) {
            spaceShip->SetImage(typeNode->GetAttributeValue(L"image", L""));
            spaceShip->SetImages(typeNode->GetAttributeValue(L"warn-image", L""), typeNode->GetAttributeValue(L"disabled-image", L""));
            break;
        }
    }

    //Set river-related values
    spaceShip->SetCoords(spaceShipNode->GetAttributeDoubleValue(L"x", 0) + 0.5, spaceNode->GetAttributeDoubleValue(L"y", 0) + 0.5);

    //Sets the space ship's looping width
    spaceShip->SetBoundingDistance(spaceNode->GetAttributeIntValue(L"width", 18));

    //Sets the space ship's speed
    spaceShip->SetSpeed(spaceNode->GetAttributeDoubleValue(L"speed", 1));

    //Add the item to the level's list
    mLevelItems.push_back(spaceShip);

    //Add the space ship to the river
    parent->AddSpaceShip(spaceShip);
}

 /**
  * Load the attributes for a road item node. The created item is added to CGame.
  * \param node Node the road is being loaded from.
  */
void CLevel::XmlRoad(const std::shared_ptr<xmlnode::CXmlNode>& node) {
    //Create a new item in the game
    shared_ptr<CRoad> road = make_shared<CRoad>(mGame);
    //Have the item handle retreiving its attributes from the node
    road->XmlLoad(node);
    //Add the item to the level's list
    mLevelItems.push_back(road);

    for (auto val : node->GetChildren()) 
    {
        XmlCar(val, node, road);
    }
}


 /**
  * Load the attributes for a car item node. The node is compared with the id of the
  * given types. If it matches, the type's attributes are applied to the object that is created. Lastly,
  * the created item is added to CGame.
  * \param carNode Node the car is being loaded from.
  * \param roadNode Node to this cars road
  * \param parent Pointer to the road this car belongs to
  **/
void CLevel::XmlCar(const std::shared_ptr<xmlnode::CXmlNode>& carNode, const std::shared_ptr<xmlnode::CXmlNode>& roadNode, std::shared_ptr<CRoad> parent) {
    //Create a new item in the game
    shared_ptr<CCar> car = make_shared<CCar>(mGame);
    //Have the item handle retreiving its attributes from the node
    car->XmlLoad(carNode);

    //Loop through the type nodes from earlier to find match
    for (auto typeNode : mCarTypes) 
    {
        //When a match is found, set the image of the item to the image attribute value of the match.
        if (typeNode->GetAttributeValue(L"id", L"none") == carNode->GetAttributeValue(L"id", L"na")) {
            car->SetImage(typeNode->GetAttributeValue(L"image1", L""));
            car->SetImages(typeNode->GetAttributeValue(L"image2", L""));
            car->SetName(typeNode->GetAttributeValue(L"name", L""));
        }

    }

    //Set position-related values
    car->SetCoords(carNode->GetAttributeIntValue(L"x", 0) + 0.5, roadNode->GetAttributeDoubleValue(L"y", 0) + 0.5);

    //Sets the boats looping width
    car->SetBoundingDistance(roadNode->GetAttributeIntValue(L"width", 18));

    car->SetSpeed(roadNode->GetAttributeDoubleValue(L"speed", 1));

    //Add the item to the level's list
    mLevelItems.push_back(car);

    //Add the car to the road
    parent->AddCar(car);
}


/**
 * Clears all items in the Level items
*/
void CLevel::Clear()
{
	mLevelItems.clear();
}

/**
 * Call from Game back to game to lead all level items.
*/
void CLevel::OnLevelLoad()
{
    //Clear mGame's list
    mGame->ItemClear();
    //Add the list to mGame
    mGame->ItemsAdd(mLevelItems);
}

/**
 * Reset the level to initial configuration
 */
void CLevel::OnLevelReset()
{
    for (shared_ptr<CHero> hero : mHero)
        hero->OnReset();
    for (auto item : mLevelItems)
    {
        item->OnReset();
    }
}