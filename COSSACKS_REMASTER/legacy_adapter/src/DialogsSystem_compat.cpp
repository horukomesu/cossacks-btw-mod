#include "../include/legacy/DialogsSystem_compat.hpp"

#include "engine_core/EngineCore.hpp"
#include "legacy/Sound_compat.hpp"
#include "engine_core/Render2D.hpp"
#include "engine_core/InputAdapter.hpp"
#include "legacy/RLCFont_compat.hpp"
#include "resource_io/ResourceIO.hpp"
#include "audio_core/AudioCore.hpp"
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

namespace legacy { namespace ui {

namespace {
    // Toggle verbose UI debug logging
    constexpr bool kDebugUI = false;

    const char* dialog_type_name(SimpleDialog* d) {
        if (dynamic_cast<GP_Button*>(d))   return "GP_Button";
        if (dynamic_cast<GPPicture*>(d))   return "GPPicture";
        if (dynamic_cast<Picture*>(d))     return "Picture";
        if (dynamic_cast<TextButton*>(d))  return "TextButton";
        if (dynamic_cast<InputBox*>(d))    return "InputBox";
        if (dynamic_cast<VScrollBar*>(d))  return "VScrollBar";
        if (dynamic_cast<ListBox*>(d))     return "ListBox";
        return "SimpleDialog";
    }
}

void SimpleDialog::draw() {
    if (OnDraw) OnDraw(this);
}

void SimpleDialog::handleClick(int mx, int my) {
    if (OnClick && mx >= x && mx <= x1 && my >= y && my <= y1) {
        // Play click sound if assigned
        if (!ClickSoundName.empty()) {
            // Allow direct file mapping e.g. "Sound\\MenuSvitok2.wav"
            if (ClickSoundName.find('.') != std::string::npos ||
                ClickSoundName.find('/') != std::string::npos ||
                ClickSoundName.find('\\') != std::string::npos) {
                legacy::sound_compat::play_file(ClickSoundName, 1.0f, 0.0f);
            } else if (ClickSoundName == std::string("SCROLL2") || ClickSoundName == std::string("SCROLL")) {
                // Generic click on menu buttons uses Svitok2
                legacy::sound_compat::play_file("Sound\\MenuSvitok2.wav", 1.0f, 0.0f);
            } else if (ClickSoundName == std::string("BACK") || ClickSoundName == std::string("SCROLLCLICK")) {
                // Back action uses SvitKlik
                legacy::sound_compat::play_file("Sound\\MenuSvitKlik.wav", 1.0f, 0.0f);
            } else {
                legacy::sound_compat::play_group(ClickSoundName, 1.0f, 0.0f);
            }
        }
        OnClick(this);
    }
}

void Picture::draw() {
    if (!Visible) return;
    legacy::SQPictureCompat* p = ActivePicture ? ActivePicture : PassivePicture;
    if (p) p->Draw(x, y);
    if (kDebugUI) {
        std::cout << "[ui] Picture draw at (" << x << "," << y << ") size ~("
                  << (x1 - x + 1) << "x" << (y1 - y + 1) << ")\n";
    }
}

void GPPicture::draw() {
    if (!Visible) return;
    if (FileID >= 0) {
        legacy::gp::GPS.ShowGP(x, y, FileID, SpriteID, Nation);
    }
    if (kDebugUI) {
        std::cout << "[ui] GPPicture draw: GPID=" << FileID
                  << " spr=" << SpriteID
                  << " pos=(" << x << "," << y << ")\n";
    }
}

void GP_Button::draw() {
    if (!Visible) return;
    const int frame = (MouseOver ? ActiveFrame : PassiveFrame);
    if (GP_File >= 0) {
        legacy::gp::GPS.ShowGP(x, y, GP_File, frame, Nation);
        // auto-fix bounds based on actual sprite size (helps precise hover)
        const int w = legacy::gp::GPS.GetGPWidth(GP_File, frame);
        const int h = legacy::gp::GPS.GetGPHeight(GP_File, frame);
        if (w > 0 && h > 0) {
            x1 = x + w - 1;
            y1 = y + h - 1;
        }
    }
    if (kDebugUI) {
        std::cout << "[ui] GP_Button draw: GPID=" << GP_File
                  << " frame=" << frame
                  << (MouseOver ? " (hover)" : " (idle)")
                  << " pos=(" << x << "," << y << ") size=(" << (x1 - x + 1)
                  << "x" << (y1 - y + 1) << ")\n";
    }
}

void TextButton::draw() {
    if (!Visible) return;
    if (Message) {
        int w = 0, h = 0;
        if (Font) {
            w = Font->GetRLCStrWidth(Message);
            h = Font->GetCharHeight();
            if (x1 <= x || y1 <= y) { x1 = x + w; y1 = y + h; }
            Font->ShowString(x, y, Message);
        } else {
            w = legacy::ui::GetStringWidth(Message);
            h = legacy::ui::GetStringHeight();
            if (x1 <= x || y1 <= y) { x1 = x + w; y1 = y + h; }
            legacy::ui::ShowString(x, y, Message, 0xFFFFFFFF);
        }
    }
    if (kDebugUI) {
        std::cout << "[ui] TextButton draw: \"" << (Message ? Message : "") << "\" pos=("
                  << x << "," << y << ") size=(" << (x1 - x + 1) << "x" << (y1 - y + 1) << ")\n";
    }
}

void InputBox::draw() {
    if (!Visible) return;
    // Background rectangle
    engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(y),
                                     static_cast<float>(x1 - x + 1), static_cast<float>(y1 - y + 1),
                                     Active ? 0.15f : 0.07f, 0.07f, 0.07f, 0.6f);
    const char* cstr = Text.c_str();
    if (AFont) AFont->ShowString(x + 4, y + 4, cstr);
    else legacy::ui::ShowString(x + 4, y + 4, cstr, 0xFFFFFFFF);
}

void VScrollBar::draw() {
    if (!Visible) return;
    if (!SMaxPos) return;
    // Draw track line: use a solid bar instead of parchment when Index<0 or to emulate line
    const bool horizontal = (LineLx > LineLy);
    // По требованию: удалить фоновые полосы (не рисуем трек вообще)
    // Compute marker position (support horizontal or vertical depending on geometry)
    int scx = x, scy = y;
    if (horizontal) {
        scx = x + (SPos * std::max(0, LineLx - ScrLx)) / SMaxPos + ScrDx;
        scy = y + ScrDy;
    } else {
        scx = x + ScrDx;
        const int trackLy = (LineIndex >= 0 ? LineLy : (y1 - y + 1));
        const int markerLy = (ScrLy > 0 ? ScrLy : legacy::gp::GPS.GetGPHeight(GP_File, ScrIndex));
        scy = y + ScrDy + (SPos * std::max(0, trackLy - markerLy)) / SMaxPos;
    }
    // Draw marker
    legacy::gp::GPS.ShowGP(scx, scy, GP_File, (StartGP_Spr ? StartGP_Spr + 4 : ScrIndex), 0);
}

void ListBox::AddItem(const char* str) {
    if (str) Items.emplace_back(str);
}

void ListBox::ClearItems() {
    Items.clear();
    CurItem = -1;
    FirstItem = 0;
}

void ListBox::SetFirstPos(int i) {
    FirstItem = std::max(0, std::min(i, static_cast<int>(Items.size())));
}

void ListBox::SetCurrentItem(int i) {
    CurItem = (i >= 0 && i < static_cast<int>(Items.size())) ? i : -1;
}

void ListBox::draw() {
    if (!Visible) return;
    // Simple background
    engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(x1 - x + 1), static_cast<float>(y1 - y + 1), 0.0f, 0.0f, 0.0f, 0.3f);
    const int maxLines = Ny > 0 ? Ny : (y1 - y + 1) / OneLy;
    for (int row = 0; row < maxLines; ++row) {
        const int idx = FirstItem + row;
        if (idx >= static_cast<int>(Items.size())) break;
        const int py = y + row * OneLy;
        const bool selected = (idx == CurItem);
        // Row background highlight
        if (selected) {
            engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(py), static_cast<float>(x1 - x + 1), static_cast<float>(OneLy), 0.8f, 0.8f, 0.2f, 0.3f);
        }
        // TODO(step 4): draw text with RLCFont; placeholder marker
        engine_core::render2d::draw_rect(static_cast<float>(x + 4), static_cast<float>(py + 4), 6.0f, 6.0f, 1.0f, 1.0f, 1.0f, 0.7f);
    }
}

