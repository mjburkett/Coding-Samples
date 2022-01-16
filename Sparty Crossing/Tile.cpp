/**
 * \file Tile.cpp
 *
 * \author Matthew Burkett
 */

#include "pch.h"
#include "Tile.h"

using namespace Gdiplus;

/**  Constructor
 * \param city The city this item is a member of
 */
CTile::CTile(CGame* city) : CItem(city)
{


}


/**
*  Destructor
*/
CTile::~CTile()
{
}

/**
* brief Load the attributes for a tile node.
*
* \param node The Xml node we are loading the item from
* \param repNumX The number of times the tile has repeated in the x direction
* \param repNumY The number of times the tile has repeated in the y direction
*/
void CTile::XmlLoad(const std::shared_ptr<xmlnode::CXmlNode>& node, int repNumX, int repNumY)
{
    SetCoords(node->GetAttributeDoubleValue(L"x", 0) + repNumX, node->GetAttributeDoubleValue(L"y", 0) + repNumY);
}


/**
 * Draws the tiles
 * \param graphics Pointer to graphics drawer
 */
void CTile::Draw(Graphics* graphics)
{
    if (mItemImage != nullptr)
    {
        int wid = mItemImage->GetWidth();
        int hit = mItemImage->GetHeight();

        graphics->DrawImage(mItemImage.get(),
            GetX(), GetY() + (OffsetDown * 2) - hit,
            wid + 1, hit + 1);
    }
}