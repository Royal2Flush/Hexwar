#include "Text.hpp"

CText::CText()
{
	// normal constructor for creating new CText object. Sets member attributes to default values.
	
	m_bInitialized = false;

	m_sText = "";
	m_sFontName = "";
	m_Color = { 0, 0, 0 };
	m_Alpha = 0;
	m_ptSize = 1;
	m_destRect = { 0, 0, 1, 1 };

	m_pScreen = g_pFramework->GetScreen();
	m_pTexture = NULL;
	m_pFont = NULL;
} //Constructor

CText::CText(const CText &other)
{
	/* copy constructor for CText.
	 * Some member attributes of CText are pointers and therefore the object only gets a shallow copy. 
	 * If the destructor of the original CText gets called (see ~CText()) some of the pointers will point to nothing. 
	 * Therefore we need to manually make deep copies of the objects pointed to.*/
	 
	m_bInitialized = other.m_bInitialized;
	m_pScreen = other.m_pScreen;

	if (m_bInitialized)
	{
		m_sText = other.m_sText;
		m_sFontName = other.m_sFontName;
		m_Color = other.m_Color;
		m_Alpha = other.m_Alpha;
		m_ptSize = other.m_ptSize;
		m_destRect = other.m_destRect;

		m_pFont = TTF_OpenFont(m_sFontName.c_str(), m_ptSize);
		if (m_pFont == NULL)
		{
			cout << "Error loading font" << m_sFontName << endl;
			g_pFramework->Quit();
			return;
		}

		SDL_Surface *surf = TTF_RenderText_Blended(m_pFont, m_sText.c_str(), m_Color);
		if (surf == nullptr)
		{
			TTF_CloseFont(m_pFont);
			m_pFont = NULL;
			cout << "Error printing following Text: " << m_sText << endl;
			return;
		}

		m_pTexture = SDL_CreateTextureFromSurface(m_pScreen, surf);
		SDL_FreeSurface(surf);
		if (m_pTexture == nullptr)
		{
			cout << "Error texturizing following Text: " << m_sText << endl;
			return;
		}

		SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(m_pTexture, m_Alpha);
	}
	else
	{
		cout << "Warning: Copying uninitialized Text" << endl;
	}
} //Copy Constructor

CText &CText::operator=(const CText &other)
{
	/* We need to define our own assignment operator.
	 * Some member attributes of CText are pointers and therefore the object only gets a shallow copy. 
	 * If the destructor of the original CText gets called (see ~CText()) some of the pointers will point to nothing. 
	 * Therefore we need to manually make deep copies of the objects pointed to.*/
	 
	if (this == &other)
		return *this;
	else
	{
		m_bInitialized = other.m_bInitialized;
		m_pScreen = other.m_pScreen;

		if (m_bInitialized)
		{
			m_sText = other.m_sText;
			m_sFontName = other.m_sFontName;
			m_Color = other.m_Color;
			m_Alpha = other.m_Alpha;
			m_ptSize = other.m_ptSize;
			m_destRect = other.m_destRect;

			if (m_pTexture != NULL)
			{
				SDL_DestroyTexture(m_pTexture);
			}
			if (m_pFont != NULL)
			{
				TTF_CloseFont(m_pFont);
				m_pFont = NULL;
			}

			m_pFont = TTF_OpenFont(m_sFontName.c_str(), m_ptSize);
			if (m_pFont == NULL)
			{
				cout << "Error loading font" << m_sFontName << endl;
				g_pFramework->Quit();
				return *this;
			}

			SDL_Surface *surf = TTF_RenderText_Blended(m_pFont, m_sText.c_str(), m_Color);
			if (surf == nullptr)
			{
				TTF_CloseFont(m_pFont);
				m_pFont = NULL;
				cout << "Error printing following Text: " << m_sText << endl;
				return *this;
			}

			m_pTexture = SDL_CreateTextureFromSurface(m_pScreen, surf);
			SDL_FreeSurface(surf);
			if (m_pTexture == nullptr)
			{
				cout << "Error texturizing following Text: " << m_sText << endl;
				return *this;
			}

			SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(m_pTexture, m_Alpha);
		}
		else
		{
			cout << "Warning: Copying uninitialized Text" << endl;
		}

		return *this;
	}
} //CText::operator=

CText::~CText()
{
	// custom destructor to prevent memory leaks from texture and font (note that framework is not destroyed)
	
	if (m_pTexture != NULL)
	{
		SDL_DestroyTexture(m_pTexture);
	}
	if (m_pFont != NULL && TTF_WasInit())
	{
		TTF_CloseFont(m_pFont);
		m_pFont = NULL;
	}
} //Destructor

