#include <windows.h>
#include <d3d9.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH  460
#define WINDOW_HEIGHT 580

typedef enum {
    GAME_SELECTOR = 0,
    GAME_LOST_SAGA,
    GAME_POINT_BLANK
} MenuState;

MenuState g_CurrentState = GAME_SELECTOR;

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS   g_d3dpp;

typedef struct {
    int id;
    const char* name;
    int value;
    int max_value;
    const char** options;
    bool is_header;
} VipFeature;

const char* opt_status[]  = { "OFF", "ACTIVE", NULL };
const char* opt_mode[]    = { "OFF", "LEGIT", "RAGE", "VVIP", NULL };
const char* opt_speed[]   = { "1.0x", "1.5x", "2.0x", "MAX", NULL };
const char* opt_opac[]    = { "LOW", "MED", "HIGH", NULL };
const char* opt_preset[]  = { "LEGIT", "RAGE", "CUSTOM", NULL };
const char* opt_cross[]   = { "OFF", "DOT", "CROSS", "CIRCLE", NULL };
const char* opt_pb_mode[] = { "OFF", "BOX", "SKELETON", "GLOW", NULL };
const char* opt_pb_speed[] = { "NORMAL", "TURBO 2x", "FAST", "MAX", NULL };

// ---------------------------------------------------------------------------
// github.com/Rm19x
// ---------------------------------------------------------------------------
VipFeature g_LostSagaFeatures[100] = {
    { 1, "[ CORE MODULE & HOOKS ]", 0, 0, NULL, true },
    { 2, "Real-time DirectX 9 Swapchain Hook", 1, 1, (const char**)opt_status, false },
    { 3, "Modular ImGui Direct Pipeline", 1, 1, (const char**)opt_status, false },
    { 4, "Dynamic Window Dragging Handler", 1, 1, (const char**)opt_status, false },
    { 5, "Toggleable Menu Key (INSERT)", 1, 1, (const char**)opt_status, false },
    { 6, "Matrix UI Accent Color Theme", 1, 1, (const char**)opt_status, false },
    { 7, "Player Entity List Buffer Reader", 1, 1, (const char**)opt_status, false },
    { 8, "2D Bounding Box ESP Engine", 1, 3, (const char**)opt_mode, false },
    { 9, "Dynamic Health Bar Overlay", 1, 1, (const char**)opt_status, false },
    { 10, "Team ID Color Coding Filter", 1, 1, (const char**)opt_status, false },

    { 11, "[ LOST SAGA VVIP COMBAT ]", 0, 0, NULL, true },
    { 12, "Instant Skill Cooldown Bypass", 1, 1, (const char**)opt_status, false },
    { 13, "Super Armor State Flag Override", 1, 1, (const char**)opt_status, false },
    { 14, "Attack Speed Multiplier Hook", 2, 3, (const char**)opt_speed, false },
    { 15, "Infinite Skill Gauge Force Fill", 1, 1, (const char**)opt_status, false },
    { 16, "Gravity & Aerial Float Modifier", 1, 1, (const char**)opt_status, false },
    { 17, "One-Hit Damage Multiplier", 1, 3, (const char**)opt_mode, false },
    { 18, "Gear Slot 4-Way Address Resolver", 1, 1, (const char**)opt_status, false },
    { 19, "No Recoil & Spread Control", 1, 1, (const char**)opt_status, false },
    { 20, "Instant Weapon Zero-Delay Switch", 1, 1, (const char**)opt_status, false },

    { 21, "[ MOVEMENT & PHYSICS ]", 0, 0, NULL, true },
    { 22, "Health Regeneration Wrapper", 1, 1, (const char**)opt_status, false },
    { 23, "Knockback Resistance Enforcer", 1, 1, (const char**)opt_status, false },
    { 24, "Movement Speed Velocity Multiplier", 1, 3, (const char**)opt_speed, false },
    { 25, "Jump Height & Trajectory Modulator", 1, 1, (const char**)opt_status, false },
    { 26, "Skill Cancellation Timing Bypass", 1, 1, (const char**)opt_status, false },
    { 27, "Grab & Catch Escape Modifier", 1, 1, (const char**)opt_status, false },
    { 28, "Wake-up Recovery Frame Reducer", 1, 1, (const char**)opt_status, false },
    { 29, "Custom Animation Speed Hook", 1, 3, (const char**)opt_speed, false },
    { 30, "Ranged Projectile Speed Maximizer", 1, 1, (const char**)opt_status, false },

    { 31, "[ WORLD & VISUAL ESP ]", 0, 0, NULL, true },
    { 32, "Area of Effect (AoE) Radius Circle", 1, 1, (const char**)opt_status, false },
    { 33, "Invisible State Flag Modifier", 1, 1, (const char**)opt_status, false },
    { 34, "Map Boundary Limit Extender", 1, 1, (const char**)opt_status, false },
    { 35, "Auto-Loot Item Magnet Simulation", 1, 1, (const char**)opt_status, false },
    { 36, "Quest Objective Marker ESP", 1, 1, (const char**)opt_status, false },
    { 37, "Custom Character Skin ID Changer", 1, 1, (const char**)opt_status, false },
    { 38, "Emote Speed Manipulator", 1, 1, (const char**)opt_status, false },
    { 39, "Lobby Room Master Enforcer", 1, 1, (const char**)opt_status, false },
    { 40, "Kick Vote Immunity Flag", 1, 1, (const char**)opt_status, false },

    { 41, "[ UTILITY & AUTOMATION ]", 0, 0, NULL, true },
    { 42, "Automated Lobby Ready Clicker", 1, 1, (const char**)opt_status, false },
    { 43, "Plaza Fishing / Mining Tracker", 1, 1, (const char**)opt_status, false },
    { 44, "Custom Chat Macro Spammer", 1, 1, (const char**)opt_status, false },
    { 45, "Whisper Message Logger Overlay", 1, 1, (const char**)opt_status, false },
    { 46, "Friend Status Notification Popup", 1, 1, (const char**)opt_status, false },
    { 47, "Guild Member Presence Tracker", 1, 1, (const char**)opt_status, false },
    { 48, "Ladder Match Stats Inspector", 1, 1, (const char**)opt_status, false },
    { 49, "Borderless Windowed Enforcer", 1, 1, (const char**)opt_status, false },
    { 50, "DirectX 9 Device Reset Handler", 1, 1, (const char**)opt_status, false },

    { 51, "[ SYSTEM & SAFETY ]", 0, 0, NULL, true },
    { 52, "Panic Key (Instant Hide)", 1, 1, (const char**)opt_status, false },
    { 53, "Overlay Opacity Slider Control", 1, 2, (const char**)opt_opac, false },
    { 54, "Matrix Theme Customizer", 1, 1, (const char**)opt_status, false },
    { 55, "Font Type & Size Selector", 1, 1, (const char**)opt_status, false },
    { 56, "Hotkey Binding Manager", 1, 1, (const char**)opt_status, false },
    { 57, "Configuration Auto-Saver (.ini)", 1, 1, (const char**)opt_status, false },
    { 58, "Preset Profile Switcher", 1, 2, (const char**)opt_preset, false },
    { 59, "Crash Exception Handler (SEH)", 1, 1, (const char**)opt_status, false },
    { 60, "Automated Error Log Dumper", 1, 1, (const char**)opt_status, false },

    { 61, "[ MEMORY & KERNEL ENGINE ]", 0, 0, NULL, true },
    { 62, "Local Player Base Pointer Resolver", 1, 1, (const char**)opt_status, false },
    { 63, "Entity List Memory Iterator", 1, 1, (const char**)opt_status, false },
    { 64, "Pattern Scanning Engine", 1, 1, (const char**)opt_status, false },
    { 65, "Dynamic Pointer Chain Calculator", 1, 1, (const char**)opt_status, false },
    { 66, "VirtualProtect Memory Wrapper", 1, 1, (const char**)opt_status, false },
    { 67, "Thread-Safe Critical Section Lock", 1, 1, (const char**)opt_status, false },
    { 68, "High-Performance Ring Buffer", 1, 1, (const char**)opt_status, false },
    { 69, "Console Debug Output Redirector", 1, 1, (const char**)opt_status, false },
    { 70, "Network Packet Sniffer Hook", 1, 1, (const char**)opt_status, false },

    { 71, "[ ADVANCED PACKET HOOKS ]", 0, 0, NULL, true },
    { 72, "Packet Latency Simulation Wrapper", 1, 1, (const char**)opt_status, false },
    { 73, "Friction & Surface Resistance Flag", 1, 1, (const char**)opt_status, false },
    { 74, "Automated Replay Buffer Trigger", 1, 1, (const char**)opt_status, false },
    { 75, "Clean Background Screenshot Taker", 1, 1, (const char**)opt_status, false },
    { 76, "Process Watcher Integrity Guard", 1, 1, (const char**)opt_status, false },
    { 77, "GitHub Auto-Update Checker API", 1, 1, (const char**)opt_status, false },
    { 78, "Player Entity Memory Reader", 1, 1, (const char**)opt_status, false },
    { 79, "Snapline Center Target Drawer", 1, 1, (const char**)opt_status, false },
    { 80, "Player Distance Meter Display", 1, 1, (const char**)opt_status, false },

    { 81, "[ RADAR & TRACKING ]", 0, 0, NULL, true },
    { 82, "Enemy Mercenary Portrait Display", 1, 1, (const char**)opt_status, false },
    { 83, "Minimap Radar Blip Extractor", 1, 1, (const char**)opt_status, false },
    { 84, "Map Boundary Edge Warning", 1, 1, (const char**)opt_status, false },
    { 85, "Cloaked Player Detector", 1, 1, (const char**)opt_status, false },
    { 86, "Spectator Count Monitor", 1, 1, (const char**)opt_status, false },
    { 87, "Custom Crosshair Overlay", 1, 3, (const char**)opt_cross, false },
    { 88, "Dynamic Crosshair Spread Indicator", 1, 1, (const char**)opt_status, false },
    { 89, "Screen Gamma Brightness Enhancer", 1, 1, (const char**)opt_status, false },
    { 90, "Fog Distance Visibility Modifier", 1, 1, (const char**)opt_status, false },

    { 91, "[ GRAPHICS & RENDERING ]", 0, 0, NULL, true },
    { 92, "Skybox Lighting Modulator", 1, 1, (const char**)opt_status, false },
    { 93, "Particle Density Filter (High FPS)", 1, 1, (const char**)opt_status, false },
    { 94, "Floating Damage Number Popups", 1, 1, (const char**)opt_status, false },
    { 95, "Hitmarker Sound Effect Trigger", 1, 1, (const char**)opt_status, false },
    { 96, "Critical Damage Screen Flash", 1, 1, (const char**)opt_status, false },
    { 97, "Watermark ('pLMEMORIES LS v1.0')", 1, 1, (const char**)opt_status, false },
    { 98, "Session Match Duration Timer", 1, 1, (const char**)opt_status, false },
    { 99, "Real-time FPS Counter Graph", 1, 1, (const char**)opt_status, false },
    { 100, "Developer: github.com/Rm19x", 1, 1, (const char**)opt_status, false }
};


