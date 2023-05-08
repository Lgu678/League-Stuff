#pragma once

#include "resource.h"

//FLAGS
static const bool		B_DEBUG = false;

//Sound Files
static const wstring	W_LASER_SOUND_PATH = L"data//sounds//laser.wav";
static const wstring	W_MAGE_DASH_SOUND_PATH = L"data//sounds//mage_afterdash.wav";
static const wstring	W_MAGE_ORB_DASH_SOUND_PATH = L"data//sounds//mageorb_afterdash.wav";
static const wstring	W_ELECTRIC_AMBIENT_SOUND_PATH = L"data//sounds//electric_ambient.wav";
static const wstring	W_SPIDER_ATTACK_SOUND_PATH = L"data//sounds//spiderattack.wav";

static const string		W_DEBUG_FILE = "data/DebugOutput.txt";
static const wstring	W_INIT_FILE = L"data/OdysseySettings.cfg";
static const wstring	W_GREEN_CURSOR_PATH = L"data/img/gui/cursor/green_cursor.png";


//~~Hud Constants Start~~//
//Health Bar
static const wstring W_HEALTH_ZERO_PATH = L"data/img/gui/hud/healthbar/health_zero.png";
static const wstring W_HEALTH_TWENTY_FIVE_PATH = L"data/img/gui/hud/healthbar/health_twenty_five.png";
static const wstring W_HEALTH_FIFTY_PATH = L"data/img/gui/hud/healthbar/health_fifty.png";
static const wstring W_HEALTH_SEVENTY_FIVE_PATH = L"data/img/gui/hud/healthbar/health_seventy_five.png";
static const wstring W_HEALTH_HUNDRED_PATH = L"data/img/gui/hud/healthbar/health_hundred.png";
static const wstring W_HEALTH_BACK = L"data/img/gui/hud/healthbar/health_backdrop.png";
static const wstring W_HEALTH_BAR = L"data/img/gui/hud/healthbar/health.png";
//Lives Bar
static const wstring W_LIVES_ZERO_PATH = L"data/img/gui/hud/lifebar/green/lives_zero.png";
static const wstring W_LIVES_ONE_PATH = L"data/img/gui/hud/lifebar/green/lives_one.png";
static const wstring W_LIVES_TWO_PATH = L"data/img/gui/hud/lifebar/green/lives_two.png";
static const wstring W_LIVES_THREE_PATH = L"data/img/gui/hud/lifebar/green/lives_three.png";
//~~Hud Constants End~~//

//Music Constants
static const wstring W_LEVEL_ONE_MUSIC_PATH = L"data/music/mainsoundtrack.wav";
static const wstring W_LEVEL_TWO_MUSIC_PATH = L"data/music/mainsoundtrack.wav";


static const wstring	W_LEVEL_ONE_NORMAL = L"data/img/gui/buttons/level1/planet1.png";
static const wstring	W_LEVEL_ONE_SELECT1 = L"data/img/gui/buttons/level1/planet1_selected1.png";
static const wstring	W_LEVEL_ONE_SELECT2 = L"data/img/gui/buttons/level1/planet1_selected2.png";
static const wstring	W_LEVEL_ONE_SELECT3 = L"data/img/gui/buttons/level1/planet1_selected3.png";
static const wstring	W_LEVEL_ONE_SELECT4 = L"data/img/gui/buttons/level1/planet1_selected4.png";
static const wstring	W_LEVEL_ONE_SELECT5 = L"data/img/gui/buttons/level1/planet1_selected5.png";

static const wstring	W_LEVEL_TWO_NORMAL = L"data/img/gui/buttons/level1/planet1.png";
static const wstring	W_LEVEL_TWO_SELECT1 = L"data/img/gui/buttons/level1/planet1_selected1.png";
static const wstring	W_LEVEL_TWO_SELECT2 = L"data/img/gui/buttons/level1/planet1_selected2.png";
static const wstring	W_LEVEL_TWO_SELECT3 = L"data/img/gui/buttons/level1/planet1_selected3.png";
static const wstring	W_LEVEL_TWO_SELECT4 = L"data/img/gui/buttons/level1/planet1_selected4.png";
static const wstring	W_LEVEL_TWO_SELECT5 = L"data/img/gui/buttons/level1/planet1_selected5.png";