bool CText::Init(string font, int ptSize, string text, unsigned char r, unsigned char g, unsigned char b, int alpha)
{
	// Initializes the text at a certain position in a certain color
	
	return Init(font, ptSize, text, { r, g, b }, alpha);
}

bool CText::Init(string fontname, int ptSize, string text, SDL_Color color, int alpha)
{
	// Initializes the text at a certain position in a certain color
	
	m_bInitialized = true;

	m_sText = text;
	m_sFontName = fontname;
	m_ptSize = ptSize;
	m_Color = color;
	m_Alpha = alpha;

	m_pFont = TTF_OpenFont(m_sFontName.c_str(), m_ptSize);
	if (m_pFont == NULL)
	{
		cout << "Error loading font" << m_sFontName << endl;
		g_pFramework->Quit();
		return false;
	}

	SDL_DestroyTexture(m_pTexture);

	SDL_Surface *surf = TTF_RenderText_Blended(m_pFont, m_sText.c_str(), m_Color);
	if (surf == nullptr)
	{
		TTF_CloseFont(m_pFont);
		m_pFont = NULL;
		cout << "Error printing following Text: " << m_sText << endl;
		return false;
	}
	m_destRect.w = surf->w;
	m_destRect.h = surf->h;
	m_destRect.x = 0;
	m_destRect.y = 0;

	m_pTexture = SDL_CreateTextureFromSurface(m_pScreen, surf);
	SDL_FreeSurface(surf);
	if (m_pTexture == nullptr)
	{
		cout << "Error texturizing following Text: " << m_sText << endl;
		return false;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_pTexture, m_Alpha);

	return true;
} //CText::Init

void CText::Render()
{
	SDL_RenderCopy(m_pScreen, m_pTexture, NULL, &m_destRect);
}

void CText::setText(string text)
{
	// sets a new text (we must create a new surface for this)
	
	m_sText = text;

	if (m_pTexture != NULL)
	{
		SDL_DestroyTexture(m_pTexture);
	}

	SDL_Surface *surf = TTF_RenderText_Blended(m_pFont, m_sText.c_str(), m_Color);
	if (surf == nullptr)
	{
		cout << "Error printing following Text: " << text << endl;
		return;
	}
	m_destRect.w = surf->w;
	m_destRect.h = surf->h;

	m_pTexture = SDL_CreateTextureFromSurface(m_pScreen, surf);
	SDL_FreeSurface(surf);
	if (m_pTexture == nullptr)
	{
		cout << "Error texturizing following Text: " << text << endl;
		return;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_pTexture, m_Alpha);
} //CText::setText

void CText::setSize(int ptSize)
{
	// sets a new size for the text (we must create a new surface for this)
	
	m_ptSize = ptSize;

	SDL_DestroyTexture(m_pTexture);

	SDL_Surface *surf = TTF_RenderText_Blended(m_pFont, m_sText.c_str(), m_Color);
	if (surf == nullptr)
	{
		cout << "Error printing Resize" << endl;
		return;
	}
	m_destRect.w = surf->w;
	m_destRect.h = surf->h;

	m_pTexture = SDL_CreateTextureFromSurface(m_pScreen, surf);
	SDL_FreeSurface(surf);
	if (m_pTexture == nullptr)
	{
		cout << "Error texturizing Resize" << endl;
		return;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_pTexture, m_Alpha);
} //CText::setSize

void CText::setColor(unsigned char r, unsigned char g, unsigned char b, int alpha)
{
	// sets a new color for the text (we must create a new surface for this)
	
	setColor({ r, g, b }, alpha);
}

void CText::setColor(SDL_Color color, int alpha)
{
	// sets a new color for the text (we must create a new surface for this)
	
	m_Color = color;
	m_Alpha = alpha;

	SDL_DestroyTexture(m_pTexture);

	SDL_Surface *surf = TTF_RenderText_Blended(m_pFont, m_sText.c_str(), m_Color);
	if (surf == nullptr)
	{
		cout << "Error changing color" << endl;
		return;
	}
	m_destRect.w = surf->w;
	m_destRect.h = surf->h;

	m_pTexture = SDL_CreateTextureFromSurface(m_pScreen, surf);
	SDL_FreeSurface(surf);
	if (m_pTexture == nullptr)
	{
		cout << "Error texturizing changed color" << endl;
		return;
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_pTexture, m_Alpha);
} //CText::setColor

void CText::setPos(int x, int y)
{
	m_destRect.x = x;
	m_destRect.y = y;
}

void CText::setMiddle(int x, int y)
{
	m_destRect.x = x - m_destRect.w / 2;
	m_destRect.y = y - m_destRect.h / 2;
}

SDL_Rect CText::getRect()
{
	return m_destRect;
}