// GP-based ComboBox: closed + active dropdown with scroll
void ComboBox::draw() {
    if (!Visible) return;
    // Draw closed panel from GP if provided
    if (GP_File >= 0) {
        legacy::gp::GPS.ShowGP(x, y, GP_File, UpPart + 1 - (MouseOver || IsActive), 0);
    }
    // Draw current line text
    if (!Lines.empty()) {
        const char* txt = Lines[std::max(0, std::min(static_cast<int>(Lines.size()) - 1, CurLine))].c_str();
        if (IsActive && ActiveFont) ActiveFont->ShowString(x + OneDx, y + OneDy, txt);
        else if (PassiveFont) PassiveFont->ShowString(x + OneDx, y + OneDy, txt);
        else legacy::ui::ShowString(x + OneDx, y + OneDy, txt, 0xFFFFFFFF);
    }
    // Active dropdown
    if (IsActive && !Lines.empty()) {
        const int XI = (x + x1) >> 1;
        const int upLy = legacy::gp::GPS.GetGPHeight(GP_File, UpPart);
        const int dropX = x + Center;
        const int dropY = y + (y1 - y + 1);
        const int itemLy = (OneLy > 0 ? OneLy : 18);
        const int maxLines = std::max(1, MaxVisible);
        const int last = std::min(static_cast<int>(Lines.size()), FirstItem + maxLines);
        // top cap
        legacy::gp::GPS.ShowGP(dropX, dropY, GP_File, UpPart + 2, 0);
        int y0 = dropY + upLy;
        for (int i = FirstItem; i < last; ++i) {
            // alternating center sprites (+5/+6)
            const int spr = UpPart + ( (i & 1) ? 6 : 5 );
            legacy::gp::GPS.ShowGP(dropX, y0, GP_File, spr, 0);
            if (i == CurLine || i == LightIndex) {
                // selected highlight (+3)
                legacy::gp::GPS.ShowGP(dropX, y0, GP_File, UpPart + 3, 0);
                if (ActiveFont) ActiveFont->ShowString(x + 2, y0 + 2, Lines[i].c_str());
                else legacy::ui::ShowString(x + 2, y0 + 2, Lines[i].c_str(), 0xFFFFFFFF);
            } else {
                if (PassiveFont) PassiveFont->ShowString(x + 2, y0 + 2, Lines[i].c_str());
                else legacy::ui::ShowString(x + 2, y0 + 2, Lines[i].c_str(), 0xFFFFFFFF);
            }
            y0 += itemLy;
        }
        // bottom cap
        legacy::gp::GPS.ShowGP(dropX, y0, GP_File, UpPart + 7, 0);
        // On click inside dropdown commit handled in ProcessDialogs()
    }
}

