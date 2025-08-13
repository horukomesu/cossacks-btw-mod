#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

#include "SQPicture_compat.hpp"
#include "GP_Draw_compat.hpp"
#include "Text_compat.hpp"

namespace legacy { namespace ui {

// Legacy usage codes for AssignSound
constexpr int CLICK_SOUND = 0x1234;
constexpr int MOUSE_SOUND = 0x4321;

class RLCFont; // forward decl for pointers

class SimpleDialog;
using VCall = bool(*)(SimpleDialog*);

class SimpleDialog {
public:
    bool Enabled{true};
    bool Visible{true};
    bool NeedToDraw{true};
    bool MouseOver{false};
    int x{0}, y{0}, x1{0}, y1{0};
    VCall OnClick{nullptr};
    VCall OnDraw{nullptr};
    VCall OnMouseOver{nullptr};
    VCall OnUserClick{nullptr};
    int UserParam{0};
    // Optional hint text shown when hovered
    const char* Hint{nullptr};
    // Sound hooks (sound group names from SOUNDLIST.TXT)
    std::string MouseSoundName;
    std::string ClickSoundName;

    virtual ~SimpleDialog() = default;
    virtual void draw();
    virtual void handleClick(int mx, int my);
    void AssignSound(int id, int USAGE);
    void AssignSound(const char* name, int USAGE);
};

class Picture : public SimpleDialog {
public:
    legacy::SQPictureCompat* ActivePicture{nullptr};
    legacy::SQPictureCompat* PassivePicture{nullptr};
    legacy::SQPictureCompat* DisabledPicture{nullptr};
    bool Transparent{false};

    void draw() override;
};

class GPPicture : public SimpleDialog {
public:
    int FileID{ -1 };
    int SpriteID{ 0 };
    uint8_t Nation{ 0 };

    void draw() override;
};

class GP_Button : public SimpleDialog {
public:
    int GP_File{-1};
    int ActiveFrame{0};
    int PassiveFrame{0};
    uint8_t Nation{0};

    void draw() override;
};

class TextButton : public SimpleDialog {
public:
    const char* Message{nullptr};
    // Optional font; if null fallback to 8x8 bitmap text
    RLCFont* Font{nullptr};
    void draw() override;
};

class InputBox : public SimpleDialog {
public:
    std::string Text;
    int MaxLen{256};
    RLCFont* AFont{nullptr};
    RLCFont* PFont{nullptr};
    bool Active{false};
    void draw() override;
};

class VScrollBar : public SimpleDialog {
public:
    // logical
    int SMaxPos{0};
    int SPos{0};
    // GP visuals
    int GP_File{-1};
    int ScrIndex{0};
    int LineIndex{0};
    int ScrDx{0};
    int ScrDy{0};
    // dragging state
    bool Drag{false};
    // legacy geometry (for L and V types)
    int LineLx{0};
    int LineLy{0};
    int ScrLx{0};
    int ScrLy{0};
    int ScrollIndex{0};
    int StartGP_Spr{0};
    int LastTime{0};
    int OnesDy{1};
    bool Zaxvat{false};
    int btnly{0};
    int sbx{0}, sby{0}, sblx{0};

    void draw() override;
};

class ListBox : public SimpleDialog {
public:
    std::vector<std::string> Items;
    int CurItem{-1};
    int FirstItem{0};
    int Ny{0};
    int OneLy{18};
    VScrollBar* VS{nullptr};
    RLCFont* AFont{nullptr};
    RLCFont* PFont{nullptr};

    void AddItem(const char* str);
    void ClearItems();
    void SetFirstPos(int i);
    void SetCurrentItem(int i);
    void draw() override;
};

class ComboBox : public SimpleDialog {
public:
    // Appearance from GP file (like legacy addGP_ComboBox)
    int GP_File{-1};
    int UpPart{0};
    int Center{0};
    int DownPart{0};
    // Fonts
    RLCFont* ActiveFont{nullptr};
    RLCFont* PassiveFont{nullptr};
    // State
    bool IsActive{false};
    int CurLine{0};
    int LightIndex{-1};
    // Geometry
    int OneLx{0};
    int OneLy{0};
    int UpLy{0};
    int FontDx{2};
    int FontDy{0};
    int OneDx{1};
    int OneDy{1};
    int DropX{0};
    int DropY{0};
    int MaxVisible{10};
    int FirstItem{0};
    // Data
    std::vector<std::string> Lines;

    void AddLine(const char* s) { if (s) Lines.emplace_back(s); }
    void draw() override;
};

class DialogsSystem {
public:
    explicit DialogsSystem(int baseX = 0, int baseY = 0);
    ~DialogsSystem();

    Picture* addPicture(SimpleDialog* parent, int dx, int dy,
                        legacy::SQPictureCompat* Active,
                        legacy::SQPictureCompat* Passive,
                        legacy::SQPictureCompat* Disabled);
    GPPicture* addGPPicture(SimpleDialog* parent, int dx, int dy, int fileID, int spriteID, uint8_t nation = 0);
    GP_Button* addGP_Button(SimpleDialog* parent, int dx, int dy, int gpFile, int activeFrame, int passiveFrame, uint8_t nation = 0);
    TextButton* addTextButton(SimpleDialog* parent, int dx, int dy, const char* text);
    InputBox* addInputBox(SimpleDialog* parent, int dx, int dy, const char* buf,
                          int maxChars, int Lx, int Ly, RLCFont* AFont, RLCFont* PFont);
    VScrollBar* addGP_ScrollBar(SimpleDialog* parent, int dx, int dy,
                                int maxPos, int pos, int gpFile,
                                int scrIndex, int lineIndex, int scrDx, int scrDy);
    // Legacy long variant (explicit line length and marker size)
    VScrollBar* addGP_ScrollBarL(SimpleDialog* parent, int dx, int dy,
                                 int maxPos, int pos, int gpFile,
                                 int scrIndex, int lineLx, int lineLy, int scrDx, int scrDy);
    ListBox* addListBox(SimpleDialog* parent, int dx, int dy, int Lx, int Ly, int Ny,
                        RLCFont* AFont, RLCFont* PFont, VScrollBar* VS);
    ComboBox* addGP_ComboBox(SimpleDialog* parent, int dx, int dy, int gpFile,
                             int upPart, int center, int downPart,
                             RLCFont* ActiveFont, RLCFont* PassiveFont,
                             char* Contence);

    void ProcessDialogs(); // handles input and updates widgets state
    void MarkToDraw();
    void RefreshView();   // draws all widgets that need redraw

    int BaseX{0};
    int BaseY{0};
    // Hint rendering params (position relative to screen; font optional)
    int HintX{18};
    int HintY{701};
    RLCFont* HintFont{nullptr};

private:
    std::vector<std::unique_ptr<SimpleDialog>> m_dialogs;
};

} } // namespace legacy::ui


