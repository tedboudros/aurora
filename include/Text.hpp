#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
public:
	Text();
	~Text();
	
	void setFont(TTF_Font *font);
	void setStyle(int style);
	void setOutline(int outline_size);
	void setHinting(int hinting);
	void setKerning(bool enable);
	void setColor(SDL_Color col);
	void setText(const char *s);
	void setText(const std::string &str);
	
	TTF_Font* getFont() const			{ return font; }
	int getStyle() const				{ return style; }
	int getOutline() const				{ return outline; }
	int getHinting() const				{ return hinting; }
	int getKerning() const				{ return kerning; }
	SDL_Color getColor() const			{ return color; }
	const std::string& getText() const	{ return str; }
	const char* getTextAsChars() const	{ return str.c_str(); }
	
	void render(SDL_Renderer *renderer, int x, int y);
	
private:
	TTF_Font *font;
	int style;
	int outline;
	int hinting;
	bool kerning;
	SDL_Color color;
	std::string str;
	SDL_Texture *tex;
	bool needUpdate;
	
	void updateTexture(SDL_Renderer *renderer);
};