DialogsSystem::DialogsSystem(int baseX, int baseY) : BaseX(baseX), BaseY(baseY) {}
DialogsSystem::~DialogsSystem() = default;

Picture* DialogsSystem::addPicture(SimpleDialog* /*parent*/, int dx, int dy,
                                   legacy::SQPictureCompat* Active,
                                   legacy::SQPictureCompat* Passive,
                                   legacy::SQPictureCompat* Disabled) {
    auto obj = std::make_unique<Picture>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    if (Active) { obj->x1 = obj->x + Active->GetLx() - 1; obj->y1 = obj->y + Active->GetLy() - 1; }
    obj->ActivePicture = Active;
    obj->PassivePicture = Passive;
    obj->DisabledPicture = Disabled;
    Picture* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    return ret;
}

GPPicture* DialogsSystem::addGPPicture(SimpleDialog* /*parent*/, int dx, int dy, int fileID, int spriteID, uint8_t nation) {
    auto obj = std::make_unique<GPPicture>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    obj->FileID = fileID;
    obj->SpriteID = spriteID;
    obj->Nation = nation;
    // Estimate bounds from GP size if possible
    const int w = legacy::gp::GPS.GetGPWidth(fileID, spriteID);
    const int h = legacy::gp::GPS.GetGPHeight(fileID, spriteID);
    obj->x1 = obj->x + (w > 0 ? w : 0) - 1;
    obj->y1 = obj->y + (h > 0 ? h : 0) - 1;
    GPPicture* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    return ret;
}

