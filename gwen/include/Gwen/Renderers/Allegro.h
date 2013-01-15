/*
	GWEN
	Copyright (c) 2011 Facepunch Studios
	See license in Gwen.h
*/

#pragma once

#ifndef GWEN_RENDERERS_ALLEGRO_H
#define GWEN_RENDERERS_ALLEGRO_H

#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

// Define to disable the Allegro renderer's support for Unicode strings.
//#define GWEN_ALLEGRO_DISABLE_UNICODE

namespace Gwen 
{
	namespace Renderer 
	{

		class Allegro : public Gwen::Renderer::Base
		{
			public:

				Allegro();
				~Allegro();

				virtual void SetDrawColor(Gwen::Color color);

				virtual void DrawFilledRect( Gwen::Rect rect );

				virtual void LoadFont( Gwen::Font* pFont );
				virtual void FreeFont( Gwen::Font* pFont );
				virtual void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text );
				virtual Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text );
	
				void StartClip();
				void EndClip();
			
				void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1=0.0f, float v1=0.0f, float u2=1.0f, float v2=1.0f );
				void LoadTexture( Gwen::Texture* pTexture );
				void FreeTexture( Gwen::Texture* pTexture );
				Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default );

				void DrawLinedRect( Gwen::Rect rect );
				void DrawPixel( int x, int y );

			protected:

				ALLEGRO_COLOR   m_Color;

		};

#ifndef GWEN_ALLEGRO_DISABLE_UNICODE

		//
		// This is the struct stored in Gwen::Font->data.
		//
		struct AllegroFontData {
			ALLEGRO_FONT* alFont;
			int paddingWidth;
		};

		//
		// A manager for Allegro's UTF8 string.
		//
		class AllegroUnicodeString {
		public:
			AllegroUnicodeString(const Gwen::UnicodeString& text);
			~AllegroUnicodeString();

			inline const ALLEGRO_USTR* get() const { return pUstr; }

		private:
			ALLEGRO_USTR* pUstr;
		};

#endif // GWEN_ALLEGRO_DISABLE_UNICODE
	}
}
#endif