VipFeature g_PointBlankFeatures[100] = {
    { 1, "[ WALLHACK & VISUAL ESP ]", 0, 0, NULL, true },
    { 2, "Player Bone Skeleton ESP 3D", 2, 3, (const char**)opt_pb_mode, false },
    { 3, "2D Box Bounding ESP (Red/Blue)", 1, 3, (const char**)opt_pb_mode, false },
    { 4, "Name Tag & Rank Badge ESP", 1, 1, (const char**)opt_status, false },
    { 5, "Health Bar & Status Percentage", 1, 1, (const char**)opt_status, false },
    { 6, "Distance Meter ESP (Meter)", 1, 1, (const char**)opt_status, false },
    { 7, "Head Dot Indicator (Hitbox)", 1, 1, (const char**)opt_status, false },
    { 8, "Chams / Wallhack Material Color", 1, 3, (const char**)opt_pb_mode, false },
    { 9, "C4 Plant & Defuse Status ESP", 1, 1, (const char**)opt_status, false },
    { 10, "Dropped Weapon & Item ESP", 1, 1, (const char**)opt_status, false },

    { 11, "[ AIMBOT & ASSIST SYSTEM ]", 0, 0, NULL, true },
    { 12, "Legitimate Mouse Aimbot Hook", 1, 1, (const char**)opt_status, false },
    { 13, "Aimbot Hotkey Selector (R-Click)", 1, 1, (const char**)opt_status, false },
    { 14, "Auto Bone Target (Head/Chest)", 1, 1, (const char**)opt_status, false },
    { 15, "Aim Prediction (Moving Target)", 1, 1, (const char**)opt_status, false },
    { 16, "FOV (Field of View) Circle Draw", 1, 1, (const char**)opt_status, false },
    { 17, "Smoothness Speed Modifier", 2, 3, (const char**)opt_pb_speed, false },
    { 18, "No Recoil & Zero Spread Force", 1, 1, (const char**)opt_status, false },
    { 19, "Auto Triggerbot (Shoot on Sight)", 1, 1, (const char**)opt_status, false },
    { 20, "Visibility Check Bypass", 1, 1, (const char**)opt_status, false },

    { 21, "[ WEAPON & DAMAGE HACKS ]", 0, 0, NULL, true },
    { 22, "Fast Reload Speed Multiplier", 1, 3, (const char**)opt_pb_speed, false },
    { 23, "Quick Weapon Switch (Knife/Gun)", 1, 1, (const char**)opt_status, false },
    { 24, "Infinite Ammo / No Reload Flag", 1, 1, (const char**)opt_status, false },
    { 25, "Rapid Fire (Fire Rate Enhancer)", 1, 3, (const char**)opt_pb_speed, false },
    { 26, "Damage Multiplier (One-Hit Body)", 1, 3, (const char**)opt_pb_mode, false },
    { 27, "No Bullet Drop (Sniper Support)", 1, 1, (const char**)opt_status, false },
    { 28, "Shotgun Pellet Spread Fixer", 1, 1, (const char**)opt_status, false },
    { 29, "Grenade Trajectory Line Predictor", 1, 1, (const char**)opt_status, false },
    { 30, "Dual Weapon Dual-Wield Enforcer", 1, 1, (const char**)opt_status, false },

    { 31, "[ PLAYER MOVEMENT HACKS ]", 0, 0, NULL, true },
    { 32, "Speedhack Velocity Multiplier", 1, 3, (const char**)opt_pb_speed, false },
    { 33, "High Jump / Gravity Reducer", 1, 1, (const char**)opt_status, false },
    { 34, "No Fall Damage Enforcer", 1, 1, (const char**)opt_status, false },
    { 35, "Super Crouch Speed Modifier", 1, 1, (const char**)opt_status, false },
    { 36, "Quick Respawn Time Bypass", 1, 1, (const char**)opt_status, false },
    { 37, "Ghost Mode / Wall Walk Float", 1, 1, (const char**)opt_status, false },
    { 38, "Infinite Stamina / Agility", 1, 1, (const char**)opt_status, false },
    { 39, "Plant/Defuse Anywhere Range", 1, 1, (const char**)opt_status, false },
    { 40, "Air Control Momentum Booster", 1, 1, (const char**)opt_status, false },

    { 41, "[ MISC & ROOM MODS ]", 0, 0, NULL, true },
    { 42, "Auto-Ready in Lobby Match", 1, 1, (const char**)opt_status, false },
    { 43, "Kick Vote Immunity Protection", 1, 1, (const char**)opt_status, false },
    { 44, "Anti-AFK Connection Maintainer", 1, 1, (const char**)opt_status, false },
    { 45, "Room Master Privilege Enforcer", 1, 1, (const char**)opt_status, false },
    { 46, "Fake Rank & Cash Display VVIP", 1, 1, (const char**)opt_status, false },
    { 47, "Custom In-game Crosshair Overlay", 2, 3, (const char**)opt_cross, false },
    { 48, "FPS Counter & Ping Graph HUD", 1, 1, (const char**)opt_status, false },
    { 49, "Fullbright / Gamma Brightness", 1, 1, (const char**)opt_status, false },
    { 50, "Skybox Color Changer (Night Mode)", 1, 1, (const char**)opt_status, false },

    { 51, "[ BYPASS & SECURITY GUARD ]", 0, 0, NULL, true },
    { 52, "Anti-Cheat Guard Bypass (HS/Vanguard)", 1, 1, (const char**)opt_status, false },
    { 53, "Memory Integrity Shield Wrapper", 1, 1, (const char**)opt_status, false },
    { 54, "Screenshot Cleaner / Blackout", 1, 1, (const char**)opt_status, false },
    { 55, "Panic Key (Instant Hide Menu - INS)", 1, 1, (const char**)opt_status, false },
    { 56, "Config Auto-Saver (.ini support)", 1, 1, (const char**)opt_status, false },
    { 57, "Preset Configuration Loader", 1, 2, (const char**)opt_preset, false },
    { 58, "Exception Crash Handler (SEH)", 1, 1, (const char**)opt_status, false },
    { 59, "Debug Log Dumper Sanitizer", 1, 1, (const char**)opt_status, false },
    { 60, "Zepetto Packet Encryption Filter", 1, 1, (const char**)opt_status, false },

    { 61, "[ KERNEL MEMORY ENGINE ]", 0, 0, NULL, true },
    { 62, "Base Pointer Entity Resolver", 1, 1, (const char**)opt_status, false },
    { 63, "DirectX 9 Present Swapchain Hook", 1, 1, (const char**)opt_status, false },
    { 64, "Matrix Projection Calculator", 1, 1, (const char**)opt_status, false },
    { 65, "Thread-Safe Critical Lock Wrapper", 1, 1, (const char**)opt_status, false },
    { 66, "VirtualProtect Memory Enforcer", 1, 1, (const char**)opt_status, false },
    { 67, "Pattern Scan Signature Engine", 1, 1, (const char**)opt_status, false },
    { 68, "Ring Buffer Packet Logger", 1, 1, (const char**)opt_status, false },
    { 69, "Console Debug Output Redirector", 1, 1, (const char**)opt_status, false },
    { 70, "Network Latency Simulator Wrapper", 1, 1, (const char**)opt_status, false },

    { 71, "[ ADVANCED COMBAT HACKS ]", 0, 0, NULL, true },
    { 72, "Auto Headshot Mode (Silent Aim)", 1, 1, (const char**)opt_status, false },
    { 73, "Wall Shot / Bullet Penetration", 1, 1, (const char**)opt_status, false },
    { 74, "No Smoke & No Flashbang Effect", 1, 1, (const char**)opt_status, false },
    { 75, "Instant Plant / Defuse Bomb Speed", 1, 1, (const char**)opt_status, false },
    { 76, "Dynamic Hitbox Expander", 1, 1, (const char**)opt_status, false },
    { 77, "Enemy Weapon Identifier Display", 1, 1, (const char**)opt_status, false },
    { 78, "Spectator Warning Alert System", 1, 1, (const char**)opt_status, false },
    { 79, "Kill Sound FX Notification", 1, 1, (const char**)opt_status, false },
    { 80, "Match Duration Timer HUD", 1, 1, (const char**)opt_status, false },

    { 81, "[ MAP & ENVIRONMENT ]", 0, 0, NULL, true },
    { 82, "Remove Map Fog & Dust Particles", 1, 1, (const char**)opt_status, false },
    { 83, "Custom Sky Texture Injector", 1, 1, (const char**)opt_status, false },
    { 84, "Surface Friction Modifier", 1, 1, (const char**)opt_status, false },
    { 85, "Sound ESP (Footstep Direction)", 1, 1, (const char**)opt_status, false },
    { 86, "Radar Minimap Hack (All Enemy)", 1, 1, (const char**)opt_status, false },
    { 87, "Auto-Switch Pistol on Empty Mag", 1, 1, (const char**)opt_status, false },
    { 88, "Zero Gravity Moon Mode", 1, 1, (const char**)opt_status, false },
    { 89, "Kill Feed Customizer Overlay", 1, 1, (const char**)opt_status, false },
    { 90, "Clan War Match Stats Inspector", 1, 1, (const char**)opt_status, false },

    { 91, "[ VVIP SYSTEM & INFO ]", 0, 0, NULL, true },
    { 92, "Custom Watermark ('PB VVIP Mr.Rm19')", 1, 1, (const char**)opt_status, false },
    { 93, "UI Accent Theme Color (Matrix)", 1, 1, (const char**)opt_status, false },
    { 94, "Font Size & Style Adjuster", 1, 1, (const char**)opt_status, false },
    { 95, "Hotkey Binding Customization", 1, 1, (const char**)opt_status, false },
    { 96, "Cloud Profile Synchronizer", 1, 1, (const char**)opt_status, false },
    { 97, "Session Statistics Logger", 1, 1, (const char**)opt_status, false },
    { 98, "Auto-Update Checker via GitHub", 1, 1, (const char**)opt_status, false },
    { 99, "Diagnostics Performance Monitor", 1, 1, (const char**)opt_status, false },
    { 100, "Developer: github.com/Rm19x", 1, 1, (const char**)opt_status, false }
};

