#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "DisplayWrapper.h"

#if defined(ST7789_DRIVER) || defined(ILI9341_DRIVER)
#ifdef ST7789_DRIVER
#include "ST7789.h"
#elif ILI9341_DRIVER
#include "ILI9341.h"
#endif
#include "DialogInput_plain_24.h"
#include "DialogInput_bold_24.h"
#include "DialogInput_plain_30.h"
#include "DialogInput_bold_30.h"
#include "IntelOneMono_Regular12pt8b.h"
#include "IntelOneMono_Regular15pt8b.h"
#include "IntelOneMono_Bold12pt8b.h"
#include "IntelOneMono_Bold15pt8b.h"
static LGFX lcd;
#else
#include "SSD1306.h"
#define SSD1306_DRIVER
#include "DialogInput_plain_12.h"
#include "DialogInput_bold_12.h"
#include "DialogInput_plain_15.h"
#include "DialogInput_bold_15.h"
static SSD1306 lcd;
#endif


DisplayWrapper::DisplayWrapper() {
	currentFont = NULL;
	useTheme = false;
}

uint16_t DisplayWrapper::getWidth(void) {
	return lcd.width();
}

uint16_t DisplayWrapper::getHeight(void) {
	return lcd.height();
}

bool DisplayWrapper::getTouch(uint16_t *x, uint16_t *y) {
	return lcd.getTouch(x,y);
}
void DisplayWrapper::display(){}

void DisplayWrapper::displayOff(){
  lcd.setBrightness(0);
  lcd.sleep();
}

uint8_t DisplayWrapper::getStringWidth(const String& strUser){
	return lcd.textWidth(strUser);
}

uint8_t DisplayWrapper::getStringHeight(const String& strUser){
	return lcd.fontHeight(currentFont);
}

void DisplayWrapper::drawRect(int16_t x, int16_t y, int16_t width, int16_t height){
	lcd.drawRect(x,y,width,height);
}

void DisplayWrapper::fillRect(int16_t x, int16_t y, int16_t width, int16_t height){
	lcd.fillRect(x,y,width,height);
}
void DisplayWrapper::drawHorizontalLine(int16_t x, int16_t y, int16_t length){
	lcd.drawFastHLine(x,y,length);
}

void DisplayWrapper::drawVerticalLine(int16_t x, int16_t y, int16_t length){
	lcd.drawFastVLine(x,y,length);
}

void DisplayWrapper::drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const uint8_t *xbm){
	lcd.drawXBitmap(x, y, xbm, width, height, useTheme ? fg : TFT_WHITE, useTheme ? bg : TFT_BLACK);
}

/* uint16_t DisplayWrapper::drawString(int16_t x, int16_t y, const String &text ){
	lcd.setCursor(x,y);
	return lcd.print(text);
} */

uint16_t DisplayWrapper::drawString(int16_t x, int16_t y, const String &text) {
    lcd.setTextDatum(lgfx::top_left);
    lcd.drawString(text.c_str(), x, y);
    return lcd.textWidth(text.c_str());
}

void DisplayWrapper::flipScreenVertically(){
	lcd.setRotation(1);
}

bool DisplayWrapper::init(){
#ifdef PIN_VTFT_CTRL
  pinMode(PIN_VTFT_CTRL, OUTPUT);
  digitalWrite(PIN_VTFT_CTRL, LOW);
#endif
	lcd.begin();
	// lcd.startWrite();// As an additional note, when startWrite is used, LovyanGFX occupies the SPI bus and operates.
#ifdef SSD1306_DRIVER
	lcd.setColorDepth(lgfx::palette_1bit);
#endif
	lcd.setRotation(3);
	// lcd.setRotation(2); - ssd1306 m32 normal layout
	//lcd.setFont(&DialogInput_plain_12);
	//lcd.setFont(&fonts::AsciiFont8x16);
	return true;
}

lgfx::LGFX_Device* DisplayWrapper::getLGFX() {
    return &lcd;
}

void DisplayWrapper::setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT textAlignment){}

void DisplayWrapper::setFont(int index){
	setFontIndex(index);
}

void DisplayWrapper::setFont(const GFXfont *fontData){
	lcd.setFont(fontData);
}

void DisplayWrapper::setFontIndex(int index){
	switch (index) {
#ifdef SSD1306_DRIVER
		case 1:
			lcd.setFont(&GFXDialogInput_bold_12);
			currentFont=&GFXDialogInput_bold_12;
			break;
		case 2:
			lcd.setFont(&GFXDialogInput_plain_15);
			currentFont=&GFXDialogInput_plain_15;
			break;
		case 3:
			lcd.setFont(&GFXDialogInput_bold_15);
			currentFont=&GFXDialogInput_bold_15;
			break;
		default:
			lcd.setFont(&GFXDialogInput_plain_12);
			currentFont=&GFXDialogInput_plain_12;
			break;
#else
		case 1:
                        lcd.setFont(&IntelOneMono_Bold12pt8b);
                        currentFont=&IntelOneMono_Bold12pt8b;
                        break;
                case 2:
                        lcd.setFont(&IntelOneMono_Regular15pt8b);
                        currentFont=&IntelOneMono_Regular15pt8b;
                        break;
                case 3:
                        lcd.setFont(&IntelOneMono_Bold15pt8b);
                        currentFont=&IntelOneMono_Bold15pt8b;
                        break;
                default:
                        lcd.setFont(&IntelOneMono_Regular12pt8b);
                        currentFont=&IntelOneMono_Regular12pt8b;
                        break;
#endif
	}
}

