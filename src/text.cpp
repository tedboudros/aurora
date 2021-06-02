#include <SDL2/SDL.h>

#include "Text.hpp"

Text::Text()
	: font(NULL), style(TTF_STYLE_NORMAL), outline(0),
	  hinting(TTF_HINTING_NORMAL), kerning(true), texture(NULL),
	  texW(0), texH(0), needUpdate(true), useTextureSize(true) {
	color.r = color.g = color.b = 0;
	color.a = 255;
}

Text::~Text()
{
	if (texture)
		SDL_DestroyTexture(texture);
}

void Text::setFont(TTF_Font *font)
{
	if (font && font != this->font)
	{
		this->font = font;
		needUpdate = true;
	}
}

void Text::setStyle(int style)
{
	if (style != this->style)
	{
		this->style = style;
		needUpdate = true;
	}
}

void Text::setOutline(int outline_size)
{
	if (outline_size != outline)
	{
		outline = outline_size;
		needUpdate = true;
	}
}

void Text::setHinting(int hinting)
{
	if (hinting != this->hinting)
	{
		this->hinting = hinting;
		needUpdate = true;
	}
}

void Text::setKerning(bool enable)
{
	if (enable != kerning)
	{
		kerning = enable;
		needUpdate = true;
	}
}

void Text::setColor(SDL_Color col)
{
	if (memcmp(&col, &color, sizeof(color)))
	{
		color = col;
		needUpdate = true;
	}
}

void Text::setText(const char *s)
{
	if (s && str != s)
	{
		str = s;
		needUpdate = true;
	}
}

void Text::setText(const std::string &str)
{
	if (str != this->str)
	{
		this->str = str;
		needUpdate = true;
	}
}

void Text::setDestRect(MultiSize rect)
{
	destRect = rect;
}

void Text::setDestPos(Size x, Size y)
{
	destRect.x = x;
	destRect.y = y;
}

void Text::setUseTextureSize(bool use)
{
	useTextureSize = use;
}

void Text::updateTexture(SDL_Renderer *renderer)
{
	if (!needUpdate)
		return;
	
	int curr_style = TTF_GetFontStyle(font);
	if (curr_style != style)
		TTF_SetFontStyle(font, style);
	
	int curr_outline = TTF_GetFontOutline(font);
	if (curr_outline != outline)
		TTF_SetFontOutline(font, outline);
	
	int curr_hinting = TTF_GetFontHinting(font);
	if (curr_hinting != hinting)
		TTF_SetFontHinting(font, hinting);
	
	int curr_kerning = TTF_GetFontKerning(font);
	bool curr_kern = (curr_kerning != 0);
	if (curr_kern != kerning)
		TTF_SetFontKerning(font, kerning);
	
	SDL_Surface *surf = TTF_RenderUTF8_Blended(font, str.c_str(), color);
	if (surf)
	{
		SDL_Texture *new_tex = SDL_CreateTextureFromSurface(renderer, surf);
		if (new_tex)
		{
			SDL_DestroyTexture(texture);
			texture = new_tex;
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		}
		SDL_FreeSurface(surf);
	}
	
	needUpdate = false;
}

void Text::render(SDL_Renderer *renderer, int x, int y)
{
	if (needUpdate)
		updateTexture(renderer);
	if (tex)
	{
		SDL_Rect destRect = { x, y, 30, 30 }; // TODO: for now
		SDL_RenderCopy(renderer, tex, NULL, &destRect);
	}
}