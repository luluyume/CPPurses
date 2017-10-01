#include "cppurses.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
using namespace cppurses;

class Center_label : public Widget {
   public:
    Center_label(Glyph_string text) : label_{std::move(text)} {
        this->brush.add_attributes(Attribute::Bold);
        this->height_policy.type(Size_policy::Fixed);
        this->height_policy.hint(1);
    }

   protected:
    bool paint_event() override {
        Painter p{this};
        p.put(label_, this->width() / 2 - label_.length() / 2, 0);
        return Widget::paint_event();
    }

   private:
    Glyph_string label_;
};

struct Attribute_select : Vertical_layout {
    Checkbox& bold = this->make_child<Checkbox>("Bold");
    Checkbox& italic = this->make_child<Checkbox>("Italics");
    Checkbox& underline = this->make_child<Checkbox>("Underline");
    Checkbox& standout = this->make_child<Checkbox>("Standout");
    Checkbox& dim = this->make_child<Checkbox>("Dim");
    Checkbox& inverse = this->make_child<Checkbox>("Inverse");
    Checkbox& invisible = this->make_child<Checkbox>("Invisible");
};

void save_file(const std::string& filename, const std::string& output) {
    std::ofstream ofs(filename);
    if (!ofs.fail()) {
        ofs << output;
    }
}

std::string load_file(const std::string& filename) {
    std::ifstream ifs(filename);
    if (ifs.fail()) {
        return "";
    }
    std::ostringstream oss;
    ifs >> oss.rdbuf();
    if (ifs.fail() && !ifs.eof()) {
        return "";
    }
    return oss.str();
}

int main() {
    using A = cppurses::Attribute;
    System sys;

    Vertical_layout main_layout;

    // Titlebar
    main_layout.make_child<Titlebar>("Notepad");

    auto& tx_attr_layout = main_layout.make_child<Horizontal_layout>();

    // Textbox
    auto& text_area = tx_attr_layout.make_child<Textbox>();
    text_area.brush.add_attributes(background(Color::Dark_gray));
    enable_border(text_area);
    set_corners(text_area.border, "╭", "╮", "╰", "╯");

    // Attributes and Color Section
    auto& attr_layout = tx_attr_layout.make_child<Vertical_layout>();
    attr_layout.width_policy.type(Size_policy::Fixed);
    attr_layout.width_policy.hint(16);

    // Color Choosers
    attr_layout.make_child<Center_label>("Foreground⤵");
    auto& foreground_chooser = attr_layout.make_child<Color_chooser>();
    foreground_chooser.color_changed.connect(slot::set_foreground(text_area));
    attr_layout.make_child<Center_label>("Background⤵");
    auto& background_chooser = attr_layout.make_child<Color_chooser>();
    background_chooser.color_changed.connect(slot::set_background(text_area));

    // Attributes Checkboxes - Connect Signals to Slots
    auto& al = attr_layout.make_child<Attribute_select>();
    al.bold.checked.connect(slot::add_new_text_attribute(text_area, A::Bold));
    al.italic.checked.connect(
        slot::add_new_text_attribute(text_area, A::Italic));
    al.underline.checked.connect(
        slot::add_new_text_attribute(text_area, A::Underline));
    al.standout.checked.connect(
        slot::add_new_text_attribute(text_area, A::Standout));
    al.dim.checked.connect(slot::add_new_text_attribute(text_area, A::Dim));
    al.inverse.checked.connect(
        slot::add_new_text_attribute(text_area, A::Inverse));
    al.invisible.checked.connect(
        slot::add_new_text_attribute(text_area, A::Invisible));
    al.bold.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Bold));
    al.italic.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Italic));
    al.underline.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Underline));
    al.standout.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Standout));
    al.dim.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Dim));
    al.inverse.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Inverse));
    al.invisible.unchecked.connect(
        slot::remove_new_text_attribute(text_area, A::Invisible));

    // Save/Load area
    auto& save_area = main_layout.make_child<Horizontal_layout>();
    save_area.height_policy.type(Size_policy::Fixed);
    save_area.height_policy.hint(1);
    auto& load_btn = save_area.make_child<Push_button>("Load");
    load_btn.width_policy.type(Size_policy::Fixed);
    load_btn.width_policy.hint(6);
    set_background(load_btn, Color::Blue);
    auto& filename_edit = save_area.make_child<Textbox>();
    filename_edit.disable_scrollwheel();
    set_background(filename_edit, Color::White);
    set_foreground(filename_edit, Color::Black);
    auto& save_btn = save_area.make_child<Push_button>("Save");
    save_btn.width_policy.type(Size_policy::Fixed);
    save_btn.width_policy.hint(6);
    set_background(save_btn, Color::Blue);

    // TODO
    // Slot<void(Glyph_string)> set_text()
    // load_btn.file_loaded.connect(slot::set_text(text_area));

    // Signals and Slots
    load_btn.clicked.connect([&text_area, &filename_edit] {
        text_area.set_text(load_file(filename_edit.contents().str()));
    });
    save_btn.clicked.connect([&text_area, &filename_edit] {
        save_file(filename_edit.contents().str(), text_area.contents().str());
    });

    sys.set_head(&main_layout);
    Focus::set_focus_to(&text_area);
    return sys.run();
}