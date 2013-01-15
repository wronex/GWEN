#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/Renderers/Allegro.h"

//#include <allegro5/allegro_image.h>
//#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

namespace Gwen 
{
	namespace Renderer 
	{

		Allegro::Allegro()
		{

		}

		Allegro::~Allegro()
		{

		}

		void Allegro::SetDrawColor( Gwen::Color color )
		{
			m_Color = al_map_rgba(color.r, color.g, color.b, color.a);
		}

		void Allegro::DrawFilledRect( Gwen::Rect rect )
		{
			Translate( rect );

			al_draw_filled_rectangle( rect.x,rect.y, rect.x+rect.w, rect.y+rect.h, m_Color );
		}

		void Allegro::LoadFont( Gwen::Font* font )
		{
			font->realsize = font->size * Scale();

			std::string fontName = Utility::UnicodeToString( font->facename );
			if ( fontName.find(".ttf" ) == std::string::npos ) 
			{
				fontName += ".ttf";
			}

			ALLEGRO_FONT *afont = al_load_font( fontName.c_str(), font->realsize, ALLEGRO_TTF_NO_KERNING );

			if (!afont) {
				// Failed to load the font
				font->data = NULL;
				return;
			}

#ifdef GWEN_ALLEGRO_DISABLE_UNICODE
				font->data = afont;
#else
				// Calculate the padding required for the HACK in MeasureText().
				// Not caching the value here would force us to perform two
				// measures in MeasureText().
				int bx, by, tw, th;
				al_get_text_dimensions( afont, ".", &bx, &by, &tw, &th );

				AllegroFontData *pData = new AllegroFontData;
				pData->alFont = afont;
				pData->paddingWidth = tw;

				font->data = pData;
#endif
		}

		void Allegro::FreeFont( Gwen::Font* pFont )
		{
			if ( !pFont->data )
				return;

#ifdef GWEN_ALLEGRO_DISABLE_UNICODE
				al_destroy_font( (ALLEGRO_FONT*)pFont->data );
#else
				AllegroFontData *pData = static_cast<AllegroFontData*>(pFont->data);
				al_destroy_font(pData->alFont);
				delete pData;
#endif

			pFont->data = NULL;
		}

		void Allegro::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text )
		{
			Translate( pos.x, pos.y );

#ifdef GWEN_ALLEGRO_DISABLE_UNICODE
				ALLEGRO_FONT *afont = (ALLEGRO_FONT*) pFont->data;

				al_draw_text( afont, m_Color, pos.x,pos.y, ALLEGRO_ALIGN_LEFT,
					Utility::UnicodeToString( text ).c_str());
#else
				AllegroFontData *pData = static_cast<AllegroFontData*>(pFont->data);

				AllegroUnicodeString altext( text );
				al_draw_ustr( pData->alFont, m_Color, pos.x,pos.y, ALLEGRO_ALIGN_LEFT, altext.get() );
#endif
		}

		Gwen::Point Allegro::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text )
		{

			// If the font doesn't exist, or the font size should be changed
			if ( !pFont->data || pFont->realsize != pFont->size * Scale() )
			{
				FreeFont( pFont );
				LoadFont( pFont );
			}

			if ( !pFont->data ) return Gwen::Point( 0, 0 );

			int bx, by, tw, th;

#ifdef GWEN_ALLEGRO_DISABLE_UNICODE
				// Measure without unicode support.
				ALLEGRO_FONT *afont = (ALLEGRO_FONT*) pFont->data;
				al_get_text_dimensions( afont, Utility::UnicodeToString( text ).c_str(),
					&bx, &by, &tw, &th );
#else
				// HACK: For some reason a string ending with a space (L' ') is
				// measured incorrecty. To fix this, pad the string with a
				// character to make sure it does not end with a space.
				AllegroFontData *pData = static_cast<AllegroFontData*>(pFont->data);

				AllegroUnicodeString altext( text + L"." );
				al_get_ustr_dimensions( pData->alFont, altext.get(), &bx, &by, &tw, &th );

				// Compensate for the padding.
				tw -= pData->paddingWidth;
#endif

			return Gwen::Point( tw, th );
		}

		void Allegro::StartClip()
		{
			Gwen::Rect rect = ClipRegion();
			al_set_clipping_rectangle( rect.x, rect.y, rect.w, rect.h );   
		};


		void Allegro::EndClip()
		{
			ALLEGRO_BITMAP *targ = al_get_target_bitmap();
			al_set_clipping_rectangle( 0, 0, al_get_bitmap_width(targ), al_get_bitmap_height(targ) ); 
		};

		void Allegro::LoadTexture( Gwen::Texture* pTexture )
		{
			if ( !pTexture ) return;
			if ( pTexture->data ) FreeTexture( pTexture );

			ALLEGRO_BITMAP *bmp = al_load_bitmap( pTexture->name.Get().c_str() );
			if ( bmp ) 
			{
				pTexture->data = bmp;
				pTexture->width = al_get_bitmap_width( bmp );
				pTexture->height = al_get_bitmap_height( bmp );
				pTexture->failed = false;
			}
			else 
			{
				pTexture->data = NULL;
				pTexture->failed = true;
			}
		};

		void Allegro::FreeTexture( Gwen::Texture* pTexture )
		{
			al_destroy_bitmap( (ALLEGRO_BITMAP*)pTexture->data );
			pTexture->data = NULL;
		}

		void Allegro::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
		{
			ALLEGRO_BITMAP *bmp = (ALLEGRO_BITMAP*) pTexture->data;

			if ( !bmp ) return DrawMissingImage( rect );

			Translate( rect );

			const unsigned int w = pTexture->width;
			const unsigned int h = pTexture->height;

			al_draw_scaled_bitmap( bmp,
				u1*w,v1*h, (u2-u1)*w,(v2-v1)*h,   // source
				rect.x,rect.y, rect.w,rect.h,     // destination
				0);
		}

		Gwen::Color Allegro::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default )
		{
			ALLEGRO_BITMAP *bmp = (ALLEGRO_BITMAP*) pTexture->data;
			if ( !bmp )
				return col_default;

			ALLEGRO_COLOR col = al_get_pixel(bmp, x,y);

			Gwen::Color gcol;
			al_unmap_rgba(col, &gcol.r, &gcol.g, &gcol.b, &gcol.a);

			return gcol;
		}

		void Allegro::DrawLinedRect( Gwen::Rect rect )
		{
			al_draw_rectangle(rect.x+0.5f,rect.y+0.5f, rect.x+0.5f+rect.w,rect.y+0.5f+rect.h, m_Color, 1.f);
		}

		void Allegro::DrawPixel( int x, int y )
		{
			al_put_pixel(x+0.5f, y+0.5f, m_Color);
		}


		////////////////////////////////////////////////////////////////////////

#ifndef GWEN_ALLEGRO_DISABLE_UNICODE

		AllegroUnicodeString::AllegroUnicodeString(
			const Gwen::UnicodeString& text )
		{
			// Converts the supplied UnicodeString to an Allegro USTR. Is there
			// a better way?
			pUstr = al_ustr_new( "" );
			for (int i = 0; i < text.size(); i++) {
				wchar_t wc = text[i];
				al_ustr_append_chr( pUstr, wc );
			}
		}

		AllegroUnicodeString::~AllegroUnicodeString() {
			al_ustr_free( pUstr );
		}

#endif
	}
}