GP_Button* DialogsSystem::addGP_Button(SimpleDialog* /*parent*/, int dx, int dy, int gpFile, int activeFrame, int passiveFrame, uint8_t nation) {
    auto obj = std::make_unique<GP_Button>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    obj->GP_File = gpFile;
    obj->ActiveFrame = activeFrame;
    obj->PassiveFrame = passiveFrame;
    obj->Nation = nation;
    const int w = legacy::gp::GPS.GetGPWidth(gpFile, passiveFrame);
    const int h = legacy::gp::GPS.GetGPHeight(gpFile, passiveFrame);
    obj->x1 = obj->x + (w > 0 ? w : 0) - 1;
    obj->y1 = obj->y + (h > 0 ? h : 0) - 1;
    GP_Button* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    if (kDebugUI) {
        std::cout << "[ui] addGP_Button: GPID=" << gpFile
                  << " active=" << activeFrame << " passive=" << passiveFrame
                  << " pos=(" << (BaseX + dx) << "," << (BaseY + dy) << ") size=("
                  << (w > 0 ? w : 0) << "x" << (h > 0 ? h : 0) << ")\n";
    }
    return ret;
}

TextButton* DialogsSystem::addTextButton(SimpleDialog* /*parent*/, int dx, int dy, const char* text) {
    auto obj = std::make_unique<TextButton>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    obj->x1 = obj->x + 100; // placeholder width until fonts are in place
    obj->y1 = obj->y + 18;
    obj->Message = text;
    TextButton* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    return ret;
}

