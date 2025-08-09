#include "../include/legacy/DialogsSystem_compat.hpp"

#include "engine_core/EngineCore.hpp"
#include "engine_core/Render2D.hpp"
#include "engine_core/InputAdapter.hpp"
#include "legacy/RLCFont_compat.hpp"
#include <algorithm>
#include <string>

namespace legacy { namespace ui {

void SimpleDialog::draw() {
    if (OnDraw) OnDraw(this);
}

void SimpleDialog::handleClick(int mx, int my) {
    if (OnClick && mx >= x && mx <= x1 && my >= y && my <= y1) {
        OnClick(this);
    }
}

void Picture::draw() {
    if (!Visible) return;
    legacy::SQPictureCompat* p = ActivePicture ? ActivePicture : PassivePicture;
    if (p) p->Draw(x, y);
}

void GPPicture::draw() {
    if (!Visible) return;
    if (FileID >= 0) {
        legacy::gp::GPS.ShowGP(x, y, FileID, SpriteID, Nation);
    }
}

void GP_Button::draw() {
    if (!Visible) return;
    const int frame = (MouseOver ? ActiveFrame : PassiveFrame);
    if (GP_File >= 0) {
        legacy::gp::GPS.ShowGP(x, y, GP_File, frame, Nation);
    }
}

void TextButton::draw() {
    if (!Visible) return;
    if (Message) {
        const int w = legacy::ui::GetStringWidth(Message);
        const int h = legacy::ui::GetStringHeight();
        if (x1 <= x || y1 <= y) { x1 = x + w; y1 = y + h; }
        legacy::ui::ShowString(x, y, Message, 0xFFFFFFFF);
    }
}

void VScrollBar::draw() {
    if (!Visible) return;
    // Draw line background
    if (GP_File >= 0) {
        legacy::gp::GPS.ShowGP(x, y, GP_File, LineIndex, 0);
        // Draw marker at position
        const int markerX = x + ScrDx + SPos;
        const int markerY = y + ScrDy;
        legacy::gp::GPS.ShowGP(markerX, markerY, GP_File, ScrIndex, 0);
    }
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

VScrollBar* DialogsSystem::addGP_ScrollBar(SimpleDialog* /*parent*/, int dx, int dy,
                                           int maxPos, int pos, int gpFile,
                                           int scrIndex, int lineIndex, int scrDx, int scrDy) {
    auto obj = std::make_unique<VScrollBar>();
    obj->x = BaseX + dx;
    obj->y = BaseY + dy;
    // approximate size from line sprite
    const int w = legacy::gp::GPS.GetGPWidth(gpFile, lineIndex);
    const int h = legacy::gp::GPS.GetGPHeight(gpFile, lineIndex);
    obj->x1 = obj->x + (w > 0 ? w : 120) - 1;
    obj->y1 = obj->y + (h > 0 ? h : 16) - 1;
    obj->SMaxPos = maxPos;
    obj->SPos = pos;
    obj->GP_File = gpFile;
    obj->ScrIndex = scrIndex;
    obj->LineIndex = lineIndex;
    obj->ScrDx = scrDx;
    obj->ScrDy = scrDy;
    VScrollBar* ret = obj.get();
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
        // Track mouse-over state
        if (ev.type == engine_core::input::MouseEventType::Move) {
            for (auto& d : m_dialogs) {
                const bool prev = d->MouseOver;
                d->MouseOver = (ev.x >= d->x && ev.x <= d->x1 && ev.y >= d->y && ev.y <= d->y1);
                if (d->MouseOver != prev) d->NeedToDraw = true;
                if (d->MouseOver && d->OnMouseOver) d->OnMouseOver(d.get());
            }
        }
        if (ev.type == engine_core::input::MouseEventType::LeftDown) {
            for (auto& d : m_dialogs) {
                if (d->MouseOver) {
                    if (d->OnUserClick) d->OnUserClick(d.get());
                    d->handleClick(ev.x, ev.y);
                    // ListBox selection by click
                    if (auto lb = dynamic_cast<ListBox*>(d.get())) {
                        const int row = (ev.y - lb->y) / lb->OneLy;
                        const int idx = lb->FirstItem + row;
                        if (idx >= 0 && idx < static_cast<int>(lb->Items.size())) {
                            lb->CurItem = idx;
                            lb->NeedToDraw = true;
                        }
                    }
                    // Scrollbar dragging start
                    if (auto vs = dynamic_cast<VScrollBar*>(d.get())) {
                        vs->Drag = true;
                        vs->NeedToDraw = true;
                    }
                }
            }
        }
        if (ev.type == engine_core::input::MouseEventType::LeftUp) {
            for (auto& d : m_dialogs) {
                if (auto vs = dynamic_cast<VScrollBar*>(d.get())) {
                    vs->Drag = false;
                }
            }
        }
        if (ev.type == engine_core::input::MouseEventType::Move) {
            for (auto& d : m_dialogs) {
                if (auto vs = dynamic_cast<VScrollBar*>(d.get()); vs && vs->Drag) {
                    // simple horizontal movement mapping to SPos (no bounds yet)
                    const int newPos = std::max(0, std::min(vs->SMaxPos, ev.x - (vs->x + vs->ScrDx)));
                    if (newPos != vs->SPos) { vs->SPos = newPos; vs->NeedToDraw = true; }
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
                }
            }
        }
    }

    // Synchronize ListBox with attached scrollbars (map SPos -> FirstItem)
    for (auto& d : m_dialogs) {
        if (auto lb = dynamic_cast<ListBox*>(d.get())) {
            if (lb->VS && lb->Items.size() > 0) {
                const int maxFirst = std::max(0, static_cast<int>(lb->Items.size()) - std::max(1, lb->Ny));
                if (lb->VS->SMaxPos > 0) {
                    const float t = static_cast<float>(lb->VS->SPos) / static_cast<float>(lb->VS->SMaxPos);
                    const int first = std::max(0, std::min(maxFirst, static_cast<int>(t * maxFirst + 0.5f)));
                    if (first != lb->FirstItem) { lb->FirstItem = first; lb->NeedToDraw = true; }
                } else {
                    if (lb->FirstItem != 0) { lb->FirstItem = 0; lb->NeedToDraw = true; }
                }
            }
        }
    }
}

void DialogsSystem::MarkToDraw() {
    for (auto& d : m_dialogs) d->NeedToDraw = true;
}

void DialogsSystem::RefreshView() {
    for (auto& d : m_dialogs) {
        if (d->NeedToDraw && d->Visible) {
            d->draw();
            d->NeedToDraw = false;
        }
    }
}

} } // namespace legacy::ui