int g_SelectedIndex = 1;
int g_ScrollOffset = 0;
bool g_MenuVisible = true;
int g_SelectorIndex = 0; // 0: Lost Saga, 1: Point Blank
#define VISIBLE_ITEMS 14

void RenderMenuText(HWND hwnd) {
    HDC hdcWindow = GetDC(hwnd);
    if (!hdcWindow) return;

    RECT rc;
    GetClientRect(hwnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    HBRUSH hBrushBg = CreateSolidBrush(g_CurrentState == GAME_POINT_BLANK ? RGB(12, 15, 20) : RGB(5, 10, 8));
    FillRect(hdcMem, &rc, hBrushBg);
    DeleteObject(hBrushBg);

    HBRUSH hHeaderBrush = CreateSolidBrush(g_CurrentState == GAME_POINT_BLANK ? RGB(20, 35, 50) : RGB(10, 25, 15));
    RECT rcHeader = { 6, 6, WINDOW_WIDTH - 6, 34 };
    FillRect(hdcMem, &rcHeader, hHeaderBrush);
    DeleteObject(hHeaderBrush);

    COLORREF borderColor = (g_CurrentState == GAME_POINT_BLANK) ? RGB(0, 180, 255) : RGB(0, 255, 65);
    HPEN hPen = CreatePen(PS_SOLID, 2, borderColor);
    HPEN hOldPen = (HPEN)SelectObject(hdcMem, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdcMem, GetStockObject(NULL_BRUSH));
    Rectangle(hdcMem, 4, 4, WINDOW_WIDTH - 4, WINDOW_HEIGHT - 4);
    SelectObject(hdcMem, hOldPen);
    SelectObject(hdcMem, hOldBrush);
    DeleteObject(hPen);

    SetBkMode(hdcMem, TRANSPARENT);
    HFONT hFontTitle = CreateFontA(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                   DEFAULT_PITCH | FF_SWISS, "Consolas");
    HFONT hFontItem  = CreateFontA(13, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                   DEFAULT_PITCH | FF_SWISS, "Consolas");

    if (g_CurrentState == GAME_SELECTOR) {
        SelectObject(hdcMem, hFontTitle);
        SetTextColor(hdcMem, RGB(255, 215, 0));
        TextOutA(hdcMem, 14, 10, "pLMEMORIES - MULTI GAME VVIP HUB", 31);

        SetTextColor(hdcMem, RGB(180, 180, 180));
        TextOutA(hdcMem, 340, 11, "v2.0 200-in-1", 13);

        SelectObject(hdcMem, hFontItem);
        SetTextColor(hdcMem, RGB(255, 255, 255));
        TextOutA(hdcMem, 30, 80, "Silakan Pilih Game Engine VVIP Anda:", 33);

        if (g_SelectorIndex == 0) {
            RECT rcSel1 = { 25, 120, WINDOW_WIDTH - 25, 170 };
            HBRUSH hSelB1 = CreateSolidBrush(RGB(15, 45, 25));
            FillRect(hdcMem, &rcSel1, hSelB1);
            DeleteObject(hSelB1);
            SetTextColor(hdcMem, RGB(0, 255, 65));
            TextOutA(hdcMem, 40, 132, "> 1. Lost Saga VVIP (100 Fitur Lengkap)", 37);
        } else {
            SetTextColor(hdcMem, RGB(160, 220, 160));
            TextOutA(hdcMem, 40, 132, "  1. Lost Saga VVIP (100 Fitur Lengkap)", 37);
        }

        if (g_SelectorIndex == 1) {
            RECT rcSel2 = { 25, 190, WINDOW_WIDTH - 25, 240 };
            HBRUSH hSelB2 = CreateSolidBrush(RGB(30, 75, 110));
            FillRect(hdcMem, &rcSel2, hSelB2);
            DeleteObject(hSelB2);
            SetTextColor(hdcMem, RGB(0, 220, 255));
            TextOutA(hdcMem, 40, 202, "> 2. Point Blank VVIP (100 Fitur Lengkap)", 39);
        } else {
            SetTextColor(hdcMem, RGB(180, 200, 220));
            TextOutA(hdcMem, 40, 202, "  2. Point Blank VVIP (100 Fitur Lengkap)", 39);
        }

        SetTextColor(hdcMem, RGB(140, 180, 200));
        TextOutA(hdcMem, 30, 330, "[UP/DOWN] Pilih Game  [ENTER] Masuk  [ESC] Keluar", 49);
        TextOutA(hdcMem, 135, WINDOW_HEIGHT - 35, "Created by Mr.Rm19", 18);

    } else {
        SelectObject(hdcMem, hFontTitle);
        if (g_CurrentState == GAME_LOST_SAGA) {
            SetTextColor(hdcMem, RGB(0, 255, 65));
            TextOutA(hdcMem, 14, 10, "LOST SAGA VVIP [100 FITUR]", 26);
        } else {
            SetTextColor(hdcMem, RGB(0, 200, 255));
            TextOutA(hdcMem, 14, 10, "POINT BLANK VVIP [100 FITUR]", 28);
        }

        SetTextColor(hdcMem, RGB(150, 180, 200));
        TextOutA(hdcMem, 340, 11, "[ESC] Menu", 10);

        if (g_MenuVisible) {
            int relativeIdx = g_SelectedIndex - g_ScrollOffset;
            if (relativeIdx >= 0 && relativeIdx < VISIBLE_ITEMS) {
                int selY = 40 + (relativeIdx * 24) - 2;
                RECT rcSel = { 10, selY, WINDOW_WIDTH - 10, selY + 20 };
                HBRUSH hSelBrush = CreateSolidBrush(g_CurrentState == GAME_POINT_BLANK ? RGB(30, 75, 110) : RGB(15, 45, 25));
                FillRect(hdcMem, &rcSel, hSelBrush);
                DeleteObject(hSelBrush);
            }
        }

        SelectObject(hdcMem, hFontItem);
        int startY = 40;
        VipFeature* currentFeatures = (g_CurrentState == GAME_LOST_SAGA) ? g_LostSagaFeatures : g_PointBlankFeatures;

        for (int i = 0; i < VISIBLE_ITEMS; i++) {
            int idx = g_ScrollOffset + i;
            if (idx >= 100) break; // 

            int y = startY + (i * 24);

            if (currentFeatures[idx].is_header) {
                SetTextColor(hdcMem, g_CurrentState == GAME_POINT_BLANK ? RGB(255, 180, 0) : RGB(0, 200, 255));
                TextOutA(hdcMem, 14, y, currentFeatures[idx].name, (int)strlen(currentFeatures[idx].name));
            } else {
                if (idx == g_SelectedIndex) {
                    SetTextColor(hdcMem, borderColor);
                    TextOutA(hdcMem, 12, y, ">", 1);
                }
                SetTextColor(hdcMem, (idx == g_SelectedIndex) ? RGB(255, 255, 255) : RGB(180, 200, 200));
                
                char lineBuf[64];
                snprintf(lineBuf, sizeof(lineBuf), "%d. %s", currentFeatures[idx].id, currentFeatures[idx].name);
                TextOutA(hdcMem, 26, y, lineBuf, (int)strlen(lineBuf));

                const char* valStr = currentFeatures[idx].options[currentFeatures[idx].value];
                SetTextColor(hdcMem, (currentFeatures[idx].value > 0) ? RGB(0, 255, 120) : RGB(255, 80, 80));
                TextOutA(hdcMem, 360, y, valStr, (int)strlen(valStr));
            }
        }

        SetTextColor(hdcMem, RGB(120, 150, 180));
        TextOutA(hdcMem, 14, WINDOW_HEIGHT - 26, "[UP/DOWN] Scroll  [LEFT/RIGHT] Toggle  [INS] Hide", 51);
    }

    BitBlt(hdcWindow, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

    DeleteObject(hFontTitle);
    DeleteObject(hFontItem);
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdcWindow);
}

void RenderFrame(HWND hwnd) {
    if (!g_MenuVisible && g_CurrentState != GAME_SELECTOR) {
        if (g_pd3dDevice) {
            g_pd3dDevice->lpVtbl->Clear(g_pd3dDevice, 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(5, 10, 8), 1.0f, 0);
            g_pd3dDevice->lpVtbl->BeginScene(g_pd3dDevice);
            g_pd3dDevice->lpVtbl->EndScene(g_pd3dDevice);
            g_pd3dDevice->lpVtbl->Present(g_pd3dDevice, NULL, NULL, NULL, NULL);
        }
        return;
    }

    RenderMenuText(hwnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(hwnd, msg, wParam, lParam);
            if (hit == HTCLIENT) return HTCAPTION;
            return hit;
        }
        case WM_KEYDOWN:
            if (wParam == VK_INSERT && g_CurrentState != GAME_SELECTOR) {
                g_MenuVisible = !g_MenuVisible;
                InvalidateRect(hwnd, NULL, FALSE);
            } else {
                if (g_CurrentState == GAME_SELECTOR) {
                    if (wParam == VK_DOWN || wParam == VK_UP) {
                        g_SelectorIndex = 1 - g_SelectorIndex;
                    } else if (wParam == VK_RETURN) {
                        g_CurrentState = (g_SelectorIndex == 0) ? GAME_LOST_SAGA : GAME_POINT_BLANK;
                        g_SelectedIndex = 1;
                        g_ScrollOffset = 0;
                    } else if (wParam == VK_ESCAPE) {
                        PostQuitMessage(0);
                    }
                } else if (g_MenuVisible) {
                    VipFeature* currentFeatures = (g_CurrentState == GAME_LOST_SAGA) ? g_LostSagaFeatures : g_PointBlankFeatures;

                    if (wParam == VK_DOWN) {
                        do {
                            g_SelectedIndex = (g_SelectedIndex + 1) % 100;
                        } while (currentFeatures[g_SelectedIndex].is_header);

                        if (g_SelectedIndex >= g_ScrollOffset + VISIBLE_ITEMS) {
                            g_ScrollOffset = g_SelectedIndex - VISIBLE_ITEMS + 1;
                        }
                        if (g_SelectedIndex == 0) { g_ScrollOffset = 0; }
                    } else if (wParam == VK_UP) {
                        do {
                            g_SelectedIndex = (g_SelectedIndex - 1 + 100) % 100;
                        } while (currentFeatures[g_SelectedIndex].is_header);

                        if (g_SelectedIndex < g_ScrollOffset) {
                            g_ScrollOffset = g_SelectedIndex;
                        }
                    } else if (wParam == VK_RIGHT) {
                        if (!currentFeatures[g_SelectedIndex].is_header) {
                            int maxV = 0;
                            while (currentFeatures[g_SelectedIndex].options != NULL && currentFeatures[g_SelectedIndex].options[maxV] != NULL && maxV < 5) maxV++;
                            currentFeatures[g_SelectedIndex].value = (currentFeatures[g_SelectedIndex].value + 1) % maxV;
                        }
                    } else if (wParam == VK_LEFT) {
                        if (!currentFeatures[g_SelectedIndex].is_header) {
                            int maxV = 0;
                            while (currentFeatures[g_SelectedIndex].options != NULL && currentFeatures[g_SelectedIndex].options[maxV] != NULL && maxV < 5) maxV++;
                            currentFeatures[g_SelectedIndex].value = (currentFeatures[g_SelectedIndex].value - 1 + maxV) % maxV;
                        }
                    } else if (wParam == VK_ESCAPE) {
                        g_CurrentState = GAME_SELECTOR;
                    }
                }
                InvalidateRect(hwnd, NULL, FALSE);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEXA wc = {
        sizeof(WNDCLASSEXA), CS_CLASSDC, WndProc, 0L, 0L,
        hInstance, NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
        "pLMEMORIESMatrixClass", NULL
    };
    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowExA(
        WS_EX_TOPMOST, wc.lpszClassName, "pLMEMORIES - 200 Features VVIP Hub by Mr.Rm19",
        WS_POPUP, 100, 50, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, wc.hInstance, NULL
    );

    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_pD3D) return 1;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

    if (FAILED(g_pD3D->lpVtbl->CreateDevice(g_pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice))) {
        g_pD3D->lpVtbl->Release(g_pD3D);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            RenderFrame(hwnd);
            Sleep(16);
        }
    }

    if (g_pd3dDevice) g_pd3dDevice->lpVtbl->Release(g_pd3dDevice);
    if (g_pD3D) g_pD3D->lpVtbl->Release(g_pD3D);
    UnregisterClassA(wc.lpszClassName, wc.hInstance);
    return 0;
}