InputBox* DialogsSystem::addInputBox(SimpleDialog* /*parent*/, int dx, int dy, const char* buf,
                                     int maxChars, int Lx, int Ly, RLCFont* AFont, RLCFont* PFont) {
    auto obj = std::make_unique<InputBox>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    obj->x1 = obj->x + std::max(10, Lx) - 1;
    obj->y1 = obj->y + std::max(10, Ly) - 1;
    obj->Text = buf ? buf : "";
    obj->MaxLen = std::max(1, maxChars);
    obj->AFont = AFont;
    obj->PFont = PFont;
    InputBox* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    return ret;
}

    VScrollBar* DialogsSystem::addGP_ScrollBar(SimpleDialog* /*parent*/, int dx, int dy,
                                           int maxPos, int pos, int gpFile,
                                           int scrIndex, int lineIndex, int scrDx, int scrDy) {
    auto obj = std::make_unique<VScrollBar>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
        // size from line sprite
        const int w = legacy::gp::GPS.GetGPWidth(gpFile, lineIndex);
        const int h = legacy::gp::GPS.GetGPHeight(gpFile, lineIndex);
        obj->x1 = obj->x + (w > 0 ? w : 120) - 1;
        obj->y1 = obj->y + (h > 0 ? h : 16) - 1;
        obj->SMaxPos = maxPos;
        obj->SPos = pos;
        obj->GP_File = gpFile;
        obj->ScrIndex = scrIndex;
        obj->LineIndex = lineIndex;
        obj->LineLx = w;
        obj->LineLy = h;
        obj->ScrLx = legacy::gp::GPS.GetGPWidth(gpFile, scrIndex);
        obj->ScrLy = legacy::gp::GPS.GetGPHeight(gpFile, scrIndex);
        obj->ScrDx = scrDx;
        obj->ScrDy = scrDy;
        if (obj->LineLx > obj->LineLy) obj->ScrDy -= obj->ScrLy >> 1; else obj->ScrDx -= obj->ScrLx >> 1;
    VScrollBar* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    return ret;
}

    VScrollBar* DialogsSystem::addGP_ScrollBarL(SimpleDialog* /*parent*/, int dx, int dy,
                                                int maxPos, int pos, int gpFile,
                                                int scrIndex, int lineLx, int lineLy, int scrDx, int scrDy) {
        auto obj = std::make_unique<VScrollBar>();
        obj->x = BaseX + dx;
        obj->y = BaseY + dy;
        obj->x1 = obj->x + lineLx - 1;
        obj->y1 = obj->y + lineLy - 1;
        obj->LineLx = lineLx;
        obj->LineLy = lineLy;
        obj->SMaxPos = maxPos;
        obj->SPos = pos;
        obj->GP_File = gpFile;
        obj->ScrollIndex = scrIndex;
        obj->ScrIndex = scrIndex;
        obj->ScrLx = legacy::gp::GPS.GetGPWidth(gpFile, scrIndex);
        obj->ScrLy = legacy::gp::GPS.GetGPHeight(gpFile, scrIndex);
        obj->ScrDx = scrDx;
        obj->ScrDy = scrDy;
        if (obj->LineLx > obj->LineLy) obj->ScrDy -= obj->ScrLy >> 1; else obj->ScrDx -= obj->ScrLx >> 1;
        VScrollBar* ret = obj.get();
        m_dialogs.emplace_back(std::move(obj));
        return ret;
    }

    ComboBox* DialogsSystem::addGP_ComboBox(SimpleDialog* /*parent*/, int dx, int dy, int gpFile,
                                            int upPart, int center, int downPart,
                                            RLCFont* AFont, RLCFont* PFont,
                                            char* /*Contence*/) {
        auto obj = std::make_unique<ComboBox>();
        obj->x = BaseX + dx;
        obj->y = BaseY + dy;
        obj->GP_File = gpFile;
        obj->UpPart = upPart;
        obj->Center = center;
        obj->DownPart = downPart;
        obj->OneLx = legacy::gp::GPS.GetGPWidth(gpFile, upPart);
        obj->OneLy = legacy::gp::GPS.GetGPHeight(gpFile, upPart + 5);
        const int Lx = obj->OneLx;
        const int Ly = legacy::gp::GPS.GetGPHeight(gpFile, upPart) + 1;
        obj->x1 = obj->x + (Lx > 0 ? Lx : 120) - 1;
        obj->y1 = obj->y + (Ly > 0 ? Ly : 16) - 1;
        obj->ActiveFont = AFont;
        obj->PassiveFont = PFont;
        obj->IsActive = false;
        obj->CurLine = 0;
        ComboBox* ret = obj.get();
        m_dialogs.emplace_back(std::move(obj));
        return ret;
    }

ListBox* DialogsSystem::addListBox(SimpleDialog* /*parent*/, int dx, int dy, int Lx, int Ly, int Ny,
                                   RLCFont* AFont, RLCFont* PFont, VScrollBar* VS) {
    auto obj = std::make_unique<ListBox>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    obj->x1 = obj->x + Lx - 1;
    obj->y1 = obj->y + Ly - 1;
    obj->Ny = Ny;
    obj->AFont = AFont;
    obj->PFont = PFont;
    obj->VS = VS;
    ListBox* ret = obj.get();
    m_dialogs.emplace_back(std::move(obj));
    return ret;
}

