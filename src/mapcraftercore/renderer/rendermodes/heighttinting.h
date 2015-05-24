/*
 * Copyright 2012-2015 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RENDERMODES_HEIGHTTINTING_H_
#define RENDERMODES_HEIGHTTINTING_H_

#include "../rendermode.h"

namespace mapcrafter {
namespace renderer {

class HeightTintingRenderer : public RenderModeRenderer {
public:
	virtual ~HeightTintingRenderer();

	void draw(RGBAImage& image, uint8_t r, uint8_t g, uint8_t b, bool high_contrast);
};

class HeightTintingRenderMode : public BaseRenderMode {
public:
	HeightTintingRenderMode(bool high_contrast);
	virtual ~HeightTintingRenderMode();

	virtual void draw(RGBAImage& image, const mc::BlockPos& pos, uint16_t id, uint16_t data);

	virtual BaseRenderModeType getType() const;

protected:
	bool high_contrast;
};

} /* namespace renderer */
} /* namespace mapcrafter */

#endif /* RENDERMODES_HEIGHTTINTING_H_ */
