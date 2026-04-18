#include <Arduino.h>
#include <LovyanGFX.hpp>

enum OLEDDISPLAY_TEXT_ALIGNMENT {
  TEXT_ALIGN_LEFT = 0,
  TEXT_ALIGN_RIGHT = 1,
  TEXT_ALIGN_CENTER = 2,
  TEXT_ALIGN_CENTER_BOTH = 3
};

enum OLEDDISPLAY_COLOR {
  BLACK = 0,
  WHITE = 1,
  INVERSE = 2,
  RED = 3,
  GREEN = 4,
  BLUE = 5
};

#define DialogInput_plain_12 0
#define DialogInput_bold_12 1
#define DialogInput_plain_15 2
#define DialogInput_bold_15 3

class DisplayWrapper {
	public:
		DisplayWrapper();
		bool getTouch(uint16_t *x, uint16_t *y);
		void display();
		void displayOff();
		void drawHorizontalLine();
		uint8_t getStringWidth(const String& usrStr);
		uint8_t getStringHeight(const String& usrStr);
		void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);
		void fillRect(int16_t x, int16_t y, int16_t width, int16_t height);
		void drawHorizontalLine(int16_t x, int16_t y, int16_t length);
		void drawVerticalLine(int16_t x, int16_t y, int16_t length);
		void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *xbm);
		uint16_t drawString(int16_t x, int16_t y, const String &text);
		void flipScreenVertically();
		bool init();
		void setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment);
		void setFont(const int index);
		void setFont(const GFXfont *fontData);
		void setFontIndex(int index);
		void setColor(OLEDDISPLAY_COLOR color);
		void setTheme(uint16_t , uint16_t);
		void clear(void);
		void setBrightness(uint8_t);
		uint16_t getWidth(void);
		uint16_t getHeight(void);
		static lgfx::LGFX_Device* getLGFX();

		// ---- Game display mode ----
		//
		// Games need full-screen rendering with an off-screen sprite
		// for flicker-free updates. On the M32Pocket (ESP32-S3), the
		// TFT SPI bus shares pins 38/39 with the rotary encoder.
		// These methods manage the pin conflict:
		//
		// enterGameMode():  allocates a full-screen sprite (once),
		//                   configures the TFT, returns the sprite.
		// pushGameFrame():  temporarily claims SPI pins, pushes the
		//                   sprite framebuffer to the TFT, then
		//                   restores the encoder pins.
		// exitGameMode():   restores the normal display for menu use.
		//
		// The sprite is allocated once and persists across game
		// sessions (no memory fragmentation). Both Morse Invaders
		// and Fight the Pileup share the same sprite.

LGFX_Sprite* enterGameMode(bool leftHanded = false);
		LGFX_Sprite* enterGameModeLandscape(bool leftHanded = false);
		void pushGameFrame();
		void exitGameMode();
		bool isInGameMode() const;
		LGFX_Sprite* getGameSprite();

	private:
		int colorConvert(OLEDDISPLAY_COLOR color);
		int color = TFT_WHITE;
		const GFXfont *currentFont;
		bool useTheme;
		uint16_t fg, bg;

// Game mode state
		LGFX_Sprite* _gameSprite = nullptr;             // portrait sprite (Invaders, Pileup)
		LGFX_Sprite* _gameSpriteLandscape = nullptr;    // landscape sprite (Radio Cave)
		bool _gameMode = false;
		bool _gameModeIsLandscape = false;              // which sprite pushGameFrame should use
};