void DialogsSystem::ProcessDialogs() {
    // Minimal input: consume clicks and dispatch to dialogs
    engine_core::input::MouseEvent ev;
    while (engine_core::input::ReadMEvent(ev)) {
        if (kDebugUI && ev.type == engine_core::input::MouseEventType::Move) {
            std::cout << "[ui] mouse move: (" << ev.x << "," << ev.y << ")\n";
        }
        // Track mouse-over state
        if (ev.type == engine_core::input::MouseEventType::Move) {
            for (auto& d : m_dialogs) {
                const bool prev = d->MouseOver;
                d->MouseOver = (ev.x >= d->x && ev.x <= d->x1 && ev.y >= d->y && ev.y <= d->y1);
                if (d->MouseOver != prev) d->NeedToDraw = true;
                if (d->MouseOver && d->OnMouseOver) d->OnMouseOver(d.get());
                // Mouse-over sound (play only on enter)
                if (d->MouseOver && !prev) {
                    if (!d->MouseSoundName.empty()) legacy::sound_compat::play_group(d->MouseSoundName, 0.6f, 0.0f);
                }
                if (kDebugUI && d->MouseOver != prev) {
                    std::cout << "[ui] hover " << (d->MouseOver ? "enter: " : "leave: ")
                              << dialog_type_name(d.get())
                              << " bounds=(" << d->x << "," << d->y << ".." << d->x1 << "," << d->y1 << ")\n";
                }
            }
        }
        if (ev.type == engine_core::input::MouseEventType::LeftDown) {
            for (auto& d : m_dialogs) {
                if (d->MouseOver) {
                    if (kDebugUI) {
                        std::cout << "[ui] click: " << dialog_type_name(d.get())
                                  << " UserParam=" << d->UserParam << " at (" << ev.x << "," << ev.y << ")\n";
                    }
                    if (d->OnUserClick) d->OnUserClick(d.get());
                    d->handleClick(ev.x, ev.y);
                    // Toggle ComboBox open/close
                    if (auto cb = dynamic_cast<ComboBox*>(d.get())) {
                        // If open and clicking inside dropdown, commit selection
                        if (cb->IsActive && cb->LightIndex >= 0 && cb->LightIndex < static_cast<int>(cb->Lines.size())) {
                            cb->CurLine = cb->LightIndex;
                            cb->IsActive = false;
                        } else {
                            cb->IsActive = !cb->IsActive;
                        }
                        cb->NeedToDraw = true;
                    }
                    // ListBox selection by click
                    if (auto lb = dynamic_cast<ListBox*>(d.get())) {
                        const int row = (ev.y - lb->y) / lb->OneLy;
                        const int idx = lb->FirstItem + row;
                        if (idx >= 0 && idx < static_cast<int>(lb->Items.size())) {
                            lb->CurItem = idx;
                            lb->NeedToDraw = true;
                            if (kDebugUI) {
                                std::cout << "[ui] ListBox select row " << row << " -> index " << idx << " (item count=" << lb->Items.size() << ")\n";
                            }
                        }
                    }
                    // Activate InputBox on click; deactivate others
                    if (auto ib = dynamic_cast<InputBox*>(d.get())) {
                        for (auto& d2 : m_dialogs) {
                            if (auto ib2 = dynamic_cast<InputBox*>(d2.get())) ib2->Active = false;
                        }
                        ib->Active = true;
                        ib->NeedToDraw = true;
                    }
                    // Activate InputBox on click; deactivate others
                    if (auto ib = dynamic_cast<InputBox*>(d.get())) {
                        for (auto& d2 : m_dialogs) {
                            if (auto ib2 = dynamic_cast<InputBox*>(d2.get())) ib2->Active = false;
                        }
                        ib->Active = true;
                        ib->NeedToDraw = true;
                    }
                    // Scrollbar dragging start
        if (auto vs = dynamic_cast<VScrollBar*>(d.get())) {
            // legacy grab only when clicking on marker region
            const bool horizontal = (vs->LineLx > vs->LineLy);
            int scx = vs->x + (horizontal ? (vs->SPos * std::max(0, vs->LineLx - vs->ScrLx)) / std::max(1, vs->SMaxPos) + vs->ScrDx : vs->ScrDx);
            int scy = vs->y + (horizontal ? vs->ScrDy : vs->ScrDy + (vs->SPos * std::max(0, (vs->LineLy > 0 ? vs->LineLy : (vs->y1 - vs->y + 1)) - vs->ScrLy)) / std::max(1, vs->SMaxPos));
            const int mw = (vs->ScrLx > 0 ? vs->ScrLx : legacy::gp::GPS.GetGPWidth(vs->GP_File, vs->ScrIndex));
            const int mh = (vs->ScrLy > 0 ? vs->ScrLy : legacy::gp::GPS.GetGPHeight(vs->GP_File, vs->ScrIndex));
            if (ev.x > scx && ev.x < scx + mw && ev.y > scy && ev.y < scy + mh) {
                vs->Drag = true;
                vs->Zaxvat = true;
                vs->sbx = ev.x;
                vs->sby = ev.y;
                vs->sblx = vs->SPos;
                vs->NeedToDraw = true;
            }
            if (kDebugUI) {
                std::cout << "[ui] VScrollBar drag start at pos=" << vs->SPos << "/" << vs->SMaxPos << "\n";
            }
        }
                }
            }
        }
        if (ev.type == engine_core::input::MouseEventType::LeftUp) {
            for (auto& d : m_dialogs) {
                if (auto vs = dynamic_cast<VScrollBar*>(d.get())) {
                    vs->Drag = false;
                    vs->Zaxvat = false;
                    if (kDebugUI) {
                        std::cout << "[ui] VScrollBar drag stop at pos=" << vs->SPos << "/" << vs->SMaxPos << "\n";
                    }
                }
            }
        }
        if (ev.type == engine_core::input::MouseEventType::Move) {
            for (auto& d : m_dialogs) {
                if (auto vs = dynamic_cast<VScrollBar*>(d.get()); vs && vs->Drag) {
                    const bool horizontal = (vs->LineLx > vs->LineLy);
                    if (horizontal) {
                        const int denom = std::max(1, vs->LineLx - vs->ScrLx);
                        const int delta = (ev.x - vs->sbx) * (vs->SMaxPos + 1) / denom;
                        const int newPos = std::max(0, std::min(vs->SMaxPos, vs->sblx + delta));
                        if (newPos != vs->SPos) { vs->SPos = newPos; vs->NeedToDraw = true; }
                    } else {
                        const int trackLy = (vs->LineLy > 0 ? vs->LineLy : (vs->y1 - vs->y + 1));
                        const int denom = std::max(1, trackLy - vs->ScrLy);
                        const int delta = (ev.y - vs->sby) * (vs->SMaxPos + 1) / denom;
                        const int newPos = std::max(0, std::min(vs->SMaxPos, vs->sblx + delta));
                        if (newPos != vs->SPos) { vs->SPos = newPos; vs->NeedToDraw = true; }
                    }
                    if (kDebugUI) {
                        std::cout << "[ui] VScrollBar drag move -> pos=" << vs->SPos << "/" << vs->SMaxPos << "\n";
                    }
                }
                if (auto cb = dynamic_cast<ComboBox*>(d.get()); cb && cb->IsActive) {
                    // Update highlight line on move
                    const int itemLy = (cb->OneLy > 0 ? cb->OneLy : 18);
                    const int dropY = cb->y + (cb->y1 - cb->y + 1) + legacy::gp::GPS.GetGPHeight(cb->GP_File, cb->UpPart);
                    const int rel = ev.y - dropY;
                    if (rel >= 0) {
                        const int idx = cb->FirstItem + rel / itemLy;
                        cb->LightIndex = std::max(0, std::min(static_cast<int>(cb->Lines.size()) - 1, idx));
                        cb->NeedToDraw = true;
                    }
                }
            }
        }
        if (ev.type == engine_core::input::MouseEventType::Wheel) {
            for (auto& d : m_dialogs) {
                if (auto vs = dynamic_cast<VScrollBar*>(d.get()); vs && d->MouseOver) {
                    // Wheel moves by fixed step
                    const int step = 16;
                    const int delta = (ev.param > 0) ? -step : step;
                    const int newPos = std::max(0, std::min(vs->SMaxPos, vs->SPos + delta));
                    if (newPos != vs->SPos) { vs->SPos = newPos; vs->NeedToDraw = true; }
                    if (kDebugUI) {
                        std::cout << "[ui] Wheel on VScrollBar -> pos=" << vs->SPos << "/" << vs->SMaxPos << " (delta=" << ev.param << ")\n";
                    }
                }
            }
        }
    }

    // Process text input for active InputBox using char queue
    uint32_t cp = 0;
    while (engine_core::input::ReadChar(cp)) {
        // Find active input box
        for (auto& d : m_dialogs) {
            auto ib = dynamic_cast<InputBox*>(d.get());
            if (!ib || !ib->Active) continue;
            bool changed = false;
            if (cp == 8 /*backspace*/ || cp == 127 /*del*/) {
                if (!ib->Text.empty()) { ib->Text.pop_back(); changed = true; }
            } else if (cp >= 32 && cp < 128) {
                if (static_cast<int>(ib->Text.size()) < ib->MaxLen) {
                    ib->Text.push_back(static_cast<char>(cp));
                    changed = true;
                }
            }
            if (changed) ib->NeedToDraw = true;
        }
    }

    // Robust hover update even if no move events were queued this frame
    int mx = 0, my = 0;
    engine_core::input::GetPointer(mx, my);
    for (auto& d : m_dialogs) {
        const bool prev = d->MouseOver;
        const bool overNow = (mx >= d->x && mx <= d->x1 && my >= d->y && my <= d->y1);
        if (overNow != prev) {
            d->MouseOver = overNow;
            d->NeedToDraw = true;
        }
    }

    // Synchronize ListBox with attached scrollbars (map SPos -> FirstItem, legacy step=10)
    for (auto& d : m_dialogs) {
        if (auto lb = dynamic_cast<ListBox*>(d.get())) {
            if (lb->VS && lb->Items.size() > 0) {
                const int maxFirst = std::max(0, static_cast<int>(lb->Items.size()) - std::max(1, lb->Ny));
                if (lb->VS->SMaxPos > 0) {
                    // Legacy maps SPos/10 -> FirstItem for list boxes (see ComplexBox code)
                    const int first = std::max(0, std::min(maxFirst, lb->VS->SPos / 10));
                    if (first != lb->FirstItem) { lb->FirstItem = first; lb->NeedToDraw = true; }
                } else {
                    if (lb->FirstItem != 0) { lb->FirstItem = 0; lb->NeedToDraw = true; }
                }
            }
        }
    }
}