void DisplayWrapper::clear(void){
	if (!useTheme)
		lcd.fillScreen(TFT_BLACK);
	else
		lcd.fillScreen(bg);
}

void DisplayWrapper::setBrightness(uint8_t brightness){
	lcd.setBrightness(brightness);
}

void DisplayWrapper::setColor(OLEDDISPLAY_COLOR color) {
	if (!useTheme) {
		this->color = colorConvert(color);
		if (color == WHITE) {
			lcd.setColor(TFT_WHITE);
			lcd.setTextColor(TFT_WHITE, TFT_BLACK);
		}
		else {
			lcd.setTextColor(TFT_BLACK, TFT_WHITE);
			lcd.setColor(TFT_BLACK);
		}
	}
	else {
		if (color == WHITE) {
			lcd.setColor(fg);
			lcd.setTextColor(fg, bg);
		}
		else {
			lcd.setTextColor(bg, fg);
			lcd.setColor(bg);
		}
	}
}


int DisplayWrapper::colorConvert(OLEDDISPLAY_COLOR color) {
	switch (color) {
		WHITE:	
			return TFT_WHITE;
		RED:	
			return TFT_RED;
		GREEN:	
			return TFT_GREEN;
		BLUE:	
			return TFT_BLUE;
		default:
      return TFT_BLACK;
	}
}

void DisplayWrapper::setTheme(uint16_t f, uint16_t b) {
if (f==b) {
    useTheme = false;
}
else {
    useTheme = true;
    fg = f; bg = b;
  }
}


//=============================================================================
// Game Display Mode
//=============================================================================
//
// On the M32Pocket (ESP32-S3 with ST7789 TFT), the SPI bus uses
// pins TFT_SCLK (38) and TFT_CS (39), which are also used by the
// rotary encoder. This creates a conflict:
//
// - lcd.begin() configures pins 38/39 as SPI → encoder breaks
// - Encoder needs pins 38/39 as GPIO with interrupts → SPI breaks
//
// Solution: the game sprite draws into its own RAM framebuffer
// (no SPI needed). Only pushGameFrame() needs SPI — it briefly
// claims the bus via lcd.startWrite(), pushes the sprite, then
// releases it via lcd.endWrite(). The encoder pins are managed
// by the caller (the game code configures them after enterGameMode
// and the Morserino menu code restores them after exitGameMode).
//
// The sprite is allocated once and persists for the lifetime of
// the device. Both Morse Invaders and Fight the Pileup share it.
//=============================================================================

LGFX_Sprite* DisplayWrapper::enterGameMode(bool leftHanded) {
    _gameMode = true;
    _gameModeIsLandscape = false;

    // Configure the TFT for game use (portrait orientation)
    lcd.setRotation(leftHanded ? 0 : 2);
    lcd.fillScreen(TFT_BLACK);

    // Allocate the sprite once — reuse across all game sessions
    if (!_gameSprite) {
        _gameSprite = new LGFX_Sprite(&lcd);
        if (!_gameSprite) {
            _gameMode = false;
            return nullptr;
        }
        _gameSprite->setPsram(false);
        _gameSprite->setColorDepth(16);
        if (!_gameSprite->createSprite(lcd.width(), lcd.height())) {
            delete _gameSprite;
            _gameSprite = nullptr;
            _gameMode = false;
            return nullptr;
        }
    }
_gameSprite->fillSprite(TFT_BLACK);
    return _gameSprite;
}

LGFX_Sprite* DisplayWrapper::enterGameModeLandscape(bool leftHanded) {
    _gameMode = true;
    _gameModeIsLandscape = true;

    // Configure the TFT for game use (landscape orientation).
    // Rotation 3 = native landscape (encoder on the right, USB on the left).
    // Rotation 1 = landscape rotated 180° (for left-handed use).
    lcd.setRotation(leftHanded ? 1 : 3);
    lcd.fillScreen(TFT_BLACK);

    // Allocate the landscape sprite once — reuse across all game sessions
    if (!_gameSpriteLandscape) {
        _gameSpriteLandscape = new LGFX_Sprite(&lcd);
        if (!_gameSpriteLandscape) {
            _gameMode = false;
            return nullptr;
        }
        _gameSpriteLandscape->setPsram(false);
        _gameSpriteLandscape->setColorDepth(16);
        if (!_gameSpriteLandscape->createSprite(lcd.width(), lcd.height())) {
            delete _gameSpriteLandscape;
            _gameSpriteLandscape = nullptr;
            _gameMode = false;
            return nullptr;
        }
    }
    _gameSpriteLandscape->fillSprite(TFT_BLACK);
    return _gameSpriteLandscape;
}

void DisplayWrapper::pushGameFrame() {
    if (!_gameMode) return;

    LGFX_Sprite* spr = _gameModeIsLandscape ? _gameSpriteLandscape : _gameSprite;
    if (!spr) return;

    // LovyanGFX startWrite/endWrite manage the SPI bus.
    // startWrite claims the bus (configures SPI pins if bus_shared).
    // endWrite releases it (allows other users of the shared bus).
    lcd.startWrite();
    spr->pushSprite(&lcd, 0, 0);
    lcd.endWrite();
}

void DisplayWrapper::exitGameMode() {
    _gameMode = false;
    // Do NOT delete the sprite — keep it for next game session.
    // Restore the normal display orientation for menu use.
    lcd.setRotation(3);
}

bool DisplayWrapper::isInGameMode() const {
    return _gameMode;
}

LGFX_Sprite* DisplayWrapper::getGameSprite() {
    return _gameModeIsLandscape ? _gameSpriteLandscape : _gameSprite;
}
