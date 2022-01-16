/**
 * \file Tile.h
 *
 * \author Matthew J. Burkett
 *
 * Base class for any tile in our game
 */

#pragma once
#include "Item.h"


/**
 * Manges the base class for any tile in our game.
 */
class CTile : public CItem {
public:
    CTile(CGame* game);

    ///  Default constructor (disabled)
    CTile() = delete;

    ///  Copy constructor (disabled)
    CTile(const CTile&) = delete;

    ~CTile();

    virtual void XmlLoad(const std::shared_ptr<xmlnode::CXmlNode>& node, int repNumX, int repNumY);

    virtual void Draw(Gdiplus::Graphics* graphics) override;

    /** Accept a visitor
     * \param visitor The visitor we accept */
    virtual void Accept(CItemVisitor* visitor) override { visitor->VisitTile(this); }
};