static const wstring	W_LEVEL_THREE_NORMAL = L"data/img/gui/buttons/level1/planet1.png";
static const wstring	W_LEVEL_THREE_SELECT1 = L"data/img/gui/buttons/level1/planet1_selected1.png";
static const wstring	W_LEVEL_THREE_SELECT2 = L"data/img/gui/buttons/level1/planet1_selected2.png";
static const wstring	W_LEVEL_THREE_SELECT3 = L"data/img/gui/buttons/level1/planet1_selected3.png";
static const wstring	W_LEVEL_THREE_SELECT4 = L"data/img/gui/buttons/level1/planet1_selected4.png";
static const wstring	W_LEVEL_THREE_SELECT5 = L"data/img/gui/buttons/level1/planet1_selected5.png";

static const wstring	W_MELEE_BOT = L"MELEE_BOT";
static const wstring	W_SKULL_BOT = L"SKULL_BOT";
static const wstring	W_SPIDER_BOT = L"SPIDER_BOT";
static const wstring	W_LAVA_BOSS = L"LAVA_BOSS";
static const wstring	W_MAGE_BOSS = L"MAGE_BOSS";
static const wstring	W_ANDROMALIUS_BOSS = L"ANDROMALIUS_BOSS";
static const wstring	W_TIGER_BOT= L"TIGER_BOT";
static const wstring	W_RAT_BOT = L"RAT_BOT";


static const wstring	W_SPLASH_SCREEN_PATH = L"data/img/gui/overlays/newsplash.png";
static const string		SAVE_PATH = "data/save/Progress.sav";
static const wstring	W_LEVEL_SELECT_SCREEN_PATH = L"data/img/gui/overlays/odyssey_space_background.png";
static const wstring	W_HELP_SCREEN_PATH = L"data/img/gui/overlays/HelpScreennew.png";
static const wstring	W_WIN_SCREEN_PATH = L"data/img/gui/overlays/MissionComplete.png";
static const wstring	W_DEATH_SCREEN_PATH = L"data/img/gui/overlays/DeathScreen.png";
static const wstring	SPRITE_TYPES_DIR = L"data/img/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";
static const wstring	W_MAIN_MENU_PATH = L"data/img/gui/overlays/newsplash.png";
static const wstring	W_PAUSE_SCREEN_PATH = L"data/img/gui/overlays/pause_screen.png";
static const wstring	W_TITLE = L"TITLE";
static const wstring	W_SCREEN_WIDTH = L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT = L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE = L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE = L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X = L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y = L"VIEWPORT_OFFSET_Y";
static const wstring	W_TOGGLE_OFFSET_Y = L"TOGGLE_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED = L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN = L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE = L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED = L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN = L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE = L"COLOR_KEY_BLUE";
static const wstring	W_CENTER_SCREEN = L"CENTER_SCREEN";

static const wstring	W_LEVEL_1_COMMAND = L"Go To Level 1";
static const wstring	W_LEVEL_1_DIR = L"data/levels/LevelOne/";
static const wstring	W_LEVEL_1_NAME = L"LevelOne.tmx";
static const string		LEVEL_1_LUA_PATH = "data/scripts/Level1.lua";

static const wstring	W_LEVEL_2_COMMAND = L"Go To Level 2";
static const wstring	W_LEVEL_2_DIR = L"data/levels/LevelTwo/";
static const wstring	W_LEVEL_2_NAME = L"LevelTwo.tmx";
static const string		LEVEL_2_LUA_PATH = "data/scripts/Level2.lua";

static const wstring	W_LEVEL_3_COMMAND = L"Go To Level 3";
static const wstring	W_LEVEL_3_DIR = L"data/levels/LevelThree/";
static const wstring	W_LEVEL_3_NAME = L"LevelThree.tmx";
static const string		LEVEL_3_LUA_PATH = "data/scripts/Level3.lua";

static const float		MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
static const wstring	IDLE(L"IDLE");
static const wstring	PRIMARY_FIRE(L"PRIMARY_FIRE");
static const wstring	SECONDARY_FIRE(L"SECONDARY_FIRE");

const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int ONE_KEY = (unsigned int)'1';
const unsigned int TWO_KEY = (unsigned int)'2';
const unsigned int THREE_KEY = (unsigned int)'3';
const unsigned int FOUR_KEY = (unsigned int)'4';
const unsigned int FIVE_KEY = (unsigned int)'5';

