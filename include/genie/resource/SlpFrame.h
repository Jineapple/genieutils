/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef GENIE_SLPFRAME_H
#define GENIE_SLPFRAME_H

#include "genie/file/ISerializable.h"

#include <istream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <vector>
#include <stdint.h>


namespace genie
{
  
class ColorPalette;

//------------------------------------------------------------------------------
/// Class for reading a frame in a slp file.
/// TODO: Check file corruption
/// TODO: One (or more??) mask for player color and outline (and shadow?)
//
class SlpFrame : protected ISerializable
{
  
public:  
  //----------------------------------------------------------------------------
  /// Constructor
  ///
  /// @param istr stream to read from
  /// @param pos position to start reading from
  /// @param file_pos position pointing at the beginning of the slp file.
  /// @param palette color palette to choose colors from
  //
  //SlpFrame(std::iostream *iostr, std::streampos pos, std::streampos file_pos, 
  //         ColorPalette *palette);
  
  SlpFrame();
  
  //----------------------------------------------------------------------------
  /// Destructor
  //
  virtual ~SlpFrame();
  
  void serializeHeader(void);
  virtual void serializeObject(void);
  
  void setColorPalette(ColorPalette *pal) { palette_ = pal; }
  
  //----------------------------------------------------------------------------
  /// Loads header data. The headers of frames are stored after the header of 
  /// the slp file.
  //
  void loadHeader(std::istream &istr);
  
  //----------------------------------------------------------------------------
  /// Loads frame data and creates an image. Frame data is located after all
  /// frame headers of the slp file.
  //
  void load(std::istream &istr);
  
  //----------------------------------------------------------------------------
  /// Returns the image created by loading the frame.
  ///
  /// @return loaded image
  //
  sf::Image* getImage() const;
  
  //----------------------------------------------------------------------------
  /// Returns the outline created by loading the frame.
  /// Outline is the border of the sprite shown if a object is behind another
  /// one.
  ///
  /// @return loaded outline
  //
  sf::Image* getOutline() const;
  
  //----------------------------------------------------------------------------
  /// Returns the mask colored with color of given players index.
  ///
  /// @param player index
  /// @return player color mask
  //
  sf::Image* getPlayerColorMask(uint8_t player) const; //TODO auto_ptr
  
  //----------------------------------------------------------------------------
  /// Get the hotspot of the frame. The Hotspot is the center of the image.
  ///
  /// @return x coordinate of the hotspot
  // TODO: Hotspot as vector
  int32_t getHotspotX() const;
  
  //----------------------------------------------------------------------------
  /// Get the hotspot of the frame. The Hotspot is the center of the image.
  ///
  /// @return y coordinate of the hotspot
  //
  int32_t getHotspotY() const;
  
  
  std::streampos file_pos_;
private:
  
  uint32_t cmd_table_offset_;
  uint32_t outline_table_offset_;
  uint32_t palette_offset_;
  uint32_t properties_;
  
  int32_t width_;
  int32_t height_;
  int32_t hotspot_x_;
  int32_t hotspot_y_;
  
  int16_t *left_edges_;
  int16_t *right_edges_;
  
  sf::Image *image_;
  sf::Image *outline_;
  //sf::Image *player_color_mask_;
  
  int32_t player_color_index_;
  
  ColorPalette *palette_;
  
  // Element for player_color vector, the vector stores position (x, y) of
  // a player color pixel and the palette index for the color
  struct PlayerColorElement
  {
    uint32_t x;
    uint32_t y;
    uint8_t index;
  };
  
  std::vector<PlayerColorElement> player_color_mask_;
  
  //----------------------------------------------------------------------------
  /// Reads the edges of the frame. An edge int is the number of pixels in 
  /// a row which are transparent. There are two 16 bit unsigned integers for
  /// each side of a row. One starting from left and the other starting from the
  /// right side.
  /// Assuming stream pointer is at beginning of edges array.
  //
  void readEdges();
  
  //----------------------------------------------------------------------------
  /// Reads pixel indeces from file and sets the pixels according to the
  /// colors from the palette.
  /// It is assumed that the stream pointer is at the start of the pixel array.
  ///
  /// @param image target to set pixel
  /// @param row row to set pixels at
  /// @param col column to set pixels from
  /// @param count how many pixels should be read
  /// @param player_col if true, pixel will be written to player color image
  //
  void readPixelsToImage(sf::Image *image, uint32_t row, uint32_t &col, 
                         uint32_t count, bool player_col = false);
  
  //----------------------------------------------------------------------------
  /// Sets the next count of pixels to given color without reading from stream.
  ///
  /// @param image target to set pixel
  /// @param row row to set pixels at
  /// @param col column to set pixels from
  /// @param count how many pixels should be set
  /// @param color color to set
  /// @param player_col if true, pixel will be written to player color image
  //
  void setPixelsToColor(sf::Image *image, uint32_t row, uint32_t &col, 
                        uint32_t count, uint8_t color_index, 
                        bool player_col = false);
  
  //----------------------------------------------------------------------------
  /// This method returns either the count stored in command byte or (if not 
  /// stored in command) the value of the next byte.
  ///
  /// @param data command byte
  //
  uint8_t getPixelCountFromData(uint8_t data);
};

}

#endif // GENIE_SLPFRAME_H