// Map legacy AssignSound(name, USAGE) to loading a WAV buffer via ResourceIO and AudioCore
void SimpleDialog::AssignSound(const char* name, int USAGE) {
    if (!name || !*name) return;
    if (USAGE == CLICK_SOUND) {
        ClickSoundName = name;
    } else if (USAGE == MOUSE_SOUND) {
        MouseSoundName = name;
    } else {
        ClickSoundName = name;
    }
}

void SimpleDialog::AssignSound(int id, int USAGE) {
    // Not used in remaster menu paths; keep for parity
    (void)id;
    (void)USAGE;
}

void DialogsSystem::MarkToDraw() {
    for (auto& d : m_dialogs) d->NeedToDraw = true;
}

void DialogsSystem::RefreshView() {
    // First pass: draw everything except open ComboBox dropdowns
    std::vector<ComboBox*> openCombos;
    for (auto& d : m_dialogs) {
        if (auto cb = dynamic_cast<ComboBox*>(d.get()); cb && cb->IsActive) {
            openCombos.push_back(cb);
            continue;
        }
        if (d->NeedToDraw && d->Visible) {
            if (kDebugUI) {
                std::cout << "[ui] draw: " << dialog_type_name(d.get())
                          << " bounds=(" << d->x << "," << d->y << ".." << d->x1 << "," << d->y1 << ")\n";
            }
            d->draw();
            d->NeedToDraw = false;
        }
    }
    // Second pass: draw open ComboBox dropdowns on top
    for (auto* cb : openCombos) {
        cb->draw();
        cb->NeedToDraw = false;
    }
    // Draw hint if any hovered dialog has one
    for (auto& d : m_dialogs) {
        if (d->Visible && d->MouseOver && d->Hint) {
            if (HintFont) {
                HintFont->ShowString(HintX, HintY, d->Hint);
            } else {
                legacy::ui::ShowString(HintX, HintY, d->Hint, 0xFFFFFFFF);
            }
            break;
        }
    }
}

} } // namespace legacy::ui